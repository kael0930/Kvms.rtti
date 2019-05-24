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

#define EXPAND(...) __VA_ARGS__
#define KV_HAS_COMMA_(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,N,...) N
#define KV_HAS_COMMA(...) EXPAND(KV_HAS_COMMA_(__VA_ARGS__,1,1,1,1,1,1,1,1,1,0))
#define KV0_ISEMPTY_(_1,_2,_3,_4,_5) _1 ## _2 ## _3 ## _4 ## _5
#define KV0_ISEMPTY(_1,_2,_3,_4,_5) KV0_ISEMPTY_(_1,_2,_3,_4,_5)
#define KV_SIZE_OF_CONCAT_(_1,_2) _1 ## _2
#define KV_SIZE_OF_CONCAT(_1,_2) KV_SIZE_OF_CONCAT_(_1,_2)
#define KV0_IS__EQ__(...) ,
#define KV_SIZE_OF_CASE0000 0
#define KV_SIZE_OF_CASE0001 1
#define KV_SIZE_OF_CASE0010 0
#define KV_SIZE_OF_CASE0011 0
#define KV_SIZE_OF_CASE0100 0
#define KV_SIZE_OF_CASE0101 0
#define KV_SIZE_OF_CASE0110 0
#define KV_SIZE_OF_CASE0111 0
#define KV_SIZE_OF_CASE1000 0
#define KV_SIZE_OF_CASE1001 0
#define KV_SIZE_OF_CASE1010 0
#define KV_SIZE_OF_CASE1011 0
#define KV_SIZE_OF_CASE1100 0
#define KV_SIZE_OF_CASE1101 0
#define KV_SIZE_OF_CASE1110 0
#define KV_SIZE_OF_CASE1111 0
#define KV_IS_EMPTY(...)\
	EXPAND(KV0_ISEMPTY(\
	KV_SIZE_OF_CASE, \
	KV_HAS_COMMA(__VA_ARGS__), \
	KV_HAS_COMMA(KV0_IS__EQ__ __VA_ARGS__), \
	KV_HAS_COMMA(__VA_ARGS__()), \
	KV_HAS_COMMA(KV0_IS__EQ__ __VA_ARGS__())\
	))
#define KV_SIZE_OF_IS_EMPTY_1(...) 0
#define KV_SIZE_OF_IS_EMPTY_0_(\
	_1, _2, _3, _4, _5, _6, _7, _8, _9, \
	_10, _11, _12, _13, _14, _15, _16, _17, _18, _19, \
	_20, _21, _22, _23, _24, _25, _26, _27, _28, _29, \
	_30, _31, _32, _33, _34, _35, _36, _37, _38, _39, \
	_40, _41, _42, _43, _44, _45, _46, _47, _48, _49, \
	_50, _51, _52, _53, _54, _55, _56, _57, _58, _59, \
	_60, _61, _62, _63, _64, N, ...) N
#define KV_SIZE_OF_IS_EMPTY_0(...) \
	EXPAND(KV_SIZE_OF_IS_EMPTY_0_(##__VA_ARGS__, \
	64, 63, 62, 61, \
	60, 59, 58, 57, 56, 55, 54, 53, 52, 51, \
	50, 49, 48, 47, 46, 45, 44, 43, 42, 41, \
	40, 39, 38, 37, 36, 35, 34, 33, 32, 31, \
	30, 29, 28, 27, 26, 25, 24, 23, 22, 21, \
	20, 19, 18, 17, 16, 15, 14, 13, 12, 11, \
	10, 9, 8, 7, 6, 5, 4, 3, 2, 1))
#define KV_SIZE_COUNT(...) \
	KV_SIZE_OF_CONCAT(KV_SIZE_OF_IS_EMPTY_, KV_IS_EMPTY(__VA_ARGS__))(__VA_ARGS__)

