// C++ translation of core/src/avm2/globals/flash/net/url_loader.rs
// flash.net.URLLoader native function definitions

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_URL_LOADER_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_URL_LOADER_H

#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Object;
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
namespace url_loader {

/// Native function definition for URLLoader.load
template<typename GC>
Value<GC> load(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Internal function to spawn fetch operation
template<typename GC>
Value<GC> spawn_fetch(
    Activation<GC>* activation,
    Object<GC>* loader_object,
    Object<GC>* url_request
);

} // namespace url_loader
} // namespace net
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_URL_LOADER_H
