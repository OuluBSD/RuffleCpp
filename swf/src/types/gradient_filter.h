// C++ translation of swf/src/types/gradient_filter.rs
// GradientFilter type for gradient-based filter effects

#ifndef RUFFLE_SWF_TYPES_GRADIENT_FILTER_H
#define RUFFLE_SWF_TYPES_GRADIENT_FILTER_H

#include <cstdint>
#include <vector>

// Forward declarations
namespace ruffle {
namespace swf {
    struct BlurFilter;
    struct GradientRecord;
    class Fixed8;
    class Fixed16;
}
}

namespace ruffle {
namespace swf {
namespace types {

/// Bit flags for GradientFilter behavior
/// 
/// Controls shadow positioning, knockout effect, and blur passes.
enum class GradientFilterFlags : uint8_t {
    InnerShadow      = 1 << 7,  // Shadow is drawn inside the shape
    Knockout         = 1 << 6,  // Only the shadow is visible
    CompositeSource  = 1 << 5,  // Composite source flag
    OnTop            = 1 << 4,  // Shadow is drawn on top
    Passes           = 0b1111   // Number of blur passes (lower 4 bits)
};

/// Allow bitwise operations on GradientFilterFlags
inline GradientFilterFlags operator&(GradientFilterFlags lhs, GradientFilterFlags rhs) {
    return static_cast<GradientFilterFlags>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
}

inline GradientFilterFlags operator|(GradientFilterFlags lhs, GradientFilterFlags rhs) {
    return static_cast<GradientFilterFlags>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

inline GradientFilterFlags& operator&=(GradientFilterFlags& lhs, GradientFilterFlags rhs) {
    lhs = lhs & rhs;
    return lhs;
}

inline GradientFilterFlags& operator|=(GradientFilterFlags& lhs, GradientFilterFlags rhs) {
    lhs = lhs | rhs;
    return lhs;
}

inline uint8_t operator&(GradientFilterFlags lhs, uint8_t rhs) {
    return static_cast<uint8_t>(lhs) & rhs;
}

/// A gradient filter that applies a gradient-based shadow or glow effect.
/// 
/// Gradient filters use a color gradient to create shadow or glow effects
/// around display objects, with configurable blur, angle, distance, and strength.
struct GradientFilter {
    /// The gradient color stops
    std::vector<GradientRecord> colors;

    /// Horizontal blur amount (16.16 fixed-point)
    Fixed16 blur_x;

    /// Vertical blur amount (16.16 fixed-point)
    Fixed16 blur_y;

    /// Angle of the gradient in radians (16.16 fixed-point)
    Fixed16 angle;

    /// Distance of the shadow/glow from the object (16.16 fixed-point)
    Fixed16 distance;

    /// Strength/opacity of the effect (8.8 fixed-point)
    Fixed8 strength;

    /// Behavior flags
    GradientFilterFlags flags;

    /// Default constructor
    GradientFilter() = default;

    /// Constructor with all parameters
    GradientFilter(
        std::vector<GradientRecord> colors_,
        Fixed16 blur_x_,
        Fixed16 blur_y_,
        Fixed16 angle_,
        Fixed16 distance_,
        Fixed8 strength_,
        GradientFilterFlags flags_
    ) : colors(std::move(colors_))
      , blur_x(blur_x_)
      , blur_y(blur_y_)
      , angle(angle_)
      , distance(distance_)
      , strength(strength_)
      , flags(flags_) {}

    /// Returns true if the shadow is drawn inside the shape
    [[nodiscard]] bool is_inner() const {
        return (flags & GradientFilterFlags::InnerShadow) != GradientFilterFlags(0);
    }

    /// Returns true if only the shadow is visible (knockout mode)
    [[nodiscard]] bool is_knockout() const {
        return (flags & GradientFilterFlags::Knockout) != GradientFilterFlags(0);
    }

    /// Returns true if the shadow is drawn on top of the object
    [[nodiscard]] bool is_on_top() const {
        return (flags & GradientFilterFlags::OnTop) != GradientFilterFlags(0);
    }

    /// Returns the number of blur passes (0-15)
    [[nodiscard]] uint8_t num_passes() const {
        return static_cast<uint8_t>(flags & GradientFilterFlags::Passes);
    }

    /// Scale the filter by the given factors
    /// 
    /// \param x Horizontal scale factor
    /// \param y Vertical scale factor
    void scale(float x, float y);

    /// Get the inner blur filter for this gradient filter
    /// 
    /// Returns a BlurFilter with the same blur amounts and pass count.
    [[nodiscard]] BlurFilter inner_blur_filter() const;

    /// Equality comparison
    bool operator==(const GradientFilter& other) const {
        return colors == other.colors
            && blur_x == other.blur_x
            && blur_y == other.blur_y
            && angle == other.angle
            && distance == other.distance
            && strength == other.strength
            && flags == other.flags;
    }

    /// Inequality comparison
    bool operator!=(const GradientFilter& other) const {
        return !(*this == other);
    }
};

} // namespace types

// Re-export at swf namespace level for convenience
using GradientFilter = types::GradientFilter;
using GradientFilterFlags = types::GradientFilterFlags;

} // namespace swf
} // namespace ruffle

#endif // RUFFLE_SWF_TYPES_GRADIENT_FILTER_H