#define P_1(_1, ...) _1
#define P_2(_1, _2, ...) _2
#define P_3(_1, _2, _3, ...) _3 
#define P_4(_1, _2, _3, _4, ...) _4
#define P_5(_1, _2, _3, _4, _5, ...) _5
#define P_6(_1, _2, _3, _4, _5, _6, ...) _6
#define P_7(_1, _2, _3, _4, _5, _6, _7, ...) _7
#define P_8(_1, _2, _3, _4, _5, _6, _7, _8, ...) _8
#define P_9(_1, _2, _3, _4, _5, _6, _7, _8, _9, ...) _9 

#define KV_CONCAT_(l, r) l ## r
#define KV_CONCAT(l, r) KV_CONCAT_(l, r)

#if !defined(_MSC_VER)
#define KV_DEFARG0(s,f,a,o)
#define KV_DEFARG1(s,f,a,v,...) f(a,v) 
#define KV_DEFARG2(s,f,a,v,...) KV_DEFARG1(s,f,a,v) s KV_DEFARG1(s,f,a,__VA_ARGS__)
#define KV_DEFARG3(s,f,a,v,...) KV_DEFARG1(s,f,a,v) s KV_DEFARG2(s,f,a,__VA_ARGS__)
#define KV_DEFARG4(s,f,a,v,...) KV_DEFARG1(s,f,a,v) s KV_DEFARG3(s,f,a,__VA_ARGS__)
#define KV_DEFARG5(s,f,a,v,...) KV_DEFARG1(s,f,a,v) s KV_DEFARG4(s,f,a,__VA_ARGS__)
#define KV_DEFARG6(s,f,a,v,...) KV_DEFARG1(s,f,a,v) s KV_DEFARG5(s,f,a,__VA_ARGS__)
#define KV_DEFARG7(s,f,a,v,...) KV_DEFARG1(s,f,a,v) s KV_DEFARG6(s,f,a,__VA_ARGS__)
#define KV_DEFARG8(s,f,a,v,...) KV_DEFARG1(s,f,a,v) s KV_DEFARG7(s,f,a,__VA_ARGS__)
#define KV_DEFARG9(s,f,a,v,...) KV_DEFARG1(s,f,a,v) s KV_DEFARG8(s,f,a,__VA_ARGS__)
#else
#define KV_DEFARG0(s,f,a,o)
#define KV_DEFARG1(s,f,a,v,...) f(EXPAND(P_1(v)) ,a ,1)
#define KV_DEFARG2(s,f,a,v,...) KV_DEFARG1(s,f,a,v) s f(EXPAND(P_2(v)) ,a ,2)
#define KV_DEFARG3(s,f,a,v,...) KV_DEFARG2(s,f,a,v) s f(EXPAND(P_3(v)) ,a ,3)
#define KV_DEFARG4(s,f,a,v,...) KV_DEFARG3(s,f,a,v) s f(EXPAND(P_4(v)) ,a ,4)
#define KV_DEFARG5(s,f,a,v,...) KV_DEFARG4(s,f,a,v) s f(EXPAND(P_5(v)) ,a ,5)
#define KV_DEFARG6(s,f,a,v,...) KV_DEFARG5(s,f,a,v) s f(EXPAND(P_6(v)) ,a ,6)
#define KV_DEFARG7(s,f,a,v,...) KV_DEFARG6(s,f,a,v) s f(EXPAND(P_7(v)) ,a ,7)
#define KV_DEFARG8(s,f,a,v,...) KV_DEFARG7(s,f,a,v) s f(EXPAND(P_8(v)) ,a ,8)
#define KV_DEFARG9(s,f,a,v,...) KV_DEFARG8(s,f,a,v) s f(EXPAND(P_9(v)) ,a ,9)
#endif

#define KV_FOREACH_(sepatator,fun,funarg,...) \
	KV_CONCAT(KV_DEFARG, KV_SIZE_COUNT(__VA_ARGS__))(sepatator, fun, funarg, __VA_ARGS__)
