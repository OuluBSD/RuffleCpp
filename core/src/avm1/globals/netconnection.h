// C++ translation of core/src/avm1/globals/netconnection.rs
//! NetConnection class for AVM1

#ifndef RUFFLE_CORE_AVM1_GLOBALS_NETCONNECTION_H
#define RUFFLE_CORE_AVM1_GLOBALS_NETCONNECTION_H

#include <cstdint>
#include <vector>
#include <optional>
#include <memory>
#include <map>
#include <rc>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename T> class AvmString;
    namespace avm1 {
        template<typename T> class Activation;
        template<typename T> class Error;
        template<typename T> class NativeObject;
        template<typename T> class Object;
        template<typename T> class Value;
        struct DeclContext;
        struct StaticDeclarations;
        template<typename T> class SystemClass;
        enum class ExecutionReason;
        class ActivationIdentifier;
        namespace globals {
            namespace shared_object {
                template<typename T> class Value;
            }
        }
    }
    namespace context {
        template<typename T> class UpdateContext;
    }
    namespace net_connection {
        template<typename T> class NetConnectionHandle;
        template<typename T> class NetConnections;
        enum class ResponderCallback;
    }
    namespace gc_arena {
        template<typename T> class Gc;
    }
    namespace flash_lso {
        namespace packet {
            class Header;
        }
        namespace types {
            template<typename T> class Value;
            using ObjectId = uint32_t;
        }
    }
    namespace string {
        template<typename T> class WStr;
    }
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace netconnection {

/// NetConnectionData holds the internal state of a NetConnection
template<typename GC>
struct NetConnectionData {
    /// The connection handle
    std::optional<net_connection::NetConnectionHandle<GC>> handle;
};

/// NetConnection represents an AVM1 NetConnection object
///
/// Allows communication with remote services via Flash Remoting or RTMP.
template<typename GC>
class NetConnection {
public:
    /// Get the connection handle
    /// @return The connection handle if connected
    std::optional<net_connection::NetConnectionHandle<GC>> handle() const {
        return data_->handle;
    }

    /// Set the connection handle
    ///
    /// @param handle The new handle, or nullopt to disconnect
    /// @return The previous handle if any
    std::optional<net_connection::NetConnectionHandle<GC>> set_handle(
        std::optional<net_connection::NetConnectionHandle<GC>> handle
    ) {
        auto prev = data_->handle;
        data_->handle = handle;
        return prev;
    }

    /// Cast a Value to a NetConnection
    ///
    /// @param value The value to cast
    /// @return Optional NetConnection if the value is a NetConnection
    static std::optional<NetConnection> cast(Value<GC> value);

    /// Send an onStatus event to the connection
    ///
    /// @param context The update context
    /// @param this_obj The NetConnection object
    /// @param code The status code
    /// @return Error if any
    static Result<void, Error<GC>> on_status_event(
        context::UpdateContext<GC>* context,
        Object<GC>* this_obj,
        const char* code
    );

    /// Send an empty onStatus event to the connection
    ///
    /// @param context The update context
    /// @param this_obj The NetConnection object
    /// @return Error if any
    static Result<void, Error<GC>> on_empty_status_event(
        context::UpdateContext<GC>* context,
        Object<GC>* this_obj
    );

    /// Send a callback response to the connection
    ///
    /// @param context The update context
    /// @param responder The responder object
    /// @param callback The callback type (result or status)
    /// @param message The AMF message
    /// @return Error if any
    static Result<void, Error<GC>> send_callback(
        context::UpdateContext<GC>* context,
        Object<GC>* responder,
        net_connection::ResponderCallback callback,
        const flash_lso::types::Value<GC>* message
    );

private:
    NetConnection(gc_arena::Gc<NetConnectionData<GC>> data)
        : data_(data) {}

    gc_arena::Gc<NetConnectionData<GC>> data_;
};

/// Property declarations for NetConnection prototype
extern const StaticDeclarations PROTO_DECLS;

/// Create the NetConnection class
///
/// @param context The declaration context for creating the class
/// @param super_proto The superclass prototype
/// @return The created system class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// NetConnection constructor
///
/// @param activation The current activation context
/// @param this_obj The object being constructed
/// @param args Constructor arguments (unused)
/// @return Value::Undefined
template<typename GC>
Value<GC> constructor(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements NetConnection.isConnected property getter
///
/// @param activation The current activation context
/// @param this_obj The NetConnection object
/// @param args Arguments (unused)
/// @return true if connected
template<typename GC>
Value<GC> is_connected(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements NetConnection.protocol property getter
///
/// @param activation The current activation context
/// @param this_obj The NetConnection object
/// @param args Arguments (unused)
/// @return The protocol string or undefined
template<typename GC>
Value<GC> protocol(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements NetConnection.uri property getter
///
/// @param activation The current activation context
/// @param this_obj The NetConnection object
/// @param args Arguments (unused)
/// @return The URI string or undefined
template<typename GC>
Value<GC> uri(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements NetConnection.addHeader method
///
/// Adds a header to be sent with subsequent calls.
///
/// @param activation The current activation context
/// @param this_obj The NetConnection object
/// @param args Arguments (name, mustUnderstand, value)
/// @return Value::Undefined
template<typename GC>
Value<GC> add_header(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements NetConnection.call method
///
/// Calls a remote method.
///
/// @param activation The current activation context
/// @param this_obj The NetConnection object
/// @param args Arguments (command, responder, ...arguments)
/// @return Value::Undefined
template<typename GC>
Value<GC> call(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements NetConnection.close method
///
/// Closes the connection.
///
/// @param activation The current activation context
/// @param this_obj The NetConnection object
/// @param args Arguments (unused)
/// @return Value::Undefined
template<typename GC>
Value<GC> close(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements NetConnection.connect method
///
/// Connects to a remote service.
///
/// @param activation The current activation context
/// @param this_obj The NetConnection object
/// @param args Arguments (command string)
/// @return Value::Undefined
template<typename GC>
Value<GC> connect(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

} // namespace netconnection
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_NETCONNECTION_H
