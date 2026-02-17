// C++ translation of core/src/avm1/error.rs
// AVM1 Error types

#ifndef RUFFLE_CORE_AVM1_ERROR_H
#define RUFFLE_CORE_AVM1_ERROR_H

#include <cstdint>
#include <string>
#include <variant>
#include <memory>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename T> class Value;
}
}
}

namespace swf {
namespace error {
    class Error;
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace error {

/// AVM1 Error types
///
/// Represents various error conditions that can occur during AVM1 execution.
template<typename GC>
class Error {
public:
    /// Error variant types
    enum class Kind {
        /// Prototype recursion limit has been exceeded
        PrototypeRecursionLimit,

        /// Execution timeout - script took too long to execute
        ExecutionTimeout,

        /// Function recursion limit exceeded
        FunctionRecursionLimit,

        /// Special recursion limit exceeded (66 levels)
        SpecialRecursionLimit,

        /// Invalid SWF - couldn't parse SWF
        InvalidSwf,

        /// Thrown value - a script has thrown a custom error
        ThrownValue
    };

private:
    Kind kind_;

    // Data for FunctionRecursionLimit variant
    uint16_t recursion_depth_;

    // Data for InvalidSwf variant
    std::shared_ptr<swf::error::Error> swf_error_;

    // Data for ThrownValue variant
    Value<GC>* thrown_value_;

public:
    /// Create a PrototypeRecursionLimit error
    static Error prototype_recursion_limit() {
        Error err;
        err.kind_ = Kind::PrototypeRecursionLimit;
        return err;
    }

    /// Create an ExecutionTimeout error
    static Error execution_timeout() {
        Error err;
        err.kind_ = Kind::ExecutionTimeout;
        return err;
    }

    /// Create a FunctionRecursionLimit error
    ///
    /// \param depth The recursion depth that was exceeded
    static Error function_recursion_limit(uint16_t depth) {
        Error err;
        err.kind_ = Kind::FunctionRecursionLimit;
        err.recursion_depth_ = depth;
        return err;
    }

    /// Create a SpecialRecursionLimit error
    static Error special_recursion_limit() {
        Error err;
        err.kind_ = Kind::SpecialRecursionLimit;
        return err;
    }

    /// Create an InvalidSwf error
    ///
    /// \param error The underlying SWF parsing error
    static Error invalid_swf(std::shared_ptr<swf::error::Error> error) {
        Error err;
        err.kind_ = Kind::InvalidSwf;
        err.swf_error_ = error;
        return err;
    }

    /// Create a ThrownValue error
    ///
    /// \param value The value that was thrown
    static Error thrown_value(Value<GC>* value) {
        Error err;
        err.kind_ = Kind::ThrownValue;
        err.thrown_value_ = value;
        return err;
    }

    /// Get the error kind
    Kind kind() const { return kind_; }

    /// Get the recursion depth for FunctionRecursionLimit errors
    uint16_t recursion_depth() const { return recursion_depth_; }

    /// Get the SWF error for InvalidSwf errors
    const std::shared_ptr<swf::error::Error>& swf_error() const { return swf_error_; }

    /// Get the thrown value for ThrownValue errors
    Value<GC>* thrown_value() const { return thrown_value_; }

    /// Get a human-readable error message
    std::string message() const {
        switch (kind_) {
            case Kind::PrototypeRecursionLimit:
                return "Prototype recursion limit has been exceeded";
            case Kind::ExecutionTimeout:
                return "A script in this movie has taken too long to execute and has been terminated.";
            case Kind::FunctionRecursionLimit:
                return std::to_string(recursion_depth_) +
                       " levels of function recursion were exceeded in one action list. "
                       "This is probably an infinite loop.";
            case Kind::SpecialRecursionLimit:
                return "66 levels of special recursion were exceeded in one action list. "
                       "This is probably an infinite loop.";
            case Kind::InvalidSwf:
                return "Couldn't parse SWF";
            case Kind::ThrownValue:
                return "A script has thrown a custom error.";
            default:
                return "Unknown error";
        }
    }

private:
    Error() : kind_(Kind::PrototypeRecursionLimit), recursion_depth_(0),
              swf_error_(nullptr), thrown_value_(nullptr) {}
};

} // namespace error
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_ERROR_H
