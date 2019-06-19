#include "KvVariant.h"

static void construct(KvVariant::Private *x, const void *copy)
{
	x->is_shared = false;

	switch (x->type)
	{
	case KvVariant::String:
		v_construct<KvString>(x, copy);
		break;
	case KvVariant::Int:
		x->data.i = copy ? *static_cast<const int *>(copy) : 0;
		break;
	case KvVariant::UInt:
		x->data.u = copy ? *static_cast<const uint *>(copy) : 0u;
		break;
	case KvVariant::Bool:
		x->data.b = copy ? *static_cast<const bool *>(copy) : false;
		break;
	case KvVariant::Double:
		x->data.d = copy ? *static_cast<const double*>(copy) : 0.0;
		break;
	case KvMetaType::Float:
		x->data.f = copy ? *static_cast<const float*>(copy) : 0.0f;
		break;
	case KvMetaType::QObjectStar:
		x->data.o = copy ? *static_cast<KvObject *const*>(copy) : 0;
		break;
	case KvVariant::LongLong:
		x->data.ll = copy ? *static_cast<const klonglong *>(copy) : K_INT64_C(0);
		break;
	case KvVariant::ULongLong:
		x->data.ull = copy ? *static_cast<const kulonglong *>(copy) : K_UINT64_C(0);
		break;
	case KvVariant::Invalid:
	case KvVariant::UserType:
		break;
	default:
		void *ptr = KvMetaType::construct(x->type, copy);
		if (!ptr) {
			x->type = KvVariant::Invalid;
		}
		else {
			x->is_shared = true;
			x->data.shared = new KvVariant::PrivateShared(ptr);
		}
		break;
	}
	x->is_null = !copy;
}

static void clear(KvVariant::Private *d)
{
	switch (d->type) {
	case KvVariant::String:
		v_clear<KvString>(d);
		break;
	case KvVariant::LongLong:
	case KvVariant::ULongLong:
	case KvVariant::Double:
	case KvMetaType::Float:
	case KvMetaType::QObjectStar:
		break;
	case KvVariant::Invalid:
	case KvVariant::UserType:
	case KvVariant::Int:
	case KvVariant::UInt:
	case KvVariant::Bool:
		break;
	default:
		KvMetaType::destroy(d->type, d->data.shared->ptr);
		delete d->data.shared;
		break;
	}

	d->type = KvVariant::Invalid;
	d->is_null = true;
	d->is_shared = false;
}

static bool isNull(const KvVariant::Private *d)
{
	switch (d->type) {
	//case KvVariant::String:
	//	return v_cast<KvString>(d)->isNull();
	case KvVariant::Locale:
	case KvVariant::RegExp:
	case KvVariant::StringList:
	case KvVariant::Map:
	case KvVariant::Hash:
	case KvVariant::List:
	case KvVariant::Invalid:
	case KvVariant::UserType:
	case KvVariant::Int:
	case KvVariant::UInt:
	case KvVariant::LongLong:
	case KvVariant::ULongLong:
	case KvVariant::Bool:
	case KvVariant::Double:
	case KvMetaType::Float:
	case KvMetaType::QObjectStar:
		break;
	}
	return d->is_null;
}

template<typename T>
inline bool compareNumericMetaType(const KvVariant::Private *const a, const KvVariant::Private *const b)
{
	return *static_cast<const T *>(a->data.shared->ptr) == *static_cast<const T *>(b->data.shared->ptr);
}

static bool compare(const KvVariant::Private *a, const KvVariant::Private *b)
{
	return false;
}

static bool convert(const KvVariant::Private *d, KvVariant::Type t, void *result, bool *ok)
{
	return false;
}

const KvVariant::Handler kv_kernel_variant_handler = {
	construct,
	clear,
	isNull,
#ifdef KV_DATASTREAM
	0,
	0,
#endif
	compare,
	convert,
	0,
#if defined(KV_DEBUG_STREAM)
	streamDebug
#endif
};

KV_CORE_EXPORT const KvVariant::Handler *coreVariantHandler()
{
	return &kv_kernel_variant_handler;
}

const KvVariant::Handler *KvVariant::handler = &kv_kernel_variant_handler;


KvVariant::~KvVariant()
{
}

KvVariant::KvVariant(Type type)
{
	create(type, 0);
}

KvVariant::KvVariant(int typeOrUserType, const void *copy)
{
	create(typeOrUserType, copy); 
	d.is_null = false;
}

KvVariant::KvVariant(int typeOrUserType, const void *copy, uint flags)
{
	if (flags) { //type is a pointer type
		d.type = typeOrUserType;
		d.data.ptr = *reinterpret_cast<void *const*>(copy);
		d.is_null = false;
	}
	else {
		create(typeOrUserType, copy);
		d.is_null = false;
	}
}

KvVariant::KvVariant(const KvVariant &other)
	: d(other.d)
{
	if (d.is_shared) 
	{
		//d.data.shared->ref.ref();
	}
	else if (other.d.type > Char && other.d.type < KvVariant::UserType) 
	{
		handler->construct(&d, other.constData());
		d.is_null = other.d.is_null;
	}
}

KvVariant::KvVariant(int val)
{
	d.is_null = false; d.type = Int; d.data.i = val;
}

