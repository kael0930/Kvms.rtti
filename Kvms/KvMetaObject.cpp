#include "KvMetaObject.h"
#include "KvMetaObjectPrivate.h"
#include "KvObject.h"
#include "KvObjectPrivate.h"

static inline const KvMetaObjectPrivate *priv(const uint* data)
{
	return reinterpret_cast<const KvMetaObjectPrivate*>(data);
}

const char * KvMetaObject::className() const
{
	return d.stringdata;
}

const KvMetaObject * KvMetaObject::superClass() const
{
	return d.superdata;
}

KvObject * KvMetaObject::cast(KvObject *obj) const
{
	if (obj)
	{
		const KvMetaObject *m = obj->metaObject();
		do 
		{
			if (m == this)
				return obj;
		} while ((m = m->d.superdata));
	}
	return 0;
}

const KvObject * KvMetaObject::cast(const KvObject *obj) const
{
	if (obj) {
		const KvMetaObject *m = obj->metaObject();
		do {
			if (m == this)
				return obj;
		} while ((m = m->d.superdata));
	}
	return 0;
}

int KvMetaObject::propertyOffset() const
{
	int offset = 0;
	const KvMetaObject *m = d.superdata;
	while (m)
	{
		offset += priv(m->d.data)->propertyCount;
		m = m->d.superdata;
	}

	return offset;
}

int KvMetaObject::indexOfProperty(const char *name) const
{
	const KvMetaObject *m = this;
	while (m)
	{
		const KvMetaObjectPrivate *d = priv(m->d.data);
		for (int i = d->propertyCount - 1; i >= 0; --i)
		{
			const char *prop = m->d.stringdata + m->d.data[d->propertyData + 3 * i];
			if (name[0] == prop[0] && strcmp(name + 1, prop + 1) == 0) {
				i += m->propertyOffset();
				return i;
			}
		}
		m = m->d.superdata;
	}

#if 0 //运行时动态属性;
	if (priv(this->d.data)->revision >= 3 && (priv(this->d.data)->flags & DynamicMetaObject)) {
		QAbstractDynamicMetaObject *me =
			const_cast<QAbstractDynamicMetaObject *>(static_cast<const QAbstractDynamicMetaObject *>(this));

		return me->createProperty(name, 0);
	}
#endif

	return -1;
}

KvMetaProperty KvMetaObject::property(int index) const
{
	int i = index;
	i -= propertyOffset();
	if (i < 0 && d.superdata)
	{
		return d.superdata->property(index);
	}

	KvMetaProperty result;
	if (i >= 0 && i < priv(d.data)->propertyCount)
	{
		int handle = priv(d.data)->propertyData + 3 * i;
		int flags = d.data[handle + 2];
		const char * type = d.stringdata + d.data[handle + 1];
		result.mobj = this;
		result.handle = handle;
		result.idx = i;

		if (flags & Kv::EnumOrFlag)
		{

		}
	}
	return result;
}

int KvMetaObject::static_metacall(Call, int, void **) const
{
	return 0;
}

int KvMetaObject::metacall(KvObject *obj, Call cl, int idx, void **argv)
{
	if (KvMetaObject *mo = obj->d_ptr->metaObject)
	{
		return static_cast<KvDynamicMetaObject *>(mo)->metaCall(cl, idx, argv);
	}
	else
	{
		return obj->kv_metacall(cl, idx, argv);
	}
}

//////////////////////////////////////////////////////////////////////////

KvMetaMethod::KvMetaMethod()
{
}

KvMetaMethod::~KvMetaMethod()
{
}


//////////////////////////////////////////////////////////////////////////

KvMetaProperty::KvMetaProperty()
	: mobj(0), handle(0), idx(0)
{
}

KvMetaProperty::~KvMetaProperty()
{
}

const char * KvMetaProperty::name() const
{
	if (!mobj)
	{
		return 0;
	}
	int handle = priv(mobj->d.data)->propertyData + 3 * idx;
	return mobj->d.stringdata + mobj->d.data[handle];
}

const char * KvMetaProperty::typeName() const
{
	if (!mobj)
		return 0;
	int handle = priv(mobj->d.data)->propertyData + 3 * idx;
	return mobj->d.stringdata + mobj->d.data[handle + 1];
}

KvVariant::Type KvMetaProperty::type() const
{
	if (!mobj)
		return KvVariant::Invalid;
	int handle = priv(mobj->d.data)->propertyData + 3 * idx;
	uint flags = mobj->d.data[handle + 2];

	uint type = flags >> 24;
	if (type == 0xff) // special value for QVariant
		type = KvVariant::LastType;
	if (type)
		return KvVariant::Type(type);
	if (isEnumType()) 
	{

	}

	return KvVariant::UserType;
}

