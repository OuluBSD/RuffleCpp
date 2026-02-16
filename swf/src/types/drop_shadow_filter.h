// C++ translation of swf/src/types/drop_shadow_filter.rs
// Drop shadow filter type for bitmap shadow effects

#ifndef RUFFLE_SWF_TYPES_DROP_SHADOW_FILTER_H
#define RUFFLE_SWF_TYPES_DROP_SHADOW_FILTER_H

#include <cstdint>
#include "color.h"

// Forward declarations
namespace ruffle {
namespace swf {
    template<typename T> class Rectangle;
    class Twips;
    class Fixed16;
    class Fixed8;
    struct BlurFilter;
    struct GlowFilter;
    enum class BlurFilterFlags : uint8_t;
    enum class GlowFilterFlags : uint8_t;
}
}

namespace ruffle {
namespace swf {
namespace types {

/// Bit flags for DropShadowFilter behavior
enum class DropShadowFilterFlags : uint8_t {
    None = 0,
    InnerShadow = 1 << 7,
    Knockout = 1 << 6,
    CompositeSource = 1 << 5,
    Passes = 0b11111  // Number of passes in bits 0-4
};

/// Allow bitwise operations on DropShadowFilterFlags
inline DropShadowFilterFlags operator|(DropShadowFilterFlags a, DropShadowFilterFlags b) {
    return static_cast<DropShadowFilterFlags>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

inline DropShadowFilterFlags operator&(DropShadowFilterFlags a, DropShadowFilterFlags b) {
    return static_cast<DropShadowFilterFlags>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}

inline bool operator&(DropShadowFilterFlags flags, DropShadowFilterFlags flag) {
    return (static_cast<uint8_t>(flags) & static_cast<uint8_t>(flag)) != 0;
}

/// A drop shadow filter for bitmap effects.
///
/// This filter applies a drop shadow effect to bitmap data,
/// creating a shadow offset from the original image.
struct DropShadowFilter {
    /// The color of the shadow
    Color color;

    /// Horizontal blur amount (16.16 fixed-point)
    Fixed16 blur_x;

    /// Vertical blur amount (16.16 fixed-point)
    Fixed16 blur_y;

    /// Angle of the shadow (16.16 fixed-point, in radians)
    Fixed16 angle;

    /// Distance of the shadow offset (16.16 fixed-point)
    Fixed16 distance;

    /// Strength of the shadow effect (8.8 fixed-point)
    Fixed8 strength;

    /// Flags controlling filter behavior
    DropShadowFilterFlags flags;

    /// Default constructor
    DropShadowFilter()
        : color()
        , blur_x(Fixed16::ZERO)
        , blur_y(Fixed16::ZERO)
        , angle(Fixed16::ZERO)
        , distance(Fixed16::ZERO)
        , strength(Fixed8::ZERO)
        , flags(DropShadowFilterFlags::None) {}

    /// Constructor with all parameters
    DropShadowFilter(
        Color color_,
        Fixed16 blur_x_,
        Fixed16 blur_y_,
        Fixed16 angle_,
        Fixed16 distance_,
        Fixed8 strength_,
        DropShadowFilterFlags flags_
    )
        : color(color_)
        , blur_x(blur_x_)
        , blur_y(blur_y_)
        , angle(angle_)
        , distance(distance_)
        , strength(strength_)
        , flags(flags_) {}

    /// Check if this is an inner shadow
    [[nodiscard]] inline bool is_inner() const {
        return flags & DropShadowFilterFlags::InnerShadow;
    }

    /// Check if this is a knockout shadow
    [[nodiscard]] inline bool is_knockout() const {
        return flags & DropShadowFilterFlags::Knockout;
    }

    /// Get the number of shadow passes (0-31)
    [[nodiscard]] inline uint8_t num_passes() const {
        return static_cast<uint8_t>(flags) & 0b11111;
    }

    /// Check if the object should be hidden (only shadow visible)
    [[nodiscard]] inline bool hide_object() const {
        return !(flags & DropShadowFilterFlags::CompositeSource);
    }

    /// Scale the blur amounts and distance by the given factors
    ///
    /// \param x Horizontal scale factor
    /// \param y Vertical scale factor
    void scale(float x, float y);

    /// Get the inner blur filter representation
    [[nodiscard]] BlurFilter inner_blur_filter() const;

    /// Get the inner glow filter representation
    [[nodiscard]] GlowFilter inner_glow_filter() const;

    /// Calculate the destination rectangle after applying this drop shadow
    ///
    /// \param source_rect The source rectangle to calculate from
    /// \return The expanded rectangle accounting for shadow spread and offset
    [[nodiscard]] Rectangle<Twips> calculate_dest_rect(const Rectangle<Twips>& source_rect) const;

    /// Equality comparison
    bool operator==(const DropShadowFilter& other) const {
        return color == other.color &&
               blur_x == other.blur_x &&
               blur_y == other.blur_y &&
               angle == other.angle &&
               distance == other.distance &&
               strength == other.strength &&
               flags == other.flags;
    }

    /// Inequality comparison
    bool operator!=(const DropShadowFilter& other) const {
        return !(*this == other);
    }
};

/// DropShadowFilterFlags helper methods
namespace DropShadowFilterFlagsHelpers {
    /// Create DropShadowFilterFlags from number of passes
    inline DropShadowFilterFlags from_passes(uint8_t num_passes) {
        auto flags = static_cast<DropShadowFilterFlags>(num_passes);
        // Debug assertion would go here in debug builds
        return flags;
    }
}

} // namespace types

// Re-export at swf namespace level for convenience
using DropShadowFilter = types::DropShadowFilter;
using DropShadowFilterFlags = types::DropShadowFilterFlags;

} // namespace swf
} // namespace ruffle

#endif // RUFFLE_SWF_TYPES_DROP_SHADOW_FILTER_H
