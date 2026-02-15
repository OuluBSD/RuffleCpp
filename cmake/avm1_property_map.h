/*
 * C++ header for AVM1 property map functionality
 * This replaces the functionality of core/src/avm1/property_map.rs
 */

#ifndef AVM1_PROPERTY_MAP_H
#define AVM1_PROPERTY_MAP_H

#include "avm1.h"
#include "avm1/value.h"
#include "avm1/object.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <optional>
#include <functional>
#include <algorithm>
#include <cctype>

namespace ruffle {

// Helper function to convert string to lowercase for case-insensitive comparison
inline std::string to_lowercase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), 
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

// Property name wrapper that handles case sensitivity
class PropertyName {
private:
    std::string name_;
    std::string lowercase_name_;  // For case-insensitive comparisons

public:
    explicit PropertyName(const std::string& name) 
        : name_(name), lowercase_name_(to_lowercase(name)) {}

    const std::string& name() const { return name_; }
    const std::string& lowercase_name() const { return lowercase_name_; }

    bool operator==(const PropertyName& other) const {
        return name_ == other.name_;
    }

    bool operator!=(const PropertyName& other) const {
        return !(*this == other);
    }
};

// Hash function for PropertyName
struct PropertyNameHash {
    std::size_t operator()(const PropertyName& prop_name) const {
        // Use case-insensitive hash for SWF compatibility
        std::size_t h = 0;
        for (char c : prop_name.lowercase_name()) {
            h ^= std::hash<char>{}(std::tolower(c)) + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        return h;
    }
};

// Case-insensitive comparator
struct CaseInsensitiveComparator {
    bool operator()(const std::string& a, const std::string& b) const {
        return to_lowercase(a) == to_lowercase(b);
    }
};

// Case-sensitive comparator
struct CaseSensitiveComparator {
    bool operator()(const std::string& a, const std::string& b) const {
        return a == b;
    }
};

// Entry enum for property map operations
template<typename V>
class Entry {
public:
    enum class Type { OCCUPIED, VACANT };

private:
    Type type_;
    std::string key_;
    V* value_ptr_;  // For occupied entries
    std::unordered_map<PropertyName, V, PropertyNameHash>* map_ptr_;  // For vacant entries

public:
    Entry(Type type, const std::string& key, V* value = nullptr, 
          std::unordered_map<PropertyName, V, PropertyNameHash>* map = nullptr)
        : type_(type), key_(key), value_ptr_(value), map_ptr_(map) {}

    Type type() const { return type_; }

    bool is_occupied() const { return type_ == Type::OCCUPIED; }
    bool is_vacant() const { return type_ == Type::VACANT; }

    V* get_occupied_value() const {
        if (type_ != Type::OCCUPIED) return nullptr;
        return value_ptr_;
    }

    void insert_value(const V& value) {
        if (type_ == Type::VACANT && map_ptr_) {
            map_ptr_->insert({PropertyName(key_), value});
        }
    }
};

// Occupied entry
template<typename V>
class OccupiedEntry {
private:
    std::unordered_map<PropertyName, V, PropertyNameHash>* map_;
    typename std::unordered_map<PropertyName, V, PropertyNameHash>::iterator iter_;

public:
    OccupiedEntry(std::unordered_map<PropertyName, V, PropertyNameHash>* map,
                  typename std::unordered_map<PropertyName, V, PropertyNameHash>::iterator iter)
        : map_(map), iter_(iter) {}

    V& get() { return iter_->second; }
    const V& get() const { return iter_->second; }

    V& get_mut() { return iter_->second; }

    V insert(V value) {
        V old_value = std::move(iter_->second);
        iter_->second = std::move(value);
        return old_value;
    }

    std::pair<std::string, V> remove_entry() {
        std::string key = iter_->first.name();
        V value = std::move(iter_->second);
        auto next_iter = map_->erase(iter_);
        (void)next_iter; // Suppress unused warning
        return {key, std::move(value)};
    }
};

// Vacant entry
template<typename V>
class VacantEntry {
private:
    std::unordered_map<PropertyName, V, PropertyNameHash>* map_;
    std::string key_;

public:
    VacantEntry(std::unordered_map<PropertyName, V, PropertyNameHash>* map, const std::string& key)
        : map_(map), key_(key) {}

    void insert(V value) {
        map_->insert({PropertyName(key_), std::move(value)});
    }
};

// Property map class for AVM1
template<typename V>
class PropertyMap {
private:
    std::unordered_map<PropertyName, V, PropertyNameHash> map_;
    std::vector<PropertyName> insertion_order_;  // To maintain insertion order

public:
    PropertyMap() = default;

    // Check if the map contains a key
    bool contains_key(const std::string& key, bool case_sensitive) const {
        if (case_sensitive) {
            return map_.find(PropertyName(key)) != map_.end();
        } else {
            // For case-insensitive search, we need to iterate
            for (const auto& [prop_name, value] : map_) {
                if (CaseInsensitiveComparator{}(prop_name.name(), key)) {
                    return true;
                }
            }
            return false;
        }
    }

