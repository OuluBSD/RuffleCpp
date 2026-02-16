// C++ translation of core/src/avm2/globals/uint.rs
//! uint implementation

#ifndef RUFFLE_CORE_AVM2_GLOBALS_UINT_H
#define RUFFLE_CORE_AVM2_GLOBALS_UINT_H

#include <vector>
#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {

/// uint constructor
/// Converts the first argument to a 32-bit unsigned integer, or returns 0 if no argument
template<typename GC>
Value<GC> uint_constructor(
    Activation<GC>* activation,
    const std::vector<Value<GC>>& args
);

/// Call handler for uint
/// Converts the first argument to a 32-bit unsigned integer, or returns 0 if no argument
template<typename GC>
Value<GC> call_handler(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements uint.toExponential
/// Note: This method should not be callable on uint values in ActionScript
/// This function is a stub that will unreachable()
template<typename GC>
Value<GC> to_exponential(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements uint.toFixed
/// Note: This method should not be callable on uint values in ActionScript
/// This function is a stub that will unreachable()
template<typename GC>
Value<GC> to_fixed(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements uint.toPrecision
/// Note: This method should not be callable on uint values in ActionScript
/// This function is a stub that will unreachable()
template<typename GC>
Value<GC> to_precision(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements uint.prototype.toString
/// Note: This method should not be callable on uint values in ActionScript
/// This function is a stub that will unreachable()
template<typename GC>
Value<GC> to_string(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements uint.prototype.valueOf
/// Note: This method should not be callable on uint values in ActionScript
/// This function is a stub that will unreachable()
template<typename GC>
Value<GC> value_of(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_UINT_H
