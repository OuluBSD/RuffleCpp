// C++ translation of core/src/avm2/globals/flash/net.rs
// flash.net namespace

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_H

#include <string>
#include <map>

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

// flash.net submodules
namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace net {

// Submodules
namespace file_reference {
    // FileReference functionality defined in file_reference.h
}

namespace local_connection {
    // LocalConnection functionality defined in local_connection.h
}

namespace net_connection {
    // NetConnection functionality defined in net_connection.h
}

namespace net_stream {
    // NetStream functionality defined in net_stream.h
}

namespace object_encoding {
    // ObjectEncoding functionality defined in object_encoding.h
}

namespace responder {
    // Responder functionality defined in responder.h
}

namespace shared_object {
    // SharedObject functionality defined in shared_object.h
}

namespace socket {
    // Socket functionality defined in socket.h
}

namespace url_loader {
    // URLLoader functionality defined in url_loader.h
}

namespace xml_socket {
    // XMLSocket functionality defined in xml_socket.h
}

/// Helper function to convert object to index map
template<typename GC>
std::map<std::string, std::string> object_to_index_map(
    Activation<GC>* activation,
    Object<GC>* obj
);

/// Helper function to parse data
template<typename GC>
std::pair<std::string, std::map<std::string, std::string>> parse_data(
    Activation<GC>* activation,
    const std::string& url,
    Value<GC>* data
);

/// Implements flash.net.navigateToURL
template<typename GC>
Value<GC> navigate_to_url(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Register class alias
template<typename GC>
Value<GC> register_class_alias(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Get class by alias
template<typename GC>
Value<GC> get_class_by_alias(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace net
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_H
