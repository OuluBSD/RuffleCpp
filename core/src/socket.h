// C++ translation of core/src/socket.rs
// Socket management for AVM1 and AVM2

#ifndef RUFFLE_CORE_SOCKET_H
#define RUFFLE_CORE_SOCKET_H

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <functional>
#include <chrono>

// Forward declarations
namespace ruffle {
namespace avm1 {
    template<typename GC>
    class Object;
    namespace globals {
        namespace xml_socket {
            template<typename GC>
            class XmlSocket;
        }
    }
}
namespace avm2 {
    template<typename GC>
    class SocketObject;
    template<typename GC>
    class Activation;
    class Avm2;
    namespace object {
        template<typename GC>
        class EventObject;
    }
}
namespace backend {
    class NavigatorBackend;
}
template<typename GC>
class UpdateContext;
template<typename GC>
class AvmString;
}

namespace ruffle {
namespace core {

/// Handle for identifying sockets
using SocketHandle = uint32_t;

/// Connection state for sockets
enum class ConnectionState {
    Connected,
    Failed,
    TimedOut,
};

/// Actions that can occur on sockets
enum class SocketAction {
    Connect,
    Data,
    Close,
};

/// Socket action data
struct SocketActionData {
    SocketAction action;
    SocketHandle handle;
    ConnectionState connection_state;
    std::vector<uint8_t> data;
};

/// Socket target - either AVM1 or AVM2 object
template<typename GC>
class SocketTarget {
public:
    enum class Kind {
        Avm1,
        Avm2,
    };
    
private:
    Kind kind;
    union {
        avm1::Object<GC> avm1_object;
        avm2::SocketObject<GC> avm2_object;
    };
    
public:
    static SocketTarget avm2(avm2::SocketObject<GC> obj) {
        SocketTarget target;
        target.kind = Kind::Avm2;
        target.avm2_object = obj;
        return target;
    }
    
    static SocketTarget avm1(avm1::Object<GC> obj) {
        SocketTarget target;
        target.kind = Kind::Avm1;
        target.avm1_object = obj;
        return target;
    }
    
    Kind get_kind() const { return kind; }
    
    avm2::SocketObject<GC> as_avm2() const {
        return kind == Kind::Avm2 ? avm2_object : avm2::SocketObject<GC>();
    }
    
    avm1::Object<GC> as_avm1() const {
        return kind == Kind::Avm1 ? avm1_object : avm1::Object<GC>();
    }
};

/// Socket data structure
template<typename GC>
class Socket {
private:
    SocketTarget<GC> target;
    bool connected;
    // Sender would be implemented with actual async channel
    
public:
    Socket(SocketTarget<GC> target) 
        : target(target), connected(false) {}
    
    SocketTarget<GC> get_target() const { return target; }
    bool is_connected() const { return connected; }
    void set_connected(bool c) { connected = c; }
};

/// Manages the collection of Sockets
template<typename GC>
class Sockets {
private:
    std::vector<std::pair<SocketHandle, Socket<GC>>> sockets;
    std::vector<SocketActionData> pending_actions;
    SocketHandle next_handle;
    
public:
    Sockets() : next_handle(1) {}
    
    /// Create empty socket manager
    static Sockets empty() {
        return Sockets();
    }
    
    /// Connect an AVM2 socket
    void connect_avm2(
        backend::NavigatorBackend* backend,
        avm2::SocketObject<GC> target,
        const std::string& host,
        uint16_t port
    ) {
        SocketTarget<GC> socket_target = SocketTarget<GC>::avm2(target);
        Socket<GC> socket(socket_target);
        SocketHandle handle = next_handle++;
        
        sockets.emplace_back(handle, std::move(socket));
        
        // TODO: Call backend->connect_socket with proper parameters
        // backend->connect_socket(sanitize_host(host), port, timeout, handle, ...);
        
        // Close existing connection if any
        if (auto existing = target.get_handle()) {
            close(*existing);
        }
    }
    
    /// Connect an AVM1 socket
    void connect_avm1(
        backend::NavigatorBackend* backend,
        avm1::Object<GC> target,
        const std::string& host,
        uint16_t port
    ) {
        // Cast to XmlSocket
        auto xml_socket = target.cast<avm1::globals::xml_socket::XmlSocket<GC>>();
        if (!xml_socket) return;
        
        SocketTarget<GC> socket_target = SocketTarget<GC>::avm1(target);
        Socket<GC> socket(socket_target);
        SocketHandle handle = next_handle++;
        
        sockets.emplace_back(handle, std::move(socket));
        
        // TODO: Call backend->connect_socket with proper parameters
        
        // Close existing connection if any
        if (auto existing = xml_socket->get_handle()) {
            close(*existing);
        }
    }
    
    /// Check if socket is connected
    bool is_connected(SocketHandle handle) const {
        for (const auto& [h, socket] : sockets) {
            if (h == handle) {
                return socket.is_connected();
            }
        }
        return false;
    }
    
    /// Send data to socket
    void send(SocketHandle handle, const std::vector<uint8_t>& data) {
        for (auto& [h, socket] : sockets) {
            if (h == handle) {
                // TODO: Send data through channel
                return;
            }
        }
    }
    
    /// Close all sockets
    void close_all() {
        for (auto& [handle, socket] : sockets) {
            close_internal(socket);
        }
        sockets.clear();
    }
    
    /// Close a specific socket
    void close(SocketHandle handle) {
        for (auto it = sockets.begin(); it != sockets.end(); ++it) {
            if (it->first == handle) {
                close_internal(it->second);
                sockets.erase(it);
                return;
            }
        }
    }
    
    /// Update sockets - process pending actions
    static void update_sockets(UpdateContext<GC>* context) {
        // Process pending actions from receiver
        // This would be called each frame to dispatch socket events
    }
    
private:
    void close_internal(const Socket<GC>& socket) {
        // Clear buffers based on target type
        switch (socket.get_target().get_kind()) {
            case SocketTarget<GC>::Kind::Avm1: {
                // Clear AVM1 read buffer
                break;
            }
            case SocketTarget<GC>::Kind::Avm2: {
                // Clear AVM2 read and write buffers
                break;
            }
        }
    }
};

/// Flash treats a socket host as a cstring, and stops reading at a null byte.
/// We need to account for this here.
inline std::string sanitize_host(const std::string& host) {
    auto pos = host.find('\0');
    if (pos != std::string::npos) {
        return host.substr(0, pos);
    }
    return host;
}

} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_SOCKET_H
