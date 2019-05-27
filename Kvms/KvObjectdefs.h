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
#include <boost/preprocessor/comparison.hpp>

#define K_OBJECT_CHECK

#ifdef K_NO_DATA_RELOCATION
#define K_OBJECT_GETSTATICMETAOBJECT \
	static const KvMetaObject &getStaticMetaObject();
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

#define KVOSTRING_HELPER(s) #s
#define KVOSTRING(s) KVOSTRING_HELPER(s)
#ifndef KV_NO_DEBUG
# define KLOCATION "\0" __FILE__ ":" KVOSTRING(__LINE__)
# ifndef KV_NO_KEYWORDS
#  define METHOD(a)   kvFlagLocation("0"#a KLOCATION)
# endif
# define SLOT(a)     kvFlagLocation("1"#a KLOCATION)
# define SIGNAL(a)   kvFlagLocation("2"#a KLOCATION)
#else
# ifndef KV_NO_KEYWORDS
#  define METHOD(a)   "0"#a
# endif
# define SLOT(a)     "1"#a
# define SIGNAL(a)   "2"#a
#endif

#define KMETHOD_CODE  0                        // member type codes
#define KSLOT_CODE    1
#define KSIGNAL_CODE  2


#define KV_BEGINE_MOC_NAMESPACE(Class)	\
struct kv_meta_stringdata_##Class{ \
	KvByteArray data[8];				\
	char stringdata[35];		\
};	\

#define KV_END_MOC_NAMESPACE()  ;

//static const uint kv_meta_data_##CLASS[8]; \

#define KV_BEGINE_REGISTER_CLASS(Class, Super,...) \
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
	\
	KV_BEGINE_REGISTER_PROPERTY()\
	KV_BEGINE_REGISTER_META(__VA_ARGS__)\

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

//method
#define kv_def_field_(t,a,v)  (*reinterpret_cast<t(*)>(a[v]))
#define kv_def_elem_0(t,a,v) kv_def_field_(t,a,v)

//slot
#define kv_def_v_elem_(a,v)  const_cast<void*>(reinterpret_cast<const void*>(&a##v))
#define kv_def_elem_1(t,a,v) kv_def_v_elem_(a,v)

//signal
#define kv_def_f_elem_(t,a,v) t a##v
#define kv_def_elem_2(t,a,v) kv_def_f_elem_(t,a,v)

//signal param
#define kv_def_s_elem_(a,v)  const_cast<void*>(reinterpret_cast<const void*>(&a##v))
#define kv_def_elem_3(t,a,v) kv_def_s_elem_(a,v)

#define kv_signal_fun_elem_0(r, data, index, element) \
	BOOST_PP_IF(BOOST_PP_EQUAL(1, index), BOOST_PP_EMPTY(), BOOST_PP_COMMA())\
	kv_def_elem_0(element, data, index)\

#define kv_signal_fun_elem_1(r, data, index, element) \
	BOOST_PP_IF(BOOST_PP_EQUAL(1,index), BOOST_PP_EMPTY,BOOST_PP_COMMA)()\
	kv_def_elem_1(element, data, index)

#define kv_signal_fun_elem_2(r, data, index, element) \
	BOOST_PP_IF(BOOST_PP_EQUAL(1,index), BOOST_PP_EMPTY,BOOST_PP_COMMA)()\
	kv_def_elem_2(element, data, index)

#define kv_metacall_elem_(r, data, index, element) \
	BOOST_PP_IF(BOOST_PP_LESS_EQUAL(2, index)\
	, kv_signal_fun_elem_0(r, data, BOOST_PP_SUB(index, 1), element)\
	, BOOST_PP_EMPTY())

#define kv_static_metacall_elem(r, data, index, element) \
		case index: data->BOOST_PP_TUPLE_ELEM(1, element)\
		(\
			BOOST_PP_SEQ_FOR_EACH_I(\
			BOOST_PP_CAT(kv_signal_fun_elem_, BOOST_PP_TUPLE_ELEM(0, element))\
			, kv_vargment, BOOST_PP_TUPLE_TO_SEQ(element))\
		); break; \

