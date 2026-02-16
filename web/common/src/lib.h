// C++ translation of web/common/src/lib.rs
//! Utility functions for the web backend.

#ifndef RUFFLE_WEB_COMMON_LIB_H
#define RUFFLE_WEB_COMMON_LIB_H

#include <string>
#include <system_error>
#include <variant>

namespace ruffle {
namespace web {
namespace common {

/// Represents a JavaScript error value
class JsError {
public:
    JsError() = default;
    explicit JsError(const std::string& value) : value_(value) {}

    const std::string& value() const { return value_; }

private:
    std::string value_;
};

/// Trait for JavaScript result handling
template<typename T>
class JsResult {
public:
    /// Warns if the result contains an error
    void warn_on_error() const;

    /// Converts to a standard Result type
    std::variant<T, JsError> into_js_result() const;
};

/// Implementation for Result-like types
template<typename T>
class JsResultImpl : public JsResult<T> {
public:
    JsResultImpl() : has_error_(false) {}
    explicit JsResultImpl(const T& value) : value_(value), has_error_(false) {}
    explicit JsResultImpl(const JsError& error) : error_(error), has_error_(true) {}

    void warn_on_error() const override {
        if (has_error_) {
            // Log warning about JavaScript error
            // In real implementation, this would use tracing::warn!
        }
    }

    std::variant<T, JsError> into_js_result() const override {
        if (has_error_) {
            return error_;
        }
        return value_;
    }

private:
    T value_;
    JsError error_;
    bool has_error_;
};

} // namespace common
} // namespace web
} // namespace ruffle

#endif // RUFFLE_WEB_COMMON_LIB_H
