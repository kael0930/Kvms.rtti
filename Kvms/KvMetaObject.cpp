#include "KvMetaObject.h"
#include "KvMetaObjectPrivate.h"
#include "KvObject.h"
#include "KvObjectPrivate.h"
#include "KvVarLengthArray.h"

//////////////////////////////////////////////////////////////////////////

/** \internal
* helper function for indexOf{Method,Slot,Signal}, returns the relative index of the method within
* the baseObject
* \a MethodType might be MethodSignal or MethodSlot, or 0 to match everything.
* \a normalizeStringData set to true if we should do a second pass for old moc generated files normalizing all the symbols.
*/

static inline const KvMetaObjectPrivate *priv(const uint* data)
{
	return reinterpret_cast<const KvMetaObjectPrivate*>(data);
}

static inline const KvByteArray stringData(const KvMetaObject *mo, int index)
{
	KV_ASSERT(priv(mo->d.data)->revision >= 7);
	const KvByteArrayDataPtr data = { const_cast<KvByteArrayData*>(&mo->d.stringdata[index]) };
	KV_ASSERT(data.ptr->ref.isStatic());
	KV_ASSERT(data.ptr->alloc == 0);
	KV_ASSERT(data.ptr->capacity == 0);
	KV_ASSERT(data.ptr->size >= 0);
	return data;
}

static inline const char *rawStringData(const KvMetaObject *mo, int index)
{
	return stringData(mo, index).data();
}

static inline KvByteArray typeNameFromTypeInfo(const KvMetaObject *mo, uint typeInfo)
{
	//if (typeInfo & IsUnresolvedType) {
	//	return stringData(mo, typeInfo & TypeNameIndexMask);
	//}
	//else 
	{
		const char *t = KvMetaType::typeName(typeInfo);
		return KvByteArray::fromRawData(t, kvstrlen(t));
	}
}

static inline const char *rawTypeNameFromTypeInfo(const KvMetaObject *mo, uint typeInfo)
{
	return typeNameFromTypeInfo(mo, typeInfo).constData();
}

static inline int typeFromTypeInfo(const KvMetaObject *mo, uint typeInfo)
{
	//if (!(typeInfo & IsUnresolvedType))
		return typeInfo;
	//return KvMetaType::type(stringData(mo, typeInfo & TypeNameIndexMask));
}

static inline const char *objectClassName(const KvMetaObject *m)
{
	return rawStringData(m, priv(m->d.data)->className);
}

static bool methodMatch(const KvMetaObject *m, int handle,
	const KvByteArray &name, int argc,
	const KvArgumentType *types)
{
	KV_ASSERT(priv(m->d.data)->revision >= 7);
	if (int(m->d.data[handle + 1]) != argc)
		return false;

	if (stringData(m, m->d.data[handle]) != name)
		return false;

	int paramsIndex = m->d.data[handle + 2] + 1;
	for (int i = 0; i < argc; ++i) {
		uint typeInfo = m->d.data[paramsIndex + i];
		if (types[i].type()) {
			if (types[i].type() != typeFromTypeInfo(m, typeInfo))
				return false;
		}
		else {
			if (types[i].name() != typeNameFromTypeInfo(m, typeInfo))
				return false;
		}
	}

	return true;
}

template<int MethodType>
static inline int indexOfMethodRelative(const KvMetaObject **baseObject,
	const char *method,
	bool normalizeStringData)
{
	for (const KvMetaObject *m = *baseObject; m; m = m->superClass())
	{
		int i = (MethodType == Kv::MethodSignal)
			? (priv(m->d.data)->signalCount - 1) : (priv(m->d.data)->methodCount - 1);
		const int end = (MethodType == Kv::MethodSlot)
			? (priv(m->d.data)->signalCount) : 0;

		if (!normalizeStringData)
		{
			if (priv(m->d.data)->revision >= 7)
			{
				for (; i >= end; --i) {
					int handle = priv(m->d.data)->methodData + 5 * i;
					//if (methodMatch(m, handle, name, argc, types)) {
					//	*baseObject = m;
					//	return i;
					//}
				}
			}
#if 0
			else if (priv(m->d.data)->revision >= 4)
			{
				for (; i >= end; --i)
				{
					const char *stringdata = m->d.stringdata + m->d.data[priv(m->d.data)->methodData + 5 * i];
					if (method[0] == stringdata[0] && strcmp(method + 1, stringdata + 1) == 0)
					{
						*baseObject = m;
						return i;
					}
				}
			}
#endif
		}
#if 0
		else if ( priv(m->d.data)->revision < 5)
		{
			for (; i >= end; --i)
			{
				const char *stringdata = (m->d.stringdata + m->d.data[priv(m->d.data)->methodData + 5 * i]);
				const KvByteArray normalizedSignature = KvMetaObject::normalizedSignature(stringdata);
				if (normalizedSignature == method)
				{
					*baseObject = m;
					return i;
				}
			}
		}
#endif
	}

	return -1;
}


