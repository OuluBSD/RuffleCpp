// C++ translation of tests/framework/src/options/known_failure.rs
// Known failure handling for test framework

#ifndef RUFFLE_TESTS_FRAMEWORK_OPTIONS_KNOWN_FAILURE_H
#define RUFFLE_TESTS_FRAMEWORK_OPTIONS_KNOWN_FAILURE_H

#include <string>
#include <variant>
#include <cstdint>

namespace ruffle {
namespace tests {
namespace framework {
namespace options {

/// Represents a known failure condition for tests.
/// 
/// This can be:
/// - None: No known failure
/// - TraceOutput: Expected trace output mismatch (with ruffle_check flag)
/// - Panic: Expected panic with a specific message
class KnownFailure {
public:
    /// No known failure
    struct NoneType {
        bool operator==(const NoneType&) const = default;
    };

    /// Trace output mismatch
    struct TraceOutput {
        bool ruffle_check;

        bool operator==(const TraceOutput& other) const {
            return ruffle_check == other.ruffle_check;
        }
    };

    /// Expected panic
    struct Panic {
        std::string message;

        bool operator==(const Panic& other) const {
            return message == other.message;
        }
    };

    using Variant = std::variant<NoneType, TraceOutput, Panic>;

private:
    Variant value_;

public:
    /// Default constructor - creates None variant
    KnownFailure() : value_(NoneType{}) {}

    /// Create from variant
    explicit KnownFailure(const Variant& value) : value_(value) {}
    explicit KnownFailure(Variant&& value) : value_(std::move(value)) {}

    /// Create None variant
    static KnownFailure none() {
        return KnownFailure(NoneType{});
    }

    /// Create TraceOutput variant
    static KnownFailure trace_output(bool ruffle_check) {
        return KnownFailure(TraceOutput{ruffle_check});
    }

    /// Create Panic variant
    static KnownFailure panic(const std::string& message) {
        return KnownFailure(Panic{message});
    }

    /// Check if this is the None variant
    bool is_none() const {
        return std::holds_alternative<NoneType>(value_);
    }

    /// Check if this is the TraceOutput variant
    bool is_trace_output() const {
        return std::holds_alternative<TraceOutput>(value_);
    }

    /// Check if this is the Panic variant
    bool is_panic() const {
        return std::holds_alternative<Panic>(value_);
    }

    /// Get the TraceOutput value (undefined if not TraceOutput variant)
    const TraceOutput& as_trace_output() const {
        return std::get<TraceOutput>(value_);
    }

    /// Get the Panic value (undefined if not Panic variant)
    const Panic& as_panic() const {
        return std::get<Panic>(value_);
    }

    /// Get the underlying variant
    const Variant& value() const { return value_; }
    Variant& value() { return value_; }

    bool operator==(const KnownFailure& other) const = default;
};

} // namespace options
} // namespace framework
} // namespace tests
} // namespace ruffle

#endif // RUFFLE_TESTS_FRAMEWORK_OPTIONS_KNOWN_FAILURE_H
