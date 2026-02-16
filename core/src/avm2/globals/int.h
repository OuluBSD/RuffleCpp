// C++ translation of core/src/avm2/globals/int.rs
//! int implementation

#ifndef RUFFLE_CORE_AVM2_GLOBALS_INT_H
#define RUFFLE_CORE_AVM2_GLOBALS_INT_H

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

/// int constructor
/// Converts the first argument to a 32-bit signed integer, or returns 0 if no argument
template<typename GC>
Value<GC> int_constructor(
    Activation<GC>* activation,
    const std::vector<Value<GC>>& args
);

/// Call handler for int
/// Converts the first argument to a 32-bit signed integer, or returns 0 if no argument
template<typename GC>
Value<GC> call_handler(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements int.toExponential
/// Note: This method should not be callable on int values in ActionScript
/// This function is a stub that will unreachable()
template<typename GC>
Value<GC> to_exponential(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements int.toFixed
/// Note: This method should not be callable on int values in ActionScript
/// This function is a stub that will unreachable()
template<typename GC>
Value<GC> to_fixed(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements int.toPrecision
/// Note: This method should not be callable on int values in ActionScript
/// This function is a stub that will unreachable()
template<typename GC>
Value<GC> to_precision(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements int.prototype.toString
/// Note: This method should not be callable on int values in ActionScript
/// This function is a stub that will unreachable()
template<typename GC>
Value<GC> to_string(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements int.prototype.valueOf
/// Note: This method should not be callable on int values in ActionScript
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

#endif // RUFFLE_CORE_AVM2_GLOBALS_INT_H
