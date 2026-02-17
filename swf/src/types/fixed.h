// C++ translation of swf/src/types/fixed.rs
//! Fixed-point types.
//!
//! Fixed8 is an 8.8 signed fixed-point number.
//! Fixed16 is a 16.16 signed fixed-point number.
//!
//! This is not meant to be a fully general fixed-point library, but instead focused on the needs of Ruffle/Flash.
//! No rounding adjustments are done. All calculations are truncated to match Flash's behavior.
//!
//! Use the From trait to convert losslessly from an integer to fixed-point.
//! Use from_f32/from_f64 methods to convert from float to fixed-point.
//! Extra precision will be truncated, and out-of-range values are saturated.

#ifndef RUFFLE_SWF_TYPES_FIXED_H
#define RUFFLE_SWF_TYPES_FIXED_H

#include <cstdint>
#include <type_traits>
#include <string>
#include <stdexcept>
#include <cmath>

namespace ruffle {
namespace swf {
namespace types {
namespace fixed {

/// A signed 8.8 fixed-point value (8 integer bits, 8 fractional bits)
class Fixed8 {
public:
    /// The number of integer bits
    static constexpr uint8_t INTEGER_BITS = 8;
    
    /// The number of fractional bits
    static constexpr uint8_t FRACTIONAL_BITS = 8;
    
    /// The fixed-point value representing 0.0
    static constexpr Fixed8 ZERO = Fixed8(0);
    
    /// The fixed-point value representing 1.0
    static constexpr Fixed8 ONE = Fixed8(1 << FRACTIONAL_BITS);
    
    /// The minimum representable value of this type
    static constexpr Fixed8 MIN = Fixed8(INT16_MIN);
    
    /// The maximum representable value of this type
    static constexpr Fixed8 MAX = Fixed8(INT16_MAX);
    
    /// Default constructor (returns 0.0)
    constexpr Fixed8() : value_(0) {}
    
    /// Returns the fixed-point value with the same bit-representation as the given value
    static constexpr Fixed8 from_bits(int16_t n) {
        return Fixed8(n);
    }
    
    /// Get the underlying bit representation
    constexpr int16_t get() const { return value_; }
    
    /// Converts an f32 floating-point value to fixed point
    /// Extra precision will be truncated, and the result will be saturated if it doesn't fit
    static Fixed8 from_f32(float n) {
        if (std::isnan(n)) return ZERO;
        int32_t result = static_cast<int32_t>(n * (1 << FRACTIONAL_BITS));
        return Fixed8(static_cast<int16_t>(result));
    }
    
    /// Converts an f64 floating-point value to fixed point
    /// Extra precision will be truncated, and the result will be saturated if it doesn't fit
    static Fixed8 from_f64(double n) {
        if (std::isnan(n)) return ZERO;
        int64_t result = static_cast<int64_t>(n * (1LL << FRACTIONAL_BITS));
        return Fixed8(static_cast<int16_t>(result));
    }
    
    /// Converts this fixed-point value to f32 floating-point
    float to_f32() const {
        return static_cast<float>(value_) / (1 << FRACTIONAL_BITS);
    }
    
    /// Converts this fixed-point value to f64 floating-point
    double to_f64() const {
        return static_cast<double>(value_) / (1 << FRACTIONAL_BITS);
    }
    
    /// Returns true if this is equal to 0.0
    constexpr bool is_zero() const { return value_ == 0; }
    
    /// Returns true if this is equal to 1.0
    constexpr bool is_one() const { return value_ == (1 << FRACTIONAL_BITS); }
    
    /// Multiplies this fixed-point by an integer, returning the integer result
    int16_t mul_int(int16_t other) const {
        int32_t n = (static_cast<int32_t>(value_) * static_cast<int32_t>(other)) >> FRACTIONAL_BITS;
        return static_cast<int16_t>(n);
    }
    
    /// Wrapping (modular) negation
    constexpr Fixed8 wrapping_neg() const {
        return Fixed8(static_cast<int16_t>(-value_));
    }
    
    /// Wrapping (modular) addition
    constexpr Fixed8 wrapping_add(Fixed8 other) const {
        return Fixed8(static_cast<int16_t>(value_ + other.value_));
    }
    
    /// Wrapping (modular) subtraction
    constexpr Fixed8 wrapping_sub(Fixed8 other) const {
        return Fixed8(static_cast<int16_t>(value_ - other.value_));
    }
    
    /// Wrapping (modular) multiplication
    constexpr Fixed8 wrapping_mul(Fixed8 other) const {
        int32_t n = (static_cast<int32_t>(value_) * static_cast<int32_t>(other.value_)) >> FRACTIONAL_BITS;
        return Fixed8(static_cast<int16_t>(n));
    }
    
    /// Wrapping (modular) division
    constexpr Fixed8 wrapping_div(Fixed8 other) const {
        int32_t n = (static_cast<int32_t>(value_) << FRACTIONAL_BITS) / static_cast<int32_t>(other.value_);
        return Fixed8(static_cast<int16_t>(n));
    }
    
