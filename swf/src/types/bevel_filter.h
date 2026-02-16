// C++ translation of swf/src/types/bevel_filter.rs
// Bevel filter type for bitmap bevel effects

#ifndef RUFFLE_SWF_TYPES_BEVEL_FILTER_H
#define RUFFLE_SWF_TYPES_BEVEL_FILTER_H

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
    enum class BlurFilterFlags : uint8_t;
}
}

namespace ruffle {
namespace swf {
namespace types {

/// Bit flags for BevelFilter behavior
enum class BevelFilterFlags : uint8_t {
    None = 0,
    InnerShadow = 1 << 7,
    Knockout = 1 << 6,
    CompositeSource = 1 << 5,
    OnTop = 1 << 4,
    Passes = 0b1111  // Number of passes in bits 0-3
};

/// Allow bitwise operations on BevelFilterFlags
inline BevelFilterFlags operator|(BevelFilterFlags a, BevelFilterFlags b) {
    return static_cast<BevelFilterFlags>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

inline BevelFilterFlags operator&(BevelFilterFlags a, BevelFilterFlags b) {
    return static_cast<BevelFilterFlags>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}

inline bool operator&(BevelFilterFlags flags, BevelFilterFlags flag) {
    return (static_cast<uint8_t>(flags) & static_cast<uint8_t>(flag)) != 0;
}

/// A bevel filter for bitmap effects.
///
/// This filter applies a bevel effect to bitmap data,
/// creating a 3D-like appearance with shadow and highlight colors.
struct BevelFilter {
    /// The color of the shadow
    Color shadow_color;

    /// The color of the highlight
    Color highlight_color;

    /// Horizontal blur amount (16.16 fixed-point)
    Fixed16 blur_x;

    /// Vertical blur amount (16.16 fixed-point)
    Fixed16 blur_y;

    /// Angle of the bevel effect (16.16 fixed-point, in radians)
    Fixed16 angle;

    /// Distance of the bevel effect (16.16 fixed-point)
    Fixed16 distance;

    /// Strength of the bevel effect (8.8 fixed-point)
    Fixed8 strength;

    /// Flags controlling filter behavior
    BevelFilterFlags flags;

    /// Default constructor
    BevelFilter()
        : shadow_color()
        , highlight_color()
        , blur_x(Fixed16::ZERO)
        , blur_y(Fixed16::ZERO)
        , angle(Fixed16::ZERO)
        , distance(Fixed16::ZERO)
        , strength(Fixed8::ZERO)
        , flags(BevelFilterFlags::None) {}

    /// Constructor with all parameters
    BevelFilter(
        Color shadow_color_,
        Color highlight_color_,
        Fixed16 blur_x_,
        Fixed16 blur_y_,
        Fixed16 angle_,
        Fixed16 distance_,
        Fixed8 strength_,
        BevelFilterFlags flags_
    )
        : shadow_color(shadow_color_)
        , highlight_color(highlight_color_)
        , blur_x(blur_x_)
        , blur_y(blur_y_)
        , angle(angle_)
        , distance(distance_)
        , strength(strength_)
        , flags(flags_) {}

    /// Check if this is an inner bevel
    [[nodiscard]] inline bool is_inner() const {
        return flags & BevelFilterFlags::InnerShadow;
    }

    /// Check if this is a knockout bevel
    [[nodiscard]] inline bool is_knockout() const {
        return flags & BevelFilterFlags::Knockout;
    }

    /// Check if the bevel is on top
    [[nodiscard]] inline bool is_on_top() const {
        return flags & BevelFilterFlags::OnTop;
    }

    /// Get the number of bevel passes (0-15)
    [[nodiscard]] inline uint8_t num_passes() const {
        return static_cast<uint8_t>(flags) & 0b1111;
    }

    /// Scale the blur amounts and distance by the given factors
    ///
    /// \param x Horizontal scale factor
    /// \param y Vertical scale factor
    void scale(float x, float y);

    /// Get the inner blur filter representation
    [[nodiscard]] BlurFilter inner_blur_filter() const;

    /// Calculate the destination rectangle after applying this bevel
    ///
    /// \param source_rect The source rectangle to calculate from
    /// \return The expanded rectangle accounting for bevel spread and offset
    [[nodiscard]] Rectangle<Twips> calculate_dest_rect(const Rectangle<Twips>& source_rect) const;

    /// Equality comparison
    bool operator==(const BevelFilter& other) const {
        return shadow_color == other.shadow_color &&
               highlight_color == other.highlight_color &&
               blur_x == other.blur_x &&
               blur_y == other.blur_y &&
               angle == other.angle &&
               distance == other.distance &&
               strength == other.strength &&
               flags == other.flags;
    }

    /// Inequality comparison
    bool operator!=(const BevelFilter& other) const {
        return !(*this == other);
    }
};

/// BevelFilterFlags helper methods
namespace BevelFilterFlagsHelpers {
    /// Create BevelFilterFlags from number of passes
    inline BevelFilterFlags from_passes(uint8_t num_passes) {
        auto flags = static_cast<BevelFilterFlags>(num_passes);
        // Debug assertion would go here in debug builds
        return flags;
    }
}

} // namespace types

// Re-export at swf namespace level for convenience
using BevelFilter = types::BevelFilter;
using BevelFilterFlags = types::BevelFilterFlags;

} // namespace swf
} // namespace ruffle

#endif // RUFFLE_SWF_TYPES_BEVEL_FILTER_H
