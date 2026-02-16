// C++ translation of core/src/local_connection.rs
// LocalConnection management for AVM1 and AVM2

#ifndef RUFFLE_CORE_LOCAL_CONNECTION_H
#define RUFFLE_CORE_LOCAL_CONNECTION_H

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <optional>
#include <variant>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename GC>
    class Object;
    
    namespace globals {
    namespace local_connection {
        template<typename GC>
        class LocalConnection;
    }
    }
}

namespace avm2 {
    template<typename GC>
    class Domain;
    
    namespace object {
        template<typename GC>
        class LocalConnectionObject;
    }
}

namespace context {
    template<typename GC>
    class UpdateContext;
}

namespace string {
    template<typename GC>
    class AvmString;
}
}
}

namespace flash_lso {
namespace types {
    template<typename T>
    class Value;
}
}

namespace ruffle {
namespace wstr {
    class WStr;
    class WString;
}
}

namespace ruffle {
namespace core {

/// LocalConnection kind - can be AVM1 or AVM2
template<typename GC>
class LocalConnectionKind {
public:
    using Avm1Object = ruffle::core::avm1::Object<GC>;
    using Avm2Domain = ruffle::core::avm2::Domain<GC>;
    using Avm2LocalConnectionObject = ruffle::core::avm2::object::LocalConnectionObject<GC>;
    using Avm1LocalConnectionObject = ruffle::core::avm1::globals::local_connection::LocalConnection<GC>;
    using UpdateContext = ruffle::core::context::UpdateContext<GC>;
    using AvmString = ruffle::core::string::AvmString<GC>;
    using AmfValue = flash_lso::types::Value<>;

private:
    std::variant<
        std::pair<Avm2Domain, Avm2LocalConnectionObject>,
        Avm1Object
    > data;

public:
    /// Construct from AVM2 domain and object
    LocalConnectionKind(Avm2Domain domain, Avm2LocalConnectionObject object)
        : data(std::make_pair(domain, object)) {}

    /// Construct from AVM1 object
    explicit LocalConnectionKind(Avm1Object object)
        : data(object) {}

    /// Send status to the connection
    void send_status(AvmString status, UpdateContext* context) {
        if (auto* avm2_data = std::get_if<std::pair<Avm2Domain, Avm2LocalConnectionObject>>(&data)) {
            avm2_data->second.send_status(context, status);
        } else if (auto* avm1_object = std::get_if<Avm1Object>(&data)) {
            auto result = Avm1LocalConnectionObject::send_status(context, *avm1_object, status);
            if (!result) {
                // Log error
            }
        }
    }

    /// Run a method on the connection
    void run_method(
        UpdateContext* context,
        AvmString method_name,
        const std::vector<AmfValue>& arguments
    ) {
        if (auto* avm2_data = std::get_if<std::pair<Avm2Domain, Avm2LocalConnectionObject>>(&data)) {
            avm2_data->second.run_method(context, avm2_data->first, method_name, arguments);
        } else if (auto* avm1_object = std::get_if<Avm1Object>(&data)) {
            auto result = Avm1LocalConnectionObject::run_method(
                context, *avm1_object, method_name, arguments
            );
            if (!result) {
                // Log error
            }
        }
    }
};

/// Queued message kind
template<typename GC>
class QueuedMessageKind {
public:
    using AvmString = ruffle::core::string::AvmString<GC>;
    using AmfValue = flash_lso::types::Value<>;
    using WString = ruffle::wstr::WString;

    enum class Type {
        Failure,
        Message,
    };

    Type type;
    WString connection_name;
    AvmString method_name;
    std::vector<AmfValue> arguments;

    /// Create failure message
    static QueuedMessageKind failure() {
        QueuedMessageKind msg;
        msg.type = Type::Failure;
        return msg;
    }

    /// Create message with arguments
    static QueuedMessageKind message(
        WString conn_name,
        AvmString method,
        std::vector<AmfValue> args
    ) {
        QueuedMessageKind msg;
        msg.type = Type::Message;
        msg.connection_name = std::move(conn_name);
        msg.method_name = method;
        msg.arguments = std::move(args);
        return msg;
    }

    /// Deliver this message
    void deliver(
        const LocalConnectionKind<GC>& source,
        ruffle::core::context::UpdateContext<GC>* context
    ) {
        if (type == Type::Failure) {
            source.send_status(AvmString::from_utf8("error"), context);
        } else {
            // Find listener and deliver message
            if (auto* receiver = context->local_connections.find_listener(connection_name)) {
                source.send_status(AvmString::from_utf8("status"), context);
                receiver->run_method(context, method_name, arguments);
            } else {
                source.send_status(AvmString::from_utf8("error"), context);
            }
        }
    }
};

/// Queued message
template<typename GC>
class QueuedMessage {
public:
    LocalConnectionKind<GC> source;
    QueuedMessageKind<GC> kind;

