// C++ translation of swf/src/types/blur_filter.rs
// Blur filter type for bitmap blur effects

#ifndef RUFFLE_SWF_TYPES_BLUR_FILTER_H
#define RUFFLE_SWF_TYPES_BLUR_FILTER_H

#include <cstdint>
#include <cmath>
#include <algorithm>

// Forward declarations
namespace ruffle {
namespace swf {
    template<typename T> class Rectangle;
    class Twips;
    class Fixed16;
}
}

namespace ruffle {
namespace swf {
namespace types {

/// Accumulative pass scales for blur filter
/// How much each pass should multiply the requested blur size by
/// These are very approximate to Flash, and not 100% exact.
/// Pass 1 would be 100%, but pass 2 would be 110%.
/// This is accumulative so you can calculate the size upfront for how many passes you'll need to perform.
constexpr double PASS_SCALES[15] = {
    1.0, 2.1, 2.7, 3.1, 3.5, 3.8, 4.0, 4.2, 4.4, 4.6, 5.0, 6.0, 6.0, 7.0, 7.0
};

/// Bit flags for BlurFilter behavior
///
/// Controls the number of blur passes (bits 3-7).
enum class BlurFilterFlags : uint8_t {
    None = 0,
    Passes = 0b11111 << 3  // Number of passes in bits 3-7
};

/// Allow bitwise operations on BlurFilterFlags
inline BlurFilterFlags operator|(BlurFilterFlags a, BlurFilterFlags b) {
    return static_cast<BlurFilterFlags>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

inline BlurFilterFlags operator&(BlurFilterFlags a, BlurFilterFlags b) {
    return static_cast<BlurFilterFlags>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}

inline bool operator&(BlurFilterFlags flags, BlurFilterFlags flag) {
    return (static_cast<uint8_t>(flags) & static_cast<uint8_t>(flag)) != 0;
}

/// A blur filter for bitmap effects.
///
/// This filter applies a Gaussian blur effect to bitmap data,
/// with configurable horizontal and vertical blur amounts.
struct BlurFilter {
    /// Horizontal blur amount (16.16 fixed-point)
    Fixed16 blur_x;

    /// Vertical blur amount (16.16 fixed-point)
    Fixed16 blur_y;

    /// Flags controlling filter behavior (passes count)
    BlurFilterFlags flags;

    /// Default constructor
    BlurFilter()
        : blur_x(Fixed16::ZERO)
        , blur_y(Fixed16::ZERO)
        , flags(BlurFilterFlags::None) {}

    /// Constructor with all parameters
    BlurFilter(Fixed16 blur_x_, Fixed16 blur_y_, BlurFilterFlags flags_)
        : blur_x(blur_x_)
        , blur_y(blur_y_)
        , flags(flags_) {}

    /// Get the number of blur passes (0-31)
    [[nodiscard]] inline uint8_t num_passes() const {
        return (static_cast<uint8_t>(flags) & 0b11111000) >> 3;
    }

    /// Scale the blur amounts by the given factors
    ///
    /// \param x Horizontal scale factor
    /// \param y Vertical scale factor
    void scale(float x, float y) {
        blur_x = scale_blur(blur_x, x);
        blur_y = scale_blur(blur_y, y);
    }

    /// Check if the filter has no visible effect
    ///
    /// Returns true if num_passes is 0 or both blur amounts are <= 1.0
    [[nodiscard]] bool impotent() const {
        return num_passes() == 0 || (blur_x <= Fixed16::ONE && blur_y <= Fixed16::ONE);
    }

    /// Calculate the destination rectangle after applying this blur
    ///
    /// \param source_rect The source rectangle to calculate from
    /// \return The expanded rectangle accounting for blur spread
    [[nodiscard]] Rectangle<Twips> calculate_dest_rect(const Rectangle<Twips>& source_rect) const;

    /// Scale a blur value by a factor
    ///
    /// \param blur The original blur value
    /// \param factor The scale factor
    /// \return The scaled blur value
    [[nodiscard]] static Fixed16 scale_blur(Fixed16 blur, float factor);

    /// Equality comparison
    bool operator==(const BlurFilter& other) const {
        return blur_x == other.blur_x &&
               blur_y == other.blur_y &&
               flags == other.flags;
    }

    /// Inequality comparison
    bool operator!=(const BlurFilter& other) const {
        return !(*this == other);
    }
};

/// BlurFilterFlags helper methods
namespace BlurFilterFlagsHelpers {
    /// Create BlurFilterFlags from number of passes
    inline BlurFilterFlags from_passes(uint8_t num_passes) {
        auto flags = static_cast<BlurFilterFlags>(num_passes << 3);
        // Debug assertion would go here in debug builds
        return flags;
    }
}

} // namespace types

// Re-export at swf namespace level for convenience
using BlurFilter = types::BlurFilter;
using BlurFilterFlags = types::BlurFilterFlags;

} // namespace swf
} // namespace ruffle

#endif // RUFFLE_SWF_TYPES_BLUR_FILTER_H
