/*
 * C++ header for AVM string representation functionality
 * This replaces the functionality of core/common/src/avm_string/repr.rs
 */

#ifndef AVM_STRING_REPR_H
#define AVM_STRING_REPR_H

#include "avm_string.h"
#include <memory>
#include <atomic>
#include <mutex>

// Forward declaration
template<typename GCContext>
class AvmStringRepr;

// Metadata structure for string representation
struct WStrMetadata {
    size_t length;
    bool is_wide;
    bool is_interned;
    
    WStrMetadata(size_t len = 0, bool wide = false, bool interned = false) 
        : length(len), is_wide(wide), is_interned(interned) {}
};

// Internal representation of AvmAtom and (owned) AvmString
template<typename GCContext>
class AvmStringRepr {
private:
    // Raw pointer to the string data
    void* ptr_;
    
    // Metadata about the string
    WStrMetadata meta_;
    
    // Capacity and interned flag
    mutable std::atomic<WStrMetadata> capacity_;
    
    // Number of characters used (for owned strings)
    mutable std::atomic<uint32_t> chars_used_;
    
    // Owner for dependent strings (nullptr if owned)
    std::shared_ptr<AvmStringRepr> owner_;
    
    // Mutex for thread-safe operations
    mutable std::mutex mutex_;

public:
    // Constructor from raw WString
    static AvmStringRepr from_raw(const WString& s, bool interned = false) {
        AvmStringRepr repr;
        repr.ptr_ = const_cast<void*>(static_cast<const void*>(s.c_str()));
        repr.meta_ = WStrMetadata(s.size(), s.is_wide(), interned);
        repr.capacity_ = WStrMetadata(s.size(), false, interned);  // Initially capacity equals length
        repr.chars_used_ = static_cast<uint32_t>(s.size());
        repr.owner_ = nullptr;
        return repr;
    }
    
    // Constructor for static string
    static AvmStringRepr from_raw_static(const char16_t* s, bool interned = false) {
        AvmStringRepr repr;
        repr.ptr_ = const_cast<char16_t*>(s);
        size_t len = std::char_traits<char16_t>::length(s);
        repr.meta_ = WStrMetadata(len, true, interned);  // Static strings are considered wide
        repr.capacity_ = WStrMetadata(0, false, interned);  // Static strings have 0 capacity
        repr.chars_used_ = 0;
        repr.owner_ = nullptr;
        return repr;
    }
    
    // Create dependent string (substring)
    static AvmStringRepr new_dependent(std::shared_ptr<AvmStringRepr> s, 
                                      size_t start, size_t end) {
        AvmStringRepr repr;
        // Extract substring from the original string
        const char16_t* original_data = reinterpret_cast<const char16_t*>(s->ptr_);
        size_t substring_len = end - start;
        
        // Allocate new memory for the substring
        char16_t* substring_data = new char16_t[substring_len + 1];
        std::copy(original_data + start, original_data + end, substring_data);
        substring_data[substring_len] = u'\0';
        
        repr.ptr_ = substring_data;
        repr.meta_ = WStrMetadata(substring_len, s->meta_.is_wide, false);  // Dependent strings not interned
        repr.capacity_ = WStrMetadata(0, false, false);  // Dependent strings have 0 capacity
        repr.chars_used_ = 0;
        repr.owner_ = s;  // Set the owner
        return repr;
    }
    
    // Try to append to the string in-place (for optimization)
    static std::optional<AvmStringRepr> try_append_inline(std::shared_ptr<AvmStringRepr> left, 
                                                         const WString& right) {
        // Check if both strings have the same character width
        if (left->meta_.is_wide != right.is_wide()) {
            return std::nullopt;
        }
        
        // For simplicity in this implementation, we'll return nullopt
        // In a real implementation, this would check if there's enough capacity
        // to append the right string to the left string in-place
        return std::nullopt;
    }
    
    // Check if this is a dependent string
    bool is_dependent() const {
        return owner_ != nullptr;
    }
    
    // Get the owner (for dependent strings)
    std::shared_ptr<AvmStringRepr> owner() const {
        return owner_;
    }
    
    // Get the string as WStr
    const WString as_wstr() const {
        if (meta_.is_wide) {
            const char16_t* data = static_cast<const char16_t*>(ptr_);
            return WString(std::u16string(data, meta_.length));
        } else {
            // Handle narrow string case
            const char* data = static_cast<const char*>(ptr_);
            return WString(std::string(data, meta_.length));
        }
    }
    
    // Check if the string is interned
    bool is_interned() const {
        return capacity_.load().is_interned;
    }
    
    // Mark the string as interned
    void mark_interned() const {
        if (is_dependent()) {
            throw std::runtime_error("Bug: tried to intern a dependent string");
        }
        
        WStrMetadata current_cap = capacity_.load();
        WStrMetadata new_cap(current_cap.length, current_cap.is_wide, true);
        capacity_.store(new_cap);
    }
    
    // Check if string is wide
    bool is_wide() const {
        return meta_.is_wide;
    }
    
    // Get length
    size_t len() const {
        return meta_.length;
    }
    
    // Destructor
    ~AvmStringRepr() {
        // Clean up if this is an owned string with allocated memory
        if (capacity_.load().length > 0 && !is_dependent()) {
            // Only deallocate if this is not a dependent string
            if (meta_.is_wide) {
                delete[] static_cast<char16_t*>(ptr_);
            } else {
                delete[] static_cast<char*>(ptr_);
            }
        }
    }
    
    // Copy constructor
    AvmStringRepr(const AvmStringRepr& other) 
        : ptr_(other.ptr_), meta_(other.meta_), 
          capacity_(other.capacity_.load()), 
          chars_used_(other.chars_used_.load()),
          owner_(other.owner_) {}
    
    // Assignment operator
    AvmStringRepr& operator=(const AvmStringRepr& other) {
        if (this != &other) {
            ptr_ = other.ptr_;
            meta_ = other.meta_;
            capacity_ = other.capacity_.load();
            chars_used_ = other.chars_used_.load();
            owner_ = other.owner_;
        }
        return *this;
    }
};

// Specialization for smart pointer deleter
template<typename GCContext>
struct AvmStringReprDeleter {
    void operator()(AvmStringRepr<GCContext>* ptr) const {
        delete ptr;
    }
};

#endif // AVM_STRING_REPR_H