#ifndef KvByteArray_h__
#define KvByteArray_h__

#include "KvUtils.h"
#include "KvArrayData.h"

#include <string>
#include <atomic>

inline uint kvstrlen(const char *str)
{
	return str ? uint(strlen(str)) : 0;
}

inline uint kvstrnlen(const char *str, uint maxlen)
{
	uint length = 0;
	if (str) {
		while (length < maxlen && *str++)
			length++;
	}
	return length;
}

inline int kvstrcmp(const char *str1, const char *str2)
{
	return (str1 && str2) ? strcmp(str1, str2)
		: (str1 ? 1 : (str2 ? -1 : 0));
}

typedef KvArrayData KvByteArrayData;

template<int N> struct KvStaticByteArrayData
{
	KvByteArrayData ba;
	char data[N + 1];

	KvByteArrayData *data_ptr() const
	{
		KV_ASSERT(ba.ref);
		return const_cast<KvByteArrayData *>(&ba);
	}
};

struct KvByteArrayDataPtr
{
	KvByteArrayData *ptr;
};

#define KV_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(size, offset) \
	KV_STATIC_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(size, offset)

#define KV_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER(size) \
	KV_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(size, sizeof(KvByteArrayData)) \

#define KvByteArrayLiteral(str) \
	([]() -> KvByteArray { \
		enum { Size = sizeof(str)-1 }; \
		static const KvStaticByteArrayData<Size> kvbytearray_literal = { \
		KV_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER(Size), \
		str }; \
		KvByteArrayDataPtr holder = { kvbytearray_literal.data_ptr() }; \
		const KvByteArray ba(holder); \
		return ba; \
	}()) \


class KV_UTILS_EXPORT KvByteArray
{
private:
	typedef KvTypedArrayData<char> Data;

public:
	inline KvByteArray();
	KvByteArray(const char *s, int size = -1);
	KvByteArray(int size, char c);
	inline KvByteArray(const KvByteArray &);
	inline ~KvByteArray();

	KvByteArray& operator=(const KvByteArray &);
	KvByteArray& operator=(const char *str);
	KvByteArray &operator+=(char c);
	KvByteArray &operator+=(const char *s);
	KvByteArray &operator+=(const KvByteArray &a);

	inline void swap(KvByteArray &other) { kvSwap(d, other.d); }

	inline int size() const;
	bool isEmpty() const;
	void resize(int size);

	KvByteArray &fill(char c, int size = -1);

	int capacity() const { return d->alloc; }
	void reserve(int size);
	void squeeze() { if (d->size < d->alloc) realloc(d->size); }

	operator const char *() const { return d->data(); }
	operator const void *() const { return d->data(); }

	char *data() { detach(); return d->data(); }
	const char *data() const { return d->data(); }
	inline const char *constData() const { return d->data(); }
	inline void detach() { if (d->ref.isShared()) realloc(d->size); }
	bool isDetached() const { return !d->ref.isShared(); }
	inline bool isSharedWith(const KvByteArray &other) const { return d == other.d; }
	void clear();

	char at(int i) const;
	char operator[](int i) const;
	char operator[](uint i) const;

	inline int count() const { return d->size; }
	int length() const { return d->size; }
	bool isNull() const { return d == KvArrayData::sharedNull(); }

	KvByteArray &append(char c);
	KvByteArray &append(const char *s);
	KvByteArray &append(const char *s, int len);
	KvByteArray &append(const KvByteArray &a);

	static KvByteArray fromRawData(const char *, int size);

	inline KvByteArray(KvByteArrayDataPtr dd)
		: d(static_cast<Data *>(dd.ptr))
	{
	}

private:
	Data *d;

	KvByteArray(Data *dd, int /*dummy*/, int /*dummy*/) : d(dd) {}
	void realloc(int alloc);
	void expand(int i);
	KvByteArray nulTerminated() const;

public:
	typedef Data * DataPtr;
	inline DataPtr &data_ptr() { return d; }
};

inline KvByteArray::KvByteArray() : d(Data::sharedNull()) {  }
inline KvByteArray::~KvByteArray() { if (!d->ref.deref()) Data::deallocate(d);; }
inline int KvByteArray::size() const
{
	return d->size;
}
inline bool KvByteArray::isEmpty() const
{
	return d->size == 0;
}
inline KvByteArray::KvByteArray(const KvByteArray &a) : d(a.d)
{
	d->ref.ref();
}

inline void KvByteArray::reserve(int asize)
{
	if (d->ref.isShared() || uint(asize) + 1u > d->alloc) {
		realloc(kvMax(uint(size()), uint(asize)) + 1u);
	}
	else 
	{
		// cannot set unconditionally, since d could be the shared_null or
		// otherwise static
		d->capacity = true;
	}
}

inline char KvByteArray::at(int i) const
{
	assert(uint(i) < uint(size())); return d->data()[i];
}
inline char KvByteArray::operator[](int i) const
{
	assert(uint(i) < uint(size())); return d->data()[i];
}
inline char KvByteArray::operator[](uint i) const
{
	assert(i < uint(size())); return d->data()[i];
}

inline KvByteArray &KvByteArray::operator+=(char c)
{
	return append(c);
}
inline KvByteArray &KvByteArray::operator+=(const char *s)
{
	return append(s);
}
inline KvByteArray &KvByteArray::operator+=(const KvByteArray &a)
{
	return append(a);
}

inline bool operator==(const KvByteArray &a1, const KvByteArray &a2) 
{ return (a1.size() == a2.size()) && (memcmp(a1.constData(), a2.constData(), a1.size()) == 0); }
inline bool operator==(const KvByteArray &a1, const char *a2) 
{ return a2 ? kvstrcmp(a1, a2) == 0 : a1.isEmpty(); }
inline bool operator==(const char *a1, const KvByteArray &a2) 
{
	return a1 ? kvstrcmp(a1, a2) == 0 : a2.isEmpty();
}
inline bool operator!=(const KvByteArray &a1, const KvByteArray &a2) 
{ return !(a1 == a2); }
inline bool operator!=(const KvByteArray &a1, const char *a2) 
{
	return a2 ? kvstrcmp(a1, a2) != 0 : !a1.isEmpty();
}
inline bool operator!=(const char *a1, const KvByteArray &a2) 
{
	return a1 ? kvstrcmp(a1, a2) != 0 : !a2.isEmpty();
}

#endif // KvByteArray_h__

