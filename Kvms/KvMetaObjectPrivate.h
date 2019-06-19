#ifndef KvMetaObjectPrivate_h__
#define KvMetaObjectPrivate_h__

#include "KvObject.h"
#include "KvObjectPrivate.h"
#include "KvVarLengthArray.h"

extern int kvMetaTypeTypeInternal(const char *);

class KvArgumentType
{
public:
	KvArgumentType(int type)
		: _type(type)
	{}
	KvArgumentType(const KvByteArray &name)
		: _type(kvMetaTypeTypeInternal(name.constData())), _name(name)
	{}
	KvArgumentType()
		: _type(0)
	{}
	int type() const
	{
		return _type;
	}
	KvByteArray name() const
	{
		if (_type && _name.isEmpty())
			const_cast<KvArgumentType *>(this)->_name = KvMetaType::typeName(_type);
		return _name;
	}
	bool operator==(const KvArgumentType &other) const
	{
		if (_type && other._type)
			return _type == other._type;
		else
			return name() == other.name();
	}
	bool operator!=(const KvArgumentType &other) const
	{
		if (_type && other._type)
			return _type != other._type;
		else
			return name() != other.name();
	}

private:
	int _type;
	KvByteArray _name;
};
KV_DECLARE_TYPEINFO(KvArgumentType, KV_MOVABLE_TYPE)

typedef KvVarLengthArray<KvArgumentType, 10> KvArgumentTypeArray;

struct KvMetaObjectPrivate
{
	int revision;
	int className;
	int classInfoCount, classInfoData;
	int methodCount, methodData;
	int propertyCount, propertyData;
	int enumeratorCount, enumeratorData;
	int constructorCount, constructorData; //since revision 2
	int flags; //since revision 3
	int signalCount; //since revision 4
	// revision 5 introduces changes in normalized signatures, no new members
	// revision 6 added qt_static_metacall as a member of each Q_OBJECT and inside QMetaObject itself

	static inline const KvMetaObjectPrivate *get(const KvMetaObject *metaobject)
	{
		return reinterpret_cast<const KvMetaObjectPrivate*>(metaobject->d.data);
	}

	static int indexOfSignalRelative(const KvMetaObject **baseObject,
		const char* name,
		bool normalizeStringData);
	static int indexOfSlotRelative(const KvMetaObject **m,
		const char *slot,
		bool normalizeStringData);
	static int originalClone(const KvMetaObject *obj, int local_method_index);

	static KvByteArray decodeMethodSignature(const char *signature,
		KvArgumentTypeArray &types);

	//defined in kvobject.cpp
	enum DisconnectType { DisconnectAll, DisconnectOne };
	static void memberIndexes(const KvObject *obj, const KvMetaMethod &member,
		int *signalIndex, int *methodIndex);
	static bool connect(const KvObject *sender, int signal_index,
		const KvObject *receiver, int method_index_relative,
		const KvMetaObject *rmeta = 0,
		int type = 0, int *types = 0);
	static bool disconnect(const KvObject *sender, int signal_index,
		const KvObject *receiver, int method_index,
		DisconnectType = DisconnectAll);
	static inline bool disconnectHelper(KvObjectPrivate::Connection *c,
		const KvObject *receiver, int method_index,
		/*QMutex *senderMutex, */DisconnectType);
};

#endif // KvMetaObjectPrivate_h__
