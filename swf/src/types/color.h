// C++ translation of swf/src/types/color.rs
// RGBA color type

#ifndef RUFFLE_SWF_TYPES_COLOR_H
#define RUFFLE_SWF_TYPES_COLOR_H

#include <cstdint>
#include <string>

namespace ruffle {
namespace swf {
namespace types {

/// An RGBA (red, green, blue, alpha) color.
///
/// All components are stored as uint8_t and have a color range of 0-255.
struct Color {
    /// The red component value.
    uint8_t r;

    /// The green component value.
    uint8_t g;

    /// The blue component value.
    uint8_t b;

    /// The alpha component value.
    uint8_t a;

    /// Default constructor - transparent black
    constexpr Color() : r(0), g(0), b(0), a(0) {}

    /// Constructor from individual components
    constexpr Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
        : r(red), g(green), b(blue), a(alpha) {}

    /// Creates a Color from a 32-bit rgb value and an alpha value.
    ///
    /// The byte-ordering of the 32-bit rgb value is XXRRGGBB.
    /// The most significant byte, represented by XX, is ignored;
    /// the alpha value is provided separately.
    /// This is followed by the red (RR), green (GG), and blue (BB) components values,
    /// respectively.
    static constexpr Color from_rgb(uint32_t rgb, uint8_t alpha) {
        uint8_t b_val = (rgb >> 0) & 0xFF;
        uint8_t g_val = (rgb >> 8) & 0xFF;
        uint8_t r_val = (rgb >> 16) & 0xFF;
        return Color(r_val, g_val, b_val, alpha);
    }

    /// Creates a Color from a 32-bit rgba value.
    ///
    /// The byte-ordering of the 32-bit rgba value is AARRGGBB.
    static constexpr Color from_rgba(uint32_t rgba) {
        uint8_t b_val = (rgba >> 0) & 0xFF;
        uint8_t g_val = (rgba >> 8) & 0xFF;
        uint8_t r_val = (rgba >> 16) & 0xFF;
        uint8_t a_val = (rgba >> 24) & 0xFF;
        return Color(r_val, g_val, b_val, a_val);
    }

    /// Converts the color to a 32-bit RGB value.
    ///
    /// The alpha value does not get stored.
    constexpr uint32_t to_rgb() const {
        return (static_cast<uint32_t>(r) << 16) |
               (static_cast<uint32_t>(g) << 8) |
               static_cast<uint32_t>(b);
    }

    /// Converts the color to a 32-bit RGBA value.
    constexpr uint32_t to_rgba() const {
        return (static_cast<uint32_t>(a) << 24) |
               (static_cast<uint32_t>(r) << 16) |
               (static_cast<uint32_t>(g) << 8) |
               static_cast<uint32_t>(b);
    }

    /// Predefined color constants
    static constexpr Color TRANSPARENT = Color(0, 0, 0, 0);
    static constexpr Color BLACK = Color(0, 0, 0, 255);
    static constexpr Color GRAY = Color(0x55, 0x55, 0x55, 255);
    static constexpr Color WHITE = Color(0xFF, 0xFF, 0xFF, 255);
    static constexpr Color RED = Color(0xFF, 0, 0, 255);
    static constexpr Color GREEN = Color(0, 0xFF, 0, 255);
    static constexpr Color BLUE = Color(0, 0, 0xFF, 255);
    static constexpr Color YELLOW = Color(0xFF, 0xFF, 0, 255);
    static constexpr Color CYAN = Color(0, 0xFF, 0xFF, 255);
    static constexpr Color MAGENTA = Color(0xFF, 0, 0xFF, 255);

    /// Equality comparison
    constexpr bool operator==(const Color& other) const {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }

    /// Inequality comparison
    constexpr bool operator!=(const Color& other) const {
        return !(*this == other);
    }

    /// Get a debug string representation
    std::string to_string() const;
};

} // namespace types

// Re-export Color at swf namespace level for convenience
using Color = types::Color;

} // namespace swf
} // namespace ruffle

#endif // RUFFLE_SWF_TYPES_COLOR_H
