#ifndef KvMath_h__
#define KvMath_h__

#include "KvGlobal.h"

#include <limits>

#if defined(KV_CC_MSVC)
#  include <intrin.h>
#endif

#if defined(KV_CC_MSVC)
#include <float.h>
#endif

#if defined(KVMATH_EXPORTS)
#  define KV_MATH_EXPORT K_DECL_EXPORT
#else
#  define KV_MATH_EXPORT K_DECL_IMPORT
#endif

#ifndef _USE_MATH_DEFINES
#  define _USE_MATH_DEFINES
#  define undef_USE_MATH_DEFINES
#endif
#include <cmath>
#ifdef undef_USE_MATH_DEFINES
#  undef _USE_MATH_DEFINES
#  undef undef_USE_MATH_DEFINES
#endif

#endif // KvMath_h__
