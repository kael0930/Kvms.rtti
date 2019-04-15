#ifndef KvScopedPointer_h__
#define KvScopedPointer_h__

#include "KvGlobal.h"

template<typename T>
struct KvScopedPointerDeleter
{
	static inline void cleanup(T *p)
	{
		// Enforce a complete type.
		// If you get a compile error here, read the section on forward declared
		// classes in the QScopedPointer documentation.
		typedef char IsIncompleteType[sizeof(T) ? 1 : -1];
		(void) sizeof(IsIncompleteType);
		delete p;
	}
};

template< typename T, typename Cleanup = KvScopedPointerDeleter<T> >
class KV_CORE_EXPORT KvScopedPointer
{
public:
	explicit inline KvScopedPointer(T *p = 0)
		:d(p)
	{

	}

	inline ~KvScopedPointer()
	{
		 T *oldD = this->d;
		 Cleanup::cleanup(oldD);
		 this->d = 0;
	}
	
	inline T &operator*() const
	{
		return *d;
	}

	inline T *operator->() const
	{
		return d;
	}

	inline bool operator!() const
	{
		return !d;
	}

	inline T *data() const
	{
		return d;
	}

	inline bool isNull() const
	{
		return !d;
	}

	inline void reset(T *other = 0)
	{
		if (d == other)
		{
			return;
		}

		T *oldD = d;
		d = other;
		Cleanup::cleanup(oldD);
	}

	inline T *take()
	{
		T *oldD = d;
		d = 0;
		return oldD;
	}

	typedef T *pointer;

private:
	T *d;

private:
	K_DISABLE_COPY(KvScopedPointer)
};

#endif // KvScopedPointer_h__
