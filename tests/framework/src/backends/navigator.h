// C++ translation of tests/framework/src/backends/navigator.rs
// Test Navigator Backend

#ifndef RUFFLE_TESTS_FRAMEWORK_BACKENDS_NAVIGATOR_H
#define RUFFLE_TESTS_FRAMEWORK_BACKENDS_NAVIGATOR_H

#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <functional>
#include <map>

// Forward declarations
namespace ruffle {
namespace core {
namespace backend {
namespace log {
    class LogBackend;
}
namespace navigator {
    class NavigatorBackend;
    class Request;
    class SuccessResponse;
    class ErrorResponse;
    enum class NavigationMethod;
}
namespace ui {
    class SocketHandle;
    enum class SocketAction;
    enum class ConnectionState;
}
}
}
}

namespace vfs {
    class VfsPath;
}

namespace async_channel {
    template<typename T> class Receiver;
    template<typename T> class Sender;
}

namespace ruffle {
namespace socket_format {
    struct SocketEvent;
}
}

namespace ruffle {
namespace tests {
namespace framework {
namespace backends {

// Forward declaration
class TestLogBackend;

/// Test response implementation
class TestResponse {
private:
    std::string url;
    std::vector<uint8_t> body;
    bool chunk_gotten;
    uint16_t status;
    bool redirected;

public:
    TestResponse(const std::string& url, const std::vector<uint8_t>& body,
                 uint16_t status = 200, bool redirected = false);

    const std::string& get_url() const { return url; }
    void set_url(const std::string& new_url) { url = new_url; }
    
    std::vector<uint8_t> get_body() const { return body; }
    uint16_t get_status() const { return status; }
    bool is_redirected() const { return redirected; }
    
    // Get next chunk (for chunked responses)
    std::vector<uint8_t> next_chunk();
    
    // Get expected length
    size_t expected_length() const { return body.size(); }
};

/// A NavigatorBackend used by tests that supports logging fetch requests.
///
/// This can be used by tests that fetch data to verify that the request is correct.
///
/// Attempting to fetch URLs containing the following "hints" will cause a simulated response:
/// * "?debug-success" -> Simulates a successful fetch, with body "Hello, World!"
/// * "?debug-error-statuscode" -> Simulates a failed fetch due to a unsuccessful status
/// * "?debug-error-dns" -> Simulates a failed fetch due to a dns resolution error
///
/// These are formatted as query params, rather than domains/whole URLs, so that real/real-invalid
/// URLs can be used in Flash Player when writing tests
class TestNavigatorBackend {
private:
    vfs::VfsPath relative_base_path;
    std::vector<ruffle::socket_format::SocketEvent> socket_events;
    std::shared_ptr<TestLogBackend> log;
    bool has_socket_events;

public:
    /// Create a new test navigator backend
    /// @param path Base path for file operations
    /// @param socket_events Optional socket events for testing
    /// @param log Optional log backend for tracing
    TestNavigatorBackend(const vfs::VfsPath& path,
                        const std::vector<ruffle::socket_format::SocketEvent>* socket_events = nullptr,
                        std::shared_ptr<TestLogBackend> log = nullptr);

    /// Navigate to URL (logs the request)
    void navigate_to_url(const std::string& url, const std::string& target,
                        const std::pair<core::backend::navigator::NavigationMethod,
                                       std::map<std::string, std::string>>* vars_method = nullptr);

    /// Fetch a resource
    /// @param request The fetch request
    /// @return Future containing the response
    std::shared_ptr<core::backend::navigator::SuccessResponse> fetch(
        const core::backend::navigator::Request& request);

    /// Resolve a URL relative to the base path
    std::string resolve_url(const std::string& url);

    /// Pre-process URL (identity function by default)
    std::string pre_process_url(const std::string& url) { return url; }

    /// Connect a socket for testing
    void connect_socket(const std::string& host, uint16_t port,
                       std::chrono::milliseconds timeout,
                       core::backend::ui::SocketHandle handle,
                       async_channel::Receiver<std::vector<uint8_t>> receiver,
                       async_channel::Sender<core::backend::ui::SocketAction> sender);
};

} // namespace backends
} // namespace framework
} // namespace tests
} // namespace ruffle

#endif // RUFFLE_TESTS_FRAMEWORK_BACKENDS_NAVIGATOR_H
