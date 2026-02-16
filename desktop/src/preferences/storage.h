// C++ translation of desktop/src/preferences/storage.rs
// Storage backend selection for preferences

#ifndef RUFFLE_DESKTOP_PREFERENCES_STORAGE_H
#define RUFFLE_DESKTOP_PREFERENCES_STORAGE_H

#include <string>
#include <string_view>
#include <memory>

// Forward declarations
namespace ruffle {
namespace core {
namespace backend {
namespace storage {
    class StorageBackend;
    class MemoryStorageBackend;
}
}
}
namespace frontend {
namespace utils {
namespace backends {
namespace storage {
    class DiskStorageBackend;
}
}
}
}
}

namespace ruffle {
namespace desktop {
namespace preferences {
namespace storage {

/// Storage backend selection
enum class StorageBackend : uint8_t {
    Disk,
    Memory,
};

/// Default value for StorageBackend
inline constexpr StorageBackend default_storage_backend() {
    return StorageBackend::Disk;
}

/// Parse a StorageBackend from a string
/// Returns std::nullopt if the string is not a valid backend
[[nodiscard]] inline std::optional<StorageBackend> from_str(std::string_view s) {
    if (s == "disk") return StorageBackend::Disk;
    if (s == "memory") return StorageBackend::Memory;
    return std::nullopt;
}

/// Get the backend as a string
[[nodiscard]] inline const char* as_str(StorageBackend backend) {
    switch (backend) {
        case StorageBackend::Disk:
            return "disk";
        case StorageBackend::Memory:
            return "memory";
        default:
            return "unknown";
    }
}

/// Create a storage backend instance
/// 
/// \param backend The storage backend type
/// \param save_directory The directory for disk storage (only used for Disk backend)
/// \return Unique pointer to the storage backend
[[nodiscard]] inline std::unique_ptr<core::backend::storage::StorageBackend> create_backend(
    StorageBackend backend,
    const std::string& save_directory
) {
    switch (backend) {
        case StorageBackend::Disk:
            return std::make_unique<frontend::utils::backends::storage::DiskStorageBackend>(save_directory);
        case StorageBackend::Memory:
            return std::make_unique<core::backend::storage::MemoryStorageBackend>();
        default:
            return std::make_unique<core::backend::storage::MemoryStorageBackend>();
    }
}

} // namespace storage
} // namespace preferences
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_PREFERENCES_STORAGE_H
