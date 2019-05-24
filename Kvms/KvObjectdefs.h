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

#define KV_BEGINE_MOC_NAMESPACE(Class)	\
struct kv_meta_stringdata_##Class{ \
	KvByteArray data[8];				\
	char stringdata[35];		\
};	\

#define KV_END_MOC_NAMESPACE()  ;

//static const uint kv_meta_data_##CLASS[8]; \

#define KV_BEGINE_REGISTER_CLASS(Class, Super) \
	const KvMetaObjectExtraData Class::staticMetaObjectExtraData = { \
		0, kv_static_metacall	\
};	\
	\
	const KvMetaObject Class::staticMetaObject = { \
		{ &Super::staticMetaObject, kv_meta_stringdata_##Class, \
		kv_meta_data_##Class, &staticMetaObjectExtraData } 	\
	};	\
	\
	const KvMetaObject *Class::metaObject() const		\
	{	\
		return KvObject::d_ptr->metaObject \
			? KvObject::d_ptr->metaObject : &staticMetaObject;	\
	} \
	\
	void *Class::kv_metacast(const char *_clname)	\
	{	\
		if (!_clname) return 0;	\
		if (!strcmp(_clname, kv_meta_stringdata_##Class))	\
			return static_cast<void*>(const_cast<Class*>(this));	\
		return 0;	\
	}	

#define KV_END_REGISTER_CLASS() 


#define KV_BEGINE_REGISTER_PROPERTY(Class, Super) \
	int Class::kv_metacall(KvMetaObject::Call _c, int _id, void **_a)	\
	{	\
		_id = Super::kv_metacall(_c, _id, _a); \
		if (_id < 0)\
			return _id;\
		if (_c == KvMetaObject::InvokeMetaMethod) {\
			if (_id < 1)\
				kv_static_metacall(this, _c, _id, _a);\
			_id -= 1;\
		}\
		else if (_c == KvMetaObject::ReadProperty) {\
			void *_v = _a[0];\
			switch (_id) {\
			case 0: *reinterpret_cast<bool*>(_v) = isEnabled(); break;\
			}\
			_id -= 1;\
		}\
		else if (_c == KvMetaObject::WriteProperty) {\
			void *_v = _a[0];\
			switch (_id) {\
			case 0: setEnabled(*reinterpret_cast<bool*>(_v)); break;\
			}\
			_id -= 1;\
		}\
		return _id;\
	}\

#define KV_END_REGISTER_PROPERTY() 


#define KV_REGISTER_PROPERTY(Class, Super) \


#define KV_BEGINE_REGISTER_METHOD(Class) \
	void Class::kv_static_metacall(KvObject *_o, KvMetaObject::Call _c, int _id, void **_a)\
	{\
		if (_c == KvMetaObject::InvokeMetaMethod) {\
			assert(staticMetaObject.cast(_o));\
			Class *_t = static_cast<Class *>(_o); \
			switch (_id) {\
			\
			default: break;\
			}\
		}\
	}\

#define KV_END_REGISTER_METHOD() 


#define P_ARG(	\
	_1, _2, _3, _4, _5, _6, _7, _8, _9, \
	_10, _11, _12, _13, _14, _15, _16, _17, _18, _19, \
	_20, _21, _22, _23, _24, _25, _26, _27, _28, _29, \
	_30, _31, _32, _33, _34, _35, _36, _37, _38, _39, \
	_40, _41, _42, _43, _44, _45, _46, _47, _48, _49, \
	_50, _51, _52, _53, _54, _55, _56, _57, _58, _59, \
	_60, _61, _62, _63, _64, N, ...) N

// if _MSC_VER need to add Expand
#define P_NARG_COUNT(...) \
	Expand(P_ARG(##__VA_ARGS__, \
	64, 63, 62, 61, 60, \
	59, 58, 57, 56, 55, 54, 53, 52, 51, 50, \
	49, 48, 47, 46, 45, 44, 43, 42, 41, 40, \
	39, 38, 37, 36, 35, 34, 33, 32, 31, 30, \
	29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
	19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
	9, 8, 7, 6, 5, 4, 3, 2, 1))\

#define P_1(_1, ...) _1
#define P_2(_1, _2, ...) _2
#define P_3(_1, _2, _3, ...) _3 
#define P_4(_1, _2, _3, _4, ...) _4
#define P_5(_1, _2, _3, _4, _5, ...) _5
#define P_6(_1, _2, _3, _4, _5, _6, ...) _6
#define P_7(_1, _2, _3, _4, _5, _6, _7, ...) _7
#define P_8(_1, _2, _3, _4, _5, _6, _7, _8, ...) _8
#define P_9(_1, _2, _3, _4, _5, _6, _7, _8, _9, ...) _9 

#define Expand(...) __VA_ARGS__
#define KV_CONCAT_(l, r) l ## r
#define KV_CONCAT(l, r) KV_CONCAT_(l, r)

#if !defined(_MSC_VER)
#define KV_DOARG0(s,f,a,o)
#define KV_DOARG1(s,f,a,v,...) f(a,v) 
#define KV_DOARG2(s,f,a,v,...) Expand(KV_DOARG1(s,f,a,v)) s KV_DOARG1(s,f,a,__VA_ARGS__)
#define KV_DOARG3(s,f,a,v,...) Expand(KV_DOARG1(s,f,a,v)) s KV_DOARG2(s,f,a,__VA_ARGS__)
#define KV_DOARG4(s,f,a,v,...) KV_DOARG1(s,f,a,v) s KV_DOARG3(s,f,a,__VA_ARGS__)
#define KV_DOARG5(s,f,a,v,...) KV_DOARG1(s,f,a,v) s KV_DOARG4(s,f,a,__VA_ARGS__)
#define KV_DOARG6(s,f,a,v,...) KV_DOARG1(s,f,a,v) s KV_DOARG5(s,f,a,__VA_ARGS__)
#define KV_DOARG7(s,f,a,v,...) KV_DOARG1(s,f,a,v) s KV_DOARG6(s,f,a,__VA_ARGS__)
#define KV_DOARG8(s,f,a,v,...) KV_DOARG1(s,f,a,v) s KV_DOARG7(s,f,a,__VA_ARGS__)
#define KV_DOARG9(s,f,a,v,...) KV_DOARG1(s,f,a,v) s KV_DOARG8(s,f,a,__VA_ARGS__)
#else
#define KV_DOARG0(s,f,a,o)
#define KV_DOARG1(s,f,a,v,...) f(Expand(P_1(v)) ,a ,a1)
#define KV_DOARG2(s,f,a,v,...) KV_DOARG1(s,f,a,v) s f(Expand(P_2(v)) ,a ,a2)
#define KV_DOARG3(s,f,a,v,...) KV_DOARG2(s,f,a,v) s f(Expand(P_3(v)) ,a ,a3)
#define KV_DOARG4(s,f,a,v,...) KV_DOARG3(s,f,a,v) s f(Expand(P_4(v)) ,a ,a4)
#define KV_DOARG5(s,f,a,v,...) KV_DOARG4(s,f,a,v) s f(Expand(P_5(v)) ,a ,a5)
#define KV_DOARG6(s,f,a,v,...) KV_DOARG5(s,f,a,v) s f(Expand(P_6(v)) ,a ,a6)
#define KV_DOARG7(s,f,a,v,...) KV_DOARG6(s,f,a,v) s f(Expand(P_7(v)) ,a ,a7)
#define KV_DOARG8(s,f,a,v,...) KV_DOARG7(s,f,a,v) s f(Expand(P_8(v)) ,a ,a8)
#define KV_DOARG9(s,f,a,v,...) KV_DOARG8(s,f,a,v) s f(Expand(P_9(v)) ,a ,a9)
#endif

#define KV_FOREACH_(sepatator,fun,funarg,...) \
	KV_CONCAT(KV_DOARG, P_NARG_COUNT(__VA_ARGS__))(sepatator, fun, funarg, __VA_ARGS__)
#define KV_FOREACH(sepatator,fun,funarg,...) \
	KV_FOREACH_(sepatator, fun, funarg, __VA_ARGS__)

#define kv_comma ,
#define kv_argment
#define kv_enum_field(t,v) t v
#define kv_def_field1(t,a,v) kv_enum_field(t,v)

#define kv_enum_elem(v)  const_cast<void*>(reinterpret_cast<const void*>(&v))
#define kv_def_field2(t,a,v) kv_enum_elem(v)

//////////////////////////////////////////////////////////////////////////
#define KV_REGISTER_SIGNAL(Class,FUNC,...) \
	void Class::FUNC(KV_FOREACH(kv_comma, kv_def_field1, kv_argment, __VA_ARGS__))	\
	{\
	void *_a[] = { 0, KV_FOREACH(kv_comma, kv_def_field2, kv_argment, __VA_ARGS__) }; \
		KvMetaObject::activate(this, &staticMetaObject, 0, _a); \
	}\

#define KV_BEGINE_REGISTER_SLOT(Class, Super) \


#endif // KvObjectdefs_h__
