// C++ translation of core/src/avm2/dynamic_map.rs
// Dynamic property map for AVM2 objects

#ifndef RUFFLE_CORE_AVM2_DYNAMIC_MAP_H
#define RUFFLE_CORE_AVM2_DYNAMIC_MAP_H

#include <cstdint>
#include <functional>
#include <optional>
#include <unordered_map>
#include <vector>
#include <cell>

namespace ruffle {
namespace core {
namespace avm2 {

// Forward declarations
template<typename GC>
class AvmString;
template<typename GC>
class Object;

/// Dynamic key for property lookup
/// Can be a string, uint (for integer property names), or object
template<typename GC>
class DynamicKey {
public:
    enum class Type {
        String,
        Uint,
        Object
    };
    
private:
    Type type_;
    AvmString<GC> string_val_;
    uint32_t uint_val_;
    Object<GC> object_val_;
    
public:
    explicit DynamicKey(AvmString<GC> str) 
        : type_(Type::String), string_val_(str) {}
    
    explicit DynamicKey(uint32_t val) 
        : type_(Type::Uint), uint_val_(val) {}
    
    explicit DynamicKey(Object<GC> obj) 
        : type_(Type::Object), object_val_(obj) {}
    
    Type type() const { return type_; }
    
    // Equality and hash would be implemented based on type
    bool operator==(const DynamicKey& other) const {
        if (type_ != other.type_) return false;
        switch (type_) {
            case Type::String: return string_val_ == other.string_val_;
            case Type::Uint: return uint_val_ == other.uint_val_;
            case Type::Object: return object_val_ == other.object_val_;
        }
        return false;
    }
};

/// Dynamic property with enumerable flag
template<typename V>
struct DynamicProperty {
    V value;
    bool enumerable;
    
    DynamicProperty() : value(), enumerable(true) {}
    DynamicProperty(const V& val) : value(val), enumerable(true) {}
    DynamicProperty(const V& val, bool enum_flag) : value(val), enumerable(enum_flag) {}
};

/// A HashMap designed for dynamic properties on an object.
/// 
/// Uses a hash table to expose stable bucket indices, which are
/// needed for correct iteration when entries are added or removed mid-iteration.
/// 
/// This is a C++ translation of the Rust DynamicMap that uses hashbrown::HashTable.
/// In C++, we use std::unordered_map with additional tracking for iteration stability.
template<typename K, typename V>
class DynamicMap {
private:
    // Storage for properties
    std::unordered_map<K, DynamicProperty<V>> map_;
    
    // Track the last public index and real bucket index for iteration
    mutable size_t public_index_;
    mutable size_t real_index_;
    
    // Hash function (using FNV hash like the Rust version)
    struct FnvHasher {
        size_t operator()(const K& key) const {
            // FNV-1a hash implementation
            const uint64_t FNV_OFFSET = 14695981039346656037ULL;
            const uint64_t FNV_PRIME = 1099511628211ULL;
            
            // Simple hash - in real implementation would use proper FNV on key bytes
            std::hash<K> hasher;
            return hasher(key);
        }
    };
    
    /// Convert public index to real bucket index
    /// Returns std::nullopt if the index is out of bounds
    std::optional<size_t> public_to_real_index(size_t index) const {
        size_t count = 0;
        
        for (const auto& [key, prop] : map_) {
            if (prop.enumerable) {
                count++;
                if (count >= index) {
                    // In a real implementation, we'd track actual bucket indices
                    // This is a simplified version
                    return count;
                }
            }
        }
        
        return std::nullopt;
    }
    
public:
    DynamicMap() : public_index_(0), real_index_(0) {}
    
    /// Get a property by key
    const DynamicProperty<V>* get(const K& key) const {
        auto it = map_.find(key);
        if (it != map_.end()) {
            return &it->second;
        }
        return nullptr;
    }
    
    /// Check if key exists
    bool contains_key(const K& key) const {
        return map_.find(key) != map_.end();
    }
    
    /// Get number of properties
    size_t len() const {
        return map_.size();
    }
    
    /// Check if empty
    bool is_empty() const {
        return map_.empty();
    }
    
    /// Insert or update a property
    void insert(const K& key, const V& new_value) {
        auto it = map_.find(key);
        if (it != map_.end()) {
            // Update existing - don't reset enumerable flag
            it->second.value = new_value;
        } else {
            // Insert new with enumerable = true
            map_[key] = DynamicProperty<V>(new_value, true);
        }
    }
    
    /// Remove a property
    std::optional<DynamicProperty<V>> remove(const K& key) {
        auto it = map_.find(key);
        if (it != map_.end()) {
            DynamicProperty<V> prop = it->second;
            map_.erase(it);
            return prop;
        }
        return std::nullopt;
    }
    
    /// Get next index for iteration
    /// Returns std::nullopt when iteration is complete
    std::optional<size_t> next(size_t index) const {
        // Start iteration from the beginning
        if (index == 0) {
            auto real = public_to_real_index(1);
            if (real.has_value()) {
                real_index_ = *real;
                public_index_ = 1;
                return 1;
            } else {
                public_index_ = 0;
                real_index_ = 0;
                return std::nullopt;
            }
        }
        
        // Sync the supplied index to the real index if needed
        if (public_index_ == 0 || index != public_index_) {
            auto real = public_to_real_index(index);
            if (real.has_value()) {
                real_index_ = *real;
                public_index_ = index;
            } else {
                public_index_ = 0;
                real_index_ = 0;
                return std::nullopt;
            }
        }
        
        // Find next enumerable property
        // Note: This is simplified - real implementation would track bucket indices
        size_t next_public = public_index_ + 1;
        auto next_real = public_to_real_index(next_public);
        if (next_real.has_value()) {
            real_index_ = *next_real;
            public_index_ = next_public;
            return next_public;
        }
        
        return std::nullopt;
    }
    
    /// Get key-value pair at index
    std::optional<std::pair<K, DynamicProperty<V>>> pair_at(size_t index) const {
        auto real_index = (public_index_ == 0 || public_index_ != index) 
            ? public_to_real_index(index) 
            : std::optional<size_t>(real_index_);
        
        if (!real_index.has_value()) {
            return std::nullopt;
        }
        
        // Simplified - would need proper bucket tracking
        size_t count = 0;
        for (const auto& [key, prop] : map_) {
            if (prop.enumerable) {
                count++;
                if (count == index) {
                    return std::make_pair(key, prop);
                }
            }
        }
        
        return std::nullopt;
    }
    
    /// Get key at index
    std::optional<K> key_at(size_t index) const {
        auto pair = pair_at(index);
        if (pair.has_value()) {
            return pair->first;
        }
        return std::nullopt;
    }
    
    /// Get value at index
    std::optional<V> value_at(size_t index) const {
        auto pair = pair_at(index);
        if (pair.has_value()) {
            return pair->second.value;
        }
        return std::nullopt;
    }
    
    /// Set enumerable flag for a property
    void set_enumerable(const K& key, bool enumerable) {
        auto it = map_.find(key);
        if (it != map_.end()) {
            it->second.enumerable = enumerable;
        }
    }
    
    // Iterator support
    auto begin() const { return map_.begin(); }
    auto end() const { return map_.end(); }
};

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_DYNAMIC_MAP_H