int KvMetaObjectPrivate::indexOfSignalRelative(const KvMetaObject **baseObject, const char* signal, bool normalizeStringData)
{
	int i = indexOfMethodRelative<Kv::MethodSignal>(baseObject, signal, normalizeStringData);
	const KvMetaObject *m = *baseObject;
	if (i >= 0 && m && m->d.superdata) {
		int conflict = m->d.superdata->indexOfMethod(signal);
		if (conflict >= 0)
			printf("KvMetaObject::indexOfSignal: signal %s from %s redefined in %s",
			signal, m->d.superdata->d.stringdata, m->d.stringdata);
	}
	return i;
}

int KvMetaObjectPrivate::indexOfSlotRelative(const KvMetaObject **m, const char *slot, bool normalizeStringData)
{
	return indexOfMethodRelative<Kv::MethodSlot>(m, slot, normalizeStringData);
}

int KvMetaObjectPrivate::originalClone(const KvMetaObject *mobj, int local_method_index)
{
	assert(local_method_index < get(mobj)->methodCount);
	int handle = get(mobj)->methodData + 5 * local_method_index;
	while (mobj->d.data[handle+4] & Kv::MethodCloned)
	{
		assert(local_method_index >0);
		handle -= 5;
		local_method_index--;
	}
	return local_method_index;
}

static void argumentTypesFromString(const char *str, const char *end,
	KvArgumentTypeArray &types)
{
	KV_ASSERT(str <= end);
	while (str != end) 
	{
		if (!types.isEmpty())
			++str; // Skip comma
		const char *begin = str;
		int level = 0;
		while (str != end && (level > 0 || *str != ',')) 
		{
			if (*str == '<')
				++level;
			else if (*str == '>')
				--level;
			++str;
		}
		types += KvArgumentType(KvByteArray(begin, str - begin));
	}
}

KvByteArray KvMetaObjectPrivate::decodeMethodSignature(const char *signature, KvArgumentTypeArray &types)
{
	KV_ASSERT(signature != 0);
	const char *lparens = strchr(signature, '(');
	if (!lparens)
		return KvByteArray();
	const char *rparens = strrchr(lparens + 1, ')');
	if (!rparens || *(rparens + 1))
		return KvByteArray();
	int nameLength = lparens - signature;
	argumentTypesFromString(lparens + 1, rparens, types);
	return KvByteArray::fromRawData(signature, nameLength);
}

//////////////////////////////////////////////////////////////////////////