KvVariant::KvVariant(uint val)
{
	d.is_null = false; d.type = UInt; d.data.u = val;
}

KvVariant::KvVariant(klonglong val)
{
	d.is_null = false; d.type = LongLong; d.data.ll = val;
}

KvVariant::KvVariant(kulonglong val)
{
	d.is_null = false; d.type = ULongLong; d.data.ull = val;
}

KvVariant::KvVariant(bool val)
{
	d.is_null = false; d.type = Bool; d.data.b = val;
}

KvVariant::KvVariant(double val)
{
	d.is_null = false; d.type = Double; d.data.d = val;
}

KvVariant::KvVariant(const KvString &val)
{
	d.is_null = false; 
	d.type = String; 
	v_construct<KvString>(&d, val);
}

KvVariant::Type KvVariant::type() const
{
	return d.type >= KvMetaType::User 
		? KvVariant::UserType : static_cast<Type>(d.type);
}

int KvVariant::userType() const
{
	return d.type;
}

const char * KvVariant::typeName() const
{
	return typeToName(Type(d.type));
}

bool KvVariant::canConvert(Type t) const
{
	//we can treat floats as double
	//the reason for not doing it the "proper" way is that QMetaType::Float's value is 135,
	//which can't be handled by canConvertMatrix
	//In addition KvVariant::Type doesn't have a Float value, so we're using KvMetaType::Float
	const uint currentType = ((d.type == KvMetaType::Float) ? KvVariant::Double : d.type);
	if (uint(t) == uint(KvMetaType::Float)) t = KvVariant::Double;

	if (currentType == uint(t))
	{
		return true;
	}

	if (currentType > KvVariant::LastCoreType || t > KvVariant::LastCoreType)
	{
	}

	if (t == String && currentType == StringList)
		return v_cast<KvStringList>(&d)->/*count*/size() == 1;
	//else
	//	return qCanConvertMatrix[t] & (1 << currentType);

	return false;
}

bool KvVariant::convert(Type t)
{
	if (d.type == uint(t))
	{
		return true;
	}

	KvVariant oldValue = *this;

	clear();
	if (!oldValue.canConvert(t))
	{
		return false;
	}

	create(t, 0);
	if (oldValue.isNull())
	{
		return false;
	}

	bool isOK = true;
	if (!handler->convert(&oldValue.d, t, data(), &isOK))
	{
		isOK = false;
	}
	d.is_null = !isOK;
	return isOK;
}

bool KvVariant::isNull() const
{
	return handler->isNull(&d);
}

void KvVariant::clear()
{
	if ((d.is_shared /*&& !d.data.shared->ref.deref()*/)
		|| (!d.is_shared && d.type < UserType && d.type > KvVariant::Char) )
	{
		handler->clear(&d);
	}
	d.type = Invalid;
	d.is_null = true;
	d.is_shared = false;
}

void KvVariant::detach()
{
	if (!d.is_shared /*|| d.data.shared->ref == 1*/)
		return;

	Private dd;
	dd.type = d.type;
	handler->construct(&dd, constData());
	//if (!d.data.shared->ref.deref())
	//	handler->clear(&d);
	d.data.shared = dd.data.shared;
}

template<typename T>
inline T kNumVariantToHelper(const KvVariant::Private &d,
	const KvVariant::Handler *handler, bool *ok, const T &val)
{
	uint t = kvMetaTypeId<T>();
	if (ok)
		*ok = true;
	if (d.type == t)
		return val;

	T ret;
	//if (!handler->convert(&d, QVariant::Type(t), &ret, ok) && ok)
	//	*ok = false;
	return ret;
}

int KvVariant::toInt(bool *ok /*= 0*/) const
{
	return kNumVariantToHelper<int>(d, handler, ok, d.data.i);
}

template <typename T>
inline T kVariantToHelper(const KvVariant::Private &d, KvVariant::Type t,
	const KvVariant::Handler *handler, T * = 0)
{
	if (d.type == t)
		return *v_cast<T>(&d);

	T ret;
	handler->convert(&d, t, &ret, 0);
	return ret;
}

bool KvVariant::toBool() const
{
	if (d.type == Bool)
		return d.data.b;

	bool res = false;
	handler->convert(&d, Bool, &res, 0);

	return res;
}

KvString KvVariant::toString() const
{
	return kVariantToHelper<KvString>(d, String, handler);
}

const char * KvVariant::typeToName(Type type)
{
	if (type == Invalid)
	{
		return 0;
	}
	if (type == UserType)
	{
		return "UserType";
	}

	return KvMetaType::typeName(type);
}

KvVariant::Type KvVariant::nameToType(const char *name)
{
	if (!name || !*name)
		return Invalid;
	if (strcmp(name, "UserType") == 0)
		return UserType;

	int metaType = KvMetaType::type(name);
	return metaType <= int(LastGuiType) ? KvVariant::Type(metaType) : UserType;
}

const void * KvVariant::constData() const
{
	return d.is_shared ? d.data.shared->ptr : reinterpret_cast<const void *>(&d.data.ptr);
}

void * KvVariant::data()
{
	detach();
	return const_cast<void *>(constData());
}

void KvVariant::create(int type, const void *copy)
{
	d.type = type;
	handler->construct(&d, copy);
}
