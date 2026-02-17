// C++ translation of frontend-utils/src/parse.rs
// TOML parsing utilities

#ifndef RUFFLE_FRONTEND_UTILS_PARSE_H
#define RUFFLE_FRONTEND_UTILS_PARSE_H

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <functional>
#include <sstream>

// Forward declarations for toml_edit equivalents
// In a real implementation, these would come from a TOML library like toml11 or toml++

namespace ruffle {
namespace frontend {
namespace utils {

/// Warning types that can occur during parsing
enum class ParseWarningType {
    InvalidToml,
    UnexpectedType,
    UnsupportedValue
};

/// A parse warning with details about what went wrong
struct ParseWarning {
    ParseWarningType type;
    std::string message;
    std::string expected;    // For UnexpectedType
    std::string actual;      // For UnexpectedType
    std::string path;        // Path to the problematic value
    std::string value;       // For UnsupportedValue

    static ParseWarning invalid_toml(const std::string& error) {
        return {ParseWarningType::InvalidToml, error, "", "", "", ""};
    }

    static ParseWarning unexpected_type(const std::string& expected, const std::string& actual, const std::string& path) {
        return {ParseWarningType::UnexpectedType, "", expected, actual, path, ""};
    }

    static ParseWarning unsupported_value(const std::string& value, const std::string& path) {
        return {ParseWarningType::UnsupportedValue, "", "", "", path, value};
    }
};

/// Parse context for tracking current path during parsing
class ParseContext {
private:
    std::vector<std::string> path;
    std::vector<ParseWarning>& warnings;

public:
    explicit ParseContext(std::vector<ParseWarning>& warnings) : warnings(warnings) {}

    void push_key(const std::string& key) {
        path.push_back(key);
    }

    void pop_key() {
        if (!path.empty()) {
            path.pop_back();
        }
    }

    std::string get_path() const {
        std::ostringstream oss;
        for (size_t i = 0; i < path.size(); ++i) {
            if (i > 0) oss << ".";
            oss << path[i];
        }
        return oss.str();
    }

    void unexpected_type(const std::string& expected, const std::string& actual) {
        warnings.push_back(ParseWarning::unexpected_type(expected, actual, get_path()));
    }

    void unsupported_value(const std::string& value) {
        warnings.push_back(ParseWarning::unsupported_value(value, get_path()));
    }
};

/// A holder over values that may be read and potentially written back to disk.
///
/// Two versions of Ruffle may have different preferences, or different values available for each preference.
/// For this reason, we store both the original toml document *and* the parsed values as we understand them.
/// Whenever we persist values back to the toml, we only edit the values we changed and leave the remaining
/// values as they originally were.
/// This way, switching between different versions will *not* wipe your settings or get Ruffle into an
/// invalid state.
template<typename T>
class DocumentHolder {
private:
    /// The original toml document (as string for now)
    std::string toml_document;

    /// The actual values stored within the toml document, as this version of Ruffle understands them.
    T inner;

public:
    DocumentHolder() = default;

    DocumentHolder(const T& values, const std::string& document)
        : toml_document(document), inner(values) {}

    /// Get the inner values (const)
    const T& get() const {
        return inner;
    }

    /// Get the inner values (mutable)
    T& get_mut() {
        return inner;
    }

    /// Allows editing of this DocumentHolder.
    ///
    /// The provided function is expected to:
    /// - Only edit the toml **as little as is required** for the given change
    /// - Ensure that the toml and values are kept in sync, such that reading the toml again provides the same values
    void edit(std::function<void(T&, std::string&)> fun) {
        fun(inner, toml_document);
    }

    /// Takes the saved values from this DocumentHolder, discarding the document.
    /// Useful when you need the result of parsing, without needing to write to it again later.
    T take() {
        return std::move(inner);
    }

    /// Serialize the document to string
    std::string serialize() const {
        return toml_document;
    }

    /// Get the document string
    const std::string& get_document() const {
        return toml_document;
    }
};

/// Parse result with values and warnings
template<typename T>
struct ParseDetails {
    DocumentHolder<T> result;
    std::vector<ParseWarning> warnings;

    const T& values() const {
        return result.get();
    }
};

/// Extension trait for reading TOML items
class ReadExt {
public:
    virtual ~ReadExt() = default;

    /// Get an item by key
    virtual std::optional<std::string> get_impl(const std::string& key) const = 0;

    /// Get a table-like item and process it
    template<typename R>
    std::optional<R> get_table_like(
        ParseContext& cx,
        const std::string& key,
        std::function<R(ParseContext&, const void*)> fun)
    {
        std::optional<R> result;
        if (auto item = get_impl(key)) {
            cx.push_key(key);
            // In real implementation, would check if item is a table
            result = fun(cx, nullptr);  // Placeholder
            cx.pop_key();
        }
        return result;
    }

    /// Parse a value from string
    template<typename T>
    std::optional<T> parse_from_str(ParseContext& cx, const std::string& key) {
        std::optional<T> result;
        cx.push_key(key);
        if (auto str = get_impl(key)) {
            // In real implementation, would parse the string
            cx.pop_key();
            return result;  // Placeholder
        }
        cx.pop_key();
        return result;
    }

    /// Get a boolean value
    std::optional<bool> get_bool(ParseContext& cx, const std::string& key) {
        cx.push_key(key);
        auto result = get_impl(key);  // Would parse as bool
        cx.pop_key();
        return std::nullopt;  // Placeholder
    }

    /// Get a float value
    std::optional<double> get_float(ParseContext& cx, const std::string& key) {
        cx.push_key(key);
        auto result = get_impl(key);  // Would parse as float
        cx.pop_key();
        return std::nullopt;  // Placeholder
    }

    /// Get an integer value
    std::optional<int64_t> get_integer(ParseContext& cx, const std::string& key) {
        cx.push_key(key);
        auto result = get_impl(key);  // Would parse as integer
        cx.pop_key();
        return std::nullopt;  // Placeholder
    }

    /// Get a float-like value (float or integer)
    std::optional<double> get_float_like(ParseContext& cx, const std::string& key) {
        cx.push_key(key);
        auto result = get_impl(key);  // Would parse as float or integer
        cx.pop_key();
        return std::nullopt;  // Placeholder
    }
};

} // namespace utils
} // namespace frontend
} // namespace ruffle

#endif // RUFFLE_FRONTEND_UTILS_PARSE_H
