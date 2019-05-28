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
#define KPROPERTY_CODE  3
#define KCLASSINFO_CODE  4


#define KV_BEGINE_REGISTER_CLASS(Class, Super,...) \
	typedef Class ThisClass;\
	typedef Super BaseClass;\
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
	KV_BEGINE_REGISTER_META(__VA_ARGS__)\
	KV_BEGINE_REGISTER_PROPERTY(__VA_ARGS__)\
	KV_COMPUTE_META_DATA(Class, Super, __VA_ARGS__)\
	KV_COMPUTE_STRING_DATA(Class, Super, __VA_ARGS__)\

#define KV_END_REGISTER_CLASS()

#define kv_comma ,
#define kv_argment a
#define kv_aargment _a
#define kv_vargment _v

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

#define kv_fun_elem_(r, data, index, element) \
	BOOST_PP_IF(BOOST_PP_LESS_EQUAL(index, 1), KV_PP_EMPTY, BOOST_PP_COMMA)()\
	kv_def_elem_1(element, data, index)

#define kv_fun_elem_0(r, data, index, element) \
	BOOST_PP_IF(BOOST_PP_LESS_EQUAL(index, 1), KV_PP_EMPTY, BOOST_PP_COMMA)()\
	kv_def_elem_0(element, data, index)\

#define kv_fun_elem_1(r, data, index, element) \
	BOOST_PP_IF(BOOST_PP_LESS_EQUAL(index, 1), KV_PP_EMPTY, BOOST_PP_COMMA)()\
	kv_def_elem_1(element, data, index)

#define kv_fun_elem_2(r, data, index, element) \
	BOOST_PP_IF(BOOST_PP_LESS_EQUAL(index, 1), KV_PP_EMPTY, BOOST_PP_COMMA)()\
	kv_def_elem_2(element, data, index)

#define kv_fun_elem_3(r, data, index, element) \
	BOOST_PP_IF(BOOST_PP_LESS_EQUAL(index, 1), KV_PP_EMPTY, BOOST_PP_COMMA)()\
	kv_def_elem_3(element, data, index)

#define kv_metacall_elem(r, data, index, element) \
	BOOST_PP_IF(BOOST_PP_LESS(index, 2)\
	, KV_PP_EMPTY\
	, kv_fun_elem_0)(r, data, BOOST_PP_SUB(index, 1), element)

#define KV_PP_DEFER(x) x KV_PP_EMPTY()
#define KV_PP_EMPTY(...) BOOST_PP_EMPTY()

#define KV_PP_SEQ_FOR_EACH_R_ID() BOOST_PP_SEQ_FOR_EACH_I 
#define KV_PP_SEQ_FOR_EACH_SR_ID(data,index,element) \
		case index: data->BOOST_PP_TUPLE_ELEM(1, element)\
		(\
		KV_PP_DEFER(KV_PP_SEQ_FOR_EACH_R_ID)()(kv_metacall_elem, kv_aargment\
			, BOOST_PP_TUPLE_TO_SEQ(element))\
		); break;

#define kv_static_metacall_elem_(r, data, index, element) \
			case index: data->BOOST_PP_TUPLE_ELEM(1, element)\
			(\
			BOOST_PP_SEQ_FOR_EACH_I(\
			BOOST_PP_CAT(kv_fun_elem_, BOOST_PP_TUPLE_ELEM(0, element))\
			, kv_aargment, BOOST_PP_TUPLE_TO_SEQ(element))\
			); break; 

#define kv_static_metacall_elem0(r, data, index, element) \
		case index: data->BOOST_PP_TUPLE_ELEM(1, element)\
		(\
		KV_PP_DEFER(KV_PP_SEQ_FOR_EACH_R_ID)()(kv_metacall_elem, kv_aargment\
			, BOOST_PP_TUPLE_TO_SEQ(element))\
		); break;

#define kv_static_metacall_elem1(r, data, index, element)\
	BOOST_PP_IIF(BOOST_PP_GREATER_EQUAL(BOOST_PP_TUPLE_ELEM(0, element), 3)\
	, KV_PP_EMPTY, KV_PP_SEQ_FOR_EACH_SR_ID)(data, index, element)

#define kv_signal_elem(r, data, index, element) \
	BOOST_PP_IF(BOOST_PP_LESS(index, 2)\
	, KV_PP_EMPTY\
	, kv_fun_elem_2)(r, data, BOOST_PP_SUB(index, 1), element)

#define kv_signal_elem2(r, data, index, element) \
	BOOST_PP_IF(BOOST_PP_LESS(index, 2)\
	, KV_PP_EMPTY\
	, kv_fun_elem_3)(r, data, BOOST_PP_SUB(index, 1), element)

