#ifndef KvVariant_h__
#define KvVariant_h__

#include "Kvms.h"
#include "KvString.h"
#include "KvMetaType.h"

class KvObject;

class KV_CORE_EXPORT KvVariant
{
public:
	enum Type {
		Invalid = 0,

		Bool = 1,
		Int = 2,
		UInt = 3,
		LongLong = 4,
		ULongLong = 5,
		Double = 6,
		Char = 7,
		Map = 8,
		List = 9,
		String = 10,
		StringList = 11,
		ByteArray = 12,
		BitArray = 13,
		Date = 14,
		Time = 15,
		DateTime = 16,
		Url = 17,
		Locale = 18,
		Rect = 19,
		RectF = 20,
		Size = 21,
		SizeF = 22,
		Line = 23,
		LineF = 24,
		Point = 25,
		PointF = 26,
		RegExp = 27,
		Hash = 28,
		EasingCurve = 29,
		LastCoreType = EasingCurve,

		// value 62 is internally reserved
		//ColorGroup = 63,

		Font = 64,
		Pixmap = 65,
		Brush = 66,
		Color = 67,
		Palette = 68,
		Icon = 69,
		Image = 70,
		Polygon = 71,
		Region = 72,
		Bitmap = 73,
		Cursor = 74,
		SizePolicy = 75,
		KeySequence = 76,
		Pen = 77,
		TextLength = 78,
		TextFormat = 79,
		Matrix = 80,
		Transform = 81,
		Matrix4x4 = 82,
		Vector2D = 83,
		Vector3D = 84,
		Vector4D = 85,
		Quaternion = 86,
		LastGuiType = Quaternion,

		UserType = 127,

		LastType = 0xffffffff // need this so that gcc >= 3.4 allocates 32 bits for Type
	};

	inline KvVariant();
	~KvVariant();
	KvVariant(Type type);
	KvVariant(int typeOrUserType, const void *copy);
	KvVariant(int typeOrUserType, const void *copy, uint flags);
	KvVariant(const KvVariant &other);

	KvVariant(const KvString &string);

	Type type() const;
	int userType() const;
	const char *typeName() const;

	bool canConvert(Type t) const;
	bool convert(Type t);

	inline bool isValid() const;
	bool isNull() const;

	void clear();

	void detach();
	inline bool isDetached() const;

	int toInt(bool *ok = 0) const;
	uint toUInt(bool *ok = 0) const;
	qlonglong toLongLong(bool *ok = 0) const;
	qulonglong toULongLong(bool *ok = 0) const;
	bool toBool() const;
	double toDouble(bool *ok = 0) const;
	float toFloat(bool *ok = 0) const;

	KvString toString() const;

#ifdef KV_DATASTREAM
	void load(QDataStream &ds);
	void save(QDataStream &ds) const;
#endif
	static const char *typeToName(Type type);
	static Type nameToType(const char *name);

	void *data();
	const void *constData() const;
	inline const void *data() const { return constData(); }


////////////////////////////////Private//////////////////////////////////////////
public:
	struct PrivateShared
	{
		inline PrivateShared(void *v) : ptr(v)/*, ref(1)*/ { }
		void *ptr;
		//QAtomicInt ref;
	};
	struct Private
	{
		inline Private() : type(Invalid), is_shared(false), is_null(true) { data.ptr = 0; }
		inline Private(const Private &other)
			: data(other.data), type(other.type),
			is_shared(other.is_shared), is_null(other.is_null)
		{}

