// C++ translation of desktop/src/preferences/read.rs
// Preferences reading functionality

#ifndef RUFFLE_DESKTOP_PREFERENCES_READ_H
#define RUFFLE_DESKTOP_PREFERENCES_READ_H

#include <string>
#include <vector>
#include <memory>
#include <optional>

// Forward declarations
namespace ruffle {
namespace desktop {
namespace cli {
    enum class GameModePreference;
    enum class OpenUrlMode;
}
namespace gui {
    enum class ThemePreference;
}
namespace log {
    enum class FilenamePattern;
}
namespace preferences {
    namespace storage {
        enum class StorageBackend;
    }
    struct LogPreferences;
    struct StoragePreferences;
    struct SavedGlobalPreferences;
}
}
}

// For TOML parsing
namespace toml {
    class Document;
}

namespace ruffle {
namespace desktop {
namespace preferences {

/// Warning encountered during preference parsing
struct ParseWarning {
    enum class Type {
        InvalidToml,
        UnexpectedType,
        UnsupportedValue,
    };

    Type type;
    std::string message;
    std::string path;
    std::string expected;
    std::string actual;
    std::string value;

    static ParseWarning invalid_toml(const std::string& error) {
        return {Type::InvalidToml, error, "", "", "", ""};
    }

    static ParseWarning unexpected_type(const std::string& expected, const std::string& actual, const std::string& path) {
        return {Type::UnexpectedType, "", path, expected, actual, ""};
    }

    static ParseWarning unsupported_value(const std::string& value, const std::string& path) {
        return {Type::UnsupportedValue, "", path, "", "", value};
    }
};

/// Parse context for tracking warnings
struct ParseContext {
    std::vector<ParseWarning> warnings;
};

/// Details of a parse operation
template<typename T>
struct ParseDetails {
    T result;
    std::vector<ParseWarning> warnings;
};

/// Extension traits for TOML document parsing
class DocumentExt {
public:
    static std::optional<std::string> parse_from_str(
        const toml::Document& doc,
        ParseContext& cx,
        const std::string& key
    );

    static std::optional<double> get_float(
        const toml::Document& doc,
        ParseContext& cx,
        const std::string& key
    );

    static std::optional<bool> get_bool(
        const toml::Document& doc,
        ParseContext& cx,
        const std::string& key
    );

    static std::optional<int64_t> get_integer(
        const toml::Document& doc,
        ParseContext& cx,
        const std::string& key
    );

    static void get_table_like(
        const toml::Document& doc,
        ParseContext& cx,
        const std::string& key,
        std::function<void(ParseContext&, const toml::Document&)> callback
    );
};

/// Read the given preferences into a guaranteed valid SavedGlobalPreferences,
/// recording any possible warnings encountered along the way.
///
/// We wish to support backwards and forwards compatibility where possible,
/// so nothing is fatal in this function.
///
/// Default values are used wherever an unknown or invalid value is found;
/// this is to support the case of, for example, a later version having different supported
/// backends than an older version.
ParseDetails<SavedGlobalPreferences> read_preferences(const std::string& input);

} // namespace preferences
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_PREFERENCES_READ_H
