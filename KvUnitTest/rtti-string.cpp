#include "gtest/gtest.h"
#include "KvObject.h"

TEST(KvString, DefaultConstructor)
{

}

TEST(KvString, EqualOperator)
{
	bool ret = false;
	ret = KvString("123") == "123";
	ret = KvString("123") == KvString("111");
	ret = true;
}