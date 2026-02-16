// C++ translation of core/src/backend/log.rs
// Logging backend interface

#ifndef RUFFLE_CORE_BACKEND_LOG_H
#define RUFFLE_CORE_BACKEND_LOG_H

#include <string>

namespace ruffle {
namespace core {
namespace backend {
namespace log {

/// Interface for logging backends
class LogBackend {
public:
    virtual ~LogBackend() = default;

    /// Trace output from AVM
    virtual void avm_trace(const std::string& message) = 0;

    /// Warning output from AVM
    virtual void avm_warning(const std::string& message) = 0;
};

/// Logging backend that just reroutes traces to the log crate
/// In C++, this uses standard logging mechanisms
class NullLogBackend : public LogBackend {
public:
    NullLogBackend() = default;

    void avm_trace(const std::string& message) override;
    void avm_warning(const std::string& message) override;
};

} // namespace log
} // namespace backend
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_BACKEND_LOG_H
