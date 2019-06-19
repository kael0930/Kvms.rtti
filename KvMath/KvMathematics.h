
#ifndef KvMathematics_h__
#define KvMathematics_h__

#include "KvMath.h"

#define KV_SINE_TABLE_SIZE 256
extern KV_MATH_EXPORT const kreal kv_sine_table[KV_SINE_TABLE_SIZE];
inline int kvCeil(kreal v)
{
	using std::ceil;
	return int(ceil(v));
}
inline int kvFloor(kreal v)
{
	using std::floor;
	return int(floor(v));
}
inline kreal kvFabs(kreal v)
{
	using std::fabs;
	return fabs(v);
}
inline kreal kvSin(kreal v)
{
	using std::sin;
	return sin(v);
}
inline kreal kvCos(kreal v)
{
	using std::cos;
	return cos(v);
}
inline kreal kvTan(kreal v)
{
	using std::tan;
	return tan(v);
}
inline kreal kvAcos(kreal v)
{
	using std::acos;
	return acos(v);
}
inline kreal kvAsin(kreal v)
{
	using std::asin;
	return asin(v);
}
inline kreal kvAtan(kreal v)
{
	using std::atan;
	return atan(v);
}
inline kreal kvAtan2(kreal y, kreal x)
{
	using std::atan2;
	return atan2(y, x);
}
inline kreal kvSqrt(kreal v)
{
	using std::sqrt;
	return sqrt(v);
}
inline kreal kvLn(kreal v)
{
	using std::log;
	return log(v);
}
inline kreal kvExp(kreal v)
{
	using std::exp;
	return exp(v);
}
inline kreal kvPow(kreal x, kreal y)
{
	using std::pow;
	return pow(x, y);
}
// TODO: use template variables (e.g. Kv::pi<type>) for these once we have C++14 support:
#ifndef M_E
#define M_E (2.7182818284590452354)
#endif
#ifndef M_LOG2E
#define M_LOG2E (1.4426950408889634074)
#endif
#ifndef M_LOG10E
#define M_LOG10E (0.43429448190325182765)
#endif
#ifndef M_LN2
#define M_LN2 (0.69314718055994530942)
#endif
#ifndef M_LN10
#define M_LN10 (2.30258509299404568402)
#endif
#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif
#ifndef M_PI_2
#define M_PI_2 (1.57079632679489661923)
#endif
#ifndef M_PI_4
#define M_PI_4 (0.78539816339744830962)
#endif
#ifndef M_1_PI
#define M_1_PI (0.31830988618379067154)
#endif
#ifndef M_2_PI
#define M_2_PI (0.63661977236758134308)
#endif
#ifndef M_2_SQRTPI
#define M_2_SQRTPI (1.12837916709551257390)
#endif
#ifndef M_SQRT2
#define M_SQRT2 (1.41421356237309504880)
#endif
#ifndef M_SQRT1_2
#define M_SQRT1_2 (0.70710678118654752440)
#endif
inline kreal kvFastSin(kreal x)
{
	int si = int(x * (0.5 * KV_SINE_TABLE_SIZE / M_PI)); // Would be more accurate with qRound, but slower.
	kreal d = x - si * (2.0 * M_PI / KV_SINE_TABLE_SIZE);
	int ci = si + KV_SINE_TABLE_SIZE / 4;
	si &= KV_SINE_TABLE_SIZE - 1;
	ci &= KV_SINE_TABLE_SIZE - 1;
	return kv_sine_table[si] + (kv_sine_table[ci] - 0.5 * kv_sine_table[si] * d) * d;
}
inline kreal kvFastCos(kreal x)
{
	int ci = int(x * (0.5 * KV_SINE_TABLE_SIZE / M_PI)); // Would be more accurate with qRound, but slower.
	kreal d = x - ci * (2.0 * M_PI / KV_SINE_TABLE_SIZE);
	int si = ci + KV_SINE_TABLE_SIZE / 4;
	si &= KV_SINE_TABLE_SIZE - 1;
	ci &= KV_SINE_TABLE_SIZE - 1;
	return kv_sine_table[si] - (kv_sine_table[ci] + 0.5 * kv_sine_table[si] * d) * d;
}
KV_DECL_CONSTEXPR inline float kvDegreesToRadians(float degrees)
{
	return degrees * float(M_PI / 180);
}
KV_DECL_CONSTEXPR inline double kvDegreesToRadians(double degrees)
{
	return degrees * (M_PI / 180);
}
KV_DECL_CONSTEXPR inline float kvRadiansToDegrees(float radians)
{
	return radians * float(180 / M_PI);
}
KV_DECL_CONSTEXPR inline double kvRadiansToDegrees(double radians)
{
	return radians * (180 / M_PI);
}
// for a given no n, finds a number p which is greater than or equal to n and is a power of 2.
KV_DECL_CONSTEXPR inline kuint32 kvNextPowerOfTwo(kuint32 v)
{
#if defined(KV_HAS_BUILTIN_CLZ)
	if (v == 0)
		return 1;
	return 2U << (31 ^ __builtin_clz(v));
#else
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	++v;
	return v;
#endif
}
KV_DECL_CONSTEXPR inline kuint64 kvNextPowerOfTwo(kuint64 v)
{
#if defined(KV_HAS_BUILTIN_CLZLL)
	if (v == 0)
		return 1;
	return K_UINT64_C(2) << (63 ^ _builtin_clzll(v));
#else
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v |= v >> 32;
	++v;
	return v;
#endif
}
KV_DECL_CONSTEXPR inline kuint32 kvNextPowerOfTwo(kint32 v)
{
	return kvNextPowerOfTwo(kuint32(v));
}
KV_DECL_CONSTEXPR inline kuint64 kvNextPowerOfTwo(kint64 v)
{
	return kvNextPowerOfTwo(kuint64(v));
}


