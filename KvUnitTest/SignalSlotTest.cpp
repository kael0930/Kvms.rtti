#include "SignalSlotTest.h"

static const uint kv_meta_data_SignalSlotTest[] = {

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
	37 + 2 * 8, 7, 7, 7, 0x0a,
	48 + 2 * 8, 7, 7, 7, 0x0a,
	68 + 2 * 8, 66, 7, 7, 0x0a,

	// properties: name, type, flags
	96 + 2 * 8, 91, 0x01095103,

	0        // eod
};

static const char kv_meta_stringdata_SignalSlotTest[] = {
	"SignalSlotTest\0\0enabledStateChanged(SignalSlotTest*)\0"
	"SayHello()\0CallOneParam(int)\0,\0"
	"CallTwoParam(int,bool)\0bool\0enabled\0"
};

KV_BEGINE_REGISTER_CLASS
	(SignalSlotTest, KvObject
	,KV_REGISTER_PROPERTY(bool, enabled, isEnabled, setEnabled)
	,KV_REGISTER_SIGNAL(enabledStateChanged, SignalSlotTest *)
	,KV_REGISTER_SLOT(SayHello)
	,KV_REGISTER_SLOT(CallOneParam, int)
	,KV_REGISTER_SLOT(CallTwoParam, int, bool)
	)
KV_END_REGISTER_CLASS()


SignalSlotTest::SignalSlotTest()
{
}


SignalSlotTest::~SignalSlotTest()
{
}

bool SignalSlotTest::isEnabled() const
{
	return m_isEnabled;
}

void SignalSlotTest::setEnabled(bool enabled)
{
	m_isEnabled = enabled;
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

