// C++ translation of core/src/avm2/globals/number.rs
//! Number implementation

#ifndef RUFFLE_CORE_AVM2_GLOBALS_NUMBER_H
#define RUFFLE_CORE_AVM2_GLOBALS_NUMBER_H

#include <vector>
#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class AvmString;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {

/// Number constructor
/// Converts the first argument to a number, or returns 0 if no argument
template<typename GC>
Value<GC> number_constructor(
    Activation<GC>* activation,
    const std::vector<Value<GC>>& args
);

/// Call handler for Number
/// Converts the first argument to a number, or returns 0.0 if no argument
template<typename GC>
Value<GC> call_handler(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Number.toExponential
/// Returns a string representation of the number in exponential notation
template<typename GC>
Value<GC> to_exponential(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Number.toFixed
/// Returns a string representation of the number with fixed decimal places
template<typename GC>
Value<GC> to_fixed(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Print number with specified precision
/// Helper function for toPrecision
template<typename GC>
AvmString<GC> print_with_precision(
    Activation<GC>* activation,
    double number,
    uint32_t wanted_digits
);

/// Implements Number.toPrecision
/// Returns a string representation of the number with specified precision
template<typename GC>
Value<GC> to_precision(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Print number with specified radix
/// Helper function for toString
template<typename GC>
AvmString<GC> print_with_radix(
    Activation<GC>* activation,
    double number,
    size_t radix
);

/// Implements Number.prototype.toString
/// Returns a string representation of the number in the specified radix
template<typename GC>
Value<GC> to_string(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Number.prototype.valueOf
/// Returns the number value itself
template<typename GC>
Value<GC> value_of(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

// Math function re-exports
// These delegate to the math module
namespace number {

template<typename GC>
Value<GC> abs(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

template<typename GC>
Value<GC> acos(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

template<typename GC>
Value<GC> asin(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

template<typename GC>
Value<GC> atan(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

template<typename GC>
Value<GC> atan2(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

template<typename GC>
Value<GC> ceil(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

template<typename GC>
Value<GC> cos(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

template<typename GC>
Value<GC> exp(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

template<typename GC>
Value<GC> floor(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

template<typename GC>
Value<GC> log(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

template<typename GC>
Value<GC> max(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

template<typename GC>
Value<GC> min(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

template<typename GC>
Value<GC> pow(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

template<typename GC>
Value<GC> random(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

template<typename GC>
Value<GC> round(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

template<typename GC>
Value<GC> sin(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

template<typename GC>
Value<GC> sqrt(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

template<typename GC>
Value<GC> tan(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace number

} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_NUMBER_H
