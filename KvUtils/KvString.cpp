#include "KvString.h"


const KvString KvString::null;

KvString::KvString()
:length(0), capacity(CAPACITY), data(stack_data)
{
	data[length] = '\0';
}


KvString::KvString(const KvString &s)
:length(0), capacity(CAPACITY), data(stack_data)
{
	copy(s);
}

KvString::KvString(const char *s)
: length(0), capacity(CAPACITY), data(stack_data)
{
	copy(s);
}

KvString::~KvString()
{
	destroy();
}

void KvString::resize(int size)
{
	allocate(size);
	length = size;
	data[length] = '\0';
}

void KvString::allocate(int size)
{
	if (size + 1 <= (capacity & CAPACITY_MASK)) return;
	int dynamic = (capacity & DYNAMIC_FLAG);
	capacity = (size + 1) | DYNAMIC_FLAG;
	char *new_data = new char[size + 1];
	do_memcpy(new_data, data, length);
	if (dynamic) delete[] data;
	data = new_data;
	data[length] = '\0';
}

void KvString::reserve(int size)
{
	if (size + 1 <= (capacity & CAPACITY_MASK)) return;
	int dynamic = (capacity & DYNAMIC_FLAG);
	capacity = (size * 2 + 1) | DYNAMIC_FLAG;
	char *new_data = new char[size * 2 + 1];
	do_memcpy(new_data, data, length);
	if (dynamic) delete[] data;
	data = new_data;
	data[length] = '\0';
}

void KvString::clear()
{
	length = 0;
	data[length] = '\0';
}

void KvString::destroy()
{
	int dynamic = (capacity & DYNAMIC_FLAG);
	length = 0;
	capacity = CAPACITY;
	if (dynamic) delete[] data;
	data = stack_data;
	data[length] = '\0';
}

void KvString::copy(const char *s, int len /*= -1*/)
{
	if (s == 0)
	{
		clear();
	}
	else
	{
		if (len == -1)
		{
			len = (int)strlen(s);
			do_copy(s, len);
		}
	}
}

void KvString::copy(const KvString &s, int len /*= -1*/)
{
	if (len == -1)
	{
		len = s.length;
	}
	do_copy(s.data, len);
}

void KvString::append(char c)
{
	do_append(length, c);
}

void KvString::append(const char *s, int len /*= -1*/)
{
	if (s == NULL) return;
	if (len == -1) len = (int)strlen(s);
	do_append(length, s, len);
}

void KvString::append(const KvString &s, int len /*= -1*/)
{
	if (len == -1) len = s.length;
	do_append(length, s.data, len);
}

void KvString::append(int pos, char c)
{
	do_append(pos, c);
}

void KvString::append(int pos, const char *s, int len /*= -1*/)
{
	if (s == NULL) return;
	if (len == -1) len = (int)strlen(s);
	do_append(pos, s, len);
}

void KvString::append(int pos, const KvString &s, int len /*= -1*/)
{
	if (len == -1) len = s.length;
	do_append(pos, s.data, len);
}

void KvString::remove()
{
	assert(length > 0 && "String::remove(): bad length");
	data[--length] = '\0';
}

void KvString::remove(int pos, int len /*= 1*/)
{
	assert(pos >= 0 && pos <= length && "String::remove(): bad position");
	assert(len >= 0 && pos + len <= length && "String::remove(): bad length");
	do_memcpy(data + pos, data + pos + len, length - pos - len);
	length -= len;
	data[length] = '\0';
}

void KvString::lower()
{
	char *s = data;
	while (*s) {
		char c = *s;
		if (c >= 'A' && c <= 'Z') *s += ('a' - 'A');
		s++;
	}
}

void KvString::upper()
{
	char *s = data;
	while (*s) {
		char c = *s;
		if (c >= 'a' && c <= 'z') *s -= ('a' - 'A');
		s++;
	}
}

KvString& KvString::operator+=(const KvString &s)
{
	do_append(length, s.data, s.length);
	return *this;
}

KvString& KvString::operator+=(const char *s)
{
	if (s == 0)
	{
		return *this;
	}
	do_append(length, s, (int)strlen((s)));
	return *this;
}

