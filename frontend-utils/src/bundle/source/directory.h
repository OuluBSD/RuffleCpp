// C++ translation of frontend-utils/src/bundle/source/directory.rs
// Bundle source implementation for directory paths

#ifndef RUFFLE_FRONTEND_UTILS_BUNDLE_SOURCE_DIRECTORY_H
#define RUFFLE_FRONTEND_UTILS_BUNDLE_SOURCE_DIRECTORY_H

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <system_error>

namespace ruffle {
namespace frontend {
namespace utils {
namespace bundle {
namespace source {

/// Forward declaration for BundleSourceError
class BundleSourceError;

/// Bundle source implementation for directory paths.
///
/// This class implements the BundleSourceImpl interface for std::filesystem::path,
/// allowing bundles to be loaded from directory paths on the filesystem.
class DirectoryBundleSource {
public:
    /// Read a file from the bundle directory.
    ///
    /// @param base_path The base directory path
    /// @param path The relative path to the file (may start with '/')
    /// @param out_error Output parameter for error information
    /// @return File contents as bytes, or empty vector on error
    static std::vector<uint8_t> read_file(
        const std::filesystem::path& base_path,
        const std::string& path,
        BundleSourceError* out_error = nullptr
    );

    /// Read a content file from the bundle's content subdirectory.
    ///
    /// @param base_path The base directory path
    /// @param path The relative path to the content file (may start with '/')
    /// @param out_error Output parameter for error information
    /// @return File contents as bytes, or empty vector on error
    static std::vector<uint8_t> read_content(
        const std::filesystem::path& base_path,
        const std::string& path,
        BundleSourceError* out_error = nullptr
    );

private:
    /// Helper function to safely join and validate paths.
    ///
    /// Ensures the resulting path is within the base directory to prevent
    /// directory traversal attacks.
    ///
    /// @param base The base directory
    /// @param relative_path The relative path to join
    /// @param out_error Output parameter for error information
    /// @return The canonicalized path, or empty path on error
    static std::filesystem::path safe_join(
        const std::filesystem::path& base,
        const std::string& relative_path,
        BundleSourceError* out_error
    );
};

} // namespace source
} // namespace bundle
} // namespace utils
} // namespace frontend
} // namespace ruffle

#endif // RUFFLE_FRONTEND_UTILS_BUNDLE_SOURCE_DIRECTORY_H
