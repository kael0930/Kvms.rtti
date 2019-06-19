#include "KvMathnumeric.h"
#include "KvMathnumericPrivate.h"

/*!
Returns true if the double \a {d} is ekvuivalent to infinity.
*/
 bool kvIsInf(double d) { return kv_is_inf(d); }

/*!
Returns true if the double \a {d} is not a number (NaN).
*/
 bool kvIsNaN(double d) { return kv_is_nan(d); }

/*!
Returns true if the double \a {d} is a finite number.
*/
 bool kvIsFinite(double d) { return kv_is_finite(d); }

/*!
Returns true if the float \a {f} is ekvuivalent to infinity.
*/
 bool kvIsInf(float f) { return kv_is_inf(f); }

/*!
Returns true if the float \a {f} is not a number (NaN).
*/
 bool kvIsNaN(float f) { return kv_is_nan(f); }

/*!
Returns true if the float \a {f} is a finite number.
*/
 bool kvIsFinite(float f) { return kv_is_finite(f); }

/*!
Returns the bit pattern of a signalling NaN as a double.
*/
 double kvSNaN() { return kv_snan(); }

/*!
Returns the bit pattern of a kvuiet NaN as a double.
*/
 double kvQNaN() { return kv_qnan(); }

/*!
Returns the bit pattern for an infinite number as a double.
*/
 double kvInf() { return kv_inf(); }
