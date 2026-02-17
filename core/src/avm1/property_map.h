// C++ translation of core/src/avm1/property_map.rs
//! The map of property names to values used by the ActionScript VM.
//! This allows for dynamically choosing case-sensitivity at runtime,
//! because SWFv6 and below is case-insensitive. This also maintains
//! the insertion order of properties, which is necessary for accurate
//! enumeration order.

#ifndef RUFFLE_CORE_AVM1_PROPERTY_MAP_H
#define RUFFLE_CORE_AVM1_PROPERTY_MAP_H

#include <cstdint>
#include <optional>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>

// Forward declarations
namespace ruffle {
namespace core {
    class WString;
    namespace string {
        class AvmString;
    }
    namespace avm1 {
        template<typename T> class Value;
    }
}
}

namespace ruffle {
namespace core {
namespace avm1 {

/// FNV-1a hash builder for case-insensitive string hashing
struct CaseInsensitiveHash {
    size_t operator()(const std::u16string& str) const {
        size_t hash = 14695981039346656037ULL;
        for (char16_t c : str) {
            // SWF case-insensitive comparison
            char16_t lower = swf_to_lowercase(c);
            hash ^= static_cast<size_t>(lower);
            hash *= 1099511628211ULL;
        }
        hash ^= 0xFF;
        return hash;
    }

private:
    static char16_t swf_to_lowercase(char16_t c) {
        // SWF-specific lowercase conversion
        if (c >= 'A' && c <= 'Z') {
            return c - 'A' + 'a';
        }
        return c;
    }
};

/// Equality comparator for case-insensitive comparison
struct CaseInsensitiveEqual {
    bool operator()(const std::u16string& lhs, const std::u16string& rhs) const {
        if (lhs.size() != rhs.size()) {
            return false;
        }
        for (size_t i = 0; i < lhs.size(); ++i) {
            if (CaseInsensitiveHash::swf_to_lowercase(lhs[i]) !=
                CaseInsensitiveHash::swf_to_lowercase(rhs[i])) {
                return false;
            }
        }
        return true;
    }
};

/// Equality comparator for case-sensitive comparison
struct CaseSensitiveEqual {
    bool operator()(const std::u16string& lhs, const std::u16string& rhs) const {
        return lhs == rhs;
    }
};

/// Entry types for PropertyMap operations
template<typename GC, typename V>
class PropertyMap;

template<typename GC, typename V>
class OccupiedEntry;

template<typename GC, typename V>
class VacantEntry;

template<typename GC, typename V>
enum class EntryType {
    Occupied,
    Vacant
};

template<typename GC, typename V>
class Entry {
public:
    Entry() = delete;

    bool is_occupied() const { return type_ == EntryType<GC, V>::Occupied; }
    bool is_vacant() const { return type_ == EntryType<GC, V>::Vacant; }

    OccupiedEntry<GC, V>& as_occupied() { return *occupied_; }
    VacantEntry<GC, V>& as_vacant() { return *vacant_; }

    static Entry occupied(OccupiedEntry<GC, V>* entry) {
        Entry e;
        e.type_ = EntryType<GC, V>::Occupied;
        e.occupied_ = entry;
        return e;
    }

    static Entry vacant(VacantEntry<GC, V>* entry) {
        Entry e;
        e.type_ = EntryType<GC, V>::Vacant;
        e.vacant_ = entry;
        return e;
    }

private:
    Entry() = default;
    EntryType<GC, V> type_;
    union {
        OccupiedEntry<GC, V>* occupied_;
        VacantEntry<GC, V>* vacant_;
    };
};

/// Occupied entry in a PropertyMap
template<typename GC, typename V>
class OccupiedEntry {
public:
    OccupiedEntry(PropertyMap<GC, V>* map, size_t index)
        : map_(map), index_(index) {}

    /// Remove and return the entry
    std::pair<string::AvmString, V> remove_entry();

