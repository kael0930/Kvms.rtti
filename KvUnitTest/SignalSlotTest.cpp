#include "SignalSlotTest.h"

KV_BEGINE_REGISTER_CLASS
	(SignalSlotTest, KvObject
	,KV_REGISTER_CONSTRUCTOR(SignalSlotTest, int)
	,KV_REGISTER_PROPERTY(enabled, bool, isEnabled, setEnabled)
	,KV_REGISTER_SIGNAL(enabledStateChanged, SignalSlotTest *)
	,KV_REGISTER_SLOT(SayHello)
	,KV_REGISTER_SLOT(CallOneParam, int)
	,KV_REGISTER_SLOT(CallTwoParam, int, bool)
	,KV_REGISTER_PROPERTY(Name, KvString, GetName, SetName)
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

KvString SignalSlotTest::GetName() const
{
	return KvString();
}

void SignalSlotTest::SetName(const KvString &)
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

