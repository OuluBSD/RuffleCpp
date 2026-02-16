// C++ translation of swf/src/types/convolution_filter.rs
// Convolution filter type

#ifndef RUFFLE_SWF_TYPES_CONVOLUTION_FILTER_H
#define RUFFLE_SWF_TYPES_CONVOLUTION_FILTER_H

#include <cstdint>
#include <vector>
#include "color.h"

namespace ruffle {
namespace swf {
namespace types {

/// Flags for ConvolutionFilter
enum class ConvolutionFilterFlags : uint8_t {
    None = 0,
    PreserveAlpha = 1 << 0,
    Clamp = 1 << 1,
};

/// Allow bitwise operations on ConvolutionFilterFlags
inline ConvolutionFilterFlags operator|(ConvolutionFilterFlags a, ConvolutionFilterFlags b) {
    return static_cast<ConvolutionFilterFlags>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

inline ConvolutionFilterFlags operator&(ConvolutionFilterFlags a, ConvolutionFilterFlags b) {
    return static_cast<ConvolutionFilterFlags>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}

inline bool operator&(ConvolutionFilterFlags flags, ConvolutionFilterFlags flag) {
    return (static_cast<uint8_t>(flags) & static_cast<uint8_t>(flag)) != 0;
}

/// A convolution filter for bitmap effects.
///
/// This filter applies a matrix-based convolution effect to bitmap data.
struct ConvolutionFilter {
    /// The number of rows in the convolution matrix.
    uint8_t num_matrix_rows;

    /// The number of columns in the convolution matrix.
    uint8_t num_matrix_cols;

    /// The convolution matrix values (row-major order).
    std::vector<float> matrix;

    /// The divisor used to normalize the convolution result.
    float divisor;

    /// The bias added to the convolution result.
    float bias;

    /// The default color for pixels outside the image bounds.
    Color default_color;

    /// Flags controlling filter behavior.
    ConvolutionFilterFlags flags;

    /// Default constructor
    ConvolutionFilter()
        : num_matrix_rows(0)
        , num_matrix_cols(0)
        , matrix()
        , divisor(1.0f)
        , bias(0.0f)
        , default_color()
        , flags(ConvolutionFilterFlags::None) {}

    /// Constructor with all parameters
    ConvolutionFilter(
        uint8_t rows,
        uint8_t cols,
        std::vector<float> mat,
        float div,
        float b,
        Color color,
        ConvolutionFilterFlags fl
    )
        : num_matrix_rows(rows)
        , num_matrix_cols(cols)
        , matrix(std::move(mat))
        , divisor(div)
        , bias(b)
        , default_color(color)
        , flags(fl) {}

    /// Check if the filter is clamped
    [[nodiscard]] inline bool is_clamped() const {
        return flags & ConvolutionFilterFlags::Clamp;
    }

    /// Check if the filter preserves alpha
    [[nodiscard]] inline bool is_preserve_alpha() const {
        return flags & ConvolutionFilterFlags::PreserveAlpha;
    }

    /// Equality comparison
    bool operator==(const ConvolutionFilter& other) const {
        return num_matrix_rows == other.num_matrix_rows &&
               num_matrix_cols == other.num_matrix_cols &&
               matrix == other.matrix &&
               divisor == other.divisor &&
               bias == other.bias &&
               default_color == other.default_color &&
               flags == other.flags;
    }

    /// Inequality comparison
    bool operator!=(const ConvolutionFilter& other) const {
        return !(*this == other);
    }
};

} // namespace types

// Re-export ConvolutionFilter at swf namespace level for convenience
using ConvolutionFilter = types::ConvolutionFilter;

} // namespace swf
} // namespace ruffle

#endif // RUFFLE_SWF_TYPES_CONVOLUTION_FILTER_H