    QueuedMessage(LocalConnectionKind<GC> src, QueuedMessageKind<GC> k)
        : source(std::move(src)), kind(std::move(k)) {}
};

/// An opaque handle to an actively listening LocalConnection.
/// Owning this handle represents ownership of a LocalConnection;
/// However, a LocalConnection must be manually closed, you can't just Drop this handle.
class LocalConnectionHandle {
private:
    ruffle::wstr::WString name;

public:
    explicit LocalConnectionHandle(ruffle::wstr::WString n) : name(std::move(n)) {}

    const ruffle::wstr::WString& get_name() const { return name; }
};

/// Manages the collection of local connections.
template<typename GC>
class LocalConnections {
public:
    using WString = ruffle::wstr::WString;
    using WStr = ruffle::wstr::WStr;
    using AvmString = ruffle::core::string::AvmString<GC>;
    using AmfValue = flash_lso::types::Value<>;

private:
    std::unordered_map<WString, LocalConnectionKind<GC>, WString::Hasher> connections;
    std::vector<QueuedMessage<GC>> messages;

public:
    /// Create empty LocalConnections
    static LocalConnections empty() {
        return LocalConnections();
    }

    /// Connect a new LocalConnection
    template<typename T>
    std::optional<LocalConnectionHandle> connect(
        const std::string& domain,
        T&& connection,
        const WStr* name
    ) {
        WString key;
        
        if (name->starts_with('_')) {
            key = name->to_ascii_lowercase();
        } else {
            key = WString::from_utf8(get_superdomain(domain));
            key.push_char(':');
            key.push_str(name);
            key.make_ascii_lowercase();
        }

        if (connections.count(key) > 0) {
            return std::nullopt;
        }

        connections[key] = LocalConnectionKind<GC>(std::forward<T>(connection));
        return LocalConnectionHandle(key);
    }

    /// Close a LocalConnection
    void close(const LocalConnectionHandle& handle) {
        connections.erase(handle.get_name());
    }

    /// Send a message to a LocalConnection
    template<typename T>
    void send(
        const std::string& domain,
        T&& source,
        AvmString connection_name,
        AvmString method_name,
        std::vector<AmfValue> arguments
    ) {
        auto conn_name = connection_name.to_ascii_lowercase();
        
        if (!conn_name.contains(':') && !conn_name.starts_with('_')) {
            auto result = WString::from_utf8(get_superdomain(domain));
            result.push_char(':');
            result.push_str(&conn_name);
            conn_name = result;
        }

        QueuedMessageKind<GC> kind;
        if (find_listener(&conn_name)) {
            kind = QueuedMessageKind<GC>::message(
                conn_name, method_name, std::move(arguments)
            );
        } else {
            kind = QueuedMessageKind<GC>::failure();
        }

        messages.emplace_back(
            LocalConnectionKind<GC>(std::forward<T>(source)),
            std::move(kind)
        );
    }

    /// Find a listener by name
    std::optional<LocalConnectionKind<GC>> find_listener(const WString* name) {
        auto it = connections.find(*name);
        if (it != connections.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    /// Update connections - process queued messages
    static void update_connections(ruffle::core::context::UpdateContext<GC>* context) {
        if (context->local_connections.messages.empty()) {
            return;
        }

        auto messages = std::move(context->local_connections.messages);
        for (auto& message : messages) {
            message.kind.deliver(message.source, context);
        }
    }

    /// Get domain from URL
    static std::string get_domain(const std::string& url) {
        // Parse URL and extract domain
        // Placeholder implementation
        if (url.find("file://") == 0) {
            return "localhost";
        }
        
        // Extract domain from URL
        auto pos = url.find("://");
        if (pos != std::string::npos) {
            pos += 3;
            auto end = url.find('/', pos);
            if (end == std::string::npos) {
                end = url.find('?', pos);
            }
            if (end == std::string::npos) {
                return url.substr(pos);
            }
            return url.substr(pos, end - pos);
        }
        
        return "unknown";
    }

    /// Get superdomain (last part of domain)
    static std::string_view get_superdomain(const std::string& domain) {
        auto pos = domain.rfind('.');
        if (pos != std::string::npos) {
            return std::string_view(domain).substr(pos + 1);
        }
        return domain;
    }

private:
    LocalConnections() = default;
};

} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_LOCAL_CONNECTION_H
