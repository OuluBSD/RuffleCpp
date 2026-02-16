// C++ translation of core/src/avm2/globals/flash/net/object_encoding.rs
// flash.net.ObjectEncoding implementation

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_OBJECT_ENCODING_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_OBJECT_ENCODING_H

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
namespace flash {
namespace net {
namespace object_encoding {

/// Implements flash.net.ObjectEncoding.dynamicPropertyWriter getter
/// Stub getter - not yet implemented
template<typename GC>
Value<GC> get_dynamic_property_writer(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.net.ObjectEncoding.dynamicPropertyWriter setter
/// Stub setter - not yet implemented
template<typename GC>
Value<GC> set_dynamic_property_writer(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace object_encoding
} // namespace net
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_OBJECT_ENCODING_H
