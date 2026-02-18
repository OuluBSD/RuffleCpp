// C++ translation of frontend-utils/src/bundle/exporter/fs_helper.rs
//! Filesystem helper for bundle export

#ifndef RUFFLE_FRONTEND_UTILS_BUNDLE_EXPORTER_FS_HELPER_H
#define RUFFLE_FRONTEND_UTILS_BUNDLE_EXPORTER_FS_HELPER_H

#include <vector>
#include <string>
#include <optional>
#include <filesystem>
#include <system_error>

// Forward declarations
namespace ruffle {
namespace frontend_utils {

namespace bundle {

struct BundleInformation;

namespace exporter {
    template<typename W> class BundleExporter;
}

} // namespace bundle
} // namespace frontend_utils
} // namespace ruffle

namespace url {
    class Url;
}

namespace ruffle {
namespace frontend_utils {
namespace bundle {
namespace exporter {

/// Error types for filesystem helper operations
enum class FilesystemHelperError {
    NonAbsolutePaths,
    NoCommonDirectory,
    NoFilesToExport,
    PathOutOfCommonDirectory,
    PathNotRepresentableAsUrl,
    ErrorExportingBundle,
    IoError
};

/// Filesystem helper for bundle export operations
template<typename P = std::filesystem::path>
class FilesystemHelper {
public:
    /// Create a new filesystem helper
    ///
    /// \param files_to_export List of files to export
    /// \return Result or error
    static std::optional<FilesystemHelper<P>> create(
        const std::vector<P>& files_to_export
    );

    /// Get the root directory for bundle paths
    const std::optional<P>& root_dir() const { return root_dir_; }

    /// Convert a real path to a bundle path
    ///
    /// \param real_path The real filesystem path
    /// \return Bundle-relative path or error
    std::optional<P> real_path_to_bundle_path(const P& real_path) const;

    /// Convert a real path to a bundle URL
    ///
    /// \param real_path The real filesystem path
    /// \return File URL or error
    std::optional<url::Url> real_path_to_bundle_url(const P& real_path) const;

    /// Write files to a bundle exporter
    ///
    /// \param exporter The bundle exporter to write to
    /// \return Success or error
    template<typename W>
    bool write_files(BundleExporter<W>* exporter) const;

    /// Export a bundle to a file
    ///
    /// \param info Bundle information
    /// \param output Output file path
    /// \return Success or error
    bool export_bundle(BundleInformation info, const P& output) const;

private:
    /// Calculate common prefix of file paths
    ///
    /// \param files List of files
    /// \return Common directory or error
    static std::optional<P> calculate_common_prefix(
        const std::vector<P>& files
    );

    std::optional<P> root_dir_;
    std::vector<P> files_to_export_;
};

// Template implementations

template<typename P>
std::optional<FilesystemHelper<P>> FilesystemHelper<P>::create(
    const std::vector<P>& files_to_export
) {
    auto root_dir = calculate_common_prefix(files_to_export);
    if (!root_dir.has_value() && !files_to_export.empty()) {
        return std::nullopt;
    }

    FilesystemHelper<P> helper;
    helper.root_dir_ = root_dir;
    helper.files_to_export_ = files_to_export;
    return helper;
}

template<typename P>
std::optional<P> FilesystemHelper<P>::calculate_common_prefix(
    const std::vector<P>& files
) {
    if (files.empty()) {
        return std::nullopt;
    }

    const auto& first_file = files.front();
    auto common_prefix = first_file.parent_path();

    if (common_prefix.empty()) {
        return std::nullopt;
    }

    for (const auto& file : files) {
        if (!file.is_absolute()) {
            return std::nullopt; // NonAbsolutePaths error
        }

        while (!file.string().starts_with(common_prefix.string())) {
            common_prefix = common_prefix.parent_path();
            if (common_prefix.empty()) {
                return std::nullopt; // NoCommonDirectory error
            }
        }
    }

    return common_prefix;
}

template<typename P>
std::optional<P> FilesystemHelper<P>::real_path_to_bundle_path(
    const P& real_path
) const {
    if (!root_dir_.has_value()) {
        return std::nullopt; // NoFilesToExport error
    }

    std::error_code ec;
    auto relative = std::filesystem::relative(real_path, *root_dir_, ec);
    if (ec) {
        return std::nullopt; // PathOutOfCommonDirectory error
    }

    return relative;
}

template<typename P>
std::optional<url::Url> FilesystemHelper<P>::real_path_to_bundle_url(
    const P& real_path
) const {
    auto bundle_path = real_path_to_bundle_path(real_path);
    if (!bundle_path.has_value()) {
        return std::nullopt;
    }

    // Prepend / to make it an absolute path for the URL
    auto url_path = std::filesystem::path("/") / *bundle_path;

#ifdef _WIN32
    // Windows: manually create file:/// URL
    std::string url_str = "file://";
    for (const auto& component : url_path) {
        url_str += "/";
        url_str += component.string();
    }
    return url::Url(url_str);
#else
    // Unix: use standard file URL conversion
    return url::Url::from_file_path(url_path);
#endif
}

template<typename P>
template<typename W>
bool FilesystemHelper<P>::write_files(BundleExporter<W>* exporter) const {
    for (const auto& file : files_to_export_) {
        auto bundle_path = real_path_to_bundle_path(file);
        if (!bundle_path.has_value()) {
            return false;
        }

        std::ifstream input(file, std::ios::binary);
        if (!input) {
            return false; // IoError
        }

        if (!exporter->write_content(*bundle_path, input)) {
            return false; // ErrorExportingBundle
        }
    }
    return true;
}

template<typename P>
bool FilesystemHelper<P>::export_bundle(
    BundleInformation info,
    const P& output
) const {
    // Convert URL if it's a local file path
    if (info.url.scheme() == "file") {
        auto file_path = info.url.to_file_path();
        if (file_path.has_value()) {
            auto bundle_url = real_path_to_bundle_url(*file_path);
            if (bundle_url.has_value()) {
                info.url = *bundle_url;
            } else {
                return false;
            }
        }
    }

    std::ofstream file(output, std::ios::binary);
    if (!file) {
        return false; // IoError
    }

    BundleExporter<W> exporter(file, info);
    if (!write_files(&exporter)) {
        return false;
    }

    return exporter.finish();
}

} // namespace exporter
} // namespace bundle
} // namespace frontend_utils
} // namespace ruffle

#endif // RUFFLE_FRONTEND_UTILS_BUNDLE_EXPORTER_FS_HELPER_H
