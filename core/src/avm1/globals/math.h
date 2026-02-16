// C++ translation of core/src/avm1/globals/math.rs
// Math class implementation

#ifndef RUFFLE_CORE_AVM1_GLOBALS_MATH_H
#define RUFFLE_CORE_AVM1_GLOBALS_MATH_H

#include <cstdint>
#include <cmath>
#include <limits>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Object;
    template<typename T> class Value;
    template<typename T> class Error;
    struct DeclContext;
    struct StaticDeclarations;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace math {

// Property declarations for Math object
extern const StaticDeclarations OBJECT_DECLS;

/// Create the Math object
template<typename GC>
Object<GC> create(DeclContext<GC>* context);

/// Math.abs
template<typename GC>
Value<GC> abs(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.min
template<typename GC>
Value<GC> min(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.max
template<typename GC>
Value<GC> max(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.sin
template<typename GC>
Value<GC> sin(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.cos
template<typename GC>
Value<GC> cos(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.atan2
template<typename GC>
Value<GC> atan2(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.tan
template<typename GC>
Value<GC> tan(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.exp
template<typename GC>
Value<GC> exp(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.log
template<typename GC>
Value<GC> log(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.sqrt
template<typename GC>
Value<GC> sqrt(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.round
template<typename GC>
Value<GC> round(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.random
template<typename GC>
Value<GC> random(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.floor
template<typename GC>
Value<GC> floor(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.ceil
template<typename GC>
Value<GC> ceil(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.atan
template<typename GC>
Value<GC> atan(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.asin
template<typename GC>
Value<GC> asin(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.acos
template<typename GC>
Value<GC> acos(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Math.pow
template<typename GC>
Value<GC> pow(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

// Math constants
constexpr double SQRT2 = 1.4142135623730951;
constexpr double SQRT1_2 = 0.7071067811865476;
constexpr double PI = 3.141592653589793;
constexpr double LOG2E = 1.4426950408889634;
constexpr double LOG10E = 0.4342944819032518;
constexpr double LN2 = 0.6931471805599453;
constexpr double LN10 = 2.302585092994046;
constexpr double E = 2.718281828459045;

} // namespace math
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_MATH_H