    /// Wrapping multiplication by integer
    constexpr int16_t wrapping_mul_int(int16_t other) const {
        int32_t n = (static_cast<int32_t>(value_) * static_cast<int32_t>(other)) >> FRACTIONAL_BITS;
        return static_cast<int16_t>(n);
    }
    
    // Operators
    constexpr Fixed8 operator-() const {
        return Fixed8(static_cast<int16_t>(-value_));
    }
    
    constexpr Fixed8 operator+(Fixed8 other) const {
        return Fixed8(static_cast<int16_t>(value_ + other.value_));
    }
    
    constexpr Fixed8& operator+=(Fixed8 other) {
        value_ += other.value_;
        return *this;
    }
    
    constexpr Fixed8 operator-(Fixed8 other) const {
        return Fixed8(static_cast<int16_t>(value_ - other.value_));
    }
    
    constexpr Fixed8& operator-=(Fixed8 other) {
        value_ -= other.value_;
        return *this;
    }
    
    constexpr Fixed8 operator*(Fixed8 other) const {
        int32_t n = (static_cast<int32_t>(value_) * static_cast<int32_t>(other.value_)) >> FRACTIONAL_BITS;
        return Fixed8(static_cast<int16_t>(n));
    }
    
    constexpr Fixed8& operator*=(Fixed8 other) {
        *this = *this * other;
        return *this;
    }
    
    constexpr Fixed8 operator*(int16_t other) const {
        return Fixed8(static_cast<int16_t>(value_ * other));
    }
    
    constexpr Fixed8& operator*=(int16_t other) {
        value_ *= other;
        return *this;
    }
    
    constexpr Fixed8 operator/(Fixed8 other) const {
        int32_t n = (static_cast<int32_t>(value_) << FRACTIONAL_BITS) / static_cast<int32_t>(other.value_);
        return Fixed8(static_cast<int16_t>(n));
    }
    
    constexpr Fixed8& operator/=(Fixed8 other) {
        *this = *this / other;
        return *this;
    }
    
    constexpr Fixed8 operator/(int16_t other) const {
        return Fixed8(static_cast<int16_t>(value_ / other));
    }
    
    constexpr Fixed8& operator/=(int16_t other) {
        value_ /= other;
        return *this;
    }
    
    constexpr bool operator==(Fixed8 other) const { return value_ == other.value_; }
    constexpr bool operator!=(Fixed8 other) const { return value_ != other.value_; }
    constexpr bool operator<(Fixed8 other) const { return value_ < other.value_; }
    constexpr bool operator<=(Fixed8 other) const { return value_ <= other.value_; }
    constexpr bool operator>(Fixed8 other) const { return value_ > other.value_; }
    constexpr bool operator>=(Fixed8 other) const { return value_ >= other.value_; }
    
    /// Convert to string
    std::string to_string() const;
    
private:
    explicit constexpr Fixed8(int16_t value) : value_(value) {}
    
    int16_t value_;
};

/// A signed 16.16 fixed-point value (16 integer bits, 16 fractional bits)
class Fixed16 {
public:
    /// The number of integer bits
    static constexpr uint8_t INTEGER_BITS = 16;
    
    /// The number of fractional bits
    static constexpr uint8_t FRACTIONAL_BITS = 16;
    
    /// The fixed-point value representing 0.0
    static constexpr Fixed16 ZERO = Fixed16(0);
    
    /// The fixed-point value representing 1.0
    static constexpr Fixed16 ONE = Fixed16(1 << FRACTIONAL_BITS);
    
    /// The minimum representable value of this type
    static constexpr Fixed16 MIN = Fixed16(INT32_MIN);
    
    /// The maximum representable value of this type
    static constexpr Fixed16 MAX = Fixed16(INT32_MAX);
    
    /// Default constructor (returns 0.0)
    constexpr Fixed16() : value_(0) {}
    
    /// Returns the fixed-point value with the same bit-representation as the given value
    static constexpr Fixed16 from_bits(int32_t n) {
        return Fixed16(n);
    }
    
    /// Get the underlying bit representation
    constexpr int32_t get() const { return value_; }
    
    /// Converts an f32 floating-point value to fixed point
    static Fixed16 from_f32(float n) {
        if (std::isnan(n)) return ZERO;
        int64_t result = static_cast<int64_t>(n * (1LL << FRACTIONAL_BITS));
        return Fixed16(static_cast<int32_t>(result));
    }
    
    /// Converts an f64 floating-point value to fixed point
    static Fixed16 from_f64(double n) {
        if (std::isnan(n)) return ZERO;
        int64_t result = static_cast<int64_t>(n * (1LL << FRACTIONAL_BITS));
        return Fixed16(static_cast<int32_t>(result));
    }
    
    /// Converts this fixed-point value to f64 floating-point
    double to_f64() const {
        return static_cast<double>(value_) / (1 << FRACTIONAL_BITS);
    }
    
    /// Returns true if this is equal to 0.0
    constexpr bool is_zero() const { return value_ == 0; }
    
