// C++ translation of wstr/src/ptr.rs
// Raw pointers to WStr slices

#ifndef RUFFLE_WSTR_PTR_H
#define RUFFLE_WSTR_PTR_H

#include "common.h"
#include <cstdint>
#include <cstddef>

namespace ruffle {
namespace wstr {

#if !defined(__SIZEOF_POINTER__) || (__SIZEOF_POINTER__ != 4 && __SIZEOF_POINTER__ != 8)
#error "WStr only supports 32-bits and 64-bits targets"
#endif

/// Mask for wide string indicator in metadata
constexpr uint32_t WIDE_MASK = 0x80000000;

/// The metadata of a WStr pointer. This is always 4 bytes wide, even on 64-bits targets.
///
/// The layout of WStr depends on the value of is_wide():
///  - if false, it has the layout of uint8_t[len]
///  - if true, it has the layout of uint16_t[len]
class WStrMetadata {
private:
    uint32_t value;

public:
    /// Assemble WStr metadata from its components.
    /// 
    /// # Safety
    /// `len` must be less than or equal to WStr::MAX_LEN.
    static constexpr WStrMetadata new_unsafe(size_t len, bool is_wide) {
        uint32_t raw = static_cast<uint32_t>(len) | (is_wide ? WIDE_MASK : 0);
        return WStrMetadata(raw);
    }
    
    /// Assemble WStr metadata from its components (safe version).
    /// Unlike new_unsafe, this is safe, but passing a len bigger
    /// than WStr::MAX_LEN will give a bogus result.
    static constexpr WStrMetadata new32(uint32_t len, bool is_wide) {
        return WStrMetadata(len | (is_wide ? WIDE_MASK : 0));
    }
    
    /// Returns whether this metadata describes a wide WStr.
    constexpr bool is_wide() const {
        return (value & WIDE_MASK) != 0;
    }
    
    /// Returns the length of the described WStr. This is never greater than WStr::MAX_LEN.
    constexpr size_t len() const {
        return value & (WIDE_MASK - 1);
    }
    
    /// Same as len(), but returns a uint32_t.
    constexpr uint32_t len32() const {
        return value & (WIDE_MASK - 1);
    }
    
    constexpr bool operator==(const WStrMetadata& other) const = default;
    constexpr bool operator!=(const WStrMetadata& other) const = default;
    constexpr bool operator<(const WStrMetadata& other) const = default;
    constexpr bool operator<=(const WStrMetadata& other) const = default;
    constexpr bool operator>(const WStrMetadata& other) const = default;
    constexpr bool operator>=(const WStrMetadata& other) const = default;

private:
    explicit constexpr WStrMetadata(uint32_t v) : value(v) {}
};

/// Creates a WStr pointer from its raw parts.
inline const WStr* from_raw_parts(const void* data, WStrMetadata metadata) {
    // This is a simplified version - actual implementation would need
    // to handle the fat pointer metadata properly
    return reinterpret_cast<const WStr*>(data);
}

/// Creates a mutable WStr pointer from its raw parts.
inline WStr* from_raw_parts_mut(void* data, WStrMetadata metadata) {
    return reinterpret_cast<WStr*>(data);
}

/// Creates a WStr pointer from a raw units buffer.
///
/// # Safety
///  - the buffer length must be less than or equal to WStr::MAX_LEN.
template<typename T>
inline const WStr* from_units(const T* units, size_t len, bool is_wide) {
    return reinterpret_cast<const WStr*>(units);
}

/// Creates a WStr pointer from a mutable, raw units buffer.
///
/// # Safety
///  - the buffer length must be less than or equal to WStr::MAX_LEN.
template<typename T>
inline WStr* from_units_mut(T* units, size_t len, bool is_wide) {
    return reinterpret_cast<WStr*>(units);
}

/// Gets a pointer to the buffer designated by ptr.
///
/// # Safety
/// ptr must have valid WStr metadata.
inline Units<const uint8_t*, const uint16_t*> units(const WStr* ptr) {
    // This would need proper metadata handling
    // Simplified for now
    return Units<const uint8_t*, const uint16_t*>::Bytes(reinterpret_cast<const uint8_t*>(ptr));
}

/// Gets a mutable pointer to the buffer designated by ptr.
///
/// # Safety
/// ptr must have valid WStr metadata.
inline Units<uint8_t*, uint16_t*> units_mut(WStr* ptr) {
    return Units<uint8_t*, uint16_t*>::Bytes(reinterpret_cast<uint8_t*>(ptr));
}

/// Gets a pointer to the nth unit of this WStr.
///
/// # Safety
///  - ptr must point to a valid WStr
///  - i must be less than or equal to metadata(ptr).len()
inline Units<const uint8_t*, const uint16_t*> offset(const WStr* ptr, size_t i) {
    // Simplified - would need proper metadata handling
    return Units<const uint8_t*, const uint16_t*>::Bytes(
        reinterpret_cast<const uint8_t*>(ptr) + i
    );
}

/// Gets a mutable pointer to the nth unit of this WStr.
///
/// # Safety
///  - ptr must point to a valid WStr
///  - i must be less than or equal to metadata(ptr).len()
inline Units<uint8_t*, uint16_t*> offset_mut(WStr* ptr, size_t i) {
    return Units<uint8_t*, uint16_t*>::Bytes(
        reinterpret_cast<uint8_t*>(ptr) + i
    );
}

/// Dereferences the nth unit of this WStr.
///
/// # Safety
///  - ptr must point to a valid WStr for reading
///  - i must be less than metadata(ptr).len()
inline uint16_t read_at(const WStr* ptr, size_t i) {
    // Simplified - would need proper metadata handling
    return static_cast<uint16_t>(reinterpret_cast<const uint8_t*>(ptr)[i]);
}

/// Returns a pointer to a subslice of this WStr.
///
/// # Safety
///  - ptr must point to a valid WStr
///  - range.start must be less than or equal to range.end
///  - range.end must be less than or equal to metadata(ptr).len()
inline const WStr* slice(const WStr* ptr, size_t start, size_t end) {
    // Simplified - would need proper metadata handling
    return reinterpret_cast<const WStr*>(
        reinterpret_cast<const uint8_t*>(ptr) + start
    );
}

/// Returns a mutable pointer to a subslice of this WStr.
///
/// # Safety
///  - ptr must point to a valid WStr
///  - range.start must be less than or equal to range.end
///  - range.end must be less than or equal to metadata(ptr).len()
inline WStr* slice_mut(WStr* ptr, size_t start, size_t end) {
    return reinterpret_cast<WStr*>(
        reinterpret_cast<uint8_t*>(ptr) + start
    );
}

} // namespace wstr
} // namespace ruffle

#endif // RUFFLE_WSTR_PTR_H
