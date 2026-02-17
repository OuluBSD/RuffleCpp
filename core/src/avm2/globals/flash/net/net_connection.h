// C++ translation of core/src/avm2/globals/flash/net/net_connection.rs
//! flash.net.NetConnection builtin/prototype

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_NET_CONNECTION_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_NET_CONNECTION_H

#include <vector>
#include <string>
#include <optional>
#include <memory>
#include <map>

// Forward declarations
namespace ruffle {
namespace core {
    class WStr;
    class AvmString;
    template<typename GC> class Activation;
    template<typename GC> class Value;
    template<typename GC> class Error;
    template<typename GC> class Object;
    template<typename GC> class ResponderObject;
    template<typename GC> class UpdateContext;

    namespace avm2 {
        template<typename GC> class NetConnectionObject;
    }

    namespace net_connection {
        class NetConnections;
        template<typename GC> class NetConnection;
    }
}
}

namespace flash_lso {
namespace packet {
    struct Header;
}
namespace types {
    enum class AMFVersion;
    struct ObjectId;
    class Value;
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace net {
namespace net_connection {

/// Connect to a server
///
/// @param activation The current activation context
/// @param this_value The NetConnection object (this)
/// @param args Arguments: [command: String]
/// @return Value::Undefined on success, Error on failure
template<typename GC>
Value<GC> connect(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Close the connection
///
/// @param activation The current activation context
/// @param this_value The NetConnection object (this)
/// @param args Unused
/// @return Value::Undefined on success, Error on failure
template<typename GC>
Value<GC> close(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Get the connected status
///
/// @param activation The current activation context
/// @param this_value The NetConnection object (this)
/// @param args Unused
/// @return Boolean indicating if connected
template<typename GC>
Value<GC> get_connected(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Get the connected proxy type
///
/// @param activation The current activation context
/// @param this_value The NetConnection object (this)
/// @param args Unused
/// @return Proxy type string, or Error 2126 if not connected
template<typename GC>
Value<GC> get_connected_proxy_type(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Get the far ID
///
/// @param activation The current activation context
/// @param this_value The NetConnection object (this)
/// @param args Unused
/// @return Far ID string, or Error 2126 if not connected
template<typename GC>
Value<GC> get_far_id(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Get the far nonce
///
/// @param activation The current activation context
/// @param this_value The NetConnection object (this)
/// @param args Unused
/// @return Far nonce string, or Error 2126 if not connected
template<typename GC>
Value<GC> get_far_nonce(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Get the near ID
///
/// @param activation The current activation context
/// @param this_value The NetConnection object (this)
/// @param args Unused
/// @return Near ID string, or Error 2126 if not connected
template<typename GC>
Value<GC> get_near_id(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Get the near nonce
///
/// @param activation The current activation context
/// @param this_value The NetConnection object (this)
/// @param args Unused
/// @return Near nonce string, or Error 2126 if not connected
template<typename GC>
Value<GC> get_near_nonce(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Get the protocol
///
/// @param activation The current activation context
/// @param this_value The NetConnection object (this)
/// @param args Unused
/// @return Protocol string, or Error 2126 if not connected
template<typename GC>
Value<GC> get_protocol(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Get the URI
///
/// @param activation The current activation context
/// @param this_value The NetConnection object (this)
/// @param args Unused
/// @return URI string, or Null if not connected
template<typename GC>
Value<GC> get_uri(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Get whether using TLS
///
/// @param activation The current activation context
/// @param this_value The NetConnection object (this)
/// @param args Unused
/// @return Boolean indicating TLS usage, or Error 2126 if not connected
template<typename GC>
Value<GC> get_using_tls(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Call a remote method
///
/// @param activation The current activation context
/// @param this_value The NetConnection object (this)
/// @param args Arguments: [command: String, responder: Responder, ...arguments]
/// @return Value::Undefined on success, Error on failure
template<typename GC>
Value<GC> call(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Add a header to the connection
///
/// Headers persist for the duration of the connection object.
/// Note: Documentation says headers persist for the object duration,
/// but in practice they are lost when opening a new connection.
/// We store them on active connection objects only.
///
/// @param activation The current activation context
/// @param this_value The NetConnection object (this)
/// @param args Arguments: [name: String, mustUnderstand: Boolean, value: *]
/// @return Value::Undefined on success
template<typename GC>
Value<GC> add_header(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Helper: Convert object to index map for AMF serialization
template<typename GC>
std::map<std::string, std::string> object_to_index_map(
    Activation<GC>* activation,
    Object<GC>* obj
);

/// Helper: Serialize value to AMF
template<typename GC>
std::optional<flash_lso::types::Value> serialize_value(
    Activation<GC>* activation,
    Value<GC> value,
    flash_lso::types::AMFVersion version,
    std::map<void*, size_t>* object_table
);

} // namespace net_connection
} // namespace net
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_NET_CONNECTION_H
