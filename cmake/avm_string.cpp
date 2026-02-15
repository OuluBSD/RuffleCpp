/*
 * C++ implementation for AVM string functionality
 * This replaces the functionality of core/common/src/avm_string/avm_string.rs
 */

#include "avm_string.h"
#include <algorithm>
#include <codecvt>
#include <locale>

// Implementation of WString
WString::WString(const std::string& utf8_str) {
    // Convert UTF-8 to UTF-16
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
    std::u16string u16str = converter.from_bytes(utf8_str);
    data.assign(u16str.begin(), u16str.end());
}

WString::WString(const std::u16string& wide_str) {
    data.assign(wide_str.begin(), wide_str.end());
}

WString::WString(const char* c_str) : WString(std::string(c_str)) {}

WString::WString(const char16_t* wide_c_str) {
    size_t len = std::char_traits<char16_t>::length(wide_c_str);
    data.assign(wide_c_str, wide_c_str + len);
}

void WString::push_str(const WString& other) {
    data.insert(data.end(), other.data.begin(), other.data.end());
}

void WString::reserve(size_t capacity) {
    data.reserve(capacity);
}

WString WString::from_utf8(const std::string& utf8) {
    return WString(utf8);
}

WString WString::from_utf8_bytes(const std::vector<uint8_t>& bytes) {
    std::string str(bytes.begin(), bytes.end());
    return WString(str);
}

WString WString::from_units(const std::vector<uint8_t>& units) {
    // Interpret byte units as UTF-16 code units
    std::vector<char16_t> chars;
    for (size_t i = 0; i < units.size(); i += 2) {
        if (i + 1 < units.size()) {
            char16_t unit = (static_cast<char16_t>(units[i + 1]) << 8) | units[i];
            chars.push_back(unit);
        }
    }
    WString result;
    result.data = chars;
    return result;
}

bool WString::operator==(const WString& other) const {
    return data == other.data;
}

// Implementation of AvmStringRepr
template<typename GCContext>
AvmStringRepr<GCContext>::AvmStringRepr(WString str, bool dependent) 
    : string_data(std::move(str)), is_dependent_flag(dependent), is_interned_flag(false) {}

template<typename GCContext>
AvmStringRepr<GCContext> AvmStringRepr<GCContext>::from_raw(WString str, bool dependent) {
    return AvmStringRepr(std::move(str), dependent);
}

template<typename GCContext>
AvmStringRepr<GCContext> AvmStringRepr<GCContext>::from_raw_static(const char16_t* str, bool dependent) {
    return AvmStringRepr(WString(str), dependent);
}

template<typename GCContext>
std::unique_ptr<AvmStringRepr<GCContext>> AvmStringRepr<GCContext>::new_dependent(
    std::shared_ptr<AvmStringRepr> parent, size_t start, size_t end) {
    // Create a dependent string that references part of the parent string
    WString substr;
    if (start < parent->string_data.size() && end <= parent->string_data.size() && start <= end) {
        substr.data.assign(parent->string_data.data.begin() + start, 
                          parent->string_data.data.begin() + end);
    }
    auto repr = std::make_unique<AvmStringRepr<GCContext>>(substr, true);
    return repr;
}

template<typename GCContext>
std::unique_ptr<AvmStringRepr<GCContext>> AvmStringRepr<GCContext>::try_append_inline(
    std::shared_ptr<AvmStringRepr> left, const std::shared_ptr<AvmStringRepr>& right) {
    // For simplicity, we'll return nullptr indicating inline append is not possible
    // In a real implementation, this would try to append in-place if possible
    return nullptr;
}

// Implementation of AvmString
template<typename GCContext>
AvmString<GCContext>::AvmString(std::shared_ptr<AvmStringRepr<GCContext>> rep) : repr(rep) {}

