// C++ translation of core/common/src/avm_string/repr.rs
// Internal representation of AVM strings

#ifndef RUFFLE_CORE_COMMON_AVM_STRING_REPR_H
#define RUFFLE_CORE_COMMON_AVM_STRING_REPR_H

#include <cstdint>
#include <memory>
#include <optional>
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

template<typename GC>
class AvmStringRepr;

/// Metadata for WStr (similar to Rust implementation)
struct WStrMetadata {
    uint32_t value;

    static WStrMetadata new32(uint32_t len, bool is_wide) {
        return WStrMetadata{len | (is_wide ? (1u << 31) : 0)};
    }

    static WStrMetadata new(uint32_t len, bool is_wide) {
        return new32(len, is_wide);
    }

    uint32_t len32() const {
        return value & 0x7FFFFFFF;
    }

    bool is_wide() const {
        return (value & (1u << 31)) != 0;
    }
};

/// Internal representation of AvmAtoms and (owned) AvmStrings.
/// Using this type directly is dangerous, as it can be used to violate
/// the interning invariants.
template<typename GC>
class AvmStringRepr {
private:
    void* ptr;                    // Pointer to string data
    WStrMetadata meta;            // Length and is_wide bit
    mutable uint32_t capacity;    // Capacity and is_interned bit
    mutable uint32_t chars_used;  // Used characters including dependents
    std::optional<std::shared_ptr<AvmStringRepr<GC>>> owner;  // Owner if dependent

    /// Create a dependent string from raw parts
    /// @param owner The owner string (optional)
    /// @param wstr The WStr slice
    /// @param interned Whether the string is interned
    static AvmStringRepr new_dependent_raw(
        std::optional<std::shared_ptr<AvmStringRepr<GC>>> owner,
        const ruffle::common::wstr::WStr* wstr,
        bool interned
    );

public:
    /// Create from a WString
    /// @param s The WString
    /// @param interned Whether the string is interned
    static AvmStringRepr from_raw(ruffle::common::wstr::WString s, bool interned);

    /// Create from a static WStr
    /// @param s The static WStr
    /// @param interned Whether the string is interned
    static AvmStringRepr from_raw_static(const ruffle::common::wstr::WStr* s, bool interned);

    /// Create a dependent string (substring)
    /// @param s The owner string
    /// @param start Start index
    /// @param end End index
    static AvmStringRepr new_dependent(
        std::shared_ptr<AvmStringRepr<GC>> s,
        size_t start,
        size_t end
    );

    /// Try to append a string inline (in-place)
    /// @param left The left (owner) string
    /// @param right The string to append
    /// @return New representation if successful, nullopt otherwise
    static std::optional<AvmStringRepr> try_append_inline(
        std::shared_ptr<AvmStringRepr<GC>> left,
        const ruffle::common::wstr::WStr* right
    );

    /// Check if this string is dependent
    bool is_dependent() const {
        return owner.has_value();
    }

    /// Get the owner string if dependent
    std::optional<std::shared_ptr<AvmStringRepr<GC>>> owner() const {
        return owner;
    }

    /// Get the underlying WStr
    const ruffle::common::wstr::WStr* as_wstr() const;

    /// Check if the string is interned
    bool is_interned() const;

    /// Mark the string as interned
    void mark_interned();

    /// Get string length
    size_t len() const {
        return meta.len32();
    }

    /// Check if string is wide
    bool is_wide() const {
        return meta.is_wide();
    }

    /// Default constructor (empty string)
    AvmStringRepr() : ptr(nullptr), meta{0}, capacity(0), chars_used(0), owner(std::nullopt) {}

    /// Destructor - cleans up owned string data
    ~AvmStringRepr();
};

} // namespace avm_string
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_COMMON_AVM_STRING_REPR_H
