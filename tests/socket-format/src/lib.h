// C++ translation of tests/socket-format/src/lib.rs
//! Socket event format for testing

#ifndef RUFFLE_TESTS_SOCKET_FORMAT_LIB_H
#define RUFFLE_TESTS_SOCKET_FORMAT_LIB_H

#include <vector>
#include <cstdint>
#include <string>
#include <variant>
#include <optional>
#include <filesystem>
#include <fstream>

namespace ruffle {
namespace tests {
namespace socket_format {

/// Socket event types for testing network communication
class SocketEvent {
public:
    /// Wait for input data that matches this
    struct Receive {
        std::vector<uint8_t> expected;
    };

    /// Send data to client
    struct Send {
        std::vector<uint8_t> payload;
    };

    /// Expect client to disconnect
    struct WaitForDisconnect {};

    /// Disconnect the client
    struct Disconnect {};

    /// Event data storage
    using EventData = std::variant<Receive, Send, WaitForDisconnect, Disconnect>;

private:
    EventData data_;

public:
    SocketEvent() = default;
    explicit SocketEvent(EventData data) : data_(std::move(data)) {}

    /// Create a Receive event
    static SocketEvent receive(std::vector<uint8_t> expected) {
        return SocketEvent(Receive{std::move(expected)});
    }

    /// Create a Send event
    static SocketEvent send(std::vector<uint8_t> payload) {
        return SocketEvent(Send{std::move(payload)});
    }

    /// Create a WaitForDisconnect event
    static SocketEvent wait_for_disconnect() {
        return SocketEvent(WaitForDisconnect{});
    }

    /// Create a Disconnect event
    static SocketEvent disconnect() {
        return SocketEvent(Disconnect{});
    }

    /// Check if this is a Receive event
    bool is_receive() const {
        return std::holds_alternative<Receive>(data_);
    }

    /// Check if this is a Send event
    bool is_send() const {
        return std::holds_alternative<Send>(data_);
    }

    /// Check if this is a WaitForDisconnect event
    bool is_wait_for_disconnect() const {
        return std::holds_alternative<WaitForDisconnect>(data_);
    }

    /// Check if this is a Disconnect event
    bool is_disconnect() const {
        return std::holds_alternative<Disconnect>(data_);
    }

    /// Get the expected data for Receive events
    const std::vector<uint8_t>* get_expected() const {
        if (auto* recv = std::get_if<Receive>(&data_)) {
            return &recv->expected;
        }
        return nullptr;
    }

    /// Get the payload for Send events
    const std::vector<uint8_t>* get_payload() const {
        if (auto* send = std::get_if<Send>(&data_)) {
            return &send->payload;
        }
        return nullptr;
    }

    /// Load events from a JSON file
    ///
    /// @param path Path to the JSON file
    /// @return Vector of SocketEvents or empty on error
    static std::optional<std::vector<SocketEvent>> from_file(const std::filesystem::path& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            return std::nullopt;
        }
        return from_stream(file);
    }

    /// Load events from a stream
    ///
    /// @param stream Input stream containing JSON
    /// @return Vector of SocketEvents or empty on error
    static std::optional<std::vector<SocketEvent>> from_stream(std::istream& stream) {
        // Note: Full JSON parsing would require a JSON library like nlohmann/json
        // This is a stub implementation - in real code would parse JSON
        // Format expected: [{"type": "Receive", "expected": [...]}, ...]
        return std::nullopt; // Stub - requires JSON parsing implementation
    }
};

} // namespace socket_format
} // namespace tests
} // namespace ruffle

#endif // RUFFLE_TESTS_SOCKET_FORMAT_LIB_H