#if (defined(KV_CC_GNU) && (KV_CC_GNU >= 500) || (defined(KV_CC_INTEL) && !defined(KV_OS_WIN))) || KV_HAS_BUILTIN(__builtin_add_overflowx)
// GCC 5, ICC 18, and Clang 3.8 have builtins to detect overflows

template <typename T> inline
typename std::enable_if<std::is_unsigned<T>::value || std::is_signed<T>::value, bool>::type
add_overflow(T v1, T v2, T *r)
{
	return __builtin_add_overflow(v1, v2, r);
}

template <typename T> inline
typename std::enable_if<std::is_unsigned<T>::value || std::is_signed<T>::value, bool>::type
sub_overflow(T v1, T v2, T *r)
{
	return __builtin_sub_overflow(v1, v2, r);
}

template <typename T> inline
typename std::enable_if<std::is_unsigned<T>::value || std::is_signed<T>::value, bool>::type
mul_overflow(T v1, T v2, T *r)
{
	return __builtin_mul_overflow(v1, v2, r);
}

#else
// Generic implementations

template <typename T> inline typename std::enable_if<std::is_unsigned<T>::value, bool>::type
add_overflow(T v1, T v2, T *r)
{
	// unsigned additions are well-defined
	*r = v1 + v2;
	return v1 > T(v1 + v2);
}

template <typename T> inline typename std::enable_if<std::is_signed<T>::value, bool>::type
add_overflow(T v1, T v2, T *r)
{
	// Here's how we calculate the overflow:
	// 1) unsigned addition is well-defined, so we can always execute it
	// 2) conversion from unsigned back to signed is implementation-
	//    defined and in the implementations we use, it's a no-op.
	// 3) signed integer overflow happens if the sign of the two input operands
	//    is the same but the sign of the result is different. In other words,
	//    the sign of the result must be the same as the sign of either
	//    operand.

	using U = typename std::make_unsigned<T>::type;
	*r = T(U(v1) + U(v2));

	// If int is two's complement, assume all integer types are too.
	if (std::is_same<int32_t, int>::value) {
		// Two's complement equivalent (generates slightly shorter code):
		//  x ^ y             is negative if x and y have different signs
		//  x & y             is negative if x and y are negative
		// (x ^ z) & (y ^ z)  is negative if x and z have different signs
		//                    AND y and z have different signs
		return ((v1 ^ *r) & (v2 ^ *r)) < 0;
	}

	bool s1 = (v1 < 0);
	bool s2 = (v2 < 0);
	bool sr = (*r < 0);
	return s1 != sr && s2 != sr;
	// also: return s1 == s2 && s1 != sr;
}

