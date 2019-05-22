#ifndef KvMetaType_h__
#define KvMetaType_h__

#include "Kvms.h"

class KV_CORE_EXPORT KvMetaType
{
public:
	enum Type {
		// these are merged with KvVariant
		Void = 0, Bool = 1, Int = 2, UInt = 3, LongLong = 4, ULongLong = 5,
		Double = 6, QChar = 7, QVariantMap = 8, QVariantList = 9,
		QString = 10, QStringList = 11, QByteArray = 12, QBitArray = 13,
		QDate = 14, QTime = 15, QDateTime = 16, QUrl = 17,
		QLocale = 18, QRect = 19, QRectF = 20, QSize = 21, QSizeF = 22,
		QLine = 23, QLineF = 24, QPoint = 25, QPointF = 26, QRegExp = 27,
		QVariantHash = 28, QEasingCurve = 29, LastCoreType = QEasingCurve,

		FirstGuiType = 63 /* QColorGroup */,

		QFont = 64, QPixmap = 65, QBrush = 66, QColor = 67, QPalette = 68,
		QIcon = 69, QImage = 70, QPolygon = 71, QRegion = 72, QBitmap = 73,
		QCursor = 74, QSizePolicy = 75, QKeySequence = 76, QPen = 77,
		QTextLength = 78, QTextFormat = 79, QMatrix = 80, QTransform = 81,
		QMatrix4x4 = 82, QVector2D = 83, QVector3D = 84, QVector4D = 85,
		QQuaternion = 86,
		LastGuiType = QQuaternion,

		FirstCoreExtType = 128 /* VoidStar */,
		VoidStar = 128, Long = 129, Short = 130, Char = 131, ULong = 132,
		UShort = 133, UChar = 134, Float = 135, QObjectStar = 136, QWidgetStar = 137,
		QVariant = 138,
		LastCoreExtType = QVariant,

		// This logic must match the one in KvGlobal.h
#if defined(KV_NO_FPU) || defined(KV_ARCH_ARM) || defined(KV_ARCH_WINDOWSCE) || defined(KV_ARCH_SYMBIAN)
		QReal = Float,
#else
		QReal = Double,
#endif

		User = 256
	};

	static int type(const char *typeName);
	static const char *typeName(int type);
	static bool isRegistered(int type);
	static void *construct(int type, const void *copy = 0);
	static void destroy(int type, void *data);
	static void unregisterType(const char *typeName);
};

template<typename T>
struct  KvMetaTypeId
{
	enum { Defined = 0 };
};

template<typename T>
struct  KvMetaTypeId2
{
	enum { Defined = KvMetaTypeId<T>::Defined };
	static inline int kv_metatype_id() { return KvMetaTypeId<T>::kv_metatype_id(); }
};

template <typename T>
inline int kMetaTypeId(T * /* dummy */ = 0)
{
	return KvMetaTypeId2<T>::kv_metatype_id();
}

#define K_DECLARE_BUILTIN_METATYPE(TYPE, NAME) \
	template<> struct KvMetaTypeId2<TYPE> \
	{ \
	enum { Defined = 1, MetaType = KvMetaType::NAME }; \
		static inline int kv_metatype_id() { return KvMetaType::NAME; } \
	}; 


#include <string>	//KvString == std::string
K_DECLARE_BUILTIN_METATYPE(std::string,QString)
K_DECLARE_BUILTIN_METATYPE(int, Int)
K_DECLARE_BUILTIN_METATYPE(uint, UInt)
K_DECLARE_BUILTIN_METATYPE(bool, Bool)
K_DECLARE_BUILTIN_METATYPE(double, Double)
//K_DECLARE_BUILTIN_METATYPE(QByteArray, QByteArray)
//K_DECLARE_BUILTIN_METATYPE(QChar, QChar)
K_DECLARE_BUILTIN_METATYPE(long, Long)
K_DECLARE_BUILTIN_METATYPE(short, Short)
K_DECLARE_BUILTIN_METATYPE(char, Char)
K_DECLARE_BUILTIN_METATYPE(signed char, Char)
K_DECLARE_BUILTIN_METATYPE(ulong, ULong)
K_DECLARE_BUILTIN_METATYPE(ushort, UShort)
K_DECLARE_BUILTIN_METATYPE(uchar, UChar)
K_DECLARE_BUILTIN_METATYPE(float, Float)
//K_DECLARE_BUILTIN_METATYPE(QObject *, QObjectStar)
//K_DECLARE_BUILTIN_METATYPE(QWidget *, QWidgetStar)
K_DECLARE_BUILTIN_METATYPE(void *, VoidStar)


#endif // KvMetaType_h__
