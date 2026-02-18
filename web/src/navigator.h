// C++ translation of web/src/navigator.rs
// Navigator backend for web

#ifndef RUFFLE_WEB_NAVIGATOR_H
#define RUFFLE_WEB_NAVIGATOR_H

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <chrono>
#include <map>
#include <functional>
#include <future>

// Forward declarations
namespace ruffle {
namespace core {
    class Player;
    namespace backend {
        namespace navigator {
            class NavigatorBackend;
            struct Request;
            class SuccessResponse;
            class ErrorResponse;
            enum class NavigationMethod;
            using OwnedFuture = std::future<std::pair<std::unique_ptr<SuccessResponse>, std::unique_ptr<ErrorResponse>>>;
        }
    }
    namespace config {
        enum class NetworkingAccessMode;
    }
    namespace socket {
        struct SocketHandle;
        enum class ConnectionState;
        enum class SocketAction;
    }
    namespace swf {
        enum class Encoding;
    }
    namespace loader {
        enum class Error;
    }
    template<typename K, typename V>
    class IndexMap;
}
}

// Socket proxy structure
struct SocketProxy {
    std::string host;
    uint16_t port;
    std::string proxy_url;
};

namespace ruffle {
namespace web {

/// The handling mode of links opening a new website.
enum class OpenUrlMode {
    /// Allow all links to open a new website.
    Allow,

    /// A confirmation dialog opens with every link trying to open a new website.
    Confirm,

    /// Deny all links to open a new website.
    Deny,
};

/// Web navigator backend implementation
/// Handles URL navigation, fetching, and socket connections in web environment
class WebNavigatorBackend : public ruffle::core::backend::navigator::NavigatorBackend {
private:
    // Note: Log subscriber would be implemented with appropriate logging framework
    bool allow_script_access;
    ruffle::core::config::NetworkingAccessMode allow_networking;
    bool upgrade_to_https;
    std::vector<std::pair<std::string, std::string>> url_rewrite_rules; // RegExp, replacement
    std::optional<std::string> base_url;
    OpenUrlMode open_url_mode;
    std::vector<SocketProxy> socket_proxies;
    std::vector<std::string> credential_allow_list;
    std::weak_ptr<ruffle::core::Player> player;

    /// Try to rewrite the URL using URL rewrite rules.
    std::optional<std::string> rewrite_url(const std::string& url) const;

public:
    /// Create a new WebNavigatorBackend
    /// 
    /// @param allow_script_access Whether to allow script access
    /// @param allow_networking Networking access mode
    /// @param upgrade_to_https Whether to upgrade HTTP to HTTPS
    /// @param url_rewrite_rules URL rewrite rules (pattern, replacement)
    /// @param base_url Base URL for relative URL resolution
    /// @param open_url_mode How to handle opening URLs
    /// @param socket_proxies WebSocket proxy configurations
    /// @param credential_allow_list List of URLs allowed to include credentials
    WebNavigatorBackend(
        bool allow_script_access,
        ruffle::core::config::NetworkingAccessMode allow_networking,
        bool upgrade_to_https,
        const std::vector<std::pair<std::string, std::string>>& url_rewrite_rules,
        const std::optional<std::string>& base_url,
        OpenUrlMode open_url_mode,
        const std::vector<SocketProxy>& socket_proxies,
        const std::vector<std::string>& credential_allow_list
    );

    /// Set the player after construction (player is created after navigator)
    void set_player(std::weak_ptr<ruffle::core::Player> player);

    // NavigatorBackend interface implementation

    /// Navigate to a URL
    /// 
    /// @param url The URL to navigate to
    /// @param target The target window/frame
    /// @param vars_method Optional navigation method and form variables
    void navigate_to_url(
        const std::string& url,
        const std::string& target,
        const std::optional<std::pair<ruffle::core::backend::navigator::NavigationMethod, 
                                       ruffle::core::IndexMap<std::string, std::string>>>& vars_method
    ) override;

