// C++ translation of core/src/avm2/globals/math.rs
//! AVM2 Math class implementation

#ifndef RUFFLE_CORE_AVM2_GLOBALS_MATH_H
#define RUFFLE_CORE_AVM2_GLOBALS_MATH_H

#include <cmath>
#include <cstdint>
#include <limits>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class ClassObject;
    template<typename T> class Object;
    template<typename T> class Value;
    template<typename T> class Error;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {

/// Math.abs - Returns the absolute value of a number
template<typename GC>
Value<GC> abs(
    Activation<GC>* activation,
    Value<GC> this_val,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.acos - Returns the arccosine of a number
template<typename GC>
Value<GC> acos(
    Activation<GC>* activation,
    Value<GC> this_val,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.asin - Returns the arcsine of a number
template<typename GC>
Value<GC> asin(
    Activation<GC>* activation,
    Value<GC> this_val,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.atan - Returns the arctangent of a number
template<typename GC>
Value<GC> atan(
    Activation<GC>* activation,
    Value<GC> this_val,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.ceil - Returns the smallest integer greater than or equal to a number
template<typename GC>
Value<GC> ceil(
    Activation<GC>* activation,
    Value<GC> this_val,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.cos - Returns the cosine of a number
template<typename GC>
Value<GC> cos(
    Activation<GC>* activation,
    Value<GC> this_val,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.exp - Returns e^x
template<typename GC>
Value<GC> exp(
    Activation<GC>* activation,
    Value<GC> this_val,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.floor - Returns the largest integer less than or equal to a number
template<typename GC>
Value<GC> floor(
    Activation<GC>* activation,
    Value<GC> this_val,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.log - Returns the natural logarithm of a number
template<typename GC>
Value<GC> log(
    Activation<GC>* activation,
    Value<GC> this_val,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.sin - Returns the sine of a number
template<typename GC>
Value<GC> sin(
    Activation<GC>* activation,
    Value<GC> this_val,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.sqrt - Returns the square root of a number
template<typename GC>
Value<GC> sqrt(
    Activation<GC>* activation,
    Value<GC> this_val,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.tan - Returns the tangent of a number
template<typename GC>
Value<GC> tan(
    Activation<GC>* activation,
    Value<GC> this_val,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.call - Constructor call handler (always throws error 1075)
template<typename GC>
Value<GC> call_handler(
    Activation<GC>* activation,
    Value<GC> this_val,
    const Value<GC>* args,
    size_t arg_count
);

/// Math allocator - Always throws error 1076 (Math cannot be instantiated)
template<typename GC>
Object<GC> math_allocator(
    ClassObject<GC> class_obj,
    Activation<GC>* activation
);

/// Math.round - Rounds a number to the nearest integer
/// Note: Flash Math.round always rounds toward infinity,
/// unlike standard round which rounds away from zero.
template<typename GC>
Value<GC> round(
    Activation<GC>* activation,
    Value<GC> this_val,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.atan2 - Returns the arctangent of y/x
template<typename GC>
Value<GC> atan2(
    Activation<GC>* activation,
    Value<GC> this_val,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.max - Returns the largest of zero or more numbers
template<typename GC>
Value<GC> max(
    Activation<GC>* activation,
    Value<GC> this_val,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.min - Returns the smallest of zero or more numbers
template<typename GC>
Value<GC> min(
    Activation<GC>* activation,
    Value<GC> this_val,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.pow - Returns base^exponent
/// Handles special cases for infinite/NaN values according to Flash behavior
template<typename GC>
Value<GC> pow(
    Activation<GC>* activation,
    Value<GC> this_val,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.random - Returns a pseudo-random number in the range [0, 1)
/// Uses a restricted set of f64 values that some SWFs implicitly rely on.
template<typename GC>
Value<GC> random(
    Activation<GC>* activation,
    Value<GC> this_val,
    const Value<GC>* args,
    size_t arg_count
);

} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_MATH_H