    // Get a value by key
    std::optional<V> get(const std::string& key, bool case_sensitive) const {
        if (case_sensitive) {
            auto it = map_.find(PropertyName(key));
            if (it != map_.end()) {
                return it->second;
            }
        } else {
            // For case-insensitive search
            for (const auto& [prop_name, value] : map_) {
                if (CaseInsensitiveComparator{}(prop_name.name(), key)) {
                    return value;
                }
            }
        }
        return std::nullopt;
    }

    // Get a mutable reference to a value by key
    std::optional<std::reference_wrapper<V>> get_mut(const std::string& key, bool case_sensitive) {
        if (case_sensitive) {
            auto it = map_.find(PropertyName(key));
            if (it != map_.end()) {
                return std::ref(it->second);
            }
        } else {
            // For case-insensitive search
            for (auto& [prop_name, value] : map_) {
                if (CaseInsensitiveComparator{}(prop_name.name(), key)) {
                    return std::ref(value);
                }
            }
        }
        return std::nullopt;
    }

    // Get a value by index (based on insertion order)
    std::optional<V> get_index(size_t index) const {
        if (index < insertion_order_.size()) {
            auto it = map_.find(insertion_order_[index]);
            if (it != map_.end()) {
                return it->second;
            }
        }
        return std::nullopt;
    }

    // Insert a key-value pair
    std::optional<V> insert(const std::string& key, V value, bool case_sensitive) {
        auto prop_name = PropertyName(key);
        auto it = map_.find(prop_name);
        
        if (it != map_.end()) {
            // Key exists, replace value
            V old_value = std::move(it->second);
            it->second = std::move(value);
            return old_value;
        } else {
            // Key doesn't exist, insert new
            map_.insert({prop_name, std::move(value)});
            insertion_order_.push_back(prop_name);
            return std::nullopt;
        }
    }

    // Remove a key-value pair
    std::optional<V> remove(const std::string& key, bool case_sensitive) {
        if (case_sensitive) {
            auto it = map_.find(PropertyName(key));
            if (it != map_.end()) {
                V value = std::move(it->second);
                map_.erase(it);
                // Remove from insertion order
                auto order_it = std::find(insertion_order_.begin(), insertion_order_.end(), 
                                         PropertyName(key));
                if (order_it != insertion_order_.end()) {
                    insertion_order_.erase(order_it);
                }
                return value;
            }
        } else {
            // For case-insensitive removal
            for (auto it = map_.begin(); it != map_.end(); ++it) {
                if (CaseInsensitiveComparator{}(it->first.name(), key)) {
                    V value = std::move(it->second);
                    auto key_to_remove = it->first;
                    map_.erase(it);
                    // Remove from insertion order
                    auto order_it = std::find(insertion_order_.begin(), insertion_order_.end(), 
                                             key_to_remove);
                    if (order_it != insertion_order_.end()) {
                        insertion_order_.erase(order_it);
                    }
                    return value;
                }
            }
        }
        return std::nullopt;
    }

    // Get entry for operations
    Entry<V> entry(const std::string& key, bool case_sensitive) {
        if (case_sensitive) {
            auto it = map_.find(PropertyName(key));
            if (it != map_.end()) {
                return Entry<V>(Entry<V>::Type::OCCUPIED, key, &it->second);
            } else {
                return Entry<V>(Entry<V>::Type::VACANT, key, nullptr, &map_);
            }
        } else {
            // For case-insensitive search
            for (auto& [prop_name, value] : map_) {
                if (CaseInsensitiveComparator{}(prop_name.name(), key)) {
                    return Entry<V>(Entry<V>::Type::OCCUPIED, key, &value);
                }
            }
            return Entry<V>(Entry<V>::Type::VACANT, key, nullptr, &map_);
        }
    }

    // Iterator for values (in reverse insertion order to match Flash's behavior)
    class Iterator {
    private:
        typename std::vector<PropertyName>::reverse_iterator order_it_;
        typename std::vector<PropertyName>::reverse_iterator order_end_;
        const std::unordered_map<PropertyName, V, PropertyNameHash>& map_;

    public:
        Iterator(typename std::vector<PropertyName>::reverse_iterator begin,
                 typename std::vector<PropertyName>::reverse_iterator end,
                 const std::unordered_map<PropertyName, V, PropertyNameHash>& map)
            : order_it_(begin), order_end_(end), map_(map) {}

        std::pair<std::string, V> operator*() const {
            auto it = map_.find(*order_it_);
            if (it != map_.end()) {
                return {it->first.name(), it->second};
            }
            return {"", V{}};
        }

        Iterator& operator++() {
            ++order_it_;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return order_it_ != other.order_it_;
        }
    };

    Iterator begin() const {
        return Iterator(insertion_order_.rbegin(), insertion_order_.rend(), map_);
    }

    Iterator end() const {
        return Iterator(insertion_order_.rend(), insertion_order_.rend(), map_);
    }
};

} // namespace ruffle

#endif // AVM1_PROPERTY_MAP_H