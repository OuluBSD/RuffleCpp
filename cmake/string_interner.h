/*
 * C++ header for AVM string interner functionality
 * This replaces the functionality of core/common/src/avm_string/interner.rs
 */

#ifndef STRING_INTERNER_H
#define STRING_INTERNER_H

#include "avm_string.h"
#include "common_strings.h"
#include <unordered_map>
#include <memory>
#include <functional>
#include <mutex>
#include <shared_mutex>

// Forward declaration
template<typename GCContext>
class AvmStringInterner;

// An interned AVM string, with fast by-pointer equality and hashing
template<typename GCContext>
class AvmAtom {
private:
    std::shared_ptr<AvmStringRepr<GCContext>> repr;

public:
    explicit AvmAtom(std::shared_ptr<AvmStringRepr<GCContext>> repr_ptr) : repr(repr_ptr) {}

    // Equality operator
    bool operator==(const AvmAtom& other) const {
        return repr.get() == other.repr.get();  // Pointer comparison
    }

    // Less-than operator for use in ordered containers
    bool operator<(const AvmAtom& other) const {
        return repr.get() < other.repr.get();
    }

    // Getter for the underlying string representation
    const WString& as_wstr() const {
        return repr->as_wstr();
    }

    // Hash function for use in unordered containers
    size_t hash() const {
        return std::hash<void*>{}(repr.get());
    }

    // Conversion to AvmString
    operator AvmString<GCContext>() const {
        return AvmString<GCContext>(repr);
    }

    // Static method to intern a static string
    static AvmAtom intern_static(GCContext& gc_context, const std::vector<uint8_t>& bytes) {
        // Create a string from the static bytes
        WString wstr = WString::from_units(bytes);
        auto repr = std::make_shared<AvmStringRepr<GCContext>>(
            AvmStringRepr<GCContext>::from_raw_static(wstr.c_str(), true));
        return AvmAtom(repr);
    }
};

// Specialize std::hash for AvmAtom to be used in unordered containers
namespace std {
    template<typename GCContext>
    struct hash<AvmAtom<GCContext>> {
        size_t operator()(const AvmAtom<GCContext>& atom) const {
            return atom.hash();
        }
    };
}

// WeakSet implementation using shared_ptr and weak_ptr
template<typename GCContext, typename T>
class WeakSet {
private:
    mutable std::shared_mutex mutex_;
    std::unordered_map<size_t, std::weak_ptr<T>> table_;
    std::hash<std::string> hasher_;

public:
    WeakSet() = default;

    // Insert a new element
    std::shared_ptr<T> insert_fresh(GCContext& gc_context, const std::string& key_str, std::shared_ptr<T> value) {
        std::unique_lock lock(mutex_);
        
        size_t hash_val = hasher_(key_str);
        table_[hash_val] = value;
        return value;
    }

    // Find an element by key
    std::shared_ptr<T> find(GCContext& gc_context, const std::string& key_str) {
        std::shared_lock lock(mutex_);
        
        size_t hash_val = hasher_(key_str);
        auto it = table_.find(hash_val);
        if (it != table_.end()) {
            auto sp = it->second.lock();  // Upgrade weak_ptr to shared_ptr
            if (sp) {
                return sp;  // Return the shared_ptr if still alive
            } else {
                // Element was destroyed, remove the weak_ptr
                std::unique_lock write_lock(mutex_);  // Need exclusive lock to modify
                table_.erase(it);  // Remove dead weak_ptr
            }
        }
        return nullptr;  // Not found or was destroyed
    }

    // Clean up dead entries
    void cleanup_dead_entries() {
        std::unique_lock lock(mutex_);
        auto it = table_.begin();
        while (it != table_.end()) {
            if (it->second.expired()) {
                it = table_.erase(it);
            } else {
                ++it;
            }
        }
    }
};

