// C++ translation of core/src/avm2/globals/boolean.rs
// Boolean implementation

#ifndef RUFFLE_CORE_AVM2_GLOBALS_BOOLEAN_H
#define RUFFLE_CORE_AVM2_GLOBALS_BOOLEAN_H

#include <vector>

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

/// Boolean constructor
/// Converts the first argument to a boolean, or returns false if no argument
template<typename GC>
Value<GC> boolean_constructor(
    Activation<GC>* activation,
    const std::vector<Value<GC>>& args
);

/// Call handler for Boolean
/// Converts the first argument to a boolean, or returns false if no argument
template<typename GC>
Value<GC> call_handler(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_BOOLEAN_H
