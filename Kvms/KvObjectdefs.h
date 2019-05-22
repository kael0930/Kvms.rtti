#ifndef KvObjectdefs_h__
#define KvObjectdefs_h__

#include "Kvms.h"

#define K_OBJECT_CHECK

#ifdef K_NO_DATA_RELOCATION
#define K_OBJECT_GETSTATICMETAOBJECT static const KvMetaObject &getStaticMetaObject();
#else
#define K_OBJECT_GETSTATICMETAOBJECT
#endif

#define K_OBJECT	\
public: \
	K_OBJECT_CHECK	\
	static const KvMetaObject staticMetaObject; \
	K_OBJECT_GETSTATICMETAOBJECT \
	virtual const KvMetaObject *metaObject() const; \
	virtual void *kv_metacast(const char *); \
	virtual int kv_metacall(KvMetaObject::Call, int, void **); \
private: \
	static const KvMetaObjectExtraData staticMetaObjectExtraData; \
	static void kv_static_metacall(KvObject *, KvMetaObject::Call, int, void **);

// macro for onaming members
#ifdef METHOD
#undef METHOD
#endif
#ifdef SLOT
#undef SLOT
#endif
#ifdef SIGNAL
#undef SIGNAL
#endif

KV_CORE_EXPORT const char *kvFlagLocation(const char *method);

#define QTOSTRING_HELPER(s) #s
#define QTOSTRING(s) QTOSTRING_HELPER(s)
#ifndef QT_NO_DEBUG
# define QLOCATION "\0" __FILE__ ":" QTOSTRING(__LINE__)
# ifndef QT_NO_KEYWORDS
#  define METHOD(a)   kvFlagLocation("0"#a QLOCATION)
# endif
# define SLOT(a)     kvFlagLocation("1"#a QLOCATION)
# define SIGNAL(a)   kvFlagLocation("2"#a QLOCATION)
#else
# ifndef QT_NO_KEYWORDS
#  define METHOD(a)   "0"#a
# endif
# define SLOT(a)     "1"#a
# define SIGNAL(a)   "2"#a
#endif

#define QMETHOD_CODE  0                        // member type codes
#define QSLOT_CODE    1
#define QSIGNAL_CODE  2


#endif // KvObjectdefs_h__
