#ifndef KvObject_h__
#define KvObject_h__

#include "KvGlobal.h"
#include "KvList.h"
#include "KvString.h"
#include "KvObjectdefs.h"
#include "KvScopedPointer.h"

#include "KvMetaObject.h"

#include "KvVariant.h"

class KvEvent;

class KvObjectPrivate;
class KvObject;
class KvObjectUserData;

struct KvMetaObject;
class KvMetaMethod;

typedef KvList<KvObject*> ObjectList;

void kv_findChildren_helper(const KvObject *parent, const KvString &name, /*const QRegExp *re,*/
	const KvMetaObject &mo, KvList<void *> *list);
KvObject *kv_findChild_helper(const KvObject *parent, const KvString &name, const KvMetaObject &mo);

struct KV_CORE_EXPORT KvObjectData
{
	virtual ~KvObjectData() = 0;

	KvObject *q_ptr;
	KvObject *parent;
	ObjectList children;

	uint isWidget : 1;
	uint pendTimer : 1;
	uint blockSig : 1;
	uint wasDeleted : 1;
	uint ownObjectName : 1;


	KvMetaObject *metaObject; // assert dynamic
};

class KV_CORE_EXPORT KvObject
{
	K_OBJECT
	K_DECLARE_PRIVATE(KvObject)

public:
	explicit KvObject(KvObject *parent = 0);
	virtual ~KvObject();

	virtual bool event(KvEvent *);
	virtual bool eventFilter(KvObject *, KvEvent *);

	KvString objectName() const;
	void setObjectName(const KvString &name);

	inline bool isWidgetType() const { return d_ptr->isWidget; }

	template<typename T>
	inline T findChild(const KvString &cName = KvString()) const
	{
		return static_cast<T>(kv_findChild_helper(this, cName, reinterpret_cast<T>(0)->staticMetaObject));
	}

	template<typename T>
	inline KvList<T> findChildren(const KvString *cName = KvString()) const
	{
		KvList<T> list;
		union 
		{
			KvList<T> *typeList;
			KvList<void *> voidList;
		} u;
		u.typeList = &list;
		kv_findChildren_helper(this, cName, reinterpret_cast<T>(0)->staticMetaObject, u.voidList);
		return list;
	}

	inline const ObjectList &children() const { return d_ptr->children; }

	void setParent(KvObject *);
	inline KvObject *parent() const { return d_ptr->parent; }
	inline bool inherits(const char *className) const
	{
		return const_cast<KvObject *>(this)->kv_metacast(className) != 0;
	}

	bool setProperty(const char *name, const KvVariant &value);
	KvVariant property(const char *name);
	//QList<QByteArray> dynamicPropertyNames() const;

	static uint registerUserData();
	void setUserData(uint id, KvObjectUserData* data);
	KvObjectUserData* userData(uint id) const;

	void installEventFilter(KvObject *);
	void removeEventFilter(KvObject *);

	static bool connect(const KvObject * sender, const char *signal,
		const KvObject *receiver, const char *member,
		Kv::ConnectionType = Kv::AutoConnection);

	static bool connect(const KvObject * sender, const KvMetaMethod &signal,
		const KvObject *receiver, const KvMetaMethod &method,
		Kv::ConnectionType = Kv::AutoConnection);

	static bool connect(const KvObject *sender, const char *signal,
		const char *member, Kv::ConnectionType = Kv::AutoConnection);

Q_SIGNALS:
	void destroyed(KvObject * = 0);

public Q_SLOTS:
	void deleteLater();

protected:
	KvObject *sender() const;
	int senderSignalIndex() const;
	int recevicers(const char *signal) const;

protected:
	KvScopedPointer<KvObjectData> d_ptr;

	//static const KvMetaObject staticKvMetaObject;

	friend struct KvMetaObject;

private:
	K_DISABLE_COPY(KvObject)
	//K_PRIVATE_SLOT(d_func(), void _q_reregisterTimers(void *))
};

class  KvObjectUserData {
public:
	virtual ~KvObjectUserData();
};

#endif // KvObject_h__
