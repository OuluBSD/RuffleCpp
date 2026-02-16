// C++ translation of core/src/avm2/globals/flash/net/local_connection.rs
// flash.net.LocalConnection implementation

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_LOCAL_CONNECTION_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_LOCAL_CONNECTION_H

#include <vector>
#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class AvmString;
    template<typename T> class TObject;
    namespace amf {
        template<typename T> class AmfVersion;
        template<typename T, typename U> class Value;
    }
    namespace error {
        enum class Error2004Type;
    }
    namespace object {
        template<typename T> class LocalConnectionObject;
    }
}
namespace local_connection {
    template<typename T> class LocalConnection;
    class LocalConnections;
}
namespace string {
    template<typename T> class AvmString;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace net {
namespace local_connection {

/// Re-export local_connection_allocator from the object module
template<typename GC>
using local_connection_allocator = ruffle::core::avm2::object::local_connection_allocator<GC>;

/// Implements flash.net.LocalConnection.domain getter
/// Returns the domain of the current movie for LocalConnection
template<typename GC>
Value<GC> get_domain(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.net.LocalConnection.send
/// Sends a method call to a connected LocalConnection
/// @param connectionName The name of the connection to send to
/// @param methodName The name of the method to call
/// @param args Additional arguments to pass to the method
template<typename GC>
Value<GC> send(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.net.LocalConnection.connect
/// Connects to a LocalConnection with the given name
/// @param connectionName The name to connect as
template<typename GC>
Value<GC> connect(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.net.LocalConnection.close
/// Closes the LocalConnection
template<typename GC>
Value<GC> close(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.net.LocalConnection.client getter
/// Returns the client object for this LocalConnection
template<typename GC>
Value<GC> get_client(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.net.LocalConnection.client setter
/// Sets the client object for this LocalConnection
/// @param client The object to use as the client
template<typename GC>
Value<GC> set_client(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace local_connection
} // namespace net
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_LOCAL_CONNECTION_H
