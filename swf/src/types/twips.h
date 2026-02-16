// C++ translation of swf/src/types/twips.rs
// Twips type for SWF coordinates

#ifndef RUFFLE_SWF_TYPES_TWIPS_H
#define RUFFLE_SWF_TYPES_TWIPS_H

#include <cstdint>
#include <cmath>

namespace ruffle {
namespace swf {
namespace types {

/// A type-safe wrapper type documenting where "twips" are used
/// in the SWF format.
///
/// A twip is 1/20th of a pixel.
/// Most coordinates in an SWF file are represented in twips.
///
/// Use the `from_pixels` and `to_pixels` methods to convert to and from
/// pixel values.
///
/// Please be careful when using twips for calculations to avoid overflows.
/// As an example, since it takes 20 twips to get 1 pixel, 2,000 pixels are
/// 40,000 twips, or `4*10^4`. If you then have two such numbers,
/// multiplying them as part of calculations yields `16*10^8`, which is
/// relatively close to the upper limit of `i32` at about `2*10^9`.
class Twips {
private:
    int32_t twips;

public:
    /// There are 20 twips in a pixel.
    static constexpr int32_t TWIPS_PER_PIXEL = 20;

    /// Default constructor - creates ZERO twips
    constexpr Twips() : twips(0) {}

    /// Create from raw twips value
    explicit constexpr Twips(int32_t t) : twips(t) {}

    /// The `Twips` object with a value of `0`.
    static constexpr Twips zero() { return Twips(0); }

    /// The `Twips` object with a value of `1` pixel.
    static constexpr Twips one_px() { return Twips(TWIPS_PER_PIXEL); }

    /// The `Twips` object with a value of `0.5` pixels.
    static constexpr Twips half_px() { return Twips(TWIPS_PER_PIXEL / 2); }

    /// Returns the number of twips.
    constexpr int32_t get() const { return twips; }

    /// Converts the given number of `pixels` into twips.
    ///
    /// This may be a lossy conversion; any precision more than a twip (1/20 pixels) is truncated.
    static Twips from_pixels(double pixels) {
        return Twips(static_cast<int32_t>(pixels * TWIPS_PER_PIXEL));
    }

    /// Converts the given number of `pixels` into twips (integer version).
    static constexpr Twips from_pixels_i32(int32_t pixels) {
        return Twips(pixels * TWIPS_PER_PIXEL);
    }

    /// Converts this twips value into pixel units.
    ///
    /// This is a lossless operation.
    double to_pixels() const {
        return static_cast<double>(twips) / TWIPS_PER_PIXEL;
    }

    /// Truncates this twips to a pixel.
    Twips trunc_to_pixel() const {
        return Twips(twips / TWIPS_PER_PIXEL * TWIPS_PER_PIXEL);
    }

    /// Rounds this twips to the nearest pixel.
    /// Rounds half-way cases to the nearest even pixel.
    Twips round_to_pixel_ties_even() const {
        return from_pixels(std::round(to_pixels()));
    }

    // Arithmetic operators
    constexpr Twips operator+(const Twips& other) const {
        return Twips(twips + other.twips);
    }

    constexpr Twips& operator+=(const Twips& other) {
        twips += other.twips;
        return *this;
    }

    constexpr Twips operator-(const Twips& other) const {
        return Twips(twips - other.twips);
    }

    constexpr Twips& operator-=(const Twips& other) {
        twips -= other.twips;
        return *this;
    }

    constexpr Twips operator*(int32_t other) const {
        return Twips(twips * other);
    }

    constexpr Twips& operator*=(int32_t other) {
        twips *= other;
        return *this;
    }

    constexpr Twips operator/(int32_t other) const {
        return Twips(twips / other);
    }

    constexpr Twips& operator/=(int32_t other) {
        twips /= other;
        return *this;
    }

    constexpr Twips operator-() const {
        return Twips(-twips);
    }

    // Comparison operators
    constexpr bool operator==(const Twips& other) const {
        return twips == other.twips;
    }

    constexpr bool operator!=(const Twips& other) const {
        return twips != other.twips;
    }

    constexpr bool operator<(const Twips& other) const {
        return twips < other.twips;
    }

    constexpr bool operator<=(const Twips& other) const {
        return twips <= other.twips;
    }

    constexpr bool operator>(const Twips& other) const {
        return twips > other.twips;
    }

    constexpr bool operator>=(const Twips& other) const {
        return twips >= other.twips;
    }
};

} // namespace types
} // namespace swf
} // namespace ruffle

#endif // RUFFLE_SWF_TYPES_TWIPS_H
