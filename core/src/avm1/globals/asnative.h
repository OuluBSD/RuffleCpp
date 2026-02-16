// C++ translation of core/src/avm1/globals/asnative.rs
// ASnative internal function

#ifndef RUFFLE_CORE_AVM1_GLOBALS_ASNATIVE_H
#define RUFFLE_CORE_AVM1_GLOBALS_ASNATIVE_H

#include <cstdint>
#include <optional>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Object;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class FunctionObject;
    using TableNativeFunction = Value<GC> (*)(Activation<GC>*, Object<GC>*, const Value<GC>*, size_t, uint16_t);
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace asnative {

/// ASnative internal function
/// Returns a native function based on category and index
template<typename GC>
Value<GC> asnative(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Internal ASnew method - returns whether function was called as constructor
template<typename GC>
Value<GC> asnew_method(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count,
    uint16_t id
);

} // namespace asnative
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_ASNATIVE_H
