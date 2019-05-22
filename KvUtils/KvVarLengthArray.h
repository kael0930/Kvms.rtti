#ifndef KvVarLengthArray_h__
#define KvVarLengthArray_h__

#include "KvGlobal.h"

template<class T, int Prealloc>
class KPodList;

template<class T, int Prealloc = 256>
class KvVarLengthArray
{
public:
	inline explicit KvVarLengthArray(int size = 0);

	inline KvVarLengthArray(const KvVarLengthArray<T, Prealloc> &other)
		: a(Prealloc), s(0), ptr(reinterpret_cast<T *>(array))
	{
		append(other.constData(), other.size());
	}

	inline ~KvVarLengthArray() {
		//if (QTypeInfo<T>::isComplex) 
		//{
		//	T *i = ptr + s;
		//	while (i-- != ptr)
		//		i->~T();
		//}
		if (ptr != reinterpret_cast<T *>(array))
			kvFree(ptr);
	}
	inline KvVarLengthArray<T, Prealloc> &operator=(const KvVarLengthArray<T, Prealloc> &other)
	{
		if (this != &other)
		{
			clear();
			append(other.constData(), other.size());
		}
		return *this;
	}

	inline void removeLast()
	{
		assert(s > 0);
		realloc(s - 1, a);
	}

	inline int size() const { return s; }
	inline int count() const { return s; }
	inline bool isEmpty() const { return (s == 0); }
	inline void resize(int size);
	inline void clear() { resize(0); }

	inline int capacity() const { return a; }
	inline void reserve(int size);

	inline T &operator[](int idx) {
		assert(idx >= 0 && idx < s);
		return ptr[idx];
	}
	inline const T &operator[](int idx) const {
		assert(idx >= 0 && idx < s);
		return ptr[idx];
	}
	inline const T &at(int idx) const { return operator[](idx); }

	T value(int i) const;
	T value(int i, const T &defaultValue) const;

	inline void append(const T &t)
	{
		if (s == a)   // i.e. s != 0
			realloc(s, s << 1);
		const int idx = s++;
		//if (QTypeInfo<T>::isComplex) 
		//{
		//	new (ptr + idx) T(t);
		//}
		//else 
		{
			ptr[idx] = t;
		}
	}

	void append(const T *buf, int size);
	inline KvVarLengthArray<T, Prealloc> &operator<<(const T &t)
	{
		append(t); return *this;
	}
	inline KvVarLengthArray<T, Prealloc> &operator+=(const T &t)
	{
		append(t); return *this;
	}

	inline T *data() { return ptr; }
	inline const T *data() const { return ptr; }
	inline const T * constData() const { return ptr; }

private:
	friend class KPodList<T, Prealloc>;
	void realloc(int size, int allloc);

	int a;      // capacity
	int s;      // size
	T *ptr;     // data
	union {
		// ### next: Use 'Prealloc * sizeof(T)' as array size
		char array[sizeof(qint64)* (((Prealloc * sizeof(T)) / sizeof(qint64)) + 1)];
	};
};

template<class T, int Prealloc>
inline KvVarLengthArray<T, Prealloc>::KvVarLengthArray(int size /*= 0*/)
:s(size)
{
	if (s > Prealloc) {
		ptr = reinterpret_cast<T *>(kvMalloc(s * sizeof(T)));
		a = s;
	}
	else {
		ptr = reinterpret_cast<T *>(array);
		a = Prealloc;
	}
	//if (QTypeInfo<T>::isComplex) {
	//	T *i = ptr + s;
	//	while (i != ptr)
	//		new (--i) T;
	//}
}


template<class T, int Prealloc>
void KvVarLengthArray<T, Prealloc>::append(const T *buf, int size)
{
	assert(buf);
	if (size <= 0)
		return;

	const int asize = s + size;

	if (asize >= a)
		realloc(s, kvMax(s * 2, asize));

	//if (QTypeInfo<T>::isComplex) 
	//{
	//	// call constructor for new objects (which can throw)
	//	while (s < asize)
	//		new (ptr + (s++)) T(*abuf++);
	//}
	//else 
	{
		kvMemCopy(&ptr[s], buf, size * sizeof(T));
		s = asize;
	}
}

template<class T, int Prealloc>
inline void KvVarLengthArray<T, Prealloc>::realloc(int asize, int aalloc)
{
	assert(aalloc >= asize);
	T *oldPtr = ptr;
	int osize = s;

	const int copySize = kvMin(asize, osize);
	if (aalloc != a) {
		ptr = reinterpret_cast<T *>(kvMalloc(aalloc * sizeof(T)));
		KV_CHECK_PTR(ptr);
		if (ptr) 
		{
			s = 0;
			a = aalloc;

			//if (QTypeInfo<T>::isStatic)
			//{
			//	QT_TRY{
			//		// copy all the old elements
			//		while (s < copySize) {
			//			new (ptr + s) T(*(oldPtr + s));
			//			(oldPtr + s)->~T();
			//			s++;
			//		}
			//	} QT_CATCH(...) {
			//		// clean up all the old objects and then free the old ptr
			//		int sClean = s;
			//		while (sClean < osize)
			//			(oldPtr + (sClean++))->~T();
			//		if (oldPtr != reinterpret_cast<T *>(array) && oldPtr != ptr)
			//			qFree(oldPtr);
			//		QT_RETHROW;
			//	}
			//}
			//else 
			{
				kvMemCopy(ptr, oldPtr, copySize * sizeof(T));
			}
		}
		else {
			ptr = oldPtr;
			return;
		}
	}
	s = copySize;

	//if (QTypeInfo<T>::isComplex) 
	//{
	//	// destroy remaining old objects
	//	while (osize > asize)
	//		(oldPtr + (--osize))->~T();
	//}

	if (oldPtr != reinterpret_cast<T *>(array) && oldPtr != ptr)
		kvFree(oldPtr);

	//if (QTypeInfo<T>::isComplex) 
	//{
	//	// call default constructor for new objects (which can throw)
	//	while (s < asize)
	//		new (ptr + (s++)) T;
	//}
	//else 
	{
		s = asize;
	}
}

#endif // KvVarLengthArray_h__