    /// Returns true if this is equal to 1.0
    constexpr bool is_one() const { return value_ == (1 << FRACTIONAL_BITS); }
    
    /// Multiplies this fixed-point by an integer, returning the integer result
    int32_t mul_int(int32_t other) const {
        int64_t n = (static_cast<int64_t>(value_) * static_cast<int64_t>(other)) >> FRACTIONAL_BITS;
        return static_cast<int32_t>(n);
    }
    
    /// Wrapping (modular) negation
    constexpr Fixed16 wrapping_neg() const {
        return Fixed16(-value_);
    }
    
    /// Wrapping (modular) addition
    constexpr Fixed16 wrapping_add(Fixed16 other) const {
        return Fixed16(value_ + other.value_);
    }
    
    /// Wrapping (modular) subtraction
    constexpr Fixed16 wrapping_sub(Fixed16 other) const {
        return Fixed16(value_ - other.value_);
    }
    
    /// Wrapping (modular) multiplication
    constexpr Fixed16 wrapping_mul(Fixed16 other) const {
        int64_t n = (static_cast<int64_t>(value_) * static_cast<int64_t>(other.value_)) >> FRACTIONAL_BITS;
        return Fixed16(static_cast<int32_t>(n));
    }
    
    /// Wrapping (modular) division
    constexpr Fixed16 wrapping_div(Fixed16 other) const {
        int64_t n = (static_cast<int64_t>(value_) << FRACTIONAL_BITS) / static_cast<int64_t>(other.value_);
        return Fixed16(static_cast<int32_t>(n));
    }
    
    /// Wrapping multiplication by integer
    constexpr int32_t wrapping_mul_int(int32_t other) const {
        int64_t n = (static_cast<int64_t>(value_) * static_cast<int64_t>(other)) >> FRACTIONAL_BITS;
        return static_cast<int32_t>(n);
    }
    
    // Operators
    constexpr Fixed16 operator-() const {
        return Fixed16(-value_);
    }
    
    constexpr Fixed16 operator+(Fixed16 other) const {
        return Fixed16(value_ + other.value_);
    }
    
    constexpr Fixed16& operator+=(Fixed16 other) {
        value_ += other.value_;
        return *this;
    }
    
    constexpr Fixed16 operator-(Fixed16 other) const {
        return Fixed16(value_ - other.value_);
    }
    
    constexpr Fixed16& operator-=(Fixed16 other) {
        value_ -= other.value_;
        return *this;
    }
    
    constexpr Fixed16 operator*(Fixed16 other) const {
        int64_t n = (static_cast<int64_t>(value_) * static_cast<int64_t>(other.value_)) >> FRACTIONAL_BITS;
        return Fixed16(static_cast<int32_t>(n));
    }
    
    constexpr Fixed16& operator*=(Fixed16 other) {
        *this = *this * other;
        return *this;
    }
    
    constexpr Fixed16 operator*(int32_t other) const {
        return Fixed16(value_ * other);
    }
    
    constexpr Fixed16& operator*=(int32_t other) {
        value_ *= other;
        return *this;
    }
    
    constexpr Fixed16 operator/(Fixed16 other) const {
        int64_t n = (static_cast<int64_t>(value_) << FRACTIONAL_BITS) / static_cast<int64_t>(other.value_);
        return Fixed16(static_cast<int32_t>(n));
    }
    
    constexpr Fixed16& operator/=(Fixed16 other) {
        *this = *this / other;
        return *this;
    }
    
    constexpr Fixed16 operator/(int32_t other) const {
        return Fixed16(value_ / other);
    }
    
    constexpr Fixed16& operator/=(int32_t other) {
        value_ /= other;
        return *this;
    }
    
    constexpr bool operator==(Fixed16 other) const { return value_ == other.value_; }
    constexpr bool operator!=(Fixed16 other) const { return value_ != other.value_; }
    constexpr bool operator<(Fixed16 other) const { return value_ < other.value_; }
    constexpr bool operator<=(Fixed16 other) const { return value_ <= other.value_; }
    constexpr bool operator>(Fixed16 other) const { return value_ > other.value_; }
    constexpr bool operator>=(Fixed16 other) const { return value_ >= other.value_; }
    
    /// Convert to string
    std::string to_string() const;
    
private:
    explicit constexpr Fixed16(int32_t value) : value_(value) {}
    
    int32_t value_;
};

// Conversion from smaller integers to Fixed8
inline Fixed8 operator*(int8_t lhs, Fixed8 rhs) { return rhs * lhs; }

// Conversion from smaller integers to Fixed16
inline Fixed16 operator*(int8_t lhs, Fixed16 rhs) { return rhs * lhs; }
inline Fixed16 operator*(int16_t lhs, Fixed16 rhs) { return rhs * lhs; }

} // namespace fixed

// Re-export for convenience
using fixed::Fixed8;
using fixed::Fixed16;

} // namespace types
} // namespace swf
} // namespace ruffle

#endif // RUFFLE_SWF_TYPES_FIXED_H
