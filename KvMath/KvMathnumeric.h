#ifndef KvMathnumeric_h__
#define KvMathnumeric_h__

#include "KvMath.h"

KV_MATH_EXPORT KV_DECL_CONST_FUNCTION bool kvIsInf(double d);
KV_MATH_EXPORT KV_DECL_CONST_FUNCTION bool kvIsNaN(double d);
KV_MATH_EXPORT KV_DECL_CONST_FUNCTION bool kvIsFinite(double d);
KV_MATH_EXPORT KV_DECL_CONST_FUNCTION bool kvIsInf(float f);
KV_MATH_EXPORT KV_DECL_CONST_FUNCTION bool kvIsNaN(float f);
KV_MATH_EXPORT KV_DECL_CONST_FUNCTION bool kvIsFinite(float f);
KV_MATH_EXPORT KV_DECL_CONST_FUNCTION double kvSNaN();
KV_MATH_EXPORT KV_DECL_CONST_FUNCTION double kvQNaN();
KV_MATH_EXPORT KV_DECL_CONST_FUNCTION double kvInf();

KV_MATH_EXPORT kuint32 kvFloatDistance(float a, float b);
KV_MATH_EXPORT kuint64 kvFloatDistance(double a, double b);

#define KV_INFINITY (KV_PREPEND_NAMESPACE(kvInf)())
#define KV_SNAN (KV_PREPEND_NAMESPACE(kvSNaN)())
#define KV_QNAN (KV_PREPEND_NAMESPACE(kvQNaN)())


#endif // KvMathnumeric_h__
