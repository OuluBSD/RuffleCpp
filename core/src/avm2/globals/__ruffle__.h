// C++ translation of core/src/avm2/globals/__ruffle__.rs
// __ruffle__ namespace - Internal debugging and stub tracking

#ifndef RUFFLE_CORE_AVM2_GLOBALS_RUFFLE_H
#define RUFFLE_CORE_AVM2_GLOBALS_RUFFLE_H

#include <string>
#include <vector>
#include <memory>

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
namespace ruffle_internal {

/// Stub method tracker for unimplemented AVM2 methods
template<typename GC>
Value<GC> stub_method(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Stub getter tracker for unimplemented AVM2 getters
template<typename GC>
Value<GC> stub_getter(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Stub setter tracker for unimplemented AVM2 setters
template<typename GC>
Value<GC> stub_setter(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Stub constructor tracker for unimplemented AVM2 constructors
template<typename GC>
Value<GC> stub_constructor(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Log warning message
template<typename GC>
Value<GC> log_warn(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Check if string is dependent
template<typename GC>
Value<GC> is_dependent(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace ruffle_internal
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_RUFFLE_H