		union Data
		{
			char c;
			int i;
			uint u;
			bool b;
			double d;
			float f;
			qreal real;
			qlonglong ll;
			qulonglong ull;
			KvObject *o;
			void *ptr;
			PrivateShared *shared;
		} data;
		uint type : 30;
		uint is_shared : 1;
		uint is_null : 1;
	};

////////////////////////////////Function//////////////////////////////////////////
public:
	 typedef void(*f_construct)(Private *, const void *);
	 typedef void(*f_clear)(Private *);
	 typedef bool(*f_null)(const Private *);
#ifdef KV_DATASTREAM
	 typedef void(*f_load)(Private *, QDataStream &);
	 typedef void(*f_save)(const Private *, QDataStream &);
#endif
	 typedef bool(*f_compare)(const Private *, const Private *);
	 typedef bool(*f_convert)(const KvVariant::Private *d, Type t, void *, bool *);
	 typedef bool(*f_canConvert)(const KvVariant::Private *d, Type t);
	 //typedef void(*f_debugStream)(QDebug, const QVariant &);
	 struct Handler {
		 f_construct construct;
		 f_clear clear;
		 f_null isNull;
#ifdef KV_DATASTREAM
		 f_load load;
		 f_save save;
#endif
		 f_compare compare;
		 f_convert convert;
		 f_canConvert canConvert;
		 //f_debugStream debugStream;
	 };

protected:
	Private d;

	static const Handler *handler;

	void create(int type, const void *copy);
};

inline KvVariant::KvVariant() {}
inline bool KvVariant::isValid() const { return d.type != Invalid; }
inline bool KvVariant::isDetached() const
{
	return !d.is_shared /*|| d.data.shared->ref == 1*/;
}

template <typename T>
inline const T *v_cast(const KvVariant::Private *d, T * = 0)
{
	return ((sizeof(T) > sizeof(KvVariant::Private::Data))
		? static_cast<const T *>(d->data.shared->ptr)
		: static_cast<const T *>(static_cast<const void *>(&d->data.c)));
}

template <typename T>
inline T *v_cast(KvVariant::Private *d, T * = 0)
{
	return ((sizeof(T) > sizeof(KvVariant::Private::Data))
		? static_cast<T *>(d->data.shared->ptr)
		: static_cast<T *>(static_cast<void *>(&d->data.c)));
}

//a simple template that avoids to allocate 2 memory chunks when creating a QVariant
template <class T> class KvVariantPrivateSharedEx : public KvVariant::PrivateShared
{
public:
	KvVariantPrivateSharedEx() : KvVariant::PrivateShared(&m_t) { }
	KvVariantPrivateSharedEx(const T&t) : KvVariant::PrivateShared(&m_t), m_t(t) { }

private:
	T m_t;
};

// constructs a new variant if copy is 0, otherwise copy-constructs
template <class T>
inline void v_construct(KvVariant::Private *x, const void *copy, T * = 0)
{
	if (sizeof(T) > sizeof(KvVariant::Private::Data)) {
		x->data.shared = copy ? new KvVariantPrivateSharedEx<T>(*static_cast<const T *>(copy))
			: new KvVariantPrivateSharedEx<T>;
		x->is_shared = true;
	}
	else {
		if (copy)
			new (&x->data.ptr) T(*static_cast<const T *>(copy));
		else
			new (&x->data.ptr) T;
	}
}

template <class T>
inline void v_construct(KvVariant::Private *x, const T &t)
{
	if (sizeof(T) > sizeof(KvVariant::Private::Data)) {
		x->data.shared = new KvVariantPrivateSharedEx<T>(t);
		x->is_shared = true;
	}
	else {
		new (&x->data.ptr) T(t);
	}
}

// deletes the internal structures
template <class T>
inline void v_clear(KvVariant::Private *d, T* = 0)
{

	if (sizeof(T) > sizeof(KvVariant::Private::Data)) {
		//now we need to cast
		//because KvVariant::PrivateShared doesn't have a virtual destructor
		delete static_cast<KvVariantPrivateSharedEx<T>*>(d->data.shared);
	}
	else {
		v_cast<T>(d)->~T();
	}

}

KV_CORE_EXPORT const KvVariant::Handler *coreVariantHandler();

#endif // KvVariant_h__
