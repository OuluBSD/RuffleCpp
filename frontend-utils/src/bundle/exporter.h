// C++ translation of frontend-utils/src/bundle/exporter.rs
//! Bundle export functionality

#ifndef RUFFLE_FRONTEND_UTILS_BUNDLE_EXPORTER_H
#define RUFFLE_FRONTEND_UTILS_BUNDLE_EXPORTER_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <memory>
#include <string>
#include <system_error>

// Forward declarations
namespace ruffle {
namespace frontend_utils {
namespace bundle {
namespace info {
    class BundleInformation;
}
}
}
}

// For ZIP functionality (would use a library like libzip or zip-cpp)
namespace zip {
    class ZipWriter;
    class ZipError;
    template<typename T> class FileOptions;
}

namespace ruffle {
namespace frontend_utils {
namespace bundle {
namespace exporter {

/// Bundle export error types
class BundleExportError : public std::runtime_error {
public:
    enum class Type {
        ZipError,
        IoError,
        MissingBundleInfo,
    };

    BundleExportError(Type type, std::string message)
        : std::runtime_error(message)
        , type_(type)
    {}

    /// Create a ZIP error
    static BundleExportError zip_error(const std::string& message) {
        return BundleExportError(Type::ZipError, "Error writing ZIP stream: " + message);
    }

    /// Create an IO error
    static BundleExportError io_error(const std::string& message) {
        return BundleExportError(Type::IoError, "IO error: " + message);
    }

    /// Create a missing bundle info error
    static BundleExportError missing_bundle_info() {
        return BundleExportError(Type::MissingBundleInfo, "Missing bundle information");
    }

    Type type() const { return type_; }

private:
    Type type_;
};

/// Result type alias for bundle export operations
template<typename T>
using BundleExportResult = std::expected<T, BundleExportError>;

/// Bundle exporter - writes bundle content to a ZIP archive
template<typename W>
class BundleExporter {
public:
    /// Create a new bundle exporter
    /// @param write Output stream (must support Write + Seek)
    /// @param info Bundle information to include
    BundleExporter(W& write, const info::BundleInformation& info)
        : writer_(std::make_unique<zip::ZipWriter>(write))
        , info_(info)
    {}

    /// Write content to the bundle
    /// @param path Path within the bundle
    /// @param content Content to write
    BundleExportResult<void> write_content(
        const std::filesystem::path& path,
        std::istream& content
    ) {
        auto options = zip::FileOptions<void>::default_options();

        auto full_path = std::filesystem::path("content") / path;

        // Start a new file in the ZIP
        auto result = writer_->start_file(full_path.string(), options);
        if (!result) {
            return std::unexpected(BundleExportError::zip_error(result.error().what()));
        }

        // Copy content to the ZIP
        std::copy(
            std::istreambuf_iterator<char>(content),
            std::istreambuf_iterator<char>(),
            std::ostreambuf_iterator<char>(*writer_)
        );

        return {};
    }

    /// Finish writing the bundle
    BundleExportResult<void> finish() {
        // Write the bundle information
        auto result = write_info();
        if (!result) {
            return std::unexpected(result.error());
        }

        // Finalize the ZIP
        auto zip_result = writer_->finish();
        if (!zip_result) {
            return std::unexpected(BundleExportError::zip_error(zip_result.error().what()));
        }

        return {};
    }

private:
    /// Write the bundle information file
    BundleExportResult<void> write_info() {
        auto options = zip::FileOptions<void>::default_options();

        auto result = writer_->start_file(BUNDLE_INFORMATION_FILENAME, options);
        if (!result) {
            return std::unexpected(BundleExportError::zip_error(result.error().what()));
        }

        // Serialize the bundle info
        info_.serialize(*writer_);

        return {};
    }

    std::unique_ptr<zip::ZipWriter> writer_;
    info::BundleInformation info_;
};

/// Bundle information filename constant
constexpr const char* BUNDLE_INFORMATION_FILENAME = "bundle_info.json";

} // namespace exporter
} // namespace bundle
} // namespace frontend_utils
} // namespace ruffle

#endif // RUFFLE_FRONTEND_UTILS_BUNDLE_EXPORTER_H
