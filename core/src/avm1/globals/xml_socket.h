// C++ translation of core/src/avm1/globals/xml_socket.rs
// XMLSocket object for AVM1

#ifndef RUFFLE_CORE_AVM1_GLOBALS_XML_SOCKET_H
#define RUFFLE_CORE_AVM1_GLOBALS_XML_SOCKET_H

#include <cstdint>
#include <deque>
#include <memory>
#include <optional>

// Forward declarations
namespace ruffle {
namespace core {
namespace string {
    template<typename T> class AvmString;
}
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Error;
    template<typename T> class Object;
    template<typename T> class Value;
    struct DeclContext;
    struct StaticDeclarations;
    template<typename T> class SystemClass;
    enum class ExecutionReason;
    enum class NativeObject;
}
namespace context {
    template<typename T> struct UpdateContext;
}
namespace display_object {
    template<typename T> class TDisplayObject;
}
namespace socket {
    class SocketHandle;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace xml_socket {

/// Internal data for XMLSocket
///
/// Holds the socket handle, timeout, and read buffer.
template<typename GC>
struct XmlSocketData {
    std::optional<socket::SocketHandle> handle;
    uint32_t timeout;
    std::deque<uint8_t> read_buffer;
};

/// XMLSocket wrapper for garbage-collected data
///
/// Provides access to the underlying socket data.
template<typename GC>
class XmlSocket {
private:
    std::shared_ptr<XmlSocketData<GC>> data_;

public:
    /// Construct a new XmlSocket
    explicit XmlSocket(std::shared_ptr<XmlSocketData<GC>> data) : data_(data) {}

    /// Get the socket handle
    std::optional<socket::SocketHandle> handle() const { return data_->handle; }

    /// Set the socket handle, returning the previous value
    std::optional<socket::SocketHandle> set_handle(socket::SocketHandle handle) {
        auto old = data_->handle;
        data_->handle = handle;
        return old;
    }

    /// Get the connection timeout in milliseconds
    uint32_t timeout() const { return data_->timeout; }

    /// Set the connection timeout in milliseconds
    void set_timeout(uint32_t new_timeout) { data_->timeout = new_timeout; }

    /// Get the read buffer
    std::deque<uint8_t>& read_buffer() { return data_->read_buffer; }

    /// Cast a Value to XmlSocket
    ///
    /// \param value The value to cast
    /// \return XmlSocket if the value is an XMLSocket, std::nullopt otherwise
    static std::optional<XmlSocket> cast(const Value<GC>& value);
};

/// Property declarations for XMLSocket prototype
extern const StaticDeclarations PROTO_DECLS;

/// Create the XMLSocket class
///
/// \param context The declaration context for creating the class
/// \param super_proto The superclass prototype
/// \return A SystemClass containing the created XMLSocket class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// Implements XMLSocket.timeout getter
///
/// Gets the connection timeout in milliseconds.
///
/// \param activation The current activation context
/// \param this_obj The XMLSocket object
/// \param args Arguments (unused)
/// \return The timeout value
template<typename GC>
Value<GC> get_timeout(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args
);

/// Implements XMLSocket.timeout setter
///
/// Sets the connection timeout in milliseconds.
///
/// \param activation The current activation context
/// \param this_obj The XMLSocket object
/// \param args Arguments (timeout value)
/// \return Value::Undefined
template<typename GC>
Value<GC> set_timeout(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args
);

/// Implements XMLSocket.close method
///
/// Closes the socket connection.
///
/// \param activation The current activation context
/// \param this_obj The XMLSocket object
/// \param args Arguments (unused)
/// \return Value::Undefined
template<typename GC>
Value<GC> close(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args
);

/// Implements XMLSocket.connect method
///
/// Connects to a host and port.
///
/// \param activation The current activation context
/// \param this_obj The XMLSocket object
/// \param args Arguments (host, port)
/// \return true if connection initiated
template<typename GC>
Value<GC> connect(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args
);

/// Implements XMLSocket.send method
///
/// Sends data over the socket.
///
/// \param activation The current activation context
/// \param this_obj The XMLSocket object
/// \param args Arguments (data string)
/// \return Value::Undefined
template<typename GC>
Value<GC> send(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args
);

/// Implements XMLSocket.onConnect callback
///
/// Called when connection is established or fails.
///
/// \param activation The current activation context
/// \param this_obj The XMLSocket object
/// \param args Arguments (success boolean)
/// \return Value::Undefined
template<typename GC>
Value<GC> on_connect(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args
);

/// Implements XMLSocket.onClose callback
///
/// Called when the socket is closed.
///
/// \param activation The current activation context
/// \param this_obj The XMLSocket object
/// \param args Arguments (unused)
/// \return Value::Undefined
template<typename GC>
Value<GC> on_close(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args
);

/// Implements XMLSocket.onData callback
///
/// Called when data is received.
///
/// \param activation The current activation context
/// \param this_obj The XMLSocket object
/// \param args Arguments (data string)
/// \return Value::Undefined
template<typename GC>
Value<GC> on_data(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args
);

/// Implements XMLSocket.onXML callback
///
/// Called when XML data is received.
///
/// \param activation The current activation context
/// \param this_obj The XMLSocket object
/// \param args Arguments (XML object)
/// \return Value::Undefined
template<typename GC>
Value<GC> on_xml(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args
);

/// XMLSocket constructor
///
/// Initializes the XMLSocket object with default timeout and empty buffer.
///
/// \param activation The current activation context
/// \param this_obj The XMLSocket object being constructed
/// \param args Arguments for the constructor (unused)
/// \return Value::Undefined
template<typename GC>
Value<GC> constructor(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args
);

} // namespace xml_socket
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_XML_SOCKET_H