KvString& KvString::operator+=(char c)
{
	do_append(length, c);
	return *this;
}

KvString& KvString::operator=(const KvString &s)
{
	if (this == &s)
	{
		return *this;
	}
	do_copy(s.data, s.length);
	return *this;

}

KvString& KvString::operator=(const char *s)
{
	if (data == s)
	{
		return *this;
	}
	
	if (s == 0)
	{
		clear();
	}
	else
	{
		do_copy(s, (int)strlen(s));
	}
	return *this;

}
bool KvString::operator==(const KvString &s) const
{
	if (length != s.length)
	{
		return false;
	}

	for (int idx = 0; idx < length; ++idx)
	{
		if (data[idx] != s.data[idx])
		{
			return false;
		}
	}

	return true;
}

void KvString::do_copy(const char *s, int len)
{
	length = len;
	if (length < (capacity & CAPACITY_MASK)) 
	{
		do_memcpy(data, s, length);
	}
	else 
	{
		int dynamic = (capacity & DYNAMIC_FLAG);
		capacity = (length + 1) | DYNAMIC_FLAG;
		char *new_data = new char[length + 1];
		do_memcpy(new_data, s, length);
		if (dynamic) delete[] data;
		data = new_data;
	}
	data[length] = '\0';
}

void KvString::do_append(int pos, char c)
{
	assert(pos >= 0 && pos <= length && "String::append(): bad position");
	int new_length = length + 1;
	if (new_length < (capacity & CAPACITY_MASK))
	{
		do_memcpy(data + pos + 1, data + pos, length - pos);
		data[pos] = c;
	}
	else
	{
		int dynamic = (capacity & DYNAMIC_FLAG);
		capacity = (new_length * 2 + 1) | DYNAMIC_FLAG;
		char *new_data = new char[new_length * 2 + 1];
		do_memcpy(new_data, data, pos);
		new_data[pos] = c;

		do_memcpy(new_data + pos + 1, data + pos, length - pos);
		if (dynamic) delete[] data;
		data = new_data;
	}
	length = new_length;
	data[length] = '\0';
}

void KvString::do_append(int pos, const char *s, int len)
{
	assert(pos >= 0 && pos <= length && "String::append(): bad position");
	int new_length = length + len;
	if (new_length < (capacity & CAPACITY_MASK)) 
	{
		do_memcpy(data + pos + len, data + pos, length - pos);
		do_memcpy(data + pos, s, len);
	}
	else 
	{
		int dynamic = (capacity & DYNAMIC_FLAG);
		capacity = (new_length * 2 + 1) | DYNAMIC_FLAG;
		char *new_data = new char[new_length * 2 + 1];
		do_memcpy(new_data, data, pos);
		do_memcpy(new_data + pos, s, len);
		do_memcpy(new_data + pos + len, data + pos, length - pos);
		if (dynamic) delete[] data;
		data = new_data;
	}
	length = new_length;
	data[length] = '\0';
}

void KvString::do_memcpy(char *dest, const char *src, int size)
{
	if (dest > src && (int)(dest - src) < size) 
	{
		dest += size - 1;
		src += size - 1;
		for (int i = size; i > 0; i--) 
		{
			*dest-- = *src--;
		}
	}
	else 
	{
#ifdef _WEBGL
		memcpy(dest, src, size);
#else
		if (size & ~15) 
		{
			for (int i = size >> 4; i > 0; i--) 
			{
				*(unsigned int*)(dest + 0) = *(const unsigned int*)(src + 0);
				*(unsigned int*)(dest + 4) = *(const unsigned int*)(src + 4);
				*(unsigned int*)(dest + 8) = *(const unsigned int*)(src + 8);
				*(unsigned int*)(dest + 12) = *(const unsigned int*)(src + 12);
				dest += 16;
				src += 16;
			}
			size &= 15;
		}
		if (size & ~3) 
		{
			for (int i = size >> 2; i > 0; i--)
			{
				*(unsigned int*)dest = *(const unsigned int*)src;
				dest += 4;
				src += 4;
			}
			size &= 3;
		}
		if (size) 
		{
			for (int i = size; i > 0; i--) 
			{
				*dest++ = *src++;
			}
		}
#endif
	}
}