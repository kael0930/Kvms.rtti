#ifndef KvMetaObject_h__
#define KvMetaObject_h__

#include "Kvms.h"
#include "KvVariant.h"

class KvObject;
class KvMetaProperty;
class KvMetaMethod;
class KvByteArray;

struct KV_CORE_EXPORT KvMetaObject
{
	const char * className() const;
	const KvMetaObject *superClass() const;

	KvObject *cast(KvObject *obj) const;
	const KvObject *cast(const KvObject *obj) const;

	int methodOffset() const;
	int propertyOffset() const;

	//int indexOfConstructor(const char *constructor) const;
	int indexOfMethod(const char *method) const;
	int indexOfSignal(const char *signal) const;
	int indexOfSlot(const char *slot) const;
	//int indexOfEnumerator(const char *name) const;
	int indexOfProperty(const char *name) const;
	//int indexOfClassInfo(const char *name) const;

	KvMetaProperty property(int index) const;

	static bool checkConnectArgs(const char *signal, const char *method);
	static KvByteArray normalizedSignature(const char *method);
	static KvByteArray normalizedType(const char *type);

	// internal index-based connect

	// internal index-based disconnect

	// internal slot-name based connect

	// internal index-based signal activation
	static void activate(KvObject *sender, int signal_index, void **argv);  //obsolete
	static void activate(KvObject *sender, int from_signal_index, int to_signal_index, void **argv); //obsolete
	static void activate(KvObject *sender, const KvMetaObject *, int local_signal_index, void **argv);
	static void activate(KvObject *sender, const KvMetaObject *, int from_local_signal_index, int to_local_signal_index, void **argv); //obsolete

	// internal guarded pointers

	enum Call {
		InvokeMetaMethod,
		ReadProperty,
		WriteProperty,
		ResetProperty,
		QueryPropertyDesignable,
		QueryPropertyScriptable,
		QueryPropertyStored,
		QueryPropertyEditable,
		QueryPropertyUser,
		CreateInstance
	};

	int static_metacall(Call, int, void **) const;
	static int metacall(KvObject *, Call, int, void **);

	struct  // private data
	{
		const KvMetaObject *superdata;
		//const char *stringdata; //revision 4
		const KvByteArrayData *stringdata; //revision 7
		const uint *data;

		//typedef void(*StaticMetacallFunction)(KvObject *, KvMetaObject::Call, int, void **);
		//StaticMetacallFunction static_metacall;
		//const KvMetaObject * const *relatedMetaObjects;
		//void *extradata; //reserved for future use
		const void *extradata;
	} d;

};

struct KvMetaObjectExtraData
{
	const KvMetaObject *objects;

	typedef void(*StaticMetaCallFunction)(KvObject *, KvMetaObject::Call, int, void **);
	StaticMetaCallFunction static_metacall;
};

struct KvDynamicMetaObject : public KvMetaObject
{
	virtual ~KvDynamicMetaObject() {}
	virtual int metaCall(KvMetaObject::Call, int _id, void **) { return _id; }
	virtual int createProperty(const char *, const char *) { return -1; }
};

class KV_CORE_EXPORT KvMetaMethod
{
public:
	KvMetaMethod();
	~KvMetaMethod();

private:

};

class KV_CORE_EXPORT KvMetaProperty
{
public:
	KvMetaProperty();
	~KvMetaProperty();

	const char *name() const;
	const char *typeName() const;
	KvVariant::Type type() const;

	bool isReadable() const;
	bool isWritable() const;

	bool isFlagType() const;
	bool isEnumType() const;
	//QMetaEnum enumerator() const;

	KvVariant read(const KvObject *obj) const;
	bool write(KvObject *obj, const KvVariant &value)  const;
	bool reset(KvObject *obj) const;

private:
	const KvMetaObject *mobj;
	uint handle;
	int idx;
	//QMetaEnum menum;
	friend struct KvMetaObject;
};

#endif // KvMetaObject_h__
