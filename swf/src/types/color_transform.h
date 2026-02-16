// C++ translation of swf/src/types/color_transform.rs
// Color transform type for color manipulation

#ifndef RUFFLE_SWF_TYPES_COLOR_TRANSFORM_H
#define RUFFLE_SWF_TYPES_COLOR_TRANSFORM_H

#include <cstdint>
#include <array>
#include "color.h"

// Forward declarations
namespace ruffle {
namespace swf {
    class Fixed8;
}
}

namespace ruffle {
namespace swf {
namespace types {

/// A color transform that modifies color values through multiplication and addition.
///
/// This transform applies the formula: result = (input * multiply) + add
/// for each RGBA channel independently.
struct ColorTransform {
    /// Red channel multiply factor (8.8 fixed-point)
    Fixed8 r_multiply;

    /// Green channel multiply factor (8.8 fixed-point)
    Fixed8 g_multiply;

    /// Blue channel multiply factor (8.8 fixed-point)
    Fixed8 b_multiply;

    /// Alpha channel multiply factor (8.8 fixed-point)
    Fixed8 a_multiply;

    /// Red channel add offset (signed 16-bit integer)
    int16_t r_add;

    /// Green channel add offset (signed 16-bit integer)
    int16_t g_add;

    /// Blue channel add offset (signed 16-bit integer)
    int16_t b_add;

    /// Alpha channel add offset (signed 16-bit integer)
    int16_t a_add;

    /// Identity transform (no change to colors)
    static const ColorTransform IDENTITY;

    /// Default constructor - creates identity transform
    ColorTransform()
        : r_multiply(Fixed8::ONE)
        , g_multiply(Fixed8::ONE)
        , b_multiply(Fixed8::ONE)
        , a_multiply(Fixed8::ONE)
        , r_add(0)
        , g_add(0)
        , b_add(0)
        , a_add(0) {}

    /// Constructor with all parameters
    ColorTransform(
        Fixed8 r_mult, Fixed8 g_mult, Fixed8 b_mult, Fixed8 a_mult,
        int16_t r_a, int16_t g_a, int16_t b_a, int16_t a_a
    )
        : r_multiply(r_mult)
        , g_multiply(g_mult)
        , b_multiply(b_mult)
        , a_multiply(a_mult)
        , r_add(r_a)
        , g_add(g_a)
        , b_add(b_a)
        , a_add(a_a) {}

    /// Creates a ColorTransform from a Color (multiplicative only)
    ///
    /// \param color The color to create the transform from
    /// \return A ColorTransform that multiplies by the given color
    static ColorTransform multiply_from(Color color);

    /// Returns the multiplicative component of this color transform in RGBA order
    /// with the values normalized [0.0, 1.0].
    [[nodiscard]] std::array<float, 4> mult_rgba_normalized() const {
        return {
            static_cast<float>(r_multiply),
            static_cast<float>(g_multiply),
            static_cast<float>(b_multiply),
            static_cast<float>(a_multiply)
        };
    }

    /// Returns the additive component of this color transform in RGBA order
    /// with the values normalized [-1.0, 1.0].
    [[nodiscard]] std::array<float, 4> add_rgba_normalized() const {
        return {
            static_cast<float>(r_add) / 255.0f,
            static_cast<float>(g_add) / 255.0f,
            static_cast<float>(b_add) / 255.0f,
            static_cast<float>(a_add) / 255.0f
        };
    }

    /// Sets the multiplicative component of this color transform.
    ///
    /// \param color The color to set as the multiplicative component
    void set_mult_color(Color color);

    /// Multiply two color transforms together (composition)
    [[nodiscard]] ColorTransform operator*(const ColorTransform& rhs) const;

    /// Multiply-assign another color transform
    ColorTransform& operator*=(const ColorTransform& rhs);

    /// Apply this color transform to a color
    [[nodiscard]] Color apply(Color color) const;

    /// Equality comparison
    bool operator==(const ColorTransform& other) const {
        return r_multiply == other.r_multiply &&
               g_multiply == other.g_multiply &&
               b_multiply == other.b_multiply &&
               a_multiply == other.a_multiply &&
               r_add == other.r_add &&
               g_add == other.g_add &&
               b_add == other.b_add &&
               a_add == other.a_add;
    }

    /// Inequality comparison
    bool operator!=(const ColorTransform& other) const {
        return !(*this == other);
    }
};

// Static constant definition
inline const ColorTransform ColorTransform::IDENTITY = ColorTransform();

} // namespace types

// Re-export at swf namespace level for convenience
using ColorTransform = types::ColorTransform;

} // namespace swf
} // namespace ruffle

#endif // RUFFLE_SWF_TYPES_COLOR_TRANSFORM_H
