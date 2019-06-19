#include "KvMetaType.h"
#include "KvMetaObject.h"

#define KV_ADD_STATIC_METATYPE(STR, TP) \
	{ STR, sizeof(STR)-1, TP }

/* Note: these MUST be in the order of the enums */
static const struct { const char * typeName; int typeNameLength; int type; } types[] = {

	/* All Core types */
	KV_ADD_STATIC_METATYPE("void", KvMetaType::Void),
	KV_ADD_STATIC_METATYPE("bool", KvMetaType::Bool),
	KV_ADD_STATIC_METATYPE("int", KvMetaType::Int),
	KV_ADD_STATIC_METATYPE("uint", KvMetaType::UInt),
	KV_ADD_STATIC_METATYPE("klonglong", KvMetaType::LongLong),
	KV_ADD_STATIC_METATYPE("kulonglong", KvMetaType::ULongLong),
	KV_ADD_STATIC_METATYPE("double", KvMetaType::Double),
	KV_ADD_STATIC_METATYPE("QChar", KvMetaType::QChar),
	KV_ADD_STATIC_METATYPE("QVariantMap", KvMetaType::QVariantMap),
	KV_ADD_STATIC_METATYPE("QVariantList", KvMetaType::QVariantList),
	KV_ADD_STATIC_METATYPE("KvString", KvMetaType::KvString),

	// let QMetaTypeId2 figure out the type at compile time
	//KV_ADD_STATIC_METATYPE("kreal", QMetaTypeId2<kreal>::MetaType),

	{ 0, 0, KvMetaType::Void }
};

/*! \internal
Similar to type(), but only looks in the static set of types.
*/
static inline int kvMetaTypeStaticType(const char *typeName, int length)
{
	int i = 0;
	while (types[i].typeName && ((length != types[i].typeNameLength)
		|| strcmp(typeName, types[i].typeName))) {
		++i;
	}
	return types[i].type;
}

static int kvMetaTypeCustomType_unlocked(const char *typeName, int length, int *firstInvalidIndex = 0)
{
	//const QVector<QCustomTypeInfo> * const ct = customTypes();
	//if (!ct)
	//	return KvMetaType::UnknownType;

	//if (firstInvalidIndex)
	//	*firstInvalidIndex = -1;
	//for (int v = 0; v < ct->count(); ++v) {
	//	const QCustomTypeInfo &customInfo = ct->at(v);
	//	if ((length == customInfo.typeName.size())
	//		&& !memcmp(typeName, customInfo.typeName.constData(), length)) {
	//		if (customInfo.alias >= 0)
	//			return customInfo.alias;
	//		return v + QMetaType::User;
	//	}
	//	if (firstInvalidIndex && (*firstInvalidIndex < 0) && customInfo.typeName.isEmpty())
	//		*firstInvalidIndex = v;
	//}
	return KvMetaType::UnknownType;
}

int KvMetaType::type(const char *typeName)
{
	int length = kvstrlen(typeName);
	if (!length)
		return 0;
	int type = kvMetaTypeStaticType(typeName, length);
	if (!type) 
	{
		//用户自定义数据类型;
	}
	return type;
}

const char * KvMetaType::typeName(int type)
{
	enum { GuiTypeCount = LastGuiType - FirstGuiType };

	if (type >= 0 && type <= LastCoreType) 
	{
		return types[type].typeName;
	}
	else if (type >= FirstGuiType && type <= LastGuiType) 
	{
		return types[type - FirstGuiType + LastCoreType + 1].typeName;
	}
	else if (type >= FirstCoreExtType && type <= LastCoreExtType) 
	{
		return types[type - FirstCoreExtType + GuiTypeCount + LastCoreType + 2].typeName;
	}
	else if (type >= User) 
	{
		//用户自定义数据类型;
	}

	return 0;
}

bool KvMetaType::isRegistered(int type)
{
	if (type >= 0 && type < User) {
		// predefined type
		return true;
	}
	//locker here TODO
	//const QVector<QCustomTypeInfo> * const ct = customTypes();
	return ((type >= User) 
		/*		&& (ct && ct->count() > type - User)
				&& !ct->at(type - User).typeName.isEmpty()*/);
}

