#include "gtest/gtest.h"
#include "KvObject.h"

# include <boost/preprocessor/stringize.hpp>
# include <boost/preprocessor/wstringize.hpp>
# include <boost/preprocessor/arithmetic/inc.hpp>
# if !defined __cplusplus
#include <wchar.h>
#endif
# include <boost/preprocessor/cat.hpp>
#
# define BEGIN typedef int BOOST_PP_CAT(test_, __LINE__)[((
# define END )==1) ? 1 : -1];

#if defined(__cplusplus)
#include <cstdio>
#if !defined(_STLP_MSVC) || _STLP_MSVC >= 1300
namespace std {}
using namespace std;
#endif
#else
#include <stdio.h>
#endif

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

#define VDATA 1,2,3,4
#define NDATA
#define DATA data
#define FDATA(x) BOOST_PP_INC(x)

TEST(Boost, StringSize)
{
# if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_MSVC()
	BEGIN sizeof(BOOST_PP_STRINGIZE(NDATA)) / sizeof(char) == 1 END
		BEGIN sizeof(BOOST_PP_WSTRINGIZE(NDATA)) / sizeof(wchar_t) == 1 END
#endif
		BEGIN sizeof(BOOST_PP_STRINGIZE(DATA)) / sizeof(char) == 5 END
		BEGIN sizeof(BOOST_PP_STRINGIZE(FDATA(1))) / sizeof(char) == 2 END
		BEGIN sizeof(BOOST_PP_STRINGIZE(FDATA(9))) / sizeof(char) == 3 END
		BEGIN sizeof(BOOST_PP_WSTRINGIZE(DATA)) / sizeof(wchar_t) == 5 END
		BEGIN sizeof(BOOST_PP_WSTRINGIZE(FDATA(1))) / sizeof(wchar_t) == 2 END
		BEGIN sizeof(BOOST_PP_WSTRINGIZE(FDATA(9))) / sizeof(wchar_t) == 3 END
#if BOOST_PP_VARIADICS
		BEGIN sizeof(BOOST_PP_STRINGIZE(VDATA)) / sizeof(char) == 8 END
		BEGIN sizeof(BOOST_PP_WSTRINGIZE(VDATA)) / sizeof(wchar_t) == 8 END
#endif

		bool ret = true;
}