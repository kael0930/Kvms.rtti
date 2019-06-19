#ifndef KvMathnumericPrivate_h__
#define KvMathnumericPrivate_h__

#include "KvMath.h"

#if !defined(KV_CC_MSVC) && (defined(KV_OS_QNX) || defined(KV_CC_INTEL))
#  include <math.h>
#  ifdef isnan
#    define KV_MATH_H_DEFINES_MACROS
namespace knumeric_std_wrapper {
	// the 'using namespace std' below is cases where the stdlib already put the math.h functions in the std namespace and undefined the macros.
	KV_DECL_CONST_FUNCTION static inline bool math_h_isnan(double d) { using namespace std; return isnan(d); }
	KV_DECL_CONST_FUNCTION static inline bool math_h_isinf(double d) { using namespace std; return isinf(d); }
	KV_DECL_CONST_FUNCTION static inline bool math_h_isfinite(double d) { using namespace std; return isfinite(d); }
	KV_DECL_CONST_FUNCTION static inline bool math_h_isnan(float f) { using namespace std; return isnan(f); }
	KV_DECL_CONST_FUNCTION static inline bool math_h_isinf(float f) { using namespace std; return isinf(f); }
	KV_DECL_CONST_FUNCTION static inline bool math_h_isfinite(float f) { using namespace std; return isfinite(f); }
}
QT_END_NAMESPACE
// These macros from math.h conflict with the real functions in the std namespace.
#    undef signbit
#    undef isnan
#    undef isinf
#    undef isfinite
#  endif // defined(isnan)
#endif

namespace knumeric_std_wrapper {
#if defined(KV_MATH_H_DEFINES_MACROS)
#  undef KV_MATH_H_DEFINES_MACROS
	KV_DECL_CONST_FUNCTION static inline bool isnan(double d) { return math_h_isnan(d); }
	KV_DECL_CONST_FUNCTION static inline bool isinf(double d) { return math_h_isinf(d); }
	KV_DECL_CONST_FUNCTION static inline bool isfinite(double d) { return math_h_isfinite(d); }
	KV_DECL_CONST_FUNCTION static inline bool isnan(float f) { return math_h_isnan(f); }
	KV_DECL_CONST_FUNCTION static inline bool isinf(float f) { return math_h_isinf(f); }
	KV_DECL_CONST_FUNCTION static inline bool isfinite(float f) { return math_h_isfinite(f); }
#else
	KV_DECL_CONST_FUNCTION static inline bool isnan(double d) { return std::isnan(d); }
	KV_DECL_CONST_FUNCTION static inline bool isinf(double d) { return std::isinf(d); }
	KV_DECL_CONST_FUNCTION static inline bool isfinite(double d) { return std::isfinite(d); }
	KV_DECL_CONST_FUNCTION static inline bool isnan(float f) { return std::isnan(f); }
	KV_DECL_CONST_FUNCTION static inline bool isinf(float f) { return std::isinf(f); }
	KV_DECL_CONST_FUNCTION static inline bool isfinite(float f) { return std::isfinite(f); }
#endif
}

KV_DECL_CONSTEXPR KV_DECL_CONST_FUNCTION static inline double kv_inf() 
{
	KV_STATIC_ASSERT_X(std::numeric_limits<double>::has_infinity,
	"platform has no definition for infinity for type double");
	return std::numeric_limits<double>::infinity();
}

// Signaling NaN
KV_DECL_CONSTEXPR KV_DECL_CONST_FUNCTION static inline double kv_snan() 
{
	KV_STATIC_ASSERT_X(std::numeric_limits<double>::has_signaling_NaN,
	"platform has no definition for signaling NaN for type double");
	return std::numeric_limits<double>::signaling_NaN();
}

// Quiet NaN
KV_DECL_CONSTEXPR KV_DECL_CONST_FUNCTION static inline double kv_qnan() 
{
	KV_STATIC_ASSERT_X(std::numeric_limits<double>::has_quiet_NaN,
	"platform has no definition for quiet NaN for type double");
	return std::numeric_limits<double>::quiet_NaN();
}

KV_DECL_CONST_FUNCTION static inline bool kv_is_inf(double d)
{
	return knumeric_std_wrapper::isinf(d);
}

KV_DECL_CONST_FUNCTION static inline bool kv_is_nan(double d)
{
	return knumeric_std_wrapper::isnan(d);
}

KV_DECL_CONST_FUNCTION static inline bool kv_is_finite(double d)
{
	return knumeric_std_wrapper::isfinite(d);
}

KV_DECL_CONST_FUNCTION static inline bool kv_is_inf(float f)
{
	return knumeric_std_wrapper::isinf(f);
}

KV_DECL_CONST_FUNCTION static inline bool kv_is_nan(float f)
{
	return knumeric_std_wrapper::isnan(f);
}

KV_DECL_CONST_FUNCTION static inline bool kv_is_finite(float f)
{
	return knumeric_std_wrapper::isfinite(f);
}

#endif // KvMathnumericPrivate_h__
