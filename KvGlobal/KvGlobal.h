#ifndef KvGlobal_h__
#define KvGlobal_h__

#ifdef __cplusplus
#  include <type_traits>
#  include <cstddef>
#  include <utility>
#endif
#ifndef __ASSEMBLER__
#  include <assert.h>
#  include <stddef.h>
#endif

#include "KvConfig.h"
#include "KvTypeInfo.h"
#include "KvNameSpace.h"
#include "KvCompilerDetaction.h"

#if !defined(KV_CC_WARNINGS)
#  define KV_NO_WARNINGS
#endif
#if defined(KV_NO_WARNINGS)
#  if defined(KV_CC_MSVC)
KV_WARNING_DISABLE_MSVC(4251) /* class 'A' needs to have dll interface for to be used by clients of class 'B'. */
KV_WARNING_DISABLE_MSVC(4244) /* 'conversion' conversion from 'type1' to 'type2', possible loss of data */
KV_WARNING_DISABLE_MSVC(4275) /* non - DLL-interface classkey 'identifier' used as base for DLL-interface classkey 'identifier' */
KV_WARNING_DISABLE_MSVC(4514) /* unreferenced inline/local function has been removed */
KV_WARNING_DISABLE_MSVC(4800) /* 'type' : forcing value to bool 'true' or 'false' (performance warning) */
KV_WARNING_DISABLE_MSVC(4097) /* typedef-name 'identifier1' used as synonym for class-name 'identifier2' */
KV_WARNING_DISABLE_MSVC(4706) /* assignment within conditional expression */
KV_WARNING_DISABLE_MSVC(4786) /* truncating debug info after 255 characters */
KV_WARNING_DISABLE_MSVC(4660) /* template-class specialization 'identifier' is already instantiated */
KV_WARNING_DISABLE_MSVC(4355) /* 'this' : used in base member initializer list */
KV_WARNING_DISABLE_MSVC(4231) /* nonstandard extension used : 'extern' before template explicit instantiation */
KV_WARNING_DISABLE_MSVC(4710) /* function not inlined */
KV_WARNING_DISABLE_MSVC(4530) /* C++ exception handler used, but unwind semantics are not enabled. Specify -GX */
#  elif defined(KV_CC_BOR)
#    pragma option -w-inl
#    pragma option -w-aus
#    pragma warn -inl
#    pragma warn -pia
#    pragma warn -ccc
#    pragma warn -rch
#    pragma warn -sig
#  endif
#endif

#if defined(KVGLOBAL_EXPORTS)
#  define KV_GLOBAL_EXPORT K_DECL_EXPORT
#else
#  define KV_GLOBAL_EXPORT K_DECL_IMPORT
#endif

typedef signed char kint8;         /* 8 bit signed */
typedef unsigned char kuint8;      /* 8 bit unsigned */
typedef short kint16;              /* 16 bit signed */
typedef unsigned short kuint16;    /* 16 bit unsigned */
typedef int kint32;                /* 32 bit signed */
typedef unsigned int kuint32;      /* 32 bit unsigned */
#if defined(KV_OS_WIN) && !defined(KV_CC_GNU) && !defined(KV_CC_MWERKS)
#  define K_INT64_C(c) c ## i64    /* signed 64 bit constant */
#  define K_UINT64_C(c) c ## ui64   /* unsigned 64 bit constant */
typedef __int64 kint64;            /* 64 bit signed */
typedef unsigned __int64 kuint64;  /* 64 bit unsigned */
#else
#  define K_INT64_C(c) static_cast<long long>(c ## LL)     /* signed 64 bit constant */
#  define K_UINT64_C(c) static_cast<unsigned long long>(c ## ULL) /* unsigned 64 bit constant */
typedef long long kint64;           /* 64 bit signed */
typedef unsigned long long kuint64; /* 64 bit unsigned */
#endif

typedef kint64 klonglong;
typedef kuint64 kulonglong;

#if defined(KV_NO_FPU) || defined(KV_ARCH_ARM) || defined(KV_ARCH_WINDOWSCE) || defined(KV_ARCH_SYMBIAN)
typedef float kreal;
#else
typedef double kreal;
#endif

/*
kuintptr and kptrdiff is guaranteed to be the same size as a pointer, i.e.

sizeof(void *) == sizeof(kvuintptr)
&& sizeof(void *) == sizeof(kvptrdiff)
*/
template <int> struct KvIntegerForSize;
template <>    struct KvIntegerForSize<1> { typedef kuint8  Unsigned; typedef kint8  Signed; };
template <>    struct KvIntegerForSize<2> { typedef kuint16 Unsigned; typedef kint16 Signed; };
template <>    struct KvIntegerForSize<4> { typedef kuint32 Unsigned; typedef kint32 Signed; };
template <>    struct KvIntegerForSize<8> { typedef kuint64 Unsigned; typedef kint64 Signed; };
template <class T> struct KvIntegerForSizeof : KvIntegerForSize<sizeof(T)> { };
typedef KvIntegerForSizeof<void*>::Unsigned kuintptr;
typedef KvIntegerForSizeof<void*>::Signed kptrdiff;

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

