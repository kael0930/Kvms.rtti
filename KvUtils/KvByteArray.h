#ifndef KvByteArray_h__
#define KvByteArray_h__

#include "KvUtils.h"
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

class KV_UTILS_EXPORT KvByteArray
{
	struct  Data
	{
		std::atomic<int> ref;
		int alloc, size;
		char *data;
		char array[1];
	};

public:
	inline KvByteArray();
	KvByteArray(const char *s);
	KvByteArray(const char *s, int size);
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
	void reserve(int size) { if (d->ref != 1 || size > d->alloc) realloc(size); }
	void squeeze() { if (d->size < d->alloc) realloc(d->size); }

	operator const char *() const { return d->data; }
	operator const void *() const { return d->data; }

	char *data() { detach(); return d->data; }
	const char *data() const { return d->data; }
	inline const char *constData() const { return d->data; }
	inline void detach() { if (d->ref != 1 || d->data != d->array) realloc(d->size); }
	bool isDetached() const { return d->ref == 1; }
	inline bool isSharedWith(const KvByteArray &other) const { return d == other.d; }
	void clear();

	char at(int i) const;
	char operator[](int i) const;
	char operator[](uint i) const;

	inline int count() const { return d->size; }
	int length() const { return d->size; }
	bool isNull() const { return d == &shared_null; }

	KvByteArray &append(char c);
	KvByteArray &append(const char *s);
	KvByteArray &append(const char *s, int len);
	KvByteArray &append(const KvByteArray &a);

private:
	static Data shared_null;
	static Data shared_empty;
	Data *d;

	KvByteArray(Data *dd, int /*dummy*/, int /*dummy*/) : d(dd) {}
	void realloc(int alloc);
	void expand(int i);
	KvByteArray nulTerminated() const;

public:
	typedef Data * DataPtr;
	inline DataPtr &data_ptr() { return d; }
};

inline KvByteArray::KvByteArray() : d(&shared_null) { d->ref++; }
inline KvByteArray::~KvByteArray() { if (!--d->ref) kvFree(d); }
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
	d->ref++;
}

inline char KvByteArray::at(int i) const
{
	assert(uint(i) < uint(size())); return d->data[i];
}
inline char KvByteArray::operator[](int i) const
{
	assert(uint(i) < uint(size())); return d->data[i];
}
inline char KvByteArray::operator[](uint i) const
{
	assert(i < uint(size())); return d->data[i];
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
#endif // KvByteArray_h__

