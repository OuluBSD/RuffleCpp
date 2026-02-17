// C++ translation of core/src/net_connection.rs
//! NetConnection management for AVM1 and AVM2

#ifndef RUFFLE_CORE_NET_CONNECTION_H
#define RUFFLE_CORE_NET_CONNECTION_H

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <future>
#include <mutex>

// Forward declarations
namespace ruffle {
namespace core {
    class Player;
    template<typename GC>
    class UpdateContext;

    namespace avm1 {
        template<typename GC>
        class Object;
        namespace globals {
            namespace netconnection {
                template<typename GC>
                class NetConnection;
            }
        }
    }

    namespace avm2 {
        template<typename GC>
        class NetConnectionObject;
        template<typename GC>
        class ResponderObject;
        template<typename GC>
        class Activation;
        template<typename GC>
        class Avm2;
        template<typename GC>
        class EventObject;
    }

    namespace backend {
        class NavigatorBackend;
        using OwnedFuture = std::function<std::future<void>()>;

        namespace navigator {
            struct ErrorResponse;
            enum class FetchReason;
            struct Request;
        }
    }

    namespace loader {
        struct Error;
    }

    namespace dynamic_root {
        template<typename T>
        class DynamicRoot;
    }

    namespace gc_arena {
        template<typename T>
        class Gc;
        template<typename T>
        class Rootable;
        template<typename T>
        class Mutation;
    }
}
}

// Forward declaration for flash_lso types
namespace flash_lso {
    namespace types {
        enum class AMFVersion;
        class Value;
    }
    namespace packet {
        struct Header;
        struct Message;
        struct Packet;
    }
}

namespace slotmap {
    template<typename Key, typename Value>
    class SlotMap;

    template<typename T>
    struct KeyBuilder;
}

namespace ruffle {
namespace core {

/// Handle for identifying NetConnections
struct NetConnectionHandle {
    uint64_t id;
    bool operator==(const NetConnectionHandle& other) const { return id == other.id; }
    bool operator!=(const NetConnectionHandle& other) const { return id != other.id; }
};

/// Responder callback types
enum class ResponderCallback {
    Result,
    Status
};

/// Handle to a responder object (AVM1 or AVM2)
class ResponderHandle {
public:
    enum class Type {
        Avm1,
        Avm2
    };

    ResponderHandle(Type type, void* handle) : type_(type), handle_(handle) {}

    Type type() const { return type_; }

    /// Call the responder callback
    template<typename GC>
    void call(
        UpdateContext<GC>* context,
        ResponderCallback callback,
        const std::shared_ptr<flash_lso::types::Value>& message
    );

private:
    Type type_;
    void* handle_;  // Opaque handle to either AVM1 or AVM2 responder
};

/// NetConnection object wrapper (AVM1 or AVM2)
template<typename GC>
class NetConnectionObject {
public:
    enum class Type {
        Avm1,
        Avm2
    };

    NetConnectionObject(Type type, void* object) : type_(type), object_(object) {}

    Type type() const { return type_; }

    /// Set the connection handle
    std::optional<NetConnectionHandle> set_handle(std::optional<NetConnectionHandle> handle);

private:
    Type type_;
    void* object_;  // Opaque handle to either AVM1 or AVM2 object
};

/// NetConnection protocol types
enum class NetConnectionProtocol {
    /// A "local" connection, caused by connecting to null
    Local,

    /// Flash Remoting protocol, caused by connecting to a http:// address
    FlashRemoting
};

/// Flash Remoting data
struct FlashRemoting {
    std::string url;
    std::vector<flash_lso::packet::Header> headers;
    std::vector<std::pair<flash_lso::packet::Message, std::optional<ResponderHandle>>> outgoing_queue;

    /// Send a message through the remoting connection
    void send(
        const std::string& command,
        std::optional<ResponderHandle> responder_handle,
        const flash_lso::types::Value& message
    );

    /// Check if there are pending packets
    bool has_pending_packet() const { return !outgoing_queue.empty(); }

    /// Set a header
    void set_header(const flash_lso::packet::Header& header);

    /// Flush the outgoing queue
    std::future<void> flush_queue(
        NetConnectionHandle self_handle,
        std::shared_ptr<std::mutex> player_mutex
    );
};

/// A single NetConnection
template<typename GC>
class NetConnection {
public:
    NetConnection(
        NetConnectionObject<GC> object,
        NetConnectionProtocol protocol,
        std::optional<FlashRemoting> remoting = std::nullopt
    ) : object_(object), protocol_(protocol), remoting_(std::move(remoting)) {}

    /// Check if the connection is connected
    bool is_connected() const {
        return protocol_ == NetConnectionProtocol::Local;
    }

    /// Get the connected proxy type
    std::optional<const char*> connected_proxy_type() const {
        if (protocol_ == NetConnectionProtocol::Local) {
            return "none";
        }
        return std::nullopt;
    }

