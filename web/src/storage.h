// C++ translation of web/src/storage.rs
//! Web storage backend using browser localStorage

#ifndef RUFFLE_WEB_STORAGE_H
#define RUFFLE_WEB_STORAGE_H

#include <vector>
#include <string>
#include <string_view>
#include <optional>

// Forward declarations
namespace ruffle {
namespace core {
namespace backend {
namespace storage {
    class StorageBackend;
}
}
}
}

// Placeholder for web_sys::Storage
namespace web_sys {
    class Storage;
}

namespace ruffle {
namespace web {

/// LocalStorageBackend - Storage backend using browser localStorage
/// 
/// This backend stores data in the browser's localStorage API,
/// using base64 encoding for binary data.
class LocalStorageBackend : public ruffle::core::backend::storage::StorageBackend {
private:
    web_sys::Storage* storage_;

public:
    /// Create a new LocalStorageBackend
    /// 
    /// \param storage The web_sys::Storage instance (localStorage or sessionStorage)
    explicit LocalStorageBackend(web_sys::Storage* storage)
        : storage_(storage) {}

    /// Get a value from storage
    /// 
    /// \param name The key to look up
    /// \return The value as bytes, or std::nullopt if not found
    std::optional<std::vector<uint8_t>> get(std::string_view name) const override;

    /// Put a value into storage
    /// 
    /// \param name The key to store under
    /// \param value The value to store
    /// \return true if successful, false otherwise
    bool put(std::string_view name, const std::vector<uint8_t>& value) override;

    /// Remove a key from storage
    /// 
    /// \param name The key to remove
    void remove_key(std::string_view name) override;
};

} // namespace web
} // namespace ruffle

#endif // RUFFLE_WEB_STORAGE_H
