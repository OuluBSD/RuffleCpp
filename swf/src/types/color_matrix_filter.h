// C++ translation of swf/src/types/color_matrix_filter.rs
// ColorMatrixFilter type for color transformation effects

#ifndef RUFFLE_SWF_TYPES_COLOR_MATRIX_FILTER_H
#define RUFFLE_SWF_TYPES_COLOR_MATRIX_FILTER_H

#include <array>
#include <cstdint>

namespace ruffle {
namespace swf {
namespace types {

/// A color matrix filter that applies a 4x5 matrix transformation to colors.
/// 
/// The matrix is stored as an array of 20 f32 values, representing:
/// - Rows 0-3: RGBA transformation coefficients (4x4 matrix)
/// - Row 4: RGBA offset values
/// 
/// Default matrix is identity (no transformation).
struct ColorMatrixFilter {
    /// The 4x5 color transformation matrix (20 values)
    /// Layout: [r,g,b,a,offset] for each output channel
    std::array<float, 20> matrix;

    /// Default constructor - creates identity matrix (no transformation)
    constexpr ColorMatrixFilter() 
        : matrix{{
            1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // r output
            0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  // g output
            0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // b output
            0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // a output
        }} {}

    /// Constructor from matrix array
    constexpr explicit ColorMatrixFilter(const std::array<float, 20>& mat)
        : matrix(mat) {}

    /// Returns true if this filter has no effect (is identity matrix)
    [[nodiscard]] bool impotent() const {
        return *this == ColorMatrixFilter();
    }

    /// Equality comparison
    constexpr bool operator==(const ColorMatrixFilter& other) const {
        return matrix == other.matrix;
    }

    /// Inequality comparison
    constexpr bool operator!=(const ColorMatrixFilter& other) const {
        return !(*this == other);
    }
};

} // namespace types

// Re-export at swf namespace level for convenience
using ColorMatrixFilter = types::ColorMatrixFilter;

} // namespace swf
} // namespace ruffle

#endif // RUFFLE_SWF_TYPES_COLOR_MATRIX_FILTER_H
