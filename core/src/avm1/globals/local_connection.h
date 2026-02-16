// C++ translation of core/src/avm1/globals/local_connection.rs
// LocalConnection class for AVM1

#ifndef RUFFLE_CORE_AVM1_GLOBALS_LOCAL_CONNECTION_H
#define RUFFLE_CORE_AVM1_GLOBALS_LOCAL_CONNECTION_H

#include <cstdint>
#include <vector>
#include <optional>
#include <memory>

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
}
namespace context {
    template<typename T> class UpdateContext;
}
namespace display_object {
    template<typename T> class TDisplayObject;
    template<typename T> class DisplayObject;
}
namespace local_connection {
    template<typename T> class LocalConnectionHandle;
    template<typename T> class LocalConnections;
}
namespace gc_arena {
    template<typename T> class Gc;
    template<typename T> class RefCell;
}
namespace flash_lso {
namespace types {
    template<typename T> class Value;
}
namespace read {
    class Reader;
}
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace local_connection {

/// LocalConnectionData holds the internal state of a LocalConnection
template<typename GC>
struct LocalConnectionData {
    /// The connection handle, if connected
    std::optional<local_connection::LocalConnectionHandle<GC>> handle;
};

/// LocalConnection represents an AVM1 LocalConnection object
///
/// Allows communication between different SWF files on the same domain.
template<typename GC>
class LocalConnection {
public:
    /// Cast a Value to a LocalConnection
    ///
    /// \param value The value to cast
    /// \return Optional LocalConnection if the value is a LocalConnection
    static std::optional<LocalConnection> cast(Value<GC> value);

    /// Check if this connection is currently connected
    ///
    /// \return true if connected
    bool is_connected() const;

    /// Connect to a named channel
    ///
    /// \param activation The current activation context
    /// \param name The channel name to connect to
    /// \param this_obj The LocalConnection object
    /// \return true if connection was successful
    bool connect(
        Activation<GC>* activation,
        AvmString<GC> name,
        Object<GC>* this_obj
    );

    /// Disconnect from the current channel
    ///
    /// \param activation The current activation context
    void disconnect(Activation<GC>* activation);

    /// Send a status callback to the connection
    ///
    /// \param context The update context
    /// \param this_obj The LocalConnection object
    /// \param status The status string
    /// \return Error if any
    static Result<void, Error<GC>> send_status(
        context::UpdateContext<GC>* context,
        Object<GC>* this_obj,
        AvmString<GC> status
    );

    /// Run a method on the connection
    ///
    /// \param context The update context
    /// \param this_obj The LocalConnection object
    /// \param method_name The method name to call
    /// \param amf_arguments The AMF-encoded arguments
    /// \return Error if any
    static Result<void, Error<GC>> run_method(
        context::UpdateContext<GC>* context,
        Object<GC>* this_obj,
        AvmString<GC> method_name,
        std::vector<flash_lso::types::Value<GC>> amf_arguments
    );

private:
    LocalConnection(gc_arena::Gc<LocalConnectionData<GC>> data)
        : data_(data) {}

    gc_arena::Gc<LocalConnectionData<GC>> data_;
};

/// Property declarations for LocalConnection prototype
extern const StaticDeclarations PROTO_DECLS;

/// Create the LocalConnection class
///
/// \param context The declaration context for creating the class
/// \param super_proto The superclass prototype
/// \return The created system class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// LocalConnection constructor
///
/// \param activation The current activation context
/// \param this_obj The object being constructed
/// \param args Constructor arguments
/// \param arg_count Number of arguments
/// \return The constructed object
template<typename GC>
Value<GC> constructor(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements LocalConnection.domain method
///
/// Gets the security domain for the current movie.
///
/// \param activation The current activation context
/// \param this_obj The LocalConnection object
/// \param args Arguments (unused)
/// \return The domain string
template<typename GC>
Value<GC> domain(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements LocalConnection.connect method
///
/// Connects to a named channel.
///
/// \param activation The current activation context
/// \param this_obj The LocalConnection object
/// \param args Arguments (channel name)
/// \param arg_count Number of arguments
/// \return true if connection was successful
template<typename GC>
Value<GC> connect(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements LocalConnection.send method
///
/// Sends a message to a connected channel.
///
/// \param activation The current activation context
/// \param this_obj The LocalConnection object
/// \param args Arguments (channel name, method name, method arguments)
/// \param arg_count Number of arguments
/// \return true if message was sent
template<typename GC>
Value<GC> send(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements LocalConnection.close method
///
/// Closes the connection.
///
/// \param activation The current activation context
/// \param this_obj The LocalConnection object
/// \param args Arguments (unused)
/// \return Value::Undefined
template<typename GC>
Value<GC> close(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements LocalConnection.isPerUser property getter
///
/// \param activation The current activation context
/// \param this_obj The LocalConnection object
/// \param args Arguments (unused)
/// \return Value::Undefined (stub)
template<typename GC>
Value<GC> is_per_user(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

} // namespace local_connection
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_LOCAL_CONNECTION_H
