// C++ translation of core/common/src/avm_string/avm_string.rs
// AVM String representation

#ifndef RUFFLE_CORE_COMMON_AVM_STRING_AVM_STRING_H
#define RUFFLE_CORE_COMMON_AVM_STRING_AVM_STRING_H

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

// Forward declarations
namespace ruffle {
namespace common {
namespace wstr {
    class WStr;
    class WString;
}
}
}

namespace ruffle {
namespace core {
namespace avm_string {

// Forward declarations
template<typename GC>
class AvmStringRepr;

template<typename GC>
class AvmAtom;

/// AVM String - garbage collected string type for AVM
template<typename GC>
class AvmString {
private:
    std::shared_ptr<AvmStringRepr<GC>> repr;

    /// Turns a string to a fully owned (non-dependent) managed string
    std::shared_ptr<AvmStringRepr<GC>> to_fully_owned(GC* mc) const;

public:
    /// Create from AvmAtom
    explicit AvmString(AvmAtom<GC> atom);

    /// Create from AvmStringRepr
    explicit AvmString(std::shared_ptr<AvmStringRepr<GC>> repr);

    /// Create an ASCII string from static bytes
    /// @param gc_context The GC mutation context
    /// @param bytes The ASCII byte array
    static AvmString new_ascii_static(GC* gc_context, const std::vector<uint8_t>& bytes);

    /// Create a UTF-8 string from a string or string view
    /// @param gc_context The GC mutation context
    /// @param string The UTF-8 string
    static AvmString new_utf8(GC* gc_context, const std::string& string);

    /// Create a string from UTF-8 bytes
    /// @param gc_context The GC mutation context
    /// @param bytes The UTF-8 byte array
    static AvmString new_utf8_bytes(GC* gc_context, const std::vector<uint8_t>& bytes);

    /// Create a string from a WString
    /// @param gc_context The GC mutation context
    /// @param string The WString
    static AvmString new(GC* gc_context, const ruffle::common::wstr::WString& string);

    /// Create a substring
    /// @param mc The GC mutation context
    /// @param string The source string
    /// @param start Start index
    /// @param end End index
    static AvmString substring(GC* mc, AvmString<GC> string, size_t start, size_t end);

    /// Check if this string is dependent (shares data with another string)
    bool is_dependent() const;

    /// Get the underlying WStr
    const ruffle::common::wstr::WStr* as_wstr() const;

    /// Get as interned atom if interned
    std::optional<AvmAtom<GC>> as_interned() const;

    /// Concatenate two strings
    /// @param mc The GC mutation context
    /// @param left Left operand
    /// @param right Right operand
    /// @return Concatenated string
    static AvmString concat(GC* mc, AvmString<GC> left, AvmString<GC> right);

    /// Check if two strings point to the same data
    /// @param this First string
    /// @param other Second string
    /// @return True if pointers are equal
    static bool ptr_eq(const AvmString<GC>& first, const AvmString<GC>& second);

    /// Check if string is empty
    bool is_empty() const;

    /// Get string length
    size_t len() const;

    /// Check if string is wide (UTF-16)
    bool is_wide() const;

    /// Equality comparison
    bool operator==(const AvmString<GC>& other) const;
    bool operator!=(const AvmString<GC>& other) const { return !(*this == other); }
};

} // namespace avm_string
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_COMMON_AVM_STRING_AVM_STRING_H