    /// Fetch a resource
    /// 
    /// @param request The request to fetch
    /// @return A future containing the response or error
    ruffle::core::backend::navigator::OwnedFuture fetch(
        ruffle::core::backend::navigator::Request request
    ) override;

    /// Resolve a URL relative to the base URL
    /// 
    /// @param url The URL to resolve
    /// @return The resolved URL or error
    std::optional<std::string> resolve_url(const std::string& url) const override;

    /// Spawn an asynchronous future
    /// 
    /// @param future The future to spawn
    void spawn_future(std::future<void> future) override;

    /// Pre-process a URL (apply rewrite rules and HTTPS upgrade)
    /// 
    /// @param url The URL to pre-process
    /// @return The processed URL
    std::string pre_process_url(std::string url) const override;

    /// Connect to a socket
    /// 
    /// @param host The host to connect to
    /// @param port The port to connect to
    /// @param timeout Connection timeout (unused for WebSocket)
    /// @param handle The socket handle
    /// @param receiver Receiver for incoming data
    /// @param sender Sender for socket actions
    void connect_socket(
        const std::string& host,
        uint16_t port,
        std::chrono::milliseconds timeout,
        ruffle::core::socket::SocketHandle handle,
        // Note: Channel types would need proper implementation
        void* receiver,
        void* sender
    ) override;

    /// Get the base URL
    const std::optional<std::string>& get_base_url() const { return base_url; }

    /// Check if script access is allowed
    bool is_script_access_allowed() const { return allow_script_access; }

    /// Check if networking is allowed
    bool is_networking_allowed() const { 
        return allow_networking == ruffle::core::config::NetworkingAccessMode::All; 
    }

    /// Check if HTTPS upgrade is enabled
    bool is_upgrade_to_https() const { return upgrade_to_https; }

    /// Get the open URL mode
    OpenUrlMode get_open_url_mode() const { return open_url_mode; }

    /// Get URL rewrite rules
    const std::vector<std::pair<std::string, std::string>>& get_url_rewrite_rules() const {
        return url_rewrite_rules;
    }

    /// Get socket proxies
    const std::vector<SocketProxy>& get_socket_proxies() const { return socket_proxies; }

    /// Get credential allow list
    const std::vector<std::string>& get_credential_allow_list() const { return credential_allow_list; }
};

/// Web response wrapper
/// Wraps the web response for compatibility with SuccessResponse interface
class WebResponseWrapper : public ruffle::core::backend::navigator::SuccessResponse {
private:
    std::optional<std::string> rewritten_url;
    // Note: Web response would be implemented with appropriate web API bindings
    std::string response_url;
    uint16_t status_code;
    bool redirected_flag;
    std::optional<std::string> content_type;
    std::vector<uint8_t> body_data;

public:
    WebResponseWrapper(
        const std::string& url,
        uint16_t status,
        bool redirected,
        const std::optional<std::string>& content_type,
        std::vector<uint8_t> body
    );

    /// Get the URL of the response
    std::string url() const override {
        return rewritten_url.value_or(response_url);
    }

    /// Set the URL of the response
    void set_url(const std::string& url) override {
        rewritten_url = url;
    }

    /// Get the response body
    std::vector<uint8_t> body() const override {
        return body_data;
    }

    /// Get the text encoding
    std::optional<ruffle::core::swf::Encoding> text_encoding() const override;

    /// Get the status code
    uint16_t status() const override {
        return status_code;
    }

    /// Check if the response was redirected
    bool redirected() const override {
        return redirected_flag;
    }

    /// Get the next chunk of data (for streaming)
    std::optional<std::vector<uint8_t>> next_chunk() override;

    /// Get the expected length of the response
    std::optional<uint64_t> expected_length() const override;
};

} // namespace web
} // namespace ruffle

#endif // RUFFLE_WEB_NAVIGATOR_H