void * KvMetaType::construct(int type, const void *copy /*= 0*/)
{
	if (copy)
	{
		switch (type) {
		case KvMetaType::VoidStar:
		case KvMetaType::QObjectStar:
		case KvMetaType::QWidgetStar:
			return new void *(*static_cast<void* const *>(copy));
		case KvMetaType::Long:
			return new long(*static_cast<const long*>(copy));
		case KvMetaType::Int:
			return new int(*static_cast<const int*>(copy));
		case KvMetaType::Short:
			return new short(*static_cast<const short*>(copy));
		case KvMetaType::Char:
			return new char(*static_cast<const char*>(copy));
		case KvMetaType::ULong:
			return new ulong(*static_cast<const ulong*>(copy));
		case KvMetaType::UInt:
			return new uint(*static_cast<const uint*>(copy));
		case KvMetaType::LongLong:
			return new klonglong(*static_cast<const klonglong*>(copy));
		case KvMetaType::ULongLong:
			return new kulonglong(*static_cast<const kulonglong*>(copy));
		case KvMetaType::UShort:
			return new ushort(*static_cast<const ushort*>(copy));
		case KvMetaType::UChar:
			return new uchar(*static_cast<const uchar*>(copy));
		case KvMetaType::Bool:
			return new bool(*static_cast<const bool*>(copy));
		case KvMetaType::Float:
			return new float(*static_cast<const float*>(copy));
		case KvMetaType::Double:
			return new double(*static_cast<const double*>(copy));

		case KvMetaType::KvString:
			return new NS(KvString)(*static_cast<const NS(KvString)*>(copy));

		case KvMetaType::Void:
			return 0;
		default:
			;
		}
	}
	else
	{
		switch (type) {
		case KvMetaType::VoidStar:
		case KvMetaType::QObjectStar:
		case KvMetaType::QWidgetStar:
			return new void *;
		case KvMetaType::Long:
			return new long;
		case KvMetaType::Int:
			return new int;
		case KvMetaType::Short:
			return new short;
		case KvMetaType::Char:
			return new char;
		case KvMetaType::ULong:
			return new ulong;
		case KvMetaType::UInt:
			return new uint;
		case KvMetaType::LongLong:
			return new klonglong;
		case KvMetaType::ULongLong:
			return new kulonglong;
		case KvMetaType::UShort:
			return new ushort;
		case KvMetaType::UChar:
			return new uchar;
		case KvMetaType::Bool:
			return new bool;
		case KvMetaType::Float:
			return new float;
		case KvMetaType::Double:
			return new double;

		case KvMetaType::KvString:
			return new NS(KvString);

		case KvMetaType::Void:
			return 0;
		default:
			;
		}
	}

#if 0 //自定义数据类型;
	Constructor constr = 0;
	if (type >= FirstGuiType && type <= LastGuiType) {
		if (!qMetaTypeGuiHelper)
			return 0;
		constr = qMetaTypeGuiHelper[type - FirstGuiType].constr;
	}
	else {
		const QVector<QCustomTypeInfo> * const ct = customTypes();
		QReadLocker locker(customTypesLock());
		if (type < User || !ct || ct->count() <= type - User)
			return 0;
		if (ct->at(type - User).typeName.isEmpty())
			return 0;
		constr = ct->at(type - User).constr;
	}

	return constr(copy);
#endif
}

void KvMetaType::destroy(int type, void *data)
{
	if (!data)
		return;
	switch (type) {
	case KvMetaType::VoidStar:
	case KvMetaType::QObjectStar:
	case KvMetaType::QWidgetStar:
		delete static_cast<void**>(data);
		break;
	case KvMetaType::Long:
		delete static_cast<long*>(data);
		break;
	case KvMetaType::Int:
		delete static_cast<int*>(data);
		break;
	case KvMetaType::Short:
		delete static_cast<short*>(data);
		break;
	case KvMetaType::Char:
		delete static_cast<char*>(data);
		break;
	case KvMetaType::ULong:
		delete static_cast<ulong*>(data);
		break;
	case KvMetaType::LongLong:
		delete static_cast<klonglong*>(data);
		break;
	case KvMetaType::ULongLong:
		delete static_cast<kulonglong*>(data);
		break;
	case KvMetaType::UInt:
		delete static_cast<uint*>(data);
		break;
	case KvMetaType::UShort:
		delete static_cast<ushort*>(data);
		break;
	case KvMetaType::UChar:
		delete static_cast<uchar*>(data);
		break;
	case KvMetaType::Bool:
		delete static_cast<bool*>(data);
		break;
	case KvMetaType::Float:
		delete static_cast<float*>(data);
		break;
	case KvMetaType::Double:
		delete static_cast<double*>(data);
		break;

	case KvMetaType::KvString:
		delete static_cast<NS(KvString)*>(data);
		break;

	case KvMetaType::Void:
		break;
	default: {
#if 0
		const QVector<QCustomTypeInfo> * const ct = customTypes();
		Destructor destr = 0;
		if (type >= FirstGuiType && type <= LastGuiType) {
			Q_ASSERT(qMetaTypeGuiHelper);

			if (!qMetaTypeGuiHelper)
				return;
			destr = qMetaTypeGuiHelper[type - FirstGuiType].destr;
		} else {
			QReadLocker locker(customTypesLock());
			if (type < User || !ct || ct->count() <= type - User)
				break;
			if (ct->at(type - User).typeName.isEmpty())
				break;
			destr = ct->at(type - User).destr;
		}
		destr(data);
#endif
		break; }
	}
}

void KvMetaType::unregisterType(const char *typeName)
{
#if 0
	QVector<QCustomTypeInfo> *ct = customTypes();
	if (!ct || !typeName)
		return;

#ifdef QT_NO_QOBJECT
	NS(QByteArray) normalizedTypeName = typeName;
#else
	NS(QByteArray) normalizedTypeName = QMetaObject::normalizedType(typeName);
#endif
	QWriteLocker locker(customTypesLock());
	for (int v = 0; v < ct->count(); ++v) {
		if (ct->at(v).typeName == typeName) {
			QCustomTypeInfo &inf = (*ct)[v];
			inf.typeName.clear();
			inf.constr = 0;
			inf.destr = 0;
			inf.alias = -1;
		}
	}
#endif
}

template <bool tryNormalizedType>
static inline int kvMetaTypeTypeImpl(const char *typeName, int length)
{
	if (!length)
		return KvMetaType::UnknownType;
	int type = kvMetaTypeStaticType(typeName, length);
	if (type == KvMetaType::UnknownType) {
		//QReadLocker locker(customTypesLock());
		type = kvMetaTypeCustomType_unlocked(typeName, length);
		if ((type == KvMetaType::UnknownType) && tryNormalizedType) {
			const NS(KvByteArray) normalizedTypeName = KvMetaObject::normalizedType(typeName);
			type = kvMetaTypeStaticType(normalizedTypeName.constData(),
				normalizedTypeName.size());
			if (type == KvMetaType::UnknownType) {
				type = kvMetaTypeCustomType_unlocked(normalizedTypeName.constData(),
					normalizedTypeName.size());
			}
		}
	}
	return type;
}

int kvMetaTypeTypeInternal(const char *typeName)
{
	return kvMetaTypeTypeImpl</*tryNormalizedType=*/false>(typeName, kvstrlen(typeName));
}