// C++ translation of scanner/src/logging.rs
// Logging utilities and mock objects for log output capture

#ifndef RUFFLE_SCANNER_LOGGING_H
#define RUFFLE_SCANNER_LOGGING_H

#include <string>
#include <vector>
#include <memory>
#include <mutex>

namespace ruffle {
namespace scanner {

// Forward declarations
namespace core {
namespace backend {
namespace log {
    class LogBackend;
}
}
}

/// Log backend that specifically discards AVM trace output
class ScanLogBackend : public core::backend::log::LogBackend {
public:
    ScanLogBackend() = default;

    /// AVM trace output - discarded
    void avm_trace(const std::string& message) override {
        (void)message; // Discard
    }

    /// AVM warning output - discarded
    void avm_warning(const std::string& message) override {
        (void)message; // Discard
    }
};

/// Thread-local log buffer for capturing error messages
class ThreadLocalLogger {
public:
    /// Get the singleton instance
    static ThreadLocalLogger& instance() {
        static ThreadLocalLogger logger;
        return logger;
    }

    /// Initialize the logger
    void init();

    /// Check if a log level is enabled
    /// @param level The log level to check
    /// @return true if the level is enabled (only Error level is enabled)
    bool enabled(int level) const {
        return level == ERROR_LEVEL;
    }

    /// Log a message
    /// @param level The log level
    /// @param message The message to log
    void log(int level, const std::string& message);

    /// Flush the log buffer
    void flush();

    /// Get the current log buffer
    /// @return Reference to the vector of logged messages
    std::vector<std::string>& buffer() {
        return buffer_;
    }

    /// Clear the log buffer
    void clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        buffer_.clear();
    }

private:
    ThreadLocalLogger() = default;

    static constexpr int ERROR_LEVEL = 3; // Error level constant

    std::vector<std::string> buffer_;
    std::mutex mutex_;
};

} // namespace scanner
} // namespace ruffle

#endif // RUFFLE_SCANNER_LOGGING_H
