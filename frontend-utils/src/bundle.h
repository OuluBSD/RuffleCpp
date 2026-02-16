// C++ translation of frontend-utils/src/bundle.rs
// Bundle handling for Ruffle game bundles

#ifndef RUFFLE_FRONTEND_UTILS_BUNDLE_H
#define RUFFLE_FRONTEND_UTILS_BUNDLE_H

#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <variant>
#include <optional>

// Forward declarations for bundle submodules
namespace ruffle {
namespace frontend {
namespace utils {
namespace bundle {
namespace info {
    class BundleInformation;
    class BundleInformationParseError;
    inline constexpr const char* BUNDLE_INFORMATION_FILENAME = "ruffle-bundle.toml";
}
namespace source {
    class BundleSource;
    class BundleSourceError;
}
}
}

// Forward declaration for parse types
namespace parse {
    struct ParseWarning;
}
}
}
}

namespace ruffle {
namespace frontend {
namespace utils {

/// Bundle error types.
///
/// Errors that can occur when loading or parsing a Ruffle bundle.
class BundleError {
public:
    /// Error kind enumeration.
    enum class Kind {
        InvalidBundleInformation,   ///< Invalid ruffle-bundle.toml
        MissingBundleInformation,   ///< Missing or corrupt ruffle-bundle.toml
        InvalidSource,              ///< Invalid bundle source
        BundleDoesntExist           ///< Bundle does not exist
    };

private:
    Kind kind_;
    std::string message_;

    BundleError(Kind kind, std::string message)
        : kind_(kind), message_(std::move(message)) {}

public:
    /// Create an InvalidBundleInformation error.
    /// @param message The error message
    /// @return BundleError
    static BundleError invalid_bundle_information(const std::string& message) {
        return BundleError(Kind::InvalidBundleInformation, message);
    }

    /// Create a MissingBundleInformation error.
    /// @return BundleError
    static BundleError missing_bundle_information() {
        return BundleError(Kind::MissingBundleInformation, "Missing or corrupt ruffle-bundle.toml");
    }

    /// Create an InvalidSource error.
    /// @param message The error message
    /// @return BundleError
    static BundleError invalid_source(const std::string& message) {
        return BundleError(Kind::InvalidSource, message);
    }

    /// Create a BundleDoesntExist error.
    /// @return BundleError
    static BundleError bundle_doesnt_exist() {
        return BundleError(Kind::BundleDoesntExist, "Bundle does not exist");
    }

    /// Get the error kind.
    /// @return The error kind
    Kind kind() const { return kind_; }

    /// Get the error message.
    /// @return The error message
    const std::string& message() const { return message_; }
};

/// A Ruffle bundle.
///
/// Represents a loaded game bundle containing SWF files and metadata.
class Bundle {
private:
    std::shared_ptr<bundle::source::BundleSource> source_;
    std::shared_ptr<bundle::info::BundleInformation> information_;
    std::vector<parse::ParseWarning> warnings_;

    Bundle(
        std::shared_ptr<bundle::source::BundleSource> source,
        std::shared_ptr<bundle::info::BundleInformation> information,
        std::vector<parse::ParseWarning> warnings
    )
        : source_(std::move(source))
        , information_(std::move(information))
        , warnings_(std::move(warnings)) {}

public:
    /// Load a bundle from a path.
    ///
    /// @param path The path to the bundle directory
    /// @return Result containing the bundle or BundleError
    static std::variant<Bundle, BundleError> from_path(const std::filesystem::path& path) {
        if (!std::filesystem::exists(path)) {
            return BundleError::bundle_doesnt_exist();
        }

        // Load source from path
        auto source_result = bundle::source::BundleSource::from_path(path);
        if (std::holds_alternative<BundleError>(source_result)) {
            return std::get<BundleError>(source_result);
        }

        auto source = std::get<std::shared_ptr<bundle::source::BundleSource>>(source_result);
        return from_source(source);
    }

    /// Load a bundle from a source.
    ///
    /// @param source The bundle source to load from
    /// @return Result containing the bundle or BundleError
    static std::variant<Bundle, BundleError> from_source(
        std::shared_ptr<bundle::source::BundleSource> source
    ) {
        // Read the bundle information file
        auto info_file_result = source->read_file(bundle::info::BUNDLE_INFORMATION_FILENAME);
        if (std::holds_alternative<BundleError>(info_file_result)) {
            return BundleError::missing_bundle_information();
        }

        auto info_file = std::get<std::vector<uint8_t>>(info_file_result);

        // Convert to string
        std::string info_text(info_file.begin(), info_file.end());

        // Parse the bundle information
        auto info_result = bundle::info::BundleInformation::parse(info_text);
        if (std::holds_alternative<BundleError>(info_result)) {
            return std::get<BundleError>(info_result);
        }

        auto [information, warnings] = std::get<std::pair<
            std::shared_ptr<bundle::info::BundleInformation>,
            std::vector<parse::ParseWarning>
        >>(info_result);

        return Bundle(source, information, warnings);
    }

    /// Get the bundle source.
    /// @return Reference to the bundle source
    const bundle::source::BundleSource& source() const {
        return *source_;
    }

    /// Get the bundle warnings.
    /// @return Reference to the warnings vector
    const std::vector<parse::ParseWarning>& warnings() const {
        return warnings_;
    }

    /// Get the bundle information.
    /// @return Reference to the bundle information
    const bundle::info::BundleInformation& information() const {
        return *information_;
    }
};

} // namespace bundle
} // namespace utils
} // namespace frontend
} // namespace ruffle

#endif // RUFFLE_FRONTEND_UTILS_BUNDLE_H
