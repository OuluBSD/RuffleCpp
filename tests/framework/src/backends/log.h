// C++ translation of tests/framework/src/backends/log.rs
// Test log backend

#ifndef RUFFLE_TESTS_FRAMEWORK_BACKENDS_LOG_H
#define RUFFLE_TESTS_FRAMEWORK_BACKENDS_LOG_H

#include <string>
#include <memory>

// Forward declarations
namespace ruffle {
namespace core {
namespace backend {
namespace log {
    class LogBackend;
}
}
}
}

namespace ruffle {
namespace tests {
namespace framework {
namespace backends {
namespace log {

/// Test log backend for capturing trace output
class TestLogBackend {
private:
    std::string trace_output_;

public:
    TestLogBackend() = default;

    /// Get and clear the trace output
    std::string get_trace_output() {
        std::string output = std::move(trace_output_);
        trace_output_.clear();
        return output;
    }
};

/// LogBackend implementation for TestLogBackend
class TestLogBackendImpl : public core::backend::log::LogBackend {
private:
    std::shared_ptr<TestLogBackend> backend_;

public:
    explicit TestLogBackendImpl(std::shared_ptr<TestLogBackend> backend)
        : backend_(std::move(backend)) {}

    void avm_trace(const std::string& message) override;
    void avm_warning(const std::string& message) override;
};

} // namespace log
} // namespace backends
} // namespace framework
} // namespace tests
} // namespace ruffle

#endif // RUFFLE_TESTS_FRAMEWORK_BACKENDS_LOG_H
