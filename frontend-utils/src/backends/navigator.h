// C++ translation of frontend-utils/src/backends/navigator.rs
// Navigator backend for non-web environments

#ifndef RUFFLE_FRONTEND_UTILS_NAVIGATOR_H
#define RUFFLE_FRONTEND_UTILS_NAVIGATOR_H

#include <string>
#include <memory>
#include <vector>
#include <unordered_set>
#include <functional>
#include <future>
#include <chrono>

// Forward declarations
namespace ruffle {
namespace core {
namespace backend {
    class NavigatorBackend;
    enum class NavigationMethod;
    enum class SocketMode;
    struct Request;
    class SuccessResponse;
    class ErrorResponse;
}
namespace socket {
    struct SocketHandle;
    enum class ConnectionState;
    enum class SocketAction;
}
namespace loader {
    enum class Error;
}
namespace indexmap {
    template<typename K, typename V>
    class IndexMap;
}
}
}

namespace url {
    class Url;
    enum class ParseError;
}

namespace ruffle {
namespace frontend_utils {
namespace backends {

/// Navigator interface for platform-specific operations
class NavigatorInterface {
public:
    virtual ~NavigatorInterface() = default;

    /// Navigate to a website URL
    virtual void navigate_to_website(const url::Url& url) = 0;

    /// Open a file from the filesystem
    virtual std::future<std::filesystem::path> open_file(
        const std::filesystem::path& path
    ) = 0;

    /// Confirm socket connection (for sandbox prompts)
    virtual std::future<bool> confirm_socket(
        const std::string& host,
        uint16_t port
    ) = 0;
};

/// Future spawner interface
template<typename Err>
class FutureSpawner {
public:
    virtual ~FutureSpawner() = default;
    virtual void spawn(std::future<void> future) = 0;
};

/// Playing content reference
class PlayingContent;

/// Socket mode for network connections
enum class SocketMode {
    Allow,
    Deny,
    Ask
};

/// External navigator backend for non-web environments
template<typename F, typename I>
class ExternalNavigatorBackend : public core::backend::NavigatorBackend {
public:
    /// Construct a navigator backend with fetch and async capability
    ExternalNavigatorBackend(
        url::Url base_url,
        std::optional<url::Url> referer,
        std::optional<std::string> cookie,
        F future_spawner,
        std::optional<url::Url> proxy,
        bool upgrade_to_https,
        std::unordered_set<std::string> socket_allowed,
        SocketMode socket_mode,
        std::shared_ptr<PlayingContent> content,
        I interface
    );

    // NavigatorBackend implementation
    void navigate_to_url(
        const std::string& url,
        const std::string& target,
        std::optional<std::pair<
            core::backend::NavigationMethod,
            core::indexmap::IndexMap<std::string, std::string>
        >> vars_method
    ) override;

    std::future<std::unique_ptr<core::backend::SuccessResponse>> fetch(
        const core::backend::Request& request
    ) override;

    std::expected<url::Url, url::ParseError> resolve_url(
        const std::string& url
    ) const override;

    void spawn_future(std::future<void> future) override;

    url::Url pre_process_url(url::Url url) const override;

    void connect_socket(
        std::string host,
        uint16_t port,
        std::chrono::duration<double> timeout,
        core::socket::SocketHandle handle,
        // Receiver and sender would be channel types
        void* receiver,
        void* sender
    ) override;

private:
    F future_spawner_;
    std::shared_ptr<void> client_;  // reqwest::Client
    url::Url base_url_;
    bool upgrade_to_https_;
    std::unordered_set<std::string> socket_allowed_;
    SocketMode socket_mode_;
    std::shared_ptr<PlayingContent> content_;
    I interface_;
};

/// Spawn a future in a tokio runtime
template<typename F>
auto spawn_tokio(F&& future) -> std::future<typename F::result_type>;

} // namespace backends
} // namespace frontend_utils
} // namespace ruffle

#endif // RUFFLE_FRONTEND_UTILS_NAVIGATOR_H