// Entry enum for lookup results
template<typename GCContext, typename T>
class Entry {
public:
    enum Type { OCCUPIED, VACANT };
    
private:
    Type type_;
    std::shared_ptr<T> occupied_value_;
    WeakSet<GCContext, T>* vacant_set_;
    size_t vacant_hash_;

public:
    Entry(std::shared_ptr<T> value) : type_(OCCUPIED), occupied_value_(value) {}
    
    Entry(WeakSet<GCContext, T>* set, size_t hash) 
        : type_(VACANT), vacant_set_(set), vacant_hash_(hash) {}
    
    Type type() const { return type_; }
    
    std::shared_ptr<T> get_occupied() const { 
        if (type_ != OCCUPIED) throw std::runtime_error("Not an occupied entry");
        return occupied_value_; 
    }
    
    std::pair<WeakSet<GCContext, T>*, size_t> get_vacant() const {
        if (type_ != VACANT) throw std::runtime_error("Not a vacant entry");
        return std::make_pair(vacant_set_, vacant_hash_);
    }
};

template<typename GCContext>
class AvmStringInterner {
private:
    WeakSet<GCContext, AvmStringRepr<GCContext>> interned_;
    CommonStrings<GCContext> common_;

public:
    AvmStringInterner(GCContext& gc_context) : common_(CommonStrings<GCContext>::create(gc_context)) {
        // Initialize common strings during construction
        initialize_common_strings(gc_context);
    }

    // Method to intern a string
    AvmAtom<GCContext> intern(GCContext& gc_context, const WString& str) {
        // Try to find existing interned string
        std::string key = std::string(str.c_str(), str.size());  // Simple key for demo
        auto existing = interned_.find(gc_context, key);
        
        if (existing) {
            // Found existing interned string
            return AvmAtom<GCContext>(existing);
        } else {
            // Create new interned string
            auto repr = std::make_shared<AvmStringRepr<GCContext>>(
                AvmStringRepr<GCContext>::from_raw(str, true));  // Mark as interned
            auto inserted = interned_.insert_fresh(gc_context, key, repr);
            return AvmAtom<GCContext>(inserted);
        }
    }

    // Method to intern a static string
    AvmAtom<GCContext> intern_static(GCContext& gc_context, const char16_t* str) {
        WString wstr(str);
        std::string key = std::string(reinterpret_cast<const char*>(str), 
                                     std::char_traits<char16_t>::length(str) * sizeof(char16_t));
        auto repr = std::make_shared<AvmStringRepr<GCContext>>(
            AvmStringRepr<GCContext>::from_raw_static(str, true));
        auto inserted = interned_.insert_fresh(gc_context, key, repr);
        return AvmAtom<GCContext>(inserted);
    }

    // Method to get an interned string if it exists
    std::optional<AvmAtom<GCContext>> get(GCContext& gc_context, const WString& str) {
        std::string key = std::string(str.c_str(), str.size());  // Simple key for demo
        auto result = interned_.find(gc_context, key);
        if (result) {
            return AvmAtom<GCContext>(result);
        }
        return std::nullopt;
    }

    // Method to create a substring
    AvmString<GCContext> substring(GCContext& gc_context, 
                                   const AvmString<GCContext>& str, 
                                   size_t start_index, 
                                   size_t end_index) {
        if (start_index == end_index) {
            // Return empty string
            return common_.str_;
        } else if (end_index == start_index + 1) {
            // Check if it's an ASCII character
            char16_t c = str.as_wstr().c_str()[start_index];
            if (c < ASCII_CHARS_LEN) {
                return static_cast<AvmString<GCContext>>(common_.ascii_chars[c]);
            }
        }
        
        // Create substring using AvmString method
        return AvmString<GCContext>::substring(gc_context, str, start_index, end_index);
    }

    // Getter for common strings
    const CommonStrings<GCContext>& get_common() const {
        return common_;
    }

private:
    void initialize_common_strings(GCContext& gc_context) {
        // Common strings are initialized in the constructor of CommonStrings
    }
};

#endif // STRING_INTERNER_H