# if defined(KV_NO_KEYWORDS)
#  define KV_NO_EMIT
# else
#   define slots
#   define signals	protected
# endif
# define K_SLOTS
# define K_SIGNALS public
# define K_PROPERTY public
# define K_PRIVATE_SLOT(d, signature)
# define K_EMI
#ifndef KV_NO_EMIT
# define emit
#endif

# define KV_PREPEND_NAMESPACE(name) ::name
# define KV_USE_NAMESPACE
# define KV_BEGIN_NAMESPACE
# define KV_END_NAMESPACE
# define KV_BEGIN_INCLUDE_NAMESPACE
# define KV_END_INCLUDE_NAMESPACE
# define KV_BEGIN_MOC_NAMESPACE
# define KV_END_MOC_NAMESPACE
# define KV_FORWARD_DECLARE_CLASS(name) class name;
# define KV_FORWARD_DECLARE_STRUCT(name) struct name;
# define KV_MANGLE_NAMESPACE(name) name

#define NS(x) KV_PREPEND_NAMESPACE(x)

#if defined(KV_OS_WIN)
#define KV_INLINE		__forceinline
#define KV_FASTCALL		__fastcall
#define KV_RESTRICT		__restrict
#elif defined(KV_OS_ANDROID)
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

#define KV_FOREVER for(;;)
#ifndef KV_NO_KEYWORDS
#  ifndef kv_foreach
#    define kv_foreach //KV_FOREACH
#  endif
#  ifndef kv_forever
#    define kv_forever KV_FOREVER
#  endif
#endif

#  define KV_UNUSED(x) (void)x;

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

KV_GLOBAL_EXPORT void kDebug(const char *, ...) /* print debug message */
#if defined(__GUN__) && !defined(__INSURE__)
__attribute__((format(printf, 1, 2)))
#endif
;

KV_GLOBAL_EXPORT void kWarning(const char *, ...) /* print warning message */
#if defined(__GUN__) && !defined(__INSURE__)
__attribute__((format(printf, 1, 2)))
#endif
;

KV_GLOBAL_EXPORT void kCritical(const char *, ...) /* print critical message */
#if defined(__GUN__) && !defined(__INSURE__)
__attribute__((format(printf, 1, 2)))
#endif
;
KV_GLOBAL_EXPORT void kFatal(const char *, ...) /* print fatal message and exit */
#if defined(__GUN__) && !defined(__INSURE__)
__attribute__((format(printf, 1, 2)))
#endif
;

inline void kv_noop(void) {}

KV_GLOBAL_EXPORT void kv_assert(const char *assertion, const char *file, int line);

#if !defined(KV_ASSERT)
#  ifndef KV_NO_DEBUG
#    define KV_ASSERT(cond) ((!(cond)) ? kv_assert(#cond,__FILE__,__LINE__) : kv_noop())
#  else
#    define KV_ASSERT(cond) kv_noop()
#  endif
#endif

KV_GLOBAL_EXPORT void kv_assert_x(const char *where, const char *what, const char *file, int line);

#if !defined(KV_ASSERT_X)
#  ifndef KV_NO_DEBUG
#    define KV_ASSERT_X(cond, where, what) ((!(cond)) ? kv_assert_x(where, what,__FILE__,__LINE__) : kv_noop())
#  else
#    define KV_ASSERT_X(cond, where, what) kv_noop()
#  endif
#endif

#if defined(__cplusplus) && defined(KV_COMPILER_STATIC_ASSERT)
#  define KV_STATIC_ASSERT(Condition) static_assert(bool(Condition), #Condition)
#  define KV_STATIC_ASSERT_X(Condition, Message) static_assert(bool(Condition), Message)
#elif defined(KV_COMPILER_STATIC_ASSERT)
// C11 mode - using the _S version in case <assert.h> doesn't do the right thing
#  define KV_STATIC_ASSERT(Condition) _Static_assert(!!(Condition), #Condition)
#  define KV_STATIC_ASSERT_X(Condition, Message) _Static_assert(!!(Condition), Message)
#else
// C89 & C99 version
#  define KV_STATIC_ASSERT_PRIVATE_JOIN(A, B) KV_STATIC_ASSERT_PRIVATE_JOIN_IMPL(A, B)
#  define KV_STATIC_ASSERT_PRIVATE_JOIN_IMPL(A, B) A ## B
#  ifdef __COUNTER__
#  define KV_STATIC_ASSERT(Condition) \
	typedef char KV_STATIC_ASSERT_PRIVATE_JOIN(kv_static_assert_result, __COUNTER__)[(Condition) ? 1 : -1];
#  else
#  define KV_STATIC_ASSERT(Condition) \
	typedef char KV_STATIC_ASSERT_PRIVATE_JOIN(kv_static_assert_result, __LINE__)[(Condition) ? 1 : -1];
#  endif /* __COUNTER__ */
#  define KV_STATIC_ASSERT_X(Condition, Message) KV_STATIC_ASSERT(Condition)
#endif

KV_GLOBAL_EXPORT void kv_check_pointer(const char *, int);
KV_GLOBAL_EXPORT void kvBadAlloc();

#ifdef KV_NO_EXCEPTIONS
#  if defined(KV_NO_DEBUG)
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

#endif // KvGlobal_h__
