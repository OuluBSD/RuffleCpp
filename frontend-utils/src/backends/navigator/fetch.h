// C++ translation of frontend-utils/src/backends/navigator/fetch.rs
// Response handling for navigator fetch operations

#ifndef RUFFLE_FRONTEND_UTILS_BACKENDS_NAVIGATOR_FETCH_H
#define RUFFLE_FRONTEND_UTILS_BACKENDS_NAVIGATOR_FETCH_H

#include <cstdint>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <vector>
#include <future>

// Forward declarations
namespace ruffle {
namespace core {
namespace swf {
    class Encoding;
}
namespace loader {
    class Error;
}
namespace backend {
namespace navigator {
    template<typename T>
    using OwnedFuture = std::future<T>;
    
    class SuccessResponse;
}
}
}
}

namespace ruffle {
namespace frontend_utils {
namespace backends {
namespace navigator {
namespace fetch {

/// Response body storage type
///
/// The response body can come from either a file or the network.
/// Network responses use shared ownership with mutex protection
/// to allow returning owned futures.
class ResponseBody {
public:
    /// File-based response body
    struct File {
        std::vector<uint8_t> data;
        std::optional<std::string> error;  // Contains error message if fetch failed
        
        File() = default;
        explicit File(const std::vector<uint8_t>& d) : data(d) {}
        explicit File(const std::string& err) : error(err) {}
    };
    
    /// Network-based response body
    /// Uses shared ownership with mutex for thread-safe access
    struct Network {
        std::shared_ptr<std::mutex> mutex;
        std::shared_ptr<void> response;  // Opaque pointer to reqwest::Response
        
        Network() : mutex(std::make_shared<std::mutex>()) {}
    };
    
private:
    std::variant<File, Network> body_;
    
public:
    /// Create a file-based response body
    static ResponseBody from_file(const std::vector<uint8_t>& data) {
        ResponseBody result;
        result.body_ = File(data);
        return result;
    }
    
    /// Create a file-based response body with error
    static ResponseBody from_file_error(const std::string& error) {
        ResponseBody result;
        result.body_ = File(error);
        return result;
    }
    
    /// Create a network-based response body
    static ResponseBody from_network(const std::shared_ptr<void>& response) {
        ResponseBody result;
        result.body_ = Network{std::make_shared<std::mutex>(), response};
        return result;
    }
    
    /// Check if this is a file-based response
    bool is_file() const {
        return std::holds_alternative<File>(body_);
    }
    
    /// Check if this is a network-based response
    bool is_network() const {
        return std::holds_alternative<Network>(body_);
    }
    
    /// Get file data if available
    const std::optional<std::vector<uint8_t>>& file_data() const {
        static const std::optional<std::vector<uint8_t>> empty;
        if (auto* file = std::get_if<File>(&body_)) {
            if (!file->error) {
                return file->data;
            }
        }
        return empty;
    }
    
    /// Get file error if available
    const std::optional<std::string>& file_error() const {
        static const std::optional<std::string> empty;
        if (auto* file = std::get_if<File>(&body_)) {
            return file->error;
        }
        return empty;
    }
    
    /// Get network response mutex
    std::shared_ptr<std::mutex> network_mutex() const {
        if (auto* network = std::get_if<Network>(&body_)) {
            return network->mutex;
        }
        return nullptr;
    }
    
    /// Get network response
    std::shared_ptr<void> network_response() const {
        if (auto* network = std::get_if<Network>(&body_)) {
            return network->response;
        }
        return nullptr;
    }
};

/// HTTP response wrapper for navigator fetch operations
///
/// Implements the SuccessResponse interface for both file and network responses.
class Response {
public:
    std::string url;
    ResponseBody response_body;
    const swf::Encoding* text_encoding;  // Optional, may be nullptr
    uint16_t status;
    bool redirected;
    
    /// Create a new Response
    ///
    /// \param url The response URL
    /// \param body The response body (file or network)
    /// \param encoding Optional text encoding
    /// \param status HTTP status code
    /// \param redirected Whether the request was redirected
    Response(const std::string& url,
             ResponseBody body,
             const swf::Encoding* encoding,
             uint16_t status,
             bool redirected)
        : url(url)
        , response_body(std::move(body))
        , text_encoding(encoding)
        , status(status)
        , redirected(redirected) {}
    
    /// Get the response URL
    std::string get_url() const {
        return url;
    }
    
    /// Set the response URL (for redirects)
    void set_url(const std::string& new_url) {
        url = new_url;
    }
    
    /// Get the response body as a vector of bytes
    /// Returns a future that resolves to the body data
    std::future<std::vector<uint8_t>> get_body() const;
    
    /// Get the text encoding
    const swf::Encoding* get_text_encoding() const {
        return text_encoding;
    }
    
    /// Get the HTTP status code
    uint16_t get_status() const {
        return status;
    }
    
    /// Check if the request was redirected
    bool is_redirected() const {
        return redirected;
    }
    
    /// Get the next chunk of data (for streaming)
    /// Returns a future that resolves to an optional chunk
    std::future<std::optional<std::vector<uint8_t>>> get_next_chunk();
    
    /// Get the expected content length
    /// Returns an error if the length cannot be determined
    std::variant<std::optional<uint64_t>, loader::Error> get_expected_length() const;
};

} // namespace fetch
} // namespace navigator
} // namespace backends
} // namespace frontend_utils
} // namespace ruffle

#endif // RUFFLE_FRONTEND_UTILS_BACKENDS_NAVIGATOR_FETCH_H