    /// Get the far ID
    std::optional<const char*> far_id() const {
        if (protocol_ == NetConnectionProtocol::Local) {
            return "";
        }
        return std::nullopt;
    }

    /// Get the far nonce
    std::optional<const char*> far_nonce() const {
        if (protocol_ == NetConnectionProtocol::Local) {
            return "0000000000000000000000000000000000000000000000000000000000000000";
        }
        return std::nullopt;
    }

    /// Get the near ID
    std::optional<const char*> near_id() const {
        if (protocol_ == NetConnectionProtocol::Local) {
            return "";
        }
        return std::nullopt;
    }

    /// Get the near nonce
    std::optional<const char*> near_nonce() const {
        if (protocol_ == NetConnectionProtocol::Local) {
            return "0000000000000000000000000000000000000000000000000000000000000000";
        }
        return std::nullopt;
    }

    /// Get the protocol string
    std::optional<const char*> protocol() const {
        if (protocol_ == NetConnectionProtocol::Local) {
            return "rtmp";
        }
        return std::nullopt;
    }

    /// Get the URI
    std::optional<std::string> uri() const {
        if (protocol_ == NetConnectionProtocol::Local) {
            return "null";  // Yes, it's a string "null", not a real null
        }
        if (protocol_ == NetConnectionProtocol::FlashRemoting && remoting_.has_value()) {
            return remoting_->url;
        }
        return std::nullopt;
    }

    /// Check if using TLS
    std::optional<bool> using_tls() const {
        if (protocol_ == NetConnectionProtocol::Local) {
            return false;
        }
        return std::nullopt;
    }

    /// Send a message
    void send(
        const std::string& command,
        std::optional<ResponderHandle> responder_handle,
        const flash_lso::types::Value& message
    );

    /// Update the connection (process pending operations)
    template<typename GC>
    void update(
        NetConnectionHandle self_handle,
        backend::NavigatorBackend* navigator,
        std::shared_ptr<std::mutex> player_mutex
    );

    /// Set a header
    void set_header(const flash_lso::packet::Header& header);

private:
    NetConnectionObject<GC> object_;
    NetConnectionProtocol protocol_;
    std::optional<FlashRemoting> remoting_;
};

/// Manages the collection of NetConnections
template<typename GC>
class NetConnections {
public:
    NetConnections() = default;

    /// Connect to local (null)
    static void connect_to_local(
        UpdateContext<GC>* context,
        NetConnectionObject<GC> target
    );

    /// Connect to Flash Remoting
    static void connect_to_flash_remoting(
        UpdateContext<GC>* context,
        NetConnectionObject<GC> target,
        const std::string& url
    );

    /// Close a connection
    static void close(
        UpdateContext<GC>* context,
        NetConnectionHandle handle,
        bool is_explicit
    );

    /// Update all connections
    static void update_connections(UpdateContext<GC>* context);

    /// Send without response
    static void send_without_response(
        UpdateContext<GC>* context,
        NetConnectionHandle handle,
        const std::string& command,
        const flash_lso::types::Value& message
    );

    /// Send with AVM2 responder
    template<typename GC>
    static void send_avm2(
        UpdateContext<GC>* context,
        NetConnectionHandle handle,
        const std::string& command,
        const flash_lso::types::Value& message,
        avm2::ResponderObject<GC> responder
    );

    /// Send with AVM1 responder
    template<typename GC>
    static void send_avm1(
        UpdateContext<GC>* context,
        NetConnectionHandle handle,
        const std::string& command,
        const flash_lso::types::Value& message,
        avm1::Object<GC> responder
    );

    /// Set a header
    void set_header(NetConnectionHandle handle, const flash_lso::packet::Header& header);

    /// Check if connected
    bool is_connected(NetConnectionHandle handle) const;

    /// Get connected proxy type
    std::optional<const char*> get_connected_proxy_type(NetConnectionHandle handle) const;

    /// Get far ID
    std::optional<const char*> get_far_id(NetConnectionHandle handle) const;

    /// Get far nonce
    std::optional<const char*> get_far_nonce(NetConnectionHandle handle) const;

    /// Get near ID
    std::optional<const char*> get_near_id(NetConnectionHandle handle) const;

    /// Get near nonce
    std::optional<const char*> get_near_nonce(NetConnectionHandle handle) const;

    /// Get protocol
    std::optional<const char*> get_protocol(NetConnectionHandle handle) const;

    /// Get URI
    std::optional<std::string> get_uri(NetConnectionHandle handle) const;

    /// Check if using TLS
    std::optional<bool> is_using_tls(NetConnectionHandle handle) const;

private:
    slotmap::SlotMap<NetConnectionHandle, NetConnection<GC>> connections_;
};

} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_NET_CONNECTION_H
