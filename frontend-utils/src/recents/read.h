// C++ translation of frontend-utils/src/recents/read.rs
//! Reading recent entries from configuration

#ifndef RUFFLE_FRONTEND_UTILS_RECENTS_READ_H
#define RUFFLE_FRONTEND_UTILS_RECENTS_READ_H

#include <string>
#include <vector>
#include <optional>

// Forward declarations
namespace ruffle {
namespace frontend {
namespace utils {
namespace content {
    class ContentDescriptor;
}
namespace recents {
    struct Recent;
    using Recents = std::vector<Recent>;
}
}
}
}

namespace ruffle {
namespace frontend {
namespace utils {
namespace recents {

/// Parse warning types
enum class ParseWarningType {
    InvalidToml,
    UnexpectedType,
    UnsupportedValue
};

/// Parse warning structure
struct ParseWarning {
    ParseWarningType type;
    std::string message;
    std::string path;  // For type/value errors
    std::string expected;  // For type errors
    std::string actual;  // For type errors
    std::string value;  // For value errors

    static ParseWarning invalid_toml(const std::string& error) {
        return {ParseWarningType::InvalidToml, error, "", "", "", ""};
    }

    static ParseWarning unexpected_type(
        const std::string& expected,
        const std::string& actual,
        const std::string& path
    ) {
        return {ParseWarningType::UnexpectedType, "", path, expected, actual, ""};
    }

    static ParseWarning unsupported_value(
        const std::string& value,
        const std::string& path
    ) {
        return {ParseWarningType::UnsupportedValue, "", path, "", "", value};
    }
};

/// Parse result with warnings
template<typename T>
struct ParseDetails {
    T result;
    std::vector<ParseWarning> warnings;

    ParseDetails() = default;
    ParseDetails(T r, std::vector<ParseWarning> w)
        : result(std::move(r)), warnings(std::move(w)) {}

    /// Get the parsed values
    const T& values() const { return result; }
};

/// Read recent entries from a TOML string.
///
/// @param input TOML-formatted string containing recent entries
/// @return ParseDetails containing the parsed recents and any warnings
///
/// The TOML format expects an array of tables like:
/// ```toml
/// [[recent]]
/// url = "https://example.com/file.swf"
/// name = "Example File"
///
/// [[recent]]
/// url = "file:///path/to/file.swf"
/// dir = "/path/to"
/// ```
ParseDetails<Recents> read_recents(const std::string& input);

} // namespace recents
} // namespace utils
} // namespace frontend
} // namespace ruffle

#endif // RUFFLE_FRONTEND_UTILS_RECENTS_READ_H