    /// Get a reference to the value
    V& get() { return map_->values_[index_]; }
    const V& get() const { return map_->values_[index_]; }

    /// Replace the value and return the old one
    V insert(V value) {
        V old_value = std::move(map_->values_[index_]);
        map_->values_[index_] = std::move(value);
        return old_value;
    }

private:
    PropertyMap<GC, V>* map_;
    size_t index_;
};

/// Vacant entry in a PropertyMap
template<typename GC, typename V>
class VacantEntry {
public:
    VacantEntry(PropertyMap<GC, V>* map, string::AvmString key)
        : map_(map), key_(key) {}

    /// Insert a value at this vacant entry
    void insert(V value) {
        map_->insert(key_, std::move(value));
    }

private:
    PropertyMap<GC, V>* map_;
    string::AvmString key_;
};

/// A map from property names to values.
/// Maintains insertion order and supports case-sensitive/insensitive lookups.
template<typename GC, typename V>
class PropertyMap {
public:
    using value_type = std::pair<string::AvmString, V>;
    using iterator = typename std::vector<value_type>::reverse_iterator;
    using const_iterator = typename std::vector<value_type>::const_reverse_iterator;

    PropertyMap() = default;

    /// Check if the map contains a key
    ///
    /// \param key The key to search for
    /// \param case_sensitive Whether to use case-sensitive comparison
    /// \return true if the key exists, false otherwise
    bool contains_key(const string::AvmString& key, bool case_sensitive) const {
        if (case_sensitive) {
            return case_sensitive_map_.find(key.str()) != case_sensitive_map_.end();
        } else {
            return case_insensitive_map_.find(key.str()) != case_insensitive_map_.end();
        }
    }

    /// Get an entry for the key (for insertion/modification)
    ///
    /// \param key The key to look up
    /// \param case_sensitive Whether to use case-sensitive comparison
    /// \return An Entry that is either occupied or vacant
    Entry<GC, V> entry(string::AvmString key, bool case_sensitive) {
        if (case_sensitive) {
            auto it = case_sensitive_map_.find(key.str());
            if (it != case_sensitive_map_.end()) {
                return Entry<GC, V>::occupied(new OccupiedEntry<GC, V>(this, it->second));
            }
        } else {
            auto it = case_insensitive_map_.find(key.str());
            if (it != case_insensitive_map_.end()) {
                return Entry<GC, V>::occupied(new OccupiedEntry<GC, V>(this, it->second));
            }
        }
        return Entry<GC, V>::vacant(new VacantEntry<GC, V>(this, key));
    }

    /// Get a value by key
    ///
    /// \param key The key to look up
    /// \param case_sensitive Whether to use case-sensitive comparison
    /// \return Pointer to the value, or nullptr if not found
    const V* get(const string::AvmString& key, bool case_sensitive) const {
        const std::u16string& key_str = key.str();
        size_t index;

        if (case_sensitive) {
            auto it = case_sensitive_map_.find(key_str);
            if (it == case_sensitive_map_.end()) {
                return nullptr;
            }
            index = it->second;
        } else {
            auto it = case_insensitive_map_.find(key_str);
            if (it == case_insensitive_map_.end()) {
                return nullptr;
            }
            index = it->second;
        }

        return &values_[index];
    }

    /// Get a mutable reference to a value by key
    ///
    /// \param key The key to look up
    /// \param case_sensitive Whether to use case-sensitive comparison
    /// \return Pointer to the value, or nullptr if not found
    V* get_mut(const string::AvmString& key, bool case_sensitive) {
        const std::u16string& key_str = key.str();
        size_t index;

        if (case_sensitive) {
            auto it = case_sensitive_map_.find(key_str);
            if (it == case_sensitive_map_.end()) {
                return nullptr;
            }
            index = it->second;
        } else {
            auto it = case_insensitive_map_.find(key_str);
            if (it == case_insensitive_map_.end()) {
                return nullptr;
            }
            index = it->second;
        }

        return &values_[index];
    }

