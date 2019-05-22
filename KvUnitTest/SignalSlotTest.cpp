#include "SignalSlotTest.h"


SignalSlotTest::SignalSlotTest()
{
}


SignalSlotTest::~SignalSlotTest()
{
}

void SignalSlotTest::SayHello()
{
	printf("sayHello....\n");
}

void SignalSlotTest::CallOneParam(int)
{
	printf("oneParam....\n");
}

void SignalSlotTest::CallTwoParam(int, bool)
{
	printf("twoParam....\n");
}

//////////////////////////////////////////////////////////////////////////

static const uint qt_meta_data_KvMode[] = {

	// content:
	6,       // revision
	0,       // classname
	0, 0, // classinfo
	4, 14, // methods
	1, 34, // properties
	0, 0, // enums/sets
	0, 0, // constructors
	0,       // flags
	1,       // signalCount

	// signals: signature, parameters, type, tag, flags
	8 + 8, 7, 7, 7, 0x05,

	// slots: signature, parameters, type, tag, flags
	37 + 2 *8, 7, 7, 7, 0x0a,
	48 + 2 * 8, 7, 7, 7, 0x0a,
	68 + 2 * 8, 66, 7, 7, 0x0a,

	// properties: name, type, flags
	96 + 2 * 8, 91, 0x01095103,

	0        // eod
};

//SignalSlotTest 14
//KvMode 6
static const char qt_meta_stringdata_KvMode[] = {
	"SignalSlotTest\0\0enabledStateChanged(SignalSlotTest*)\0"
	"SayHello()\0CallOneParam(int)\0,\0"
	"CallTwoParam(int,bool)\0bool\0enabled\0"
};

void SignalSlotTest::kv_static_metacall(KvObject *_o, KvMetaObject::Call _c, int _id, void **_a)
{
	if (_c == KvMetaObject::InvokeMetaMethod) {
		assert(staticMetaObject.cast(_o));
		SignalSlotTest *_t = static_cast<SignalSlotTest *>(_o);
		switch (_id) {
		case 0: _t->enabledStateChanged((*reinterpret_cast<SignalSlotTest*(*)>(_a[1]))); break;
		case 1: _t->SayHello(); break;
		case 2: _t->CallOneParam((*reinterpret_cast<int(*)>(_a[1]))); break;
		case 3: _t->CallTwoParam((*reinterpret_cast<int(*)>(_a[1])), (*reinterpret_cast<bool(*)>(_a[2]))); break;
		default:;
		}
	}
}

const KvMetaObjectExtraData SignalSlotTest::staticMetaObjectExtraData = {
	0, kv_static_metacall
};

const KvMetaObject SignalSlotTest::staticMetaObject = {
	{ &KvObject::staticMetaObject, qt_meta_stringdata_KvMode,
	qt_meta_data_KvMode, &staticMetaObjectExtraData }
};

#ifdef K_NO_DATA_RELOCATION
const KvMetaObject &KvMode::getStaticMetaObject() { return staticMetaObject; }
#endif //K_NO_DATA_RELOCATION

const KvMetaObject *SignalSlotTest::metaObject() const
{
	return KvObject::d_ptr->metaObject ? KvObject::d_ptr->metaObject : &staticMetaObject;
}

void *SignalSlotTest::kv_metacast(const char *_clname)
{
	if (!_clname) return 0;
	if (!strcmp(_clname, qt_meta_stringdata_KvMode))
		return static_cast<void*>(const_cast<SignalSlotTest*>(this));
	return KvObject::kv_metacast(_clname);
}

int SignalSlotTest::kv_metacall(KvMetaObject::Call _c, int _id, void **_a)
{
	_id = KvObject::kv_metacall(_c, _id, _a);
	if (_id < 0)
		return _id;
	if (_c == KvMetaObject::InvokeMetaMethod) {
		if (_id < 1)
			kv_static_metacall(this, _c, _id, _a);
		_id -= 1;
	}
	else if (_c == KvMetaObject::ReadProperty) {
		void *_v = _a[0];
		switch (_id) {
		case 0: *reinterpret_cast<bool*>(_v) = isEnabled(); break;
		}
		_id -= 1;
	}
	else if (_c == KvMetaObject::WriteProperty) {
		void *_v = _a[0];
		switch (_id) {
		case 0: setEnabled(*reinterpret_cast<bool*>(_v)); break;
		}
		_id -= 1;
	}
	else if (_c == KvMetaObject::ResetProperty) {
		_id -= 1;
	}
	else if (_c == KvMetaObject::QueryPropertyDesignable) {
		_id -= 1;
	}
	else if (_c == KvMetaObject::QueryPropertyScriptable) {
		_id -= 1;
	}
	else if (_c == KvMetaObject::QueryPropertyStored) {
		_id -= 1;
	}
	else if (_c == KvMetaObject::QueryPropertyEditable) {
		_id -= 1;
	}
	else if (_c == KvMetaObject::QueryPropertyUser) {
		_id -= 1;
	}
	return _id;
}

bool SignalSlotTest::isEnabled() const
{
	return m_isEnabled;
}

void SignalSlotTest::setEnabled(bool enabled)
{
	m_isEnabled = enabled;
}

// SIGNAL 0
void SignalSlotTest::enabledStateChanged(SignalSlotTest * _t1)
{
	void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
	KvMetaObject::activate(this, &staticMetaObject, 0, _a);
}
