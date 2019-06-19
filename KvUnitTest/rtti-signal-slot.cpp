#include "SignalSlotTest.h"

#include "gtest/gtest.h"

SignalSlotTest *sl;

TEST(SignalSlotTest, Propery)
{
	//sl = new SignalSlotTest();
	//bool r = true;
	//KvVariant v = sl->property("enabled");
	//bool s = v.toBool();
	//sl->setProperty("enabled", false);
	//v = sl->property("enabled");
	//s = v.toBool();

	//bool ret = true;
}

TEST(SignalSlotTest, Connect)
{
	bool ret = false;
	ret = KvObject::connect(sl, SIGNAL(enabledStateChanged(SignalSlotTest*)), sl, SLOT(deleteLater()));
	ret = true;
}

TEST(SignalSlotTest, Signal)
{
	//emit sl->enabledStateChanged(NULL);
	bool ret = true;
}

TEST(SignalSlotTest, Slot)
{

}