#define KV_PP_SEQ_FOR_EACH_R_ID() BOOST_PP_SEQ_FOR_EACH_I 
#define KV_PP_DEFER(x) x BOOST_PP_EMPTY()
#define kv_static_metacall_elem0(r, data, index, element) \
		case index: data->BOOST_PP_TUPLE_ELEM(1, element)\
		(\
			KV_PP_DEFER(KV_PP_SEQ_FOR_EACH_R_ID)()(kv_metacall_elem_, kv_vargment\
			, BOOST_PP_TUPLE_TO_SEQ(element))\
		); break; \

#define kv_signal_fun_elem_ex(r, data, index, element) \
	BOOST_PP_IF(BOOST_PP_LESS_EQUAL(2, index)\
	, kv_def_elem_2(element, data, BOOST_PP_SUB(index, 1))\
	, BOOST_PP_EMPTY())

#define kv_signal_fun_elem_ex2(r, data, index, element) \
	BOOST_PP_IF(BOOST_PP_LESS_EQUAL(2, index)\
	, kv_def_elem_3(element, data, BOOST_PP_SUB(index, 1))\
	, BOOST_PP_EMPTY())

#define kv_signal_metacall(index,...)\
	void ThisClass::BOOST_PP_TUPLE_ELEM(1, __VA_ARGS__)(\
	KV_PP_DEFER(KV_PP_SEQ_FOR_EACH_R_ID)()(kv_signal_fun_elem_ex, kv_vargment\
	, BOOST_PP_TUPLE_TO_SEQ(__VA_ARGS__)))\
	{\
		void *kv_vargment[] = { 0, \
		KV_PP_DEFER(KV_PP_SEQ_FOR_EACH_R_ID)()(kv_signal_fun_elem_ex2, kv_vargment\
		, BOOST_PP_TUPLE_TO_SEQ(__VA_ARGS__))\
		}; \
		KvMetaObject::activate(this, &staticMetaObject, index, kv_vargment); \
	}\

#define kv_signal_metacall_elem(r, data, index, element) \
	BOOST_PP_IF(BOOST_PP_EQUAL(2, BOOST_PP_TUPLE_ELEM(0, element)), \
	kv_signal_metacall, BOOST_PP_EMPTY)(index,element)

#define KV_BEGINE_REGISTER_META(...) \
	void ThisClass::kv_static_metacall(KvObject *_o, KvMetaObject::Call _c,\
										int _id, void **kv_vargment)\
	{\
		if (_c == KvMetaObject::InvokeMetaMethod) {\
			assert(staticMetaObject.cast(_o)); \
			ThisClass *_t = static_cast<ThisClass *>(_o); \
			switch (_id) {\
			BOOST_PP_SEQ_FOR_EACH_I(kv_static_metacall_elem0, _t\
				, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))\
			}\
		}\
	}\
	\
	BOOST_PP_SEQ_FOR_EACH_I(kv_signal_metacall_elem, _t\
	, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))\

#define KV_META_REGISTER(Method,Function,...)\
	(Method, Function, __VA_ARGS__)\

#define KV_REGISTER_METHOD(Function,...) \
	KV_META_REGISTER(KMETHOD_CODE, Function, __VA_ARGS__)

#define KV_REGISTER_SLOT(Function,...) \
	KV_META_REGISTER(KSLOT_CODE, Function, __VA_ARGS__)

#define KV_REGISTER_SIGNAL(Function,...) \
	KV_META_REGISTER(KSIGNAL_CODE, Function, __VA_ARGS__)\

//#define Kv_FOR_EACH_ELEM(method,...)\
//	BOOST_PP_SEQ_FOR_EACH_I(BOOST_PP_CAT(kv_signal_fun_elem_, method)\
//	, kv_vargment, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))
//
//#define KV_REGISTER_SIGNAL_0(Function,...)\
//	void ThisClass::Function(\
//			BOOST_PP_IF(BOOST_PP_IS_EMPTY(__VA_ARGS__)\
//			,BOOST_PP_EMPTY\
//			, Kv_FOR_EACH_ELEM)(KSIGNAL_CODE, __VA_ARGS__))\
//	{\
//		void *kv_vargment[] = { 0,\
//			BOOST_PP_IF(BOOST_PP_IS_EMPTY(__VA_ARGS__)\
//				,BOOST_PP_EMPTY\
//				,Kv_FOR_EACH_ELEM)(3,__VA_ARGS__)\
//		 };\
//		KvMetaObject::activate(this, &staticMetaObject, 0, kv_vargment);\
//	}\
//

#define KV_END_REGISTER_META()

#endif // KvObjectdefs_h__
