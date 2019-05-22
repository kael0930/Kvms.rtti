#include "gtest/gtest.h"
#include "KvObject.h"

#include <iostream>

char digits[] =
{
	'0', '1', '2', '3', '4', '5',
	'6', '7', '8', '9', 'a', 'b',
	'c', 'd', 'e', 'f', 'g', 'h',
	'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't',
	'u', 'v', 'w', 'x', 'y', 'z'
};

char */*std::string*/ ToUnsignedString(int i, int shift)
{
	char  buf[32] = { 0 };
	char * pBuf = buf;
	int charPos = 32;
	int radix = 1 << shift;
	int mask = radix - 1;
	do
	{
		pBuf[--charPos] = digits[i&mask];
		i = i >> shift;
	} while (i != 0);

	int strLen = 32 - charPos;
	pBuf = pBuf + charPos;

	//while (strLen)
	//{
	//	str.push_back(*pBuf);
	//	pBuf++;
	//	strLen--;
	//}

	//return str;

	return pBuf;
}
std::string ToHexString(int i)
{
	return ToUnsignedString(i, 4);
}

std::string ToOctalString(int i)
{
	return	ToUnsignedString(i, 3);
}

std::string ToBinaryString(int i)
{
	return	ToUnsignedString(i, 1);
}


TEST(sylixos, cal)
{
	int a1;
	char a2;
	int *a3;
	char *a4;
	int *a5[10];
	char *a6[10];
	float a7;
	double a8;
	struct 
	{
		int a; char b;
	}a9;

	struct 
	{
		int a; char b;
	}*a11;

	union 
	{
		int a; char b;
	}a22;

	void(*a33)(void);

	union 
	{
		int a; char b;
	}c;
	c.a = 0x123456789;

	int a44[5][5], i;
	int *p = (int*)(a44 + 1);
	for (i = 0; i < 20; ++i)
	{
		*p++ = i;
	}
	a44[3][2];

	int a55 = 50; a55 >>= 2;

	int ret = 0;
	ret = sizeof(a1);
	ret = sizeof(a2);
	ret = sizeof(a3);
	ret = sizeof(a4);
	ret = sizeof(a5);
	ret = sizeof(a6);
	ret = sizeof(a7);
	ret = sizeof(a8);
	ret = sizeof(a9);
	ret = sizeof(a11);
	ret = sizeof(a22);
	ret = sizeof(a33);
	ret = c.b;
	ret = a44[3][2]/*sizeof(a44)*/;
	ret = a55/*sizeof(a55)*/;

	std::cout << ToHexString(200).c_str() << std::endl;
	std::cout << ToOctalString(200).c_str() << std::endl;
	std::cout << ToBinaryString(200).c_str() << std::endl;

	ret = 0;
}