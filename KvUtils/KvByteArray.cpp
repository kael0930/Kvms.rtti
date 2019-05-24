#include "KvByteArray.h"

#define IS_RAW_DATA(d) ((d)->data != (d)->array)

int AllocMore(int alloc, int extra)
{
	assert(alloc >= 0 && extra >= 0);
	assert(alloc < (1 << 30) - extra && "AllocMore Requested size is too large!");

	if (alloc == 0 && extra == 0)
		return 0;
	const int page = 1 << 12; //4096kb
	int nalloc;
	alloc += extra;
	if (alloc < 1 << 6) //64kb
	{
		nalloc = (1 << 3) + ((alloc >> 3) << 3);
	}
	else  
	{
		// don't do anything if the loop will overflow signed int.
		if (alloc >= INT_MAX / 2)
			return INT_MAX;
		nalloc = (alloc < page) ? 1 << 3 : page;
		while (nalloc < alloc) 
		{
			if (nalloc <= 0)
				return INT_MAX;
			nalloc *= 2;
		}
	}
	return nalloc - extra;
}


KvByteArray::Data KvByteArray::shared_null = { (1),
0, 0, shared_null.array, { 0 } };
KvByteArray::Data KvByteArray::shared_empty = { (1),
0, 0, shared_empty.array, { 0 } };

KvByteArray::KvByteArray(const char *s)
{
	if (!s)
	{
		d = &shared_null;
	}
	else if (!*s)
	{
		d = &shared_empty;
	}
	else
	{
		int len = kvstrlen(s);
		d = static_cast<Data*>(kvMalloc(sizeof(Data)+len));
		KV_CHECK_PTR(d);
		d->ref = 0;;
		d->alloc = d->size = len;
		d->data = d->array;
		memcpy(d->array, s, len + 1); // include null terminator
	}
	d->ref++;
}

KvByteArray::KvByteArray(const char *s, int size)
{
	if (!s)
	{
		d = &shared_null;
	}
	else if (size <= 0)
	{
		d = &shared_empty;
	}
	else
	{
		d = static_cast<Data *>(kvMalloc(sizeof(Data)+size));
		KV_CHECK_PTR(d);
		d->ref = 0;
		d->alloc = d->size = size;
		d->data = d->array;
		memcpy(d->array, s, size);
		d->array[size] = '\0';
	}
	d->ref++;
}

KvByteArray::KvByteArray(int size, char c)
{
	if (size <= 0) 
	{
		d = &shared_null;
	}
	else 
	{
		d = static_cast<Data *>(kvMalloc(sizeof(Data)+size));
		KV_CHECK_PTR(d);
		d->ref = 0;
		d->alloc = d->size = size;
		d->data = d->array;
		d->array[size] = '\0';
		memset(d->array, c, size);
	}
	d->ref++;
}

void KvByteArray::resize(int size)
{
	if (size <= 0) 
	{
		Data *x = &shared_empty;
		x->ref++;
		if (!--d->ref)
			kvFree(d);
		d = x;
	}
	else if (d == &shared_null) 
	{
		// Optimize the idiom:
		Data *x = static_cast<Data *>(kvMalloc(sizeof(Data)+size));
		KV_CHECK_PTR(x);
		x->ref = 1;
		x->alloc = x->size = size;
		x->data = x->array;
		x->array[size] = '\0';
		(void)d->ref--; // cannot be 0, x points to shared_null
		d = x;
	}
	else 
	{
		if (d->ref != 1
			|| size > d->alloc
			|| (size < d->size && size < d->alloc >> 1))
		{
			realloc(AllocMore(size, sizeof(Data)));
		}
		if (d->alloc >= size) 
		{
			d->size = size;
			if (d->data == d->array) 
			{
				d->array[size] = '\0';
			}
		}
	}
}

KvByteArray & KvByteArray::fill(char c, int size /*= -1*/)
{
	resize(size < 0 ? d->size : size);
	if (d->size)
		memset(d->data, c, d->size);
	return *this;
}

void KvByteArray::clear()
{
	if (!--d->ref)
		kvFree(d);
	d = &shared_null;
	d->ref++;
}

void KvByteArray::realloc(int alloc)
{
	if (d->ref != 1 || d->data != d->array) 
	{
		Data *x = static_cast<Data *>(kvMalloc(sizeof(Data)+alloc));
		KV_CHECK_PTR(x);
		x->size = kvMin(alloc, d->size);
		::memcpy(x->array, d->data, x->size);
		x->array[x->size] = '\0';
		x->ref = 1;
		x->alloc = alloc;
		x->data = x->array;
		if (!--d->ref)
			kvFree(d);
		d = x;
	}
	else 
	{
		Data *x = static_cast<Data *>(kvRealloc(d, sizeof(Data)+alloc));
		KV_CHECK_PTR(x);
		x->alloc = alloc;
		x->data = x->array;
		d = x;
	}
}

KvByteArray& KvByteArray::operator=(const char *str)
{
	Data *x;
	if (!str) 
	{
		x = &shared_null;
	}
	else if (!*str) 
	{
		x = &shared_empty;
	}
	else 
	{
		int len = kvstrlen(str);
		if (d->ref != 1
			|| len > d->alloc
			|| (len < d->size && len < d->alloc >> 1))
		{
			realloc(len);
		}
		x = d;
		memcpy(x->data, str, len + 1); // include null terminator
		x->size = len;
	}
	x->ref++;
	if (!--d->ref)
		kvFree(d);
	d = x;
	return *this;
}

KvByteArray& KvByteArray::operator=(const KvByteArray &other)
{
	other.d->ref++;
	if (!--d->ref)
		kvFree(d);
	d = other.d;
	return *this;
}


KvByteArray& KvByteArray::append(char ch)
{
	if (d->ref != 1 || d->size + 1 > d->alloc)
		realloc(AllocMore(d->size + 1, sizeof(Data)));
	d->data[d->size++] = ch;
	d->data[d->size] = '\0';
	return *this;
}

KvByteArray& KvByteArray::append(const char *str)
{
	if (str) {
		int len = kstrlen(str);
		if (d->ref != 1 || d->size + len > d->alloc)
			realloc(AllocMore(d->size + len, sizeof(Data)));
		memcpy(d->data + d->size, str, len + 1); // include null terminator
		d->size += len;
	}
	return *this;
}


KvByteArray &KvByteArray::append(const char *str, int len)
{
	if (len < 0)
		len = kstrlen(str);
	if (str && len) {
		if (d->ref != 1 || d->size + len > d->alloc)
			realloc(AllocMore(d->size + len, sizeof(Data)));
		memcpy(d->data + d->size, str, len); // include null terminator
		d->size += len;
		d->data[d->size] = '\0';
	}
	return *this;
}

KvByteArray &KvByteArray::append(const KvByteArray &ba)
{
	if ((d == &shared_null || d == &shared_empty) && !IS_RAW_DATA(ba.d)) {
		*this = ba;
	}
	else if (ba.d != &shared_null) {
		if (d->ref != 1 || d->size + ba.d->size > d->alloc)
			realloc(AllocMore(d->size + ba.d->size, sizeof(Data)));
		memcpy(d->data + d->size, ba.d->data, ba.d->size);
		d->size += ba.d->size;
		d->data[d->size] = '\0';
	}
	return *this;
}