#ifndef KvGlobal_h__
#define KvGlobal_h__

#include <stddef.h>
#include <windows.h>

#include "Kvms.h"
#include "KvNameSpace.h"

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

typedef signed char qint8;         /* 8 bit signed */
typedef unsigned char quint8;      /* 8 bit unsigned */
typedef short qint16;              /* 16 bit signed */
typedef unsigned short quint16;    /* 16 bit unsigned */
typedef int qint32;                /* 32 bit signed */
typedef unsigned int quint32;      /* 32 bit unsigned */
#if defined(Q_OS_WIN) && !defined(Q_CC_GNU) && !defined(Q_CC_MWERKS)
#  define Q_INT64_C(c) c ## i64    /* signed 64 bit constant */
#  define Q_UINT64_C(c) c ## ui64   /* unsigned 64 bit constant */
typedef __int64 qint64;            /* 64 bit signed */
typedef unsigned __int64 quint64;  /* 64 bit unsigned */
#else
#  define Q_INT64_C(c) static_cast<long long>(c ## LL)     /* signed 64 bit constant */
#  define Q_UINT64_C(c) static_cast<unsigned long long>(c ## ULL) /* unsigned 64 bit constant */
typedef long long qint64;           /* 64 bit signed */
typedef unsigned long long quint64; /* 64 bit unsigned */
#endif

typedef qint64 qlonglong;
typedef quint64 qulonglong;

#if defined(QT_NO_FPU) || defined(QT_ARCH_ARM) || defined(QT_ARCH_WINDOWSCE) || defined(QT_ARCH_SYMBIAN)
typedef float qreal;
#else
typedef double qreal;
#endif

# if defined(QT_NO_KEYWORDS)
#  define QT_NO_EMIT
# else
#   define slots
#   define signals protected
# endif
# define Q_SLOTS
# define Q_SIGNALS protected
# define Q_PRIVATE_SLOT(d, signature)
# define Q_EMIT
#ifndef QT_NO_EMIT
# define emit
#endif

# define QT_PREPEND_NAMESPACE(name) ::name
# define QT_USE_NAMESPACE
# define QT_BEGIN_NAMESPACE
# define QT_END_NAMESPACE
# define QT_BEGIN_INCLUDE_NAMESPACE
# define QT_END_INCLUDE_NAMESPACE
# define QT_BEGIN_MOC_NAMESPACE
# define QT_END_MOC_NAMESPACE
# define QT_FORWARD_DECLARE_CLASS(name) class name;
# define QT_FORWARD_DECLARE_STRUCT(name) struct name;
# define QT_MANGLE_NAMESPACE(name) name

#define NS(x) QT_PREPEND_NAMESPACE(x)

#define K_DISABLE_COPY(Class) \
	Class(const Class &);	\
	Class &operator=(const Class &); 

//#define K_FOREVER for(;;)
//#ifndef KV_NO_KEYWORDS
//#  ifndef foreach
//#    define foreach Q_FOREACH
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

#endif // KvGlobal_h__
