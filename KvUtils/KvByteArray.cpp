#include "KvByteArray.h"

#define IS_RAW_DATA(d) ((d)->offset != sizeof(KvByteArrayData))

KvByteArray::KvByteArray(const char *s, int size)
{
	if (!s) {
		d = Data::sharedNull();
	}
	else {
		if (size < 0)
			size = int(strlen(s));
		if (!size) {
			d = Data::allocate(0);
		}
		else {
			d = Data::allocate(uint(size) + 1u);
			KV_CHECK_PTR(d);
			d->size = size;
			memcpy(d->data(), s, size);
			d->data()[size] = '\0';
		}
	}
}

KvByteArray::KvByteArray(int size, char c)
{
	if (size <= 0) {
		d = Data::allocate(0);
	}
	else {
		d = Data::allocate(uint(size) + 1u);
		KV_CHECK_PTR(d);
		d->size = size;
		memset(d->data(), c, size);
		d->data()[size] = '\0';
	}
}

void KvByteArray::resize(int size)
{
	if (size < 0)
		size = 0;

	if (IS_RAW_DATA(d) && !d->ref.isShared() && size < d->size) {
		d->size = size;
		return;
	}

	if (size == 0 && !d->capacity) {
		Data *x = Data::allocate(0);
		if (!d->ref.deref())
			Data::deallocate(d);
		d = x;
	}
	else if (d->size == 0 && d->ref.isStatic()) {
		Data *x = Data::allocate(uint(size) + 1u);
		KV_CHECK_PTR(x);
		x->size = size;
		x->data()[size] = '\0';
		d = x;
	}
	else {
		if (d->ref.isShared() || uint(size) + 1u > d->alloc
			|| (!d->capacity && size < d->size
			&& uint(size) + 1u < uint(d->alloc >> 1)))
			realloc(uint(size) + 1u);
		if (d->alloc) {
			d->size = size;
			d->data()[size] = '\0';
		}
	}
}

KvByteArray & KvByteArray::fill(char c, int size /*= -1*/)
{
	resize(size < 0 ? d->size : size);
	if (d->size)
		memset(d->data(), c, d->size);
	return *this;
}

void KvByteArray::clear()
{
	if (!d->ref.deref())
		Data::deallocate(d);
	d = Data::sharedNull();
}

void KvByteArray::realloc(int alloc)
{
	if (d->ref.isShared() || IS_RAW_DATA(d)) {
		Data *x = Data::allocate(alloc);
		KV_CHECK_PTR(x);
		x->size = kvMin(int(alloc) - 1, d->size);
		::memcpy(x->data(), d->data(), x->size);
		x->data()[x->size] = '\0';
		if (!d->ref.deref())
			Data::deallocate(d);
		d = x;
	}
	else {
		Data *x = Data::reallocateUnaligned(d, alloc);
		KV_CHECK_PTR(x);
		d = x;
	}
}

KvByteArray& KvByteArray::operator=(const char *str)
{
	Data *x;
	if (!str) {
		x = Data::sharedNull();
	}
	else if (!*str) {
		x = Data::allocate(0);
	}
	else {
		const int len = int(strlen(str));
		const uint fullLen = len + 1;
		if (d->ref.isShared() || fullLen > d->alloc
			|| (len < d->size && fullLen < uint(d->alloc >> 1)))
			realloc(fullLen);
		x = d;
		memcpy(x->data(), str, fullLen); // include null terminator
		x->size = len;
	}
	x->ref.ref();
	if (!d->ref.deref())
		Data::deallocate(d);
	d = x;
	return *this;
}

KvByteArray& KvByteArray::operator=(const KvByteArray &other)
{
	other.d->ref.ref();
	if (!d->ref.deref())
		Data::deallocate(d);
	d = other.d;
	return *this;
}


KvByteArray& KvByteArray::append(char ch)
{
	if (d->ref.isShared() || uint(d->size) + 2u > d->alloc)
		realloc(uint(d->size) + 2u);
	d->data()[d->size++] = ch;
	d->data()[d->size] = '\0';
	return *this;
}

KvByteArray& KvByteArray::append(const char *str)
{
	if (str) {
		const int len = int(strlen(str));
		if (d->ref.isShared() || uint(d->size + len) + 1u > d->alloc)
			realloc(uint(d->size + len) + 1u);
		memcpy(d->data() + d->size, str, len + 1); // include null terminator
		d->size += len;
	}
	return *this;
}


KvByteArray &KvByteArray::append(const char *str, int len)
{
	if (len < 0)
		len = kvstrlen(str);
	if (str && len) {
		if (d->ref.isShared() || uint(d->size + len) + 1u > d->alloc)
			realloc(uint(d->size + len) + 1u);
		memcpy(d->data() + d->size, str, len); // include null terminator
		d->size += len;
		d->data()[d->size] = '\0';
	}
	return *this;
}

KvByteArray &KvByteArray::append(const KvByteArray &ba)
{
	if (d->size == 0 && d->ref.isStatic() && !IS_RAW_DATA(ba.d)) {
		*this = ba;
	}
	else if (ba.d->size != 0) {
		if (d->ref.isShared() || uint(d->size + ba.d->size) + 1u > d->alloc)
			realloc(uint(d->size + ba.d->size) + 1u);
		memcpy(d->data() + d->size, ba.d->data(), ba.d->size);
		d->size += ba.d->size;
		d->data()[d->size] = '\0';
	}
	return *this;
}

KvByteArray KvByteArray::fromRawData(const char *data, int size)
{
	Data *x;
	if (!data) {
		x = Data::sharedNull();
	}
	else if (!size) {
		x = Data::allocate(0);
	}
	else {
		x = Data::fromRawData(data, size);
		KV_CHECK_PTR(x);
	}
	KvByteArrayDataPtr dataPtr = { x };
	return KvByteArray(dataPtr);
}
