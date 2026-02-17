// C++ translation of core/src/avm2/object/local_connection_object.rs
//! LocalConnection object implementation for AVM2

#ifndef RUFFLE_CORE_AVM2_OBJECT_LOCAL_CONNECTION_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_LOCAL_CONNECTION_OBJECT_H

#include <memory>
#include <cstdint>
#include <format>
#include <optional>
#include <vector>
#include <cell>

// Forward declarations
namespace ruffle {
namespace core {
    class WString;
    template<typename GC> class UpdateContext;

    namespace avm2 {
        template<typename T> class Activation;
        template<typename T> class ScriptObjectData;
        template<typename T> class TObject;
        template<typename T> class Gc;
        template<typename T> class GcWeak;
        template<typename T> class ClassObject;
        template<typename T> class Object;
        template<typename T> class Error;
        template<typename T> class Value;
        template<typename T> class AvmString;
        template<typename T> class Domain;
        template<typename T> class Mutation;
        namespace amf {
            template<typename T> class Value;
        }
        namespace function {
            template<typename T> class FunctionArgs;
        }
    }
    namespace local_connection {
        template<typename T> class LocalConnectionHandle;
        template<typename T> class LocalConnections;
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Weak reference to LocalConnectionObject data
template<typename GC>
class LocalConnectionObjectWeak;

/// Data structure for LocalConnectionObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class LocalConnectionObjectData {
public:
    LocalConnectionObjectData() = default;

    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    /// The connection handle, if connected
    std::optional<local_connection::LocalConnectionHandle<GC>> connection_handle;

    /// The client object for callbacks
    Object<GC> client;

    // Friend classes for accessing private members
    friend class LocalConnectionObject<GC>;
    friend class LocalConnectionObjectWeak<GC>;
};

/// LocalConnectionObject - AVM2 LocalConnection object representation
/// Represents a LocalConnection instance in ActionScript 3
template<typename GC>
class LocalConnectionObject {
public:
    using DataType = LocalConnectionObjectData<GC>;
    using WeakType = LocalConnectionObjectWeak<GC>;

    LocalConnectionObject() = default;
    explicit LocalConnectionObject(Gc<GC, DataType> data) : data_(data) {}

    /// Check if this connection is currently connected
    /// @return true if connected
    bool is_connected() const {
        return data_->connection_handle.has_value();
    }

    /// Get the client object
    /// @return The client object
    Object<GC> client() const {
        return data_->client;
    }

    /// Set the client object
    ///
    /// @param mc The mutation context
    /// @param client The client object to set
    void set_client(Mutation<GC>* mc, Object<GC> client);

    /// Connect to a named channel
    ///
    /// @param activation The current activation context
    /// @param name The channel name to connect to
    /// @return true if connection was successful
    bool connect(Activation<GC>* activation, AvmString<GC> name);

    /// Disconnect from the current channel
    ///
    /// @param activation The current activation context
    void disconnect(Activation<GC>* activation);

    /// Send a status callback to the connection
    ///
    /// @param context The update context
    /// @param status The status string
    void send_status(UpdateContext<GC>* context, AvmString<GC> status);

    /// Run a method on the connection
    ///
    /// @param context The update context
    /// @param domain The domain to run the method in
    /// @param method_name The method name to call
    /// @param amf_arguments The AMF-encoded arguments
    void run_method(
        UpdateContext<GC>* context,
        Domain<GC> domain,
        AvmString<GC> method_name,
        std::vector<amf::Value<GC>> amf_arguments
    );

    /// Get the base script object data (for TObject trait)
    Gc<GC, ScriptObjectData<GC>> gc_base() const {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base;
    }

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to LocalConnectionObject
template<typename GC>
class LocalConnectionObjectWeak {
public:
    LocalConnectionObjectWeak() = default;
    explicit LocalConnectionObjectWeak(GcWeak<GC, LocalConnectionObjectData<GC>> data)
        : data_(data) {}

private:
    GcWeak<GC, LocalConnectionObjectData<GC>> data_;
};

// Template method implementations

template<typename GC>
void LocalConnectionObject<GC>::set_client(Mutation<GC>* mc, Object<GC> client) {
    data_->client = client;
}

template<typename GC>
bool LocalConnectionObject<GC>::connect(Activation<GC>* activation, AvmString<GC> name) {
    if (is_connected()) {
        return false;
    }

    auto connection_handle = activation->context()->local_connections.connect(
        &LocalConnections<GC>::get_domain(activation->context()->root_swf.url()),
        std::make_pair(activation->domain(), *this),
        name
    );
    auto result = connection_handle.has_value();

    data_->connection_handle = connection_handle;

    return result;
}

template<typename GC>
void LocalConnectionObject<GC>::disconnect(Activation<GC>* activation) {
    if (data_->connection_handle.has_value()) {
        activation->context()->local_connections.close(data_->connection_handle.value());
        data_->connection_handle = std::nullopt;
    }
}

template<typename GC>
void LocalConnectionObject<GC>::send_status(UpdateContext<GC>* context, AvmString<GC> status) {
    // Would need proper implementation with EventObject creation
    // This is a simplified stub
}

template<typename GC>
void LocalConnectionObject<GC>::run_method(
    UpdateContext<GC>* context,
    Domain<GC> domain,
    AvmString<GC> method_name,
    std::vector<amf::Value<GC>> amf_arguments
) {
    // Would need proper implementation with AMF deserialization
    // This is a simplified stub
}

} // namespace avm2
} // namespace core
} // namespace ruffle

// Debug formatter specialization
template<typename GC>
struct std::formatter<ruffle::core::avm2::LocalConnectionObject<GC>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const ruffle::core::avm2::LocalConnectionObject<GC>& obj, FormatContext& ctx) {
        return std::format_to(ctx.out(), "LocalConnectionObject(ptr={})",
            static_cast<const void*>(obj.data_.get()));
    }
};

#endif // RUFFLE_CORE_AVM2_OBJECT_LOCAL_CONNECTION_OBJECT_H