#define KV_FOREACH(sepatator,fun,funarg,...) \
	KV_FOREACH_(sepatator, fun, funarg, __VA_ARGS__)

#define KV_BEGINE_MOC_NAMESPACE(Class)	\
struct kv_meta_stringdata_##Class{ \
	KvByteArray data[8];				\
	char stringdata[35];		\
};	\

#define KV_END_MOC_NAMESPACE()  ;

//static const uint kv_meta_data_##CLASS[8]; \

#define KV_BEGINE_REGISTER_CLASS(Class, Super) \
	typedef Class ThisClass; \
	typedef Super BaseClass; \
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


#define KV_BEGINE_REGISTER_PROPERTY() \
	int ThisClass::kv_metacall(KvMetaObject::Call _c, int _id, void **_a)	\
{	\
	_id = BaseClass::kv_metacall(_c, _id, _a); \
	if (_id < 0)\
		return _id; \
	if (_c == KvMetaObject::InvokeMetaMethod) {\
	if (_id < 1)\
		kv_static_metacall(this, _c, _id, _a); \
		_id -= 1; \
	}\
	else if (_c == KvMetaObject::ReadProperty) {\
		void *_v = _a[0]; \
		switch (_id) {\
		}\
		_id -= 1; \
		}\
	else if (_c == KvMetaObject::WriteProperty) {\
		void *_v = _a[0]; \
		switch (_id) {\
		}\
		_id -= 1; \
	}

#define KV_REGISTER_READ_PROPERTY(INDEX, FUNC,TYPE) \
			case INDEX: *reinterpret_cast<TYPE*>(_v) = FUNC(); break; \

#define KV_REGISTER_WRITE_PROPERTY(INDEX, FUNC,TYPE) \
			case INDEX: FUNC(*reinterpret_cast<TYPE*>(_v)); break; \

#define KV_END_REGISTER_PROPERTY() \
	return _id; \
}\


#define KV_REGISTER_PROPERTY(Class, Super) \

#define kv_comma ,
#define kv_argment a
#define kv_vargment _a
#define kv_def_field_(t,a,v) t a##v
#define kv_def_field(t,a,v) kv_def_field_(t,a, v)

#define kv_def_s_elem_(a,v)  const_cast<void*>(reinterpret_cast<const void*>(&a##v))
#define kv_def_s_elem(t,a,v) kv_def_s_elem_(a,v)

#define kv_def_v_elem_(t,a,v)  (*reinterpret_cast<t(*)>(a[v]))
#define kv_def_v_elem(t,a,v) kv_def_v_elem_(t,a,v)

#define KV_BEGINE_REGISTER_META() \
	void ThisClass::kv_static_metacall(KvObject *_o, KvMetaObject::Call _c, int _id, void **kv_vargment)\
	{\
		static int idx = 0; \
		if (_c == KvMetaObject::InvokeMetaMethod) {\
				assert(staticMetaObject.cast(_o)); \
				ThisClass *_t = static_cast<ThisClass *>(_o); \
			switch (_id) {\

#define KV_META_REGISTER_METHOD() 

#define KV_REGISTER_SIGNAL_SLOT(INDEX, FUNC,...) \
			case INDEX: _t->FUNC(KV_FOREACH(kv_comma, kv_def_v_elem, kv_vargment, __VA_ARGS__)); break;
	
#define KV_END_REGISTER_META() \
			}\
		}\
	}\

#define KV_REGISTER_SIGNAL(FUNC,...) \
	void ThisClass::FUNC(KV_FOREACH(kv_comma, kv_def_field, kv_argment, __VA_ARGS__))	\
	{\
		int i = KV_SIZE_COUNT(__VA_ARGS__); \
		void *kv_vargment[] = { 0, KV_FOREACH(kv_comma, kv_def_s_elem, kv_argment, __VA_ARGS__) }; \
		KvMetaObject::activate(this, &staticMetaObject, 0, kv_vargment); \
	}\

#endif // KvObjectdefs_h__
