#ifndef KvGlobal_h__
#define KvGlobal_h__

#include <time.h>
#include <math.h>
#include <float.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <setjmp.h>
#include <assert.h>
#include <stddef.h>

#include <windows.h>	//switch 语句包含“default”标签但未包含“case”标签;

#define K_DECL_EXPORT     __declspec(dllexport)
#define K_DECL_IMPORT     __declspec(dllimport)

#if defined(KVGLOBAL_EXPORTS)
#  define KV_GLOBAL_EXPORT K_DECL_EXPORT
#else
#  define KV_GLOBAL_EXPORT K_DECL_IMPORT
#endif

#include "KvNameSpace.h"

typedef signed char qint8;         /* 8 bit signed */
typedef unsigned char quint8;      /* 8 bit unsigned */
typedef short qint16;              /* 16 bit signed */
typedef unsigned short quint16;    /* 16 bit unsigned */
typedef int qint32;                /* 32 bit signed */
typedef unsigned int quint32;      /* 32 bit unsigned */
#if defined(K_OS_WIN) && !defined(K_CC_GNU) && !defined(K_CC_MWERKS)
#  define K_INT64_C(c) c ## i64    /* signed 64 bit constant */
#  define K_UINT64_C(c) c ## ui64   /* unsigned 64 bit constant */
typedef __int64 qint64;            /* 64 bit signed */
typedef unsigned __int64 quint64;  /* 64 bit unsigned */
#else
#  define K_INT64_C(c) static_cast<long long>(c ## LL)     /* signed 64 bit constant */
#  define K_UINT64_C(c) static_cast<unsigned long long>(c ## ULL) /* unsigned 64 bit constant */
typedef long long qint64;           /* 64 bit signed */
typedef unsigned long long quint64; /* 64 bit unsigned */
#endif

typedef qint64 qlonglong;
typedef quint64 qulonglong;

#if defined(Kv_NO_FPU) || defined(Kv_ARCH_ARM) || defined(Kv_ARCH_WINDOWSCE) || defined(Kv_ARCH_SYMBIAN)
typedef float qreal;
#else
typedef double qreal;
#endif

/*
quintptr and qptrdiff is guaranteed to be the same size as a pointer, i.e.

sizeof(void *) == sizeof(quintptr)
&& sizeof(void *) == sizeof(qptrdiff)
*/
template <int> struct KvIntegerForSize;
template <>    struct KvIntegerForSize<1> { typedef quint8  Unsigned; typedef qint8  Signed; };
template <>    struct KvIntegerForSize<2> { typedef quint16 Unsigned; typedef qint16 Signed; };
template <>    struct KvIntegerForSize<4> { typedef quint32 Unsigned; typedef qint32 Signed; };
template <>    struct KvIntegerForSize<8> { typedef quint64 Unsigned; typedef qint64 Signed; };
template <class T> struct KvIntegerForSizeof : KvIntegerForSize<sizeof(T)> { };
typedef KvIntegerForSizeof<void*>::Unsigned quintptr;
typedef KvIntegerForSizeof<void*>::Signed qptrdiff;

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

# if defined(Kv_NO_KEYWORDS)
#  define Kv_NO_EMIT
# else
#   define slots
#   define signals	protected
# endif
# define K_SLOTS
# define K_SIGNALS public
# define K_PROPERTY public
# define K_PRIVATE_SLOT(d, signature)
# define K_EMI
#ifndef Kv_NO_EMIT
# define emit
#endif

# define Kv_PREPEND_NAMESPACE(name) ::name
# define Kv_USE_NAMESPACE
# define Kv_BEGIN_NAMESPACE
# define Kv_END_NAMESPACE
# define Kv_BEGIN_INCLUDE_NAMESPACE
# define Kv_END_INCLUDE_NAMESPACE
# define Kv_BEGIN_MOC_NAMESPACE
# define Kv_END_MOC_NAMESPACE
# define Kv_FORWARD_DECLARE_CLASS(name) class name;
# define Kv_FORWARD_DECLARE_STRUCT(name) struct name;
# define Kv_MANGLE_NAMESPACE(name) name

#define NS(x) Kv_PREPEND_NAMESPACE(x)

#ifdef _WIN32
#define KV_INLINE		__forceinline
#define KV_FASTCALL		__fastcall
#define KV_RESTRICT		__restrict
#elif _ANDROID
#define KV_INLINE		__inline__
#define KV_FASTCALL
#define KV_RESTRICT		__restrict
#else
#define KV_INLINE		__inline__ __attribute__ ((always_inline))
#define KV_FASTCALL
#define KV_RESTRICT		__restrict
#endif