    /// Get a value by index (based on insertion order)
    ///
    /// \param index The index to get
    /// \return Pointer to the value, or nullptr if index is out of bounds
    const V* get_index(size_t index) const {
        if (index >= values_.size()) {
            return nullptr;
        }
        return &values_[index];
    }

    /// Insert a value into the map
    ///
    /// \param key The key to insert
    /// \param value The value to insert
    /// \param case_sensitive Whether to use case-sensitive comparison
    /// \return The old value if the key existed, nullopt otherwise
    std::optional<V> insert(string::AvmString key, V value, bool case_sensitive) {
        auto entry = this->entry(key, case_sensitive);
        if (entry.is_occupied()) {
            return std::make_optional(entry.as_occupied().insert(std::move(value)));
        } else {
            entry.as_vacant().insert(std::move(value));
            return std::nullopt;
        }
    }

    /// Iterate over all entries in Flash's iteration order (most recently added first)
    ///
    /// \return Iterator range
    auto iter() const {
        return std::make_pair(values_.rbegin(), values_.rend());
    }

    /// Remove a key from the map
    ///
    /// \param key The key to remove
    /// \param case_sensitive Whether to use case-sensitive comparison
    /// \return The removed value, or nullopt if the key didn't exist
    std::optional<V> remove(const string::AvmString& key, bool case_sensitive) {
        const std::u16string& key_str = key.str();
        auto map_it = case_sensitive ?
            case_sensitive_map_.find(key_str) :
            case_insensitive_map_.find(key_str);

        if (map_it == (case_sensitive ? case_sensitive_map_.end() : case_insensitive_map_.end())) {
            return std::nullopt;
        }

        size_t index = map_it->second;
        V value = std::move(values_[index]);

        // Remove from both maps
        if (case_sensitive) {
            case_sensitive_map_.erase(map_it);
        } else {
            case_insensitive_map_.erase(map_it);
        }

        // Shift indices for entries after the removed one
        for (auto& pair : case_sensitive_map_) {
            if (pair.second > index) {
                pair.second--;
            }
        }
        for (auto& pair : case_insensitive_map_) {
            if (pair.second > index) {
                pair.second--;
            }
        }

        // Remove from values vector
        values_.erase(values_.begin() + index);

        return std::make_optional(std::move(value));
    }

    /// Get the number of entries in the map
    size_t size() const { return values_.size(); }

    /// Check if the map is empty
    bool empty() const { return values_.empty(); }

private:
    void insert(string::AvmString key, V value) {
        size_t index = values_.size();
        values_.emplace_back(key, std::move(value));

        case_sensitive_map_[key.str()] = index;
        case_insensitive_map_[key.str()] = index;
    }

    // Storage for values in insertion order
    std::vector<value_type> values_;

    // Case-sensitive map: key string -> index in values_
    std::unordered_map<std::u16string, size_t> case_sensitive_map_;

    // Case-insensitive map: key string -> index in values_
    std::unordered_map<std::u16string, size_t, CaseInsensitiveHash, CaseInsensitiveEqual> case_insensitive_map_;
};

// OccupiedEntry implementations

template<typename GC, typename V>
std::pair<string::AvmString, V> OccupiedEntry<GC, V>::remove_entry() {
    auto key = map_->values_[index_].first;
    auto value = std::move(map_->values_[index_].second);

    // Remove from both maps
    map_->case_sensitive_map_.erase(key.str());
    map_->case_insensitive_map_.erase(key.str());

    // Shift indices for entries after the removed one
    for (auto& pair : map_->case_sensitive_map_) {
        if (pair.second > index_) {
            pair.second--;
        }
    }
    for (auto& pair : map_->case_insensitive_map_) {
        if (pair.second > index_) {
            pair.second--;
        }
    }

    // Remove from values vector
    map_->values_.erase(map_->values_.begin() + index_);

    return std::make_pair(key, std::move(value));
}

} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_PROPERTY_MAP_H
