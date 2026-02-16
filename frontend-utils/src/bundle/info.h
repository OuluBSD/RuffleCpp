// C++ translation of frontend-utils/src/bundle/info.rs
// Bundle information parsing and serialization

#ifndef RUFFLE_FRONTEND_UTILS_BUNDLE_INFO_H
#define RUFFLE_FRONTEND_UTILS_BUNDLE_INFO_H

#include <string>
#include <variant>
#include <iosfwd>

// Forward declarations
namespace ruffle {
namespace frontend {
namespace utils {

class PlayerOptions;

namespace parse {
    class DocumentHolder;
    struct ParseDetails;
    struct ParseWarning;
}

} // namespace utils
} // namespace frontend
} // namespace ruffle

namespace ruffle {
namespace frontend {
namespace utils {
namespace bundle {
namespace info {

/// Bundle information filename constant.
inline constexpr const char* BUNDLE_INFORMATION_FILENAME = "ruffle-bundle.toml";

/// Bundle information parse error types.
///
/// Errors that can occur when parsing bundle information from TOML.
class BundleInformationParseError {
public:
    /// Error kind enumeration.
    enum class Kind {
        InvalidToml,          ///< File is not valid TOML
        InvalidBundleSection, ///< Invalid or missing [bundle] section
        InvalidName,          ///< Invalid or missing bundle.name
        InvalidUrl            ///< Invalid or missing bundle.url
    };

private:
    Kind kind_;
    std::string message_;

    BundleInformationParseError(Kind kind, std::string message)
        : kind_(kind), message_(std::move(message)) {}

public:
    /// Create an InvalidToml error.
    /// @return BundleInformationParseError
    static BundleInformationParseError invalid_toml() {
        return BundleInformationParseError(Kind::InvalidToml, "File is not valid TOML");
    }

    /// Create an InvalidBundleSection error.
    /// @return BundleInformationParseError
    static BundleInformationParseError invalid_bundle_section() {
        return BundleInformationParseError(Kind::InvalidBundleSection, 
            "Invalid or missing [bundle] section");
    }

    /// Create an InvalidName error.
    /// @return BundleInformationParseError
    static BundleInformationParseError invalid_name() {
        return BundleInformationParseError(Kind::InvalidName, 
            "Invalid or missing bundle.name");
    }

    /// Create an InvalidUrl error.
    /// @return BundleInformationParseError
    static BundleInformationParseError invalid_url() {
        return BundleInformationParseError(Kind::InvalidUrl, 
            "Invalid or missing bundle.url");
    }

    /// Get the error kind.
    /// @return The error kind
    Kind kind() const { return kind_; }

    /// Get the error message.
    /// @return The error message
    const std::string& message() const { return message_; }
};

/// Bundle information structure.
///
/// Contains metadata about a Ruffle bundle including name, URL, and player options.
class BundleInformation {
public:
    std::string name;           ///< Bundle name
    std::string url;            ///< Bundle URL
    PlayerOptions player;       ///< Player options

    /// Default constructor.
    BundleInformation() = default;

    /// Constructor with parameters.
    ///
    /// @param name The bundle name
    /// @param url The bundle URL
    /// @param player The player options
    BundleInformation(std::string name, std::string url, PlayerOptions player)
        : name(std::move(name))
        , url(std::move(url))
        , player(std::move(player)) {}

    /// Parse bundle information from a TOML string.
    ///
    /// @param input The TOML string to parse
    /// @return Result containing ParseDetails or BundleInformationParseError
    static std::variant<parse::ParseDetails<BundleInformation>, BundleInformationParseError>
    parse(const std::string& input);

    /// Serialize bundle information to a writer.
    ///
    /// @param writer The output stream to write to
    /// @return true on success, false on error
    bool serialize(std::ostream& writer) const;
};

} // namespace info
} // namespace bundle
} // namespace utils
} // namespace frontend
} // namespace ruffle

#endif // RUFFLE_FRONTEND_UTILS_BUNDLE_INFO_H
