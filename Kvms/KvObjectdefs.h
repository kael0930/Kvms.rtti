#ifndef KvObjectdefs_h__
#define KvObjectdefs_h__

#include "Kvms.h"

#define  BOOST_PP_VARIADICS 1

#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/tuple.hpp>
#include <boost/preprocessor/list.hpp>
#include <boost/preprocessor/array.hpp>
#include <boost/preprocessor/variadic.hpp>
#include <boost/preprocessor/control.hpp>
#include <boost/preprocessor/facilities.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/punctuation.hpp>
#include <boost/preprocessor/arithmetic.hpp>

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
	}\
	KV_BEGINE_REGISTER_PROPERTY()\
	KV_BEGINE_REGISTER_META()\

#define KV_END_REGISTER_CLASS()	\



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
			\
			}\
			_id -= 1; \
		}\
		else if (_c == KvMetaObject::WriteProperty) {\
			void *_v = _a[0]; \
			switch (_id) {\
			\
			}\
			_id -= 1; \
		}\
		return _id; \
	}\

#define KV_REGISTER_READ_PROPERTY(INDEX, FUNC,TYPE) \
			case INDEX: *reinterpret_cast<TYPE*>(_v) = FUNC(); break; \

#define KV_REGISTER_WRITE_PROPERTY(INDEX, FUNC,TYPE) \
			case INDEX: FUNC(*reinterpret_cast<TYPE*>(_v)); break; \

#define KV_REGISTER_PROPERTY(Class, Super) \

#define KV_END_REGISTER_PROPERTY()

#define kv_comma ,
#define kv_argment a
#define kv_vargment _a

#define kv_def_field_(t,a,v) t a##v
#define kv_def_elem_0(t,a,v) kv_def_field_(t,a,v)

#define kv_def_s_elem_(a,v)  const_cast<void*>(reinterpret_cast<const void*>(&a##v))
#define kv_def_elem_1(t,a,v) kv_def_s_elem_(a,v)

#define kv_def_v_elem_(t,a,v)  (*reinterpret_cast<t(*)>(a[v]))
#define kv_def_elem_2(t,a,v) kv_def_v_elem_(t,a,v)

#define kv_signal_fun_elem_0(r, funarg, index, element) \
	BOOST_PP_IF(BOOST_PP_EQUAL(0,index), BOOST_PP_EMPTY,BOOST_PP_COMMA)()\
	kv_def_elem_0(element, funarg,BOOST_PP_ADD(index,1))\

#define kv_signal_fun_elem_1(r, funarg, index, element) \
	BOOST_PP_IF(BOOST_PP_EQUAL(0,index), BOOST_PP_EMPTY,BOOST_PP_COMMA)()\
	kv_def_elem_1(element, funarg, BOOST_PP_ADD(index,1))

#define kv_signal_fun_elem_2(r, funarg, index, element) \
	BOOST_PP_IF(BOOST_PP_EQUAL(0,index), BOOST_PP_EMPTY,BOOST_PP_COMMA)()\
	kv_def_elem_2(element, funarg, BOOST_PP_ADD(index,1))

#define Kv_FOR_EACH_ELEM(method,...)\
	BOOST_PP_SEQ_FOR_EACH_I(BOOST_PP_CAT(kv_signal_fun_elem_, method)\
	,kv_vargment,BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)) 

# define KV_META_METHOD_SEQ \
	(1)(2)(3)(4)(5)(6)(7)(8)(9) \
	(10)(11)(12)(13)(14)(15)(16)(17)(18)(19) \
	(20)(21)(22)(23)(24)(25)(26)(27)(28)(29) \
	(30)(31)(32)(33)(34)(35)(36)(37)(38)(39) \
	(40)(41)(42)(43)(44)(45)(46)(47)(48)(49) \
	(50)(51)(52)(53)(54)(55)(56)(57)(58)(59) \
	(60)(61)(62)(63)(64)(65)(66)(67)(68)(69) \
	(70)(71)(72)(73)(74)(75)(76)(77)(78)(79) \
	(80)(81)(82)(83)(84)(85)(86)(87)(88)(89) \
	(90)(91)(92)(93)(94)(95)(96)(97)(98)(99) \
	(100)

#define MACRO(r, data, elem) BOOST_PP_CAT(elem, data)

#define KV_BEGINE_REGISTER_META() \
	void ThisClass::kv_static_metacall(KvObject *_o, KvMetaObject::Call _c,\
										int _id, void **kv_vargment)\
	{\
		if (_c == KvMetaObject::InvokeMetaMethod) {\
				assert(staticMetaObject.cast(_o)); \
				ThisClass *_t = static_cast<ThisClass *>(_o); \
			switch (_id) {\
			\
			BOOST_PP_SEQ_FOR_EACH(MACRO,__X__,KV_META_METHOD_SEQ)\
			}\
		}\
	}\

#define KV_META_REGISTER_METHOD(Function,...) \
	BOOST_PP_SEQ_INSERT(KV_META_METHOD_SEQ,2,(Function(\
			BOOST_PP_IF(BOOST_PP_IS_EMPTY(__VA_ARGS__)\
			,BOOST_PP_EMPTY\
			,Kv_FOR_EACH_ELEM)(2,__VA_ARGS__)))\
	)\

#define KV_REGISTER_SIGNAL_SLOT(Function,...) \
		KV_META_REGISTER_METHOD(Function,__VA_ARGS__)

#define KV_END_REGISTER_META()

#define KV_REGISTER_SIGNAL(Function,...)\
	void ThisClass::Function(\
			BOOST_PP_IF(BOOST_PP_IS_EMPTY(__VA_ARGS__)\
			,BOOST_PP_EMPTY\
			,Kv_FOR_EACH_ELEM)(0,__VA_ARGS__))\
	{\
		void *kv_vargment[] = { 0,\
			BOOST_PP_IF(BOOST_PP_IS_EMPTY(__VA_ARGS__)\
				,BOOST_PP_EMPTY\
				,Kv_FOR_EACH_ELEM)(1,__VA_ARGS__)\
		 };\
		KvMetaObject::activate(this, &staticMetaObject, 0, kv_vargment);\
	}\

#endif // KvObjectdefs_h__
