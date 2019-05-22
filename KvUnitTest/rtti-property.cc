#include "gtest/gtest.h"
#include "KvObject.h"

TEST(KvRttiTest, DefaultConstructor)
{

}

TEST(KvRttiTest, Property)
{
	KvObject *obj = new KvObject();

	KvVariant v = obj->property("objectName");
	KvString s = v.toString();
	obj->setProperty("objectName", KvString("Kvobject"));
	v = obj->property("objectName");
	s = v.toString();
	int i = 0;
}