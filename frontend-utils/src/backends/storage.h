// C++ translation of frontend-utils/src/backends/storage.rs
// Disk-based storage backend for shared objects

#ifndef RUFFLE_FRONTEND_UTILS_BACKENDS_STORAGE_H
#define RUFFLE_FRONTEND_UTILS_BACKENDS_STORAGE_H

#include <string>
#include <vector>
#include <optional>
#include <filesystem>

namespace ruffle {
namespace frontend {
namespace utils {
namespace backends {
namespace storage {

/// Disk-based storage backend for shared objects.
///
/// This backend stores shared objects as .sol files on disk.
/// It verifies paths to prevent accessing files outside of the Ruffle directory.
class DiskStorageBackend {
private:
    std::filesystem::path shared_objects_path_;

    /// Get the path for a shared object file.
    /// @param name The name of the shared object
    /// @return The full path to the .sol file
    std::filesystem::path get_shared_object_path(const std::string& name) const {
        return shared_objects_path_ / (name + ".sol");
    }

    /// Verifies that the path contains no `..` components to prevent
    /// accessing files outside of the Ruffle directory.
    /// @param path The path to verify
    /// @return true if the path is allowed, false otherwise
    static bool is_path_allowed(const std::filesystem::path& path) {
        for (const auto& component : path) {
            if (component == "..") {
                return false;
            }
        }
        return true;
    }

public:
    /// Create a new DiskStorageBackend.
    ///
    /// Creates the base directory if it doesn't exist yet.
    /// @param shared_objects_path The path to store shared objects
    explicit DiskStorageBackend(const std::filesystem::path& shared_objects_path)
        : shared_objects_path_(shared_objects_path) {
        // Create base dir if one doesn't exist yet
        if (!std::filesystem::exists(shared_objects_path_)) {
            std::error_code ec;
            if (!std::filesystem::create_directories(shared_objects_path_, ec)) {
                // Log warning if directory creation fails
                // tracing::warn!("Unable to create storage dir {}", ec.message());
            }
        }
    }

    /// Get a shared object by name.
    ///
    /// @param name The name of the shared object
    /// @return The shared object data, or std::nullopt if not found or error
    std::optional<std::vector<uint8_t>> get(const std::string& name) const {
        auto path = get_shared_object_path(name);
        if (!is_path_allowed(path)) {
            return std::nullopt;
        }

        std::error_code ec;
        auto data = std::filesystem::read_file(path, ec);
        if (ec) {
            // tracing::warn!("Unable to read file \"{}\": {}", name, ec.message());
            return std::nullopt;
        }

        return std::vector<uint8_t>(data.begin(), data.end());
    }

    /// Store a shared object by name.
    ///
    /// @param name The name of the shared object
    /// @param value The data to store
    /// @return true on success, false on failure
    bool put(const std::string& name, const std::vector<uint8_t>& value) {
        auto path = get_shared_object_path(name);
        if (!is_path_allowed(path)) {
            return false;
        }

        // Create parent directory if needed
        if (auto parent_dir = path.parent_path(); !parent_dir.empty()) {
            if (!std::filesystem::exists(parent_dir)) {
                std::error_code ec;
                if (!std::filesystem::create_directories(parent_dir, ec)) {
                    // tracing::warn!("Unable to create storage dir {}", ec.message());
                    return false;
                }
            }
        }

        std::error_code ec;
        std::filesystem::write_file(path, value, ec);
        if (ec) {
            // tracing::warn!("Unable to save file {}", ec.message());
            return false;
        }

        return true;
    }

    /// Remove a shared object by name.
    ///
    /// @param name The name of the shared object to remove
    void remove_key(const std::string& name) {
        auto path = get_shared_object_path(name);
        if (!is_path_allowed(path)) {
            return;
        }
        std::error_code ec;
        std::filesystem::remove(path, ec);
    }
};

} // namespace storage
} // namespace backends
} // namespace utils
} // namespace frontend
} // namespace ruffle

#endif // RUFFLE_FRONTEND_UTILS_BACKENDS_STORAGE_H
