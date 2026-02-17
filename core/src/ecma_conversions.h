// C++ translation of core/src/ecma_conversions.rs
// ECMA-262 compliant numerical conversions

#ifndef RUFFLE_CORE_ECMA_CONVERSIONS_H
#define RUFFLE_CORE_ECMA_CONVERSIONS_H

#include <cstdint>
#include <cmath>
#include <limits>

namespace ruffle {
namespace core {

/// Converts a double to a uint8_t with ECMAScript ToUInt8 wrapping behavior.
/// The value will be wrapped modulo 2^8.
inline uint8_t f64_to_wrapping_u8(double n) {
    if (!std::isfinite(n)) {
        return 0;
    }
    return static_cast<uint8_t>(std::fmod(std::trunc(n), 256.0));
}

/// Converts a double to a uint16_t with ECMAScript ToUInt16 wrapping behavior.
/// The value will be wrapped modulo 2^16.
inline uint16_t f64_to_wrapping_u16(double n) {
    if (!std::isfinite(n)) {
        return 0;
    }
    return static_cast<uint16_t>(std::fmod(std::trunc(n), 65536.0));
}

/// Converts a double to an int16_t with ECMAScript wrapping behavior.
/// The value will be wrapped in the range [-2^15, 2^15).
inline int16_t f64_to_wrapping_i16(double n) {
    return static_cast<int16_t>(f64_to_wrapping_u16(n));
}

/// Converts a double to a uint32_t with ECMAScript ToUInt32 wrapping behavior.
/// The value will be wrapped modulo 2^32.
inline uint32_t f64_to_wrapping_u32(double n) {
    if (!std::isfinite(n)) {
        return 0;
    }
    return static_cast<uint32_t>(std::fmod(std::trunc(n), 4294967296.0));
}

/// Converts a double to an int32_t with ECMAScript ToInt32 wrapping behavior.
/// The value will be wrapped in the range [-2^31, 2^31).
inline int32_t f64_to_wrapping_i32(double n) {
    return static_cast<int32_t>(f64_to_wrapping_u32(n));
}

/// Implements the IEEE-754 "Round to nearest, ties to even" rounding rule.
/// (e.g., both 1.5 and 2.5 will round to 2).
/// This also clamps out-of-range values and NaN to int32_t::MIN.
inline int32_t round_to_even(double n) {
    double out = std::rint(n);  // rint uses round-to-nearest, ties-to-even
    
    // Clamp out-of-range values to int32_t::MIN
    if (std::isfinite(out) && out <= std::numeric_limits<int32_t>::max()) {
        return static_cast<int32_t>(out);
    }
    return std::numeric_limits<int32_t>::min();
}

} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_ECMA_CONVERSIONS_H