#define K_DISABLE_COPY(Class) \
	Class(const Class &);	\
	Class& operator=(const Class &); 

//#define K_FOREVER for(;;)
//#ifndef KV_NO_KEYWORDS
//#  ifndef foreach
//#    define foreach K_FOREACH
//#  endif
//#  ifndef forever
//#    define forever K_FOREVER
//#  endif
//#endif

template <typename T>
static inline T *kGetPtrHelper(T *ptr) { return ptr; }
template <typename Wrapper>
static inline typename Wrapper::pointer kGetPtrHelper(const Wrapper &p) { return p.data(); }

#define K_DECLARE_PRIVATE(Class) \
	inline Class##Private * d_func() { return reinterpret_cast<Class##Private *>(kGetPtrHelper(d_ptr)); } \
	inline /*const*/ Class##Private * d_func() const { return reinterpret_cast</*const*/ Class##Private *>(kGetPtrHelper(d_ptr)); } \
	friend class Class##Private; 

#define K_DECLARE_PUBLIC(Class) \
	inline Class *q_func() { return static_cast<Class *>(q_ptr); } \
	inline /*const*/ Class *q_func() const { return static_cast</*const*/ Class *>(q_ptr); } \
	friend class Class; 

#define K_D(Class) Class##Private * const d = d_func();
#define K_Q(Class) Class * const q = q_func();


#ifdef KV_NO_EXCEPTIONS
#  define KV_TRY if (true)
#  define KV_CATCH(A) else
#  define KV_THROW(A) kv_noop()
#  define KV_RETHROW kv_noop()
#else
#  define KV_TRY try
#  define KV_CATCH(A) catch (A)
#  define KV_THROW(A) throw A
#  define KV_RETHROW throw
#endif

inline void kv_noop(void) {}

KV_GLOBAL_EXPORT void kv_check_pointer(const char *, int);
KV_GLOBAL_EXPORT void kvBadAlloc();

#ifdef KV_NO_EXCEPTIONS
#  if defined(Kv_NO_DEBUG)
#    define KV_CHECK_PTR(p) kv_noop()
#  else
#    define KV_CHECK_PTR(p) do {if(!(p))kv_check_pointer(__FILE__,__LINE__);} while (0)
#  endif
#else
#  define KV_CHECK_PTR(p) do { if (!(p)) kvBadAlloc(); } while (0)
#endif

template <typename T>
inline T *kv_check_ptr(T *p) { KV_CHECK_PTR(p); return p; }

template <typename T>
inline void kvSwap(T &value1, T &value2)
{
#ifdef Kv_NO_STL
	const T t = value1;
	value1 = value2;
	value2 = t;
#else
	using std::swap;
	swap(value1, value2);
#endif
}

KV_GLOBAL_EXPORT void *kvMalloc(size_t size);
KV_GLOBAL_EXPORT void kvFree(void *ptr);
KV_GLOBAL_EXPORT void *kvRealloc(void *ptr, size_t size);
KV_GLOBAL_EXPORT void *kvMallocAligned(size_t size, size_t alignment);
KV_GLOBAL_EXPORT void *kvReallocAligned(void *ptr, size_t size, size_t oldsize, size_t alignment);
KV_GLOBAL_EXPORT void kvFreeAligned(void *ptr);
KV_GLOBAL_EXPORT void *kvMemCopy(void *dest, const void *src, size_t n);
KV_GLOBAL_EXPORT void *kvMemSet(void *dest, int c, size_t n);

template <typename T>
 inline const T &kvMin(const T &a, const T &b) { return (a < b) ? a : b; }
template <typename T>
 inline const T &kvMax(const T &a, const T &b) { return (a < b) ? b : a; }
template <typename T>
 inline const T &kvBound(const T &min, const T &val, const T &max)
{
	return kvMax(min, kvMin(max, val));
}

 inline uint kstrlen(const char *str)
 {
	 return str ? uint(strlen(str)) : 0;
 }

 inline uint kstrnlen(const char *str, uint maxlen)
 {
	 uint length = 0;
	 if (str) {
		 while (length < maxlen && *str++)
			 length++;
	 }
	 return length;
 }

 inline int kstrcmp(const char *str1, const char *str2)
 {
	 return (str1 && str2) ? strcmp(str1, str2)
		 : (str1 ? 1 : (str2 ? -1 : 0));
 }

#endif // KvGlobal_h__
