// C++ translation of core/src/avm2/globals/flash/net/xml_socket.rs
// flash.net.XMLSocket helper functions

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_XML_SOCKET_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_XML_SOCKET_H

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
    template<typename T> class AvmString;
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace net {
namespace xml_socket {

/// Gets the movie domain for XMLSocket connection
/// Returns the domain from the movie URL, or "localhost" for file:// URLs
template<typename GC>
Value<GC> get_domain(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace xml_socket
} // namespace net
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_XML_SOCKET_H