template<typename GCContext>
AvmString<GCContext> AvmString<GCContext>::new_ascii_static(GCContext& gc_context, const std::vector<uint8_t>& bytes) {
    // Convert ASCII bytes to WString
    std::string ascii_str(bytes.begin(), bytes.end());
    WString wstr = WString::from_utf8(ascii_str);
    auto repr = std::make_shared<AvmStringRepr<GCContext>>(
        AvmStringRepr<GCContext>::from_raw_static(wstr.c_str(), false));
    return AvmString<GCContext>(repr);
}

template<typename GCContext>
AvmString<GCContext> AvmString<GCContext>::new_utf8(GCContext& gc_context, const std::string& utf8_string) {
    WString wstr = WString::from_utf8(utf8_string);
    auto repr = std::make_shared<AvmStringRepr<GCContext>>(
        AvmStringRepr<GCContext>::from_raw(std::move(wstr), false));
    return AvmString<GCContext>(repr);
}

template<typename GCContext>
AvmString<GCContext> AvmString<GCContext>::new_utf8_bytes(GCContext& gc_context, const std::vector<uint8_t>& bytes) {
    WString wstr = WString::from_utf8_bytes(bytes);
    auto repr = std::make_shared<AvmStringRepr<GCContext>>(
        AvmStringRepr<GCContext>::from_raw(std::move(wstr), false));
    return AvmString<GCContext>(repr);
}

template<typename GCContext>
AvmString<GCContext> AvmString<GCContext>::new_string(GCContext& gc_context, const WString& string) {
    auto repr = std::make_shared<AvmStringRepr<GCContext>>(
        AvmStringRepr<GCContext>::from_raw(string, false));
    return AvmString<GCContext>(repr);
}

template<typename GCContext>
AvmString<GCContext> AvmString<GCContext>::substring(GCContext& gc_context, 
                                                     const AvmString& string, size_t start, size_t end) {
    auto repr = AvmStringRepr<GCContext>::new_dependent(string.repr, start, end);
    return AvmString<GCContext>(std::shared_ptr<AvmStringRepr<GCContext>>(repr.release()));
}

template<typename GCContext>
const WString& AvmString<GCContext>::as_wstr() const {
    return repr->as_wstr();
}

template<typename GCContext>
bool AvmString<GCContext>::is_dependent() const {
    return repr->is_dependent();
}

template<typename GCContext>
bool AvmString<GCContext>::is_wide() const {
    for (auto ch : repr->as_wstr().c_str()) {
        if (ch > 0xFF) return true;
    }
    return false;
}

template<typename GCContext>
AvmString<GCContext> AvmString<GCContext>::concat(GCContext& mc, 
                                                  const AvmString& left, const AvmString& right) {
    if (left.is_empty()) {
        return right;
    } else if (right.is_empty()) {
        return left;
    } else {
        // Perform concatenation
        WString result = left.as_wstr();
        result.push_str(right.as_wstr());
        
        // Apply growth logic similar to the original
        size_t new_size = left.len() + right.len();
        size_t new_capacity;
        if (new_size < 32) {
            new_capacity = 32;
        } else if (new_size > 1024 * 1024) {
            new_capacity = new_size + 1024 * 1024;
        } else {
            new_capacity = new_size * 2;
        }
        
        result.reserve(new_capacity);
        auto repr = std::make_shared<AvmStringRepr<GCContext>>(
            AvmStringRepr<GCContext>::from_raw(std::move(result), false));
        return AvmString<GCContext>(repr);
    }
}

template<typename GCContext>
bool AvmString<GCContext>::ptr_eq(const AvmString& this_str, const AvmString& other_str) {
    return this_str.repr.get() == other_str.repr.get();
}

template<typename GCContext>
bool AvmString<GCContext>::operator==(const AvmString& other) const {
    // Fast path for identical objects
    if (this->repr.get() == other.repr.get()) {
        return true;
    }
    
    // Compare the actual string content
    return this->as_wstr() == other.as_wstr();
}

// Explicit template instantiation for common GC contexts
// In a real implementation, this would be done for the actual GC context types used