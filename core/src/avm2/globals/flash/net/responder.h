// C++ translation of core/src/avm2/globals/flash/net/responder.rs
// flash.net.Responder implementation

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_RESPONDER_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_RESPONDER_H

#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    namespace object {
        template<typename T> class ResponderObject;
        template<typename T> class Responder_allocator;
    }
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace net {
namespace responder {

/// Re-export responder_allocator from the object module
template<typename GC>
using responder_allocator = ruffle::core::avm2::object::responder_allocator<GC>;

/// Implements flash.net.Responder constructor
/// Sets up the result and status callbacks
template<typename GC>
Value<GC> init(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace responder
} // namespace net
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_RESPONDER_H