template <typename T> inline typename std::enable_if<std::is_unsigned<T>::value, bool>::type
sub_overflow(T v1, T v2, T *r)
{
	// unsigned subtractions are well-defined
	*r = v1 - v2;
	return v1 < v2;
}

template <typename T> inline typename std::enable_if<std::is_signed<T>::value, bool>::type
sub_overflow(T v1, T v2, T *r)
{
	// See above for explanation. This is the same with some signs reversed.
	// We can't use add_overflow(v1, -v2, r) because it would be UB if
	// v2 == std::numeric_limits<T>::min().

	using U = typename std::make_unsigned<T>::type;
	*r = T(U(v1) - U(v2));

	if (std::is_same<int32_t, int>::value)
		return ((v1 ^ *r) & (~v2 ^ *r)) < 0;

	bool s1 = (v1 < 0);
	bool s2 = !(v2 < 0);
	bool sr = (*r < 0);
	return s1 != sr && s2 != sr;
	// also: return s1 == s2 && s1 != sr;
}

template <typename T> inline
typename std::enable_if<std::is_unsigned<T>::value || std::is_signed<T>::value, bool>::type
mul_overflow(T v1, T v2, T *r)
{
	// use the next biggest type
	// Note: for 64-bit systems where __int128 isn't supported, this will cause an error.
	using LargerInt = KvIntegerForSize<sizeof(T)* 2>;
	using Larger = typename std::conditional<std::is_signed<T>::value,
		typename LargerInt::Signed, typename LargerInt::Unsigned>::type;
	Larger lr = Larger(v1) * Larger(v2);
	*r = T(lr);
	return lr > std::numeric_limits<T>::max() || lr < std::numeric_limits<T>::min();
}

#  if defined(KV_CC_MSVC) && defined(KV_PROCESSOR_X86)
// We can use intrinsics for the unsigned operations with MSVC
template <> inline bool add_overflow(unsigned v1, unsigned v2, unsigned *r)
{
	return _addcarry_u32(0, v1, v2, r);
}

// 32-bit mul_overflow is fine with the generic code above

#    if defined(KV_PROCESSOR_X86_64)
template <> inline bool add_overflow(kuint64 v1, kuint64 v2, kuint64 *r)
{
	return _addcarry_u64(0, v1, v2, reinterpret_cast<unsigned __int64 *>(r));
}

#    pragma intrinsic(_umul128)
template <> inline bool mul_overflow(kuint64 v1, kuint64 v2, kuint64 *r)
{
	// use 128-bit multiplication with the _umul128 intrinsic
	// https://msdn.microsoft.com/en-us/library/3dayytw9.aspx
	kuint64 high;
	*r = _umul128(v1, v2, &high);
	return high;
}

#    pragma intrinsic(_mul128)
template <> inline bool mul_overflow(kint64 v1, kint64 v2, kint64 *r)
{
	// Use 128-bit multiplication with the _mul128 intrinsic
	// https://msdn.microsoft.com/en-us/library/82cxdw50.aspx

	// This is slightly more complex than the unsigned case above: the sign bit
	// of 'low' must be replicated as the entire 'high', so the only valid
	// values for 'high' are 0 and -1.

	kint64 high;
	*r = _mul128(v1, v2, &high);
	if (high == 0)
		return *r < 0;
	if (high == -1)
		return *r >= 0;
	return true;
}
#    endif // x86-64
#  endif // MSVC x86
#endif // !GCC

#endif // KvMathematics_h__