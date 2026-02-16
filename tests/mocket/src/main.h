// C++ translation of tests/mocket/src/main.rs
// Mocket - Socket test server for Flash socket testing

#ifndef RUFFLE_TESTS_MOCKET_MAIN_H
#define RUFFLE_TESTS_MOCKET_MAIN_H

#include <string>
#include <vector>
#include <filesystem>
#include <cstdint>

// Forward declarations
namespace ruffle {
namespace socket_format {
    class SocketEvent;
}
}

namespace ruffle {
namespace tests {
namespace mocket {

namespace fs = std::filesystem;

/// Flash socket policy string
inline const std::vector<uint8_t> POLICY = []() {
    const char* policy_str = "<?xml version=\"1.0\"?>\n"
        "<!DOCTYPE cross-domain-policy SYSTEM \"http://www.adobe.com/xml/dtds/cross-domain-policy.dtd\">\n"
        "<cross-domain-policy>\n"
        "<allow-access-from domain=\"*\" to-ports=\"*\"/>\n"
        "</cross-domain-policy>\0";
    return std::vector<uint8_t>(policy_str, policy_str + strlen(policy_str));
}();

/// Command-line options for mocket
struct Opt {
    /// Path to a socket.json file
    fs::path file_path;
};

/// Main entry point for mocket
///
/// @param opt The command-line options
/// @return 0 on success, error code on failure
[[nodiscard]] int main_func(const Opt& opt);

/// Initialize tracing subscriber with environment filter
void init_tracing();

/// Handle socket policy request
/// @param stream The socket stream
/// @return true if policy was sent successfully
bool send_socket_policy(int stream_fd);

/// Process receive event
/// @param stream_fd The socket file descriptor
/// @param expected The expected data
/// @return true if received data matches expected
bool process_receive(int stream_fd, const std::vector<uint8_t>& expected);

/// Process send event
/// @param stream_fd The socket file descriptor
/// @param payload The data to send
/// @return true if data was sent successfully
bool process_send(int stream_fd, std::vector<uint8_t>& payload);

/// Process wait for disconnect event
/// @param stream_fd The socket file descriptor
/// @return true if client disconnected as expected
bool process_wait_for_disconnect(int stream_fd);

/// Process disconnect event
/// @param stream_fd The socket file descriptor
void process_disconnect(int stream_fd);

} // namespace mocket
} // namespace tests
} // namespace ruffle

#endif // RUFFLE_TESTS_MOCKET_MAIN_H
