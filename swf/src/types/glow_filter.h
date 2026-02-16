// C++ translation of swf/src/types/glow_filter.rs
// Glow filter type for bitmap glow effects

#ifndef RUFFLE_SWF_TYPES_GLOW_FILTER_H
#define RUFFLE_SWF_TYPES_GLOW_FILTER_H

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

/// Bit flags for GlowFilter behavior
enum class GlowFilterFlags : uint8_t {
    None = 0,
    InnerGlow = 1 << 7,
    Knockout = 1 << 6,
    CompositeSource = 1 << 5,
    Passes = 0b11111  // Number of passes in bits 0-4
};

/// Allow bitwise operations on GlowFilterFlags
inline GlowFilterFlags operator|(GlowFilterFlags a, GlowFilterFlags b) {
    return static_cast<GlowFilterFlags>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

inline GlowFilterFlags operator&(GlowFilterFlags a, GlowFilterFlags b) {
    return static_cast<GlowFilterFlags>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}

inline bool operator&(GlowFilterFlags flags, GlowFilterFlags flag) {
    return (static_cast<uint8_t>(flags) & static_cast<uint8_t>(flag)) != 0;
}

/// A glow filter for bitmap effects.
///
/// This filter applies a glow effect to bitmap data,
/// with configurable color, blur amounts, and strength.
struct GlowFilter {
    /// The color of the glow
    Color color;

    /// Horizontal blur amount (16.16 fixed-point)
    Fixed16 blur_x;

    /// Vertical blur amount (16.16 fixed-point)
    Fixed16 blur_y;

    /// Strength of the glow effect (8.8 fixed-point)
    Fixed8 strength;

    /// Flags controlling filter behavior
    GlowFilterFlags flags;

    /// Default constructor
    GlowFilter()
        : color()
        , blur_x(Fixed16::ZERO)
        , blur_y(Fixed16::ZERO)
        , strength(Fixed8::ZERO)
        , flags(GlowFilterFlags::None) {}

    /// Constructor with all parameters
    GlowFilter(Color color_, Fixed16 blur_x_, Fixed16 blur_y_, Fixed8 strength_, GlowFilterFlags flags_)
        : color(color_)
        , blur_x(blur_x_)
        , blur_y(blur_y_)
        , strength(strength_)
        , flags(flags_) {}

    /// Check if this is an inner glow
    [[nodiscard]] inline bool is_inner() const {
        return flags & GlowFilterFlags::InnerGlow;
    }

    /// Check if this is a knockout glow
    [[nodiscard]] inline bool is_knockout() const {
        return flags & GlowFilterFlags::Knockout;
    }

    /// Check if this uses composite source
    [[nodiscard]] inline bool composite_source() const {
        return flags & GlowFilterFlags::CompositeSource;
    }

    /// Get the number of glow passes (0-31)
    [[nodiscard]] inline uint8_t num_passes() const {
        return static_cast<uint8_t>(flags) & 0b11111;
    }

    /// Scale the blur amounts by the given factors
    ///
    /// \param x Horizontal scale factor
    /// \param y Vertical scale factor
    void scale(float x, float y);

    /// Calculate the destination rectangle after applying this glow
    ///
    /// \param source_rect The source rectangle to calculate from
    /// \return The expanded rectangle accounting for glow spread
    [[nodiscard]] Rectangle<Twips> calculate_dest_rect(const Rectangle<Twips>& source_rect) const;

    /// Get the inner blur filter representation
    [[nodiscard]] BlurFilter inner_blur_filter() const;

    /// Equality comparison
    bool operator==(const GlowFilter& other) const {
        return color == other.color &&
               blur_x == other.blur_x &&
               blur_y == other.blur_y &&
               strength == other.strength &&
               flags == other.flags;
    }

    /// Inequality comparison
    bool operator!=(const GlowFilter& other) const {
        return !(*this == other);
    }
};

/// GlowFilterFlags helper methods
namespace GlowFilterFlagsHelpers {
    /// Create GlowFilterFlags from number of passes
    inline GlowFilterFlags from_passes(uint8_t num_passes) {
        auto flags = static_cast<GlowFilterFlags>(num_passes);
        // Debug assertion would go here in debug builds
        return flags;
    }
}

} // namespace types

// Re-export at swf namespace level for convenience
using GlowFilter = types::GlowFilter;
using GlowFilterFlags = types::GlowFilterFlags;

} // namespace swf
} // namespace ruffle

#endif // RUFFLE_SWF_TYPES_GLOW_FILTER_H
