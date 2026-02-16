// C++ translation of core/src/backend/storage.rs
// Storage backend interface

#ifndef RUFFLE_CORE_BACKEND_STORAGE_H
#define RUFFLE_CORE_BACKEND_STORAGE_H

#include <vector>
#include <string>
#include <optional>
#include <unordered_map>

namespace ruffle {
namespace core {
namespace backend {
namespace storage {

/// Interface for storage backends
/// Provides key-value storage for persistent data
class StorageBackend {
public:
    virtual ~StorageBackend() = default;

    /// Get a value by name
    /// Returns std::nullopt if the key doesn't exist
    virtual std::optional<std::vector<uint8_t>> get(const std::string& name) const = 0;

    /// Put a value by name
    /// Returns true if successful
    virtual bool put(const std::string& name, const std::vector<uint8_t>& value) = 0;

    /// Get the size of a value by name
    /// Returns std::nullopt if the key doesn't exist
    virtual std::optional<size_t> get_size(const std::string& name) const {
        auto value = get(name);
        if (value.has_value()) {
            return value->size();
        }
        return std::nullopt;
    }

    /// Remove a key
    virtual void remove_key(const std::string& name) = 0;
};

/// In-memory storage backend implementation
/// Uses a hash map to store key-value pairs
class MemoryStorageBackend : public StorageBackend {
private:
    std::unordered_map<std::string, std::vector<uint8_t>> map;

public:
    MemoryStorageBackend() = default;

    std::optional<std::vector<uint8_t>> get(const std::string& name) const override {
        auto it = map.find(name);
        if (it != map.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    bool put(const std::string& name, const std::vector<uint8_t>& value) override {
        map[name] = value;
        return true;
    }

    void remove_key(const std::string& name) override {
        map.erase(name);
    }
};

} // namespace storage
} // namespace backend
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_BACKEND_STORAGE_H