#define kv_signal_metacall(index,...)\
	void ThisClass::BOOST_PP_TUPLE_ELEM(1, __VA_ARGS__)(\
	KV_PP_DEFER(KV_PP_SEQ_FOR_EACH_R_ID)()(kv_signal_elem, kv_aargment\
	, BOOST_PP_TUPLE_TO_SEQ(__VA_ARGS__)))\
	{\
		void *kv_aargment[] = { 0, \
			\
			KV_PP_DEFER(KV_PP_SEQ_FOR_EACH_R_ID)()(kv_signal_elem2, kv_aargment\
			, BOOST_PP_TUPLE_TO_SEQ(__VA_ARGS__))\
		}; \
		KvMetaObject::activate(this, &staticMetaObject, index, kv_aargment); \
	}

#define kv_signal_metacall_elem(r, data, index, element) \
	BOOST_PP_IF(BOOST_PP_EQUAL(2, BOOST_PP_TUPLE_ELEM(0, element)), \
	kv_signal_metacall, KV_PP_EMPTY)(index, element)

#define KV_BEGINE_REGISTER_CLASSINFO(...) \

#define KV_REGISTER_CLASSINFO(Key,Value)\
	(Method, Key, Value)\

#define KV_END_REGISTER_CLASSINFO()

#define KV_BEGINE_REGISTER_META(...) \
	void ThisClass::kv_static_metacall(KvObject *_o, KvMetaObject::Call _c,\
										int _id, void **kv_aargment)\
	{\
		if (_c == KvMetaObject::InvokeMetaMethod) {\
			assert(staticMetaObject.cast(_o)); \
			ThisClass *_t = static_cast<ThisClass *>(_o); \
			switch (_id) {\
			BOOST_PP_SEQ_FOR_EACH_I(kv_static_metacall_elem1, _t\
				, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))\
			}\
		}\
	}\
	\
	BOOST_PP_SEQ_FOR_EACH_I(kv_signal_metacall_elem, _t\
	, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))\

#define KV_META_REGISTER(Method,Function,...)\
	(KCLASSINFO_CODE, Function, __VA_ARGS__)\

#define KV_REGISTER_METHOD(Function,...) \
	KV_META_REGISTER(KMETHOD_CODE, Function, __VA_ARGS__)

#define KV_REGISTER_SLOT(Function,...) \
	KV_META_REGISTER(KSLOT_CODE, Function, __VA_ARGS__)

#define KV_REGISTER_SIGNAL(Function,...) \
	KV_META_REGISTER(KSIGNAL_CODE, Function, __VA_ARGS__)\

#define KV_END_REGISTER_META()

#define KV_PP_SEQ_FOR_EACH_RR_ID(data,index,element) \
		case index: *reinterpret_cast<BOOST_PP_TUPLE_ELEM(1, element)*>(data)\
		= BOOST_PP_TUPLE_ELEM(3, element)(); break;

#define kv_metacall_read_elem(r, data, index, element)\
	BOOST_PP_IIF(BOOST_PP_EQUAL(BOOST_PP_TUPLE_ELEM(0, element), 3)\
	, KV_PP_SEQ_FOR_EACH_RR_ID, KV_PP_EMPTY)(data, index, element)

#define KV_PP_SEQ_FOR_EACH_WR_ID(data,index,element) \
		case index: BOOST_PP_TUPLE_ELEM(4, element)\
		(*reinterpret_cast<BOOST_PP_TUPLE_ELEM(1, element)*>(data)); break;

#define kv_metacall_write_elem(r, data, index, element)\
	BOOST_PP_IIF(BOOST_PP_EQUAL(BOOST_PP_TUPLE_ELEM(0, element), 3)\
	, KV_PP_SEQ_FOR_EACH_WR_ID, KV_PP_EMPTY)(data, index, element)

#define KV_BEGINE_REGISTER_PROPERTY(...) \
	int ThisClass::kv_metacall(KvMetaObject::Call _c, int _id, void **kv_aargment)	\
	{	\
		_id = BaseClass::kv_metacall(_c, _id, kv_aargment); \
		if (_id < 0)\
			return _id; \
		if (_c == KvMetaObject::InvokeMetaMethod) {\
			if (_id < BOOST_PP_SEQ_SIZE(BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)))\
				kv_static_metacall(this, _c, _id, kv_aargment); \
			_id -= 1; \
		}\
		else if (_c == KvMetaObject::ReadProperty) {\
			\
			void *kv_vargment = kv_aargment[0]; \
			switch (_id) {\
			BOOST_PP_SEQ_FOR_EACH_I(kv_metacall_read_elem, kv_vargment\
			, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))\
			}\
			_id -= 1; \
		}\
		else if (_c == KvMetaObject::WriteProperty) {\
			void *_v = _a[0]; \
			switch (_id) {\
			BOOST_PP_SEQ_FOR_EACH_I(kv_metacall_write_elem, kv_vargment\
			, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))\
			}\
			_id -= 1; \
		}\
		return _id; \
	}\

#define KV_REGISTER_PROPERTY(Type, Name, R, W)\
	(KPROPERTY_CODE, Type, Name, R, W)\

#define KV_END_REGISTER_PROPERTY()

#define KV_COMPUTE_META_DATA(Class, Super,...)\

#define KV_COMPUTE_STRING_DATA(Class, Super,...)\

#endif // KvObjectdefs_h__