bool KvMetaProperty::isReadable() const
{
	if (!mobj)
		return false;
	int flags = mobj->d.data[handle + 2];
	return flags &  Kv::Readable;
}

bool KvMetaProperty::isWritable() const
{
	if (!mobj)
	{
		return false;
	}
	int flags = mobj->d.data[handle + 2];
	return flags & Kv::Writable;
}

bool KvMetaProperty::isFlagType() const
{
	return false;
}

bool KvMetaProperty::isEnumType() const
{
	return false;
}

KvVariant KvMetaProperty::read(const KvObject *obj) const
{
	if (!obj || !mobj)
	{
		return KvVariant();
	}

	uint t = KvVariant::Int;
	if (isEnumType())
	{
	}
	else
	{
		int handle = priv(mobj->d.data)->propertyData + 3 * idx;
		uint flags = mobj->d.data[handle + 2];
		const char *typeName = mobj->d.stringdata + mobj->d.data[handle + 1];
		t = (flags >> 24);
		if (t == 0xff)
		{
			t = KvVariant::LastType;
		}
		if (t == KvVariant::Invalid)
		{
			t = KvMetaType::type(typeName);
		}
		if (t == KvVariant::Invalid)
		{
			t = KvVariant::nameToType(typeName);
		}

		if (t == KvVariant::Invalid || t == KvVariant::UserType)
		{
			if (t == KvVariant::Invalid)
			{
				printf("KvMetaProperty::read: Unable to handle unregistered datatype '%s' for property '%s::%s'", 
					typeName, mobj->className(), name());
			}
		}
	}

	// -1 (unchanged): normal qt_metacall, result stored in argv[0]
	// changed: result stored directly in value
	int status = -1;
	KvVariant value;
	void *argv[] = { 0, &value, &status };
	if (t == KvVariant::LastType) 
	{
		argv[0] = &value;
	}
	else 
	{
		value = KvVariant(t, (void*)0);
		argv[0] = value.data();
	}
	KvMetaObject::metacall(const_cast<KvObject*>(obj), KvMetaObject::ReadProperty,
		idx + mobj->propertyOffset(), argv);

	if (status != -1)
	{
		return value;
	}
	if (t != KvVariant::LastType && argv[0] != value.data())
	{
		// pointer or reference
		return KvVariant((KvVariant::Type)t, argv[0]);
	}

	return value;
}

bool KvMetaProperty::write(KvObject *obj, const KvVariant &value) const
{
	if (!obj || !isWritable())
	{
		return false;
	}

	KvVariant v = value;
	uint t = KvVariant::Invalid;
	if (isEnumType())
	{
		if (v.type() == KvVariant::String)
		{
		} 
		else if (v.type() != KvVariant::Int && v.type() != KvVariant::UInt)
		{
		}
		v.convert(KvVariant::Int);
	}
	else
	{
		int handle = priv(mobj->d.data)->propertyData + 3 * idx;
		uint flags = mobj->d.data[handle + 2];
		t = flags >> 24;
		if (t == 0xff)
		{
			t = KvVariant::LastType;
		}
		if (t == KvVariant::Invalid)
		{
			//在此判定是不是用户自定义属性;
			const char *typeName = mobj->d.stringdata + mobj->d.data[handle + 1];
			const char *vtypeName = value.typeName();
			if (vtypeName && strcmp(typeName, vtypeName) == 0)
			{
				t = value.userType();
			}
			else
			{
				t = KvVariant::nameToType(typeName);
			}
		}
		if (t == KvVariant::Invalid)
		{
			return false;
		}
		if (t != KvVariant::LastType 
			&& t != (uint)value.userType() 
			&& (t < KvMetaType::User && !v.convert((KvVariant::Type)t)))
		{
			return false;
		}
	}

	// -1 (unchanged): normal qt_metacall, result stored in argv[0]
	// changed: result stored directly in value, return the value of status
	int status = -1;
	// the flags variable is used by the declarative module to implement
	// interception of property writes.
	int flags = 0;
	void *argv[] = { 0, &v, &status, &flags };
	if (t == KvVariant::LastType)
		argv[0] = &v;
	else
		argv[0] = v.data();
	KvMetaObject::metacall(obj, KvMetaObject::WriteProperty, idx + mobj->propertyOffset(), argv);
	return status;
}

//////////////////////////////////////////////////////////////////////////

int KvMetaObjectPrivate::indexOfSignalRelative(const KvMetaObject **baseObject, const char* name, bool normalizeStringData)
{
	return -1;
}

int KvMetaObjectPrivate::indexOfSlotRelative(const KvMetaObject **m, const char *slot, bool normalizeStringData)
{
	return -1;
}

int KvMetaObjectPrivate::originalClone(const KvMetaObject *obj, int local_method_index)
{
	return -1;
}
