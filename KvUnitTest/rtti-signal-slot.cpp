#include "SignalSlotTest.h"

#include "gtest/gtest.h"

SignalSlotTest *sl;

TEST(SignalSlotTest, Connect)
{
	sl = new SignalSlotTest();

	//signature nedd remove whitespace SignalSlotTest * space 
	bool ret = KvObject::connect(sl, SIGNAL(enabledStateChanged(SignalSlotTest*)), sl, SLOT(deleteLater()));
	ret = true;
}

TEST(SignalSlotTest, Signal)
{
	emit sl->enabledStateChanged(NULL);
	bool ret = true;
}

TEST(SignalSlotTest, Slot)
{

}
