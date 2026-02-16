// C++ translation of core/src/avm2/globals/flash/utils/proxy.rs
// flash.utils.Proxy

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_UTILS_PROXY_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_UTILS_PROXY_H

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
namespace utils {
namespace proxy {

// Re-export proxy_allocator from the object module
template<typename GC>
using proxy_allocator = ruffle::core::avm2::object::proxy_allocator<GC>;

/// Implements isAttribute
template<typename GC>
Value<GC> is_attribute(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace proxy
} // namespace utils
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_UTILS_PROXY_H