const char * KvMetaObject::className() const
{
	//return d.stringdata;
	return objectClassName(this);
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

int KvMetaObject::methodOffset() const
{
	int offset = 0;
	const KvMetaObject *m = d.superdata;
	while (m) {
		offset += priv(m->d.data)->methodCount;
		m = m->d.superdata;
	}
	return offset;
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

int KvMetaObject::indexOfMethod(const char *method) const
{
	const KvMetaObject *m = this;
	int i = indexOfMethodRelative<Kv::MethodMethod>(&m, method, false);
	if (i < 0) {
		m = this;
		i = indexOfMethodRelative<Kv::MethodMethod>(&m, method, true);
	}
	if (i >= 0)
		i += m->methodOffset();
	return i;
}

int KvMetaObject::indexOfSignal(const char *signal) const
{
	const KvMetaObject *m = this;
	int i = KvMetaObjectPrivate::indexOfSignalRelative(&m, signal, false);
	if (i < 0) {
		m = this;
		i = KvMetaObjectPrivate::indexOfSignalRelative(&m, signal, true);
	}
	if (i >= 0)
		i += m->methodOffset();
	return i;
}

int KvMetaObject::indexOfSlot(const char *slot) const
{
	const KvMetaObject *m = this;
	int i = KvMetaObjectPrivate::indexOfSlotRelative(&m, slot, false);
	if (i < 0)
		i = KvMetaObjectPrivate::indexOfSlotRelative(&m, slot, true);
	if (i >= 0)
		i += m->methodOffset();
	return i;
}

int KvMetaObject::indexOfProperty(const char *name) const
{
	const KvMetaObject *m = this;
	while (m)
	{
		const KvMetaObjectPrivate *d = priv(m->d.data);
		for (int i = d->propertyCount - 1; i >= 0; --i)
		{
#if 0
			const char *prop = m->d.stringdata + m->d.data[d->propertyData + 3 * i];
#else
			const char *prop = rawStringData(m, m->d.data[d->propertyData + 3 * i]);
#endif
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
		//const char * type = d.stringdata + d.data[handle + 1];
		result.mobj = this;
		result.handle = handle;
		result.idx = i;

		if (flags & Kv::EnumOrFlag)
		{

		}
	}
	return result;
}

bool KvMetaObject::checkConnectArgs(const char *signal, const char *method)
{
	const char *s1 = signal;
	const char *s2 = method;
	while (*s1++ != '(') {}                        // scan to first '('
	while (*s2++ != '(') {}
	if (*s2 == ')' || kvstrcmp(s1, s2) == 0)        // method has no args or
		return true;                                //   exact match
	int s1len = kvstrlen(s1);
	int s2len = kvstrlen(s2);
	if (s2len < s1len && strncmp(s1, s2, s2len - 1) == 0 && s1[s2len - 1] == ',')
		return true;                                // method has less args
	return false;
}

#ifndef UTILS_H
static inline bool is_ident_char(char s)
{
	return ((s >= 'a' && s <= 'z')
		|| (s >= 'A' && s <= 'Z')
		|| (s >= '0' && s <= '9')
		|| s == '_'
		);
}

static inline bool is_space(char s)
{
	return (s == ' ' || s == '\t');
}
#endif

static void kvRemoveWhitespace(const char *s, char *d)
{
	char last = 0;
	while (*s && is_space(*s))
		s++;
	while (*s) {
		while (*s && !is_space(*s))
			last = *d++ = *s++;
		while (*s && is_space(*s))
			s++;
		if (*s && ((is_ident_char(*s) && is_ident_char(last))
			|| ((*s == ':') && (last == '<')))) {
			last = *d++ = ' ';
		}
	}
	*d = '\0';
}

static char *kvNormalizeType(char *d, int &templdepth, KvByteArray &result)
{
	const char *t = d;
	while (*d && (templdepth
		|| (*d != ',' && *d != ')'))) {
		if (*d == '<')
			++templdepth;
		if (*d == '>')
			--templdepth;
		++d;
	}
	//if (strncmp("void", t, d - t) != 0)
	//	result += normalizeTypeInternal(t, d);

	return d;
}

KvByteArray KvMetaObject::normalizedSignature(const char *method)
{
	KvByteArray result;
	if (!method || !*method)
		return result;
	int len = int(strlen(method));
	KvVarLengthArray<char> stackbuf(len + 1);
	char *d = stackbuf.data();
	kvRemoveWhitespace(method, d);

	result.reserve(len);

	int argdepth = 0;
	int templdepth = 0;
	while (*d) {
		if (argdepth == 1) {
			d = kvNormalizeType(d, templdepth, result);
			if (!*d) //most likely an invalid signature.
				break;
		}
		if (*d == '(')
			++argdepth;
		if (*d == ')')
			--argdepth;
		result += *d++;
	}

	return result;
}

KvByteArray KvMetaObject::normalizedType(const char *type)
{
	KvByteArray result;

	if (!type || !*type)
		return result;

	KvVarLengthArray<char> stackbuf(kvstrlen(type) + 1);
	kvRemoveWhitespace(type, stackbuf.data());
	int templdepth = 0;
	kvNormalizeType(stackbuf.data(), templdepth, result);

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
#if 0
	return mobj->d.stringdata + mobj->d.data[handle];
#else
	return rawStringData(mobj, mobj->d.data[handle]);
#endif

}

const char * KvMetaProperty::typeName() const
{
	if (!mobj)
		return 0;
	int handle = priv(mobj->d.data)->propertyData + 3 * idx;
#if 0
	return mobj->d.stringdata + mobj->d.data[handle + 1];
#else
	return rawTypeNameFromTypeInfo(mobj, mobj->d.data[handle + 1]);
#endif

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
		const char *typeName = 0;
#if 0
		typeName = mobj->d.stringdata + mobj->d.data[handle + 1];

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
#else
		uint typeInfo = mobj->d.data[handle + 1];
		if (!(typeInfo & Kv::IsUnresolvedType))
			t = typeInfo;
		else {
			typeName = rawStringData(mobj, typeInfo & Kv::TypeNameIndexMask);
			t = KvMetaType::type(typeName);
		}
		if (t == KvMetaType::UnknownType) {
			const char *typeName = rawStringData(mobj, typeInfo & Kv::TypeNameIndexMask);
		}
#endif
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
#if 0
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

#else
		const char *typeName = 0;
		KV_ASSERT(priv(mobj->d.data)->revision >= 7);
		uint typeInfo = mobj->d.data[handle + 1];
		if (!(typeInfo & Kv::IsUnresolvedType))
			t = typeInfo;
		else {
			typeName = rawStringData(mobj, typeInfo & Kv::TypeNameIndexMask);
			t = KvMetaType::type(typeName);
			//if (t == KvMetaType::UnknownType)
			//	t = registerPropertyType();
			//if (t == KvMetaType::UnknownType)
			//	return false;
		}
		if (t != KvMetaType::QVariant && int(t) != value.userType()) 
		{
			if (!value.isValid()) 
			{
				//if (isResettable())
				//	return reset(object);
				//v = QVariant(t, 0);
			}
			else if (!v.convert((KvVariant::Type)t)) 
			{
				return false;
			}
		}
#endif
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
