/*
 * C++ header for AVM string functionality
 * This replaces the functionality of core/common/src/avm_string/avm_string.rs
 */

#ifndef AVM_STRING_H
#define AVM_STRING_H

#include <string>
#include <memory>
#include <cstring>
#include <vector>
#include <utility>

// Forward declaration
template<typename T>
class GCObject;

// Represents a wide string similar to WStr/WString
class WString {
private:
    std::vector<char16_t> data;

public:
    WString() = default;
    explicit WString(const std::string& utf8_str);
    explicit WString(const std::u16string& wide_str);
    explicit WString(const char* c_str);
    explicit WString(const char16_t* wide_c_str);
    
    size_t size() const { return data.size(); }
    bool empty() const { return data.empty(); }
    const char16_t* c_str() const { return data.data(); }
    
    void push_str(const WString& other);
    void reserve(size_t capacity);
    
    static WString from_utf8(const std::string& utf8);
    static WString from_utf8_bytes(const std::vector<uint8_t>& bytes);
    static WString from_units(const std::vector<uint8_t>& units);
    
    bool operator==(const WString& other) const;
    bool operator!=(const WString& other) const { return !(*this == other); }
};

// Represents the string representation
template<typename GCContext>
class AvmStringRepr {
private:
    WString string_data;
    bool is_dependent_flag;
    bool is_interned_flag;

public:
    AvmStringRepr(WString str, bool dependent = false);
    
    const WString& as_wstr() const { return string_data; }
    bool is_dependent() const { return is_dependent_flag; }
    bool is_interned() const { return is_interned_flag; }
    
    static AvmStringRepr from_raw(WString str, bool dependent);
    static AvmStringRepr from_raw_static(const char16_t* str, bool dependent);
    static std::unique_ptr<AvmStringRepr> new_dependent(std::shared_ptr<AvmStringRepr> parent, 
                                                        size_t start, size_t end);
    static std::unique_ptr<AvmStringRepr> try_append_inline(std::shared_ptr<AvmStringRepr> left, 
                                                           const std::shared_ptr<AvmStringRepr>& right);
};

// Main AVM string class (garbage collected)
template<typename GCContext>
class AvmString {
private:
    std::shared_ptr<AvmStringRepr<GCContext>> repr;

public:
    explicit AvmString(std::shared_ptr<AvmStringRepr<GCContext>> rep);

    // Factory methods
    static AvmString new_ascii_static(GCContext& gc_context, const std::vector<uint8_t>& bytes);
    static AvmString new_utf8(GCContext& gc_context, const std::string& utf8_string);
    static AvmString new_utf8_bytes(GCContext& gc_context, const std::vector<uint8_t>& bytes);
    static AvmString new_string(GCContext& gc_context, const WString& string);
    static AvmString substring(GCContext& gc_context, const AvmString& string, 
                               size_t start, size_t end);

    // Methods
    const WString& as_wstr() const;
    bool is_dependent() const;
    bool is_empty() const { return repr->as_wstr().empty(); }
    size_t len() const { return repr->as_wstr().size(); }
    bool is_wide() const; // Check if string contains wide characters
    
    // String operations
    static AvmString concat(GCContext& mc, const AvmString& left, const AvmString& right);
    static bool ptr_eq(const AvmString& this_str, const AvmString& other_str);
    
    // Equality operators
    bool operator==(const AvmString& other) const;
    bool operator!=(const AvmString& other) const { return !(*this == other); }
};

// Hash specialization for AvmString to be used in unordered containers
namespace std {
    template<typename GCContext>
    struct hash<AvmString<GCContext>> {
        size_t operator()(const AvmString<GCContext>& str) const {
            return hash<std::u16string>{}(std::u16string(str.as_wstr().c_str(), str.len()));
        }
    };
}

#endif // AVM_STRING_H