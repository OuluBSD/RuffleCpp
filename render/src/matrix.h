// C++ translation of render/src/matrix.rs
// Transformation matrix for Flash display objects

#ifndef RUFFLE_RENDER_MATRIX_H
#define RUFFLE_RENDER_MATRIX_H

#include <cstdint>
#include <optional>
#include <cmath>

// Forward declarations
namespace swf {
    struct Point;
    struct PointDelta;
    struct Rectangle;
    class Twips;
    struct Fixed16;
    struct Matrix;
}

namespace ruffle {
namespace render {

/// The transformation matrix used by Flash display objects.
///
/// This matrix is used to transform coordinates between different spaces
/// (e.g., from local object space to stage space).
///
/// The matrix layout is:
/// | a  c  tx |
/// | b  d  ty |
/// | 0  0  1  |
struct Matrix {
    /// Serialized as `scale_x` in SWF files
    float a;

    /// Serialized as `rotate_skew_0` in SWF files
    float b;

    /// Serialized as `rotate_skew_1` in SWF files
    float c;

    /// Serialized as `scale_y` in SWF files
    float d;

    /// Serialized as `transform_x` in SWF files
    swf::Twips tx;

    /// Serialized as `transform_y` in SWF files
    swf::Twips ty;

    /// Identity matrix constant
    static const Matrix IDENTITY;

    /// Zero matrix constant
    static const Matrix ZERO;

    /// Twips to pixels conversion matrix
    static const Matrix TWIPS_TO_PIXELS;

    /// Pixels to twips conversion matrix
    static const Matrix PIXELS_TO_TWIPS;

    /// Default constructor creates identity matrix
    constexpr Matrix()
        : a(1.0f), b(0.0f), c(0.0f), d(1.0f), tx(swf::Twips::ZERO), ty(swf::Twips::ZERO)
    {}

    /// Create a scale matrix
    static constexpr Matrix scale(float scale_x, float scale_y) {
        return Matrix{
            scale_x, 0.0f, 0.0f, scale_y,
            swf::Twips::ZERO, swf::Twips::ZERO
        };
    }

    /// Create a rotation matrix
    static Matrix rotate(float angle) {
        float cos_a = std::cos(angle);
        float sin_a = std::sin(angle);
        return Matrix{
            cos_a, sin_a, -sin_a, cos_a,
            swf::Twips::ZERO, swf::Twips::ZERO
        };
    }

    /// Create a translation matrix
    static Matrix translate(const swf::Twips& x, const swf::Twips& y) {
        return Matrix{
            1.0f, 0.0f, 0.0f, 1.0f,
            x, y
        };
    }

    /// Create a box transform matrix
    static constexpr Matrix create_box(
        float scale_x, float scale_y,
        const swf::Twips& translate_x, const swf::Twips& translate_y
    ) {
        return Matrix{
            scale_x, 0.0f, 0.0f, scale_y,
            translate_x, translate_y
        };
    }

    /// Create a box transform matrix with rotation
    static Matrix create_box_with_rotation(
        float scale_x, float scale_y,
        float rotation,
        const swf::Twips& translate_x, const swf::Twips& translate_y
    ) {
        float cos_r = std::cos(rotation);
        float sin_r = std::sin(rotation);
        return Matrix{
            cos_r * scale_x, sin_r * scale_y,
            -sin_r * scale_x, cos_r * scale_y,
            translate_x, translate_y
        };
    }

    /// Create a box matrix from a rectangle
    static Matrix create_box_from_rectangle(const swf::Rectangle<swf::Twips>& rect);

    /// Create a gradient box matrix
    static Matrix create_gradient_box(
        float width, float height,
        float rotation,
        const swf::Twips& translate_x, const swf::Twips& translate_y
    );

    /// Calculate the determinant of the matrix
    constexpr float determinant() const {
        return a * d - b * c;
    }

    /// Calculate the inverse of the matrix
    std::optional<Matrix> inverse() const;

    /// Matrix multiplication
    Matrix operator*(const Matrix& rhs) const {
        float rhs_tx = static_cast<float>(rhs.tx.get());
        float rhs_ty = static_cast<float>(rhs.ty.get());

        int32_t out_tx = round_to_i32(a * rhs_tx + c * rhs_ty) + tx.get();
        int32_t out_ty = round_to_i32(b * rhs_tx + d * rhs_ty) + ty.get();

        return Matrix{
            a * rhs.a + c * rhs.b,
            b * rhs.a + d * rhs.b,
            a * rhs.c + c * rhs.d,
            b * rhs.c + d * rhs.d,
            swf::Twips::new_twips(out_tx),
            swf::Twips::new_twips(out_ty)
        };
    }

    /// Multiply matrix by point
    swf::Point operator*(const swf::Point<swf::Twips>& point) const;

    /// Multiply matrix by point delta
    swf::PointDelta operator*(const swf::PointDelta<swf::Twips>& delta) const;

    /// Multiply matrix by rectangle
    swf::Rectangle<swf::Twips> operator*(const swf::Rectangle<swf::Twips>& rhs) const;

    /// Matrix multiplication assignment
    Matrix& operator*=(const Matrix& rhs) {
        *this = *this * rhs;
        return *this;
    }

    /// Check equality
    bool operator==(const Matrix& other) const = default;

    /// Convert from SWF matrix
    static Matrix from_swf(const swf::Matrix& matrix) {
        return Matrix{
            matrix.a.to_f32(),
            matrix.b.to_f32(),
            matrix.c.to_f32(),
            matrix.d.to_f32(),
            matrix.tx,
            matrix.ty
        };
    }

    /// Convert to SWF matrix
    swf::Matrix to_swf() const;
};

/// Initialize static constants
inline const Matrix Matrix::IDENTITY{
    1.0f, 0.0f, 0.0f, 1.0f,
    swf::Twips::ZERO, swf::Twips::ZERO
};

inline const Matrix Matrix::ZERO{
    0.0f, 0.0f, 0.0f, 0.0f,
    swf::Twips::ZERO, swf::Twips::ZERO
};

inline const Matrix Matrix::TWIPS_TO_PIXELS{
    1.0f / swf::Twips::TWIPS_PER_PIXEL, 0.0f, 0.0f,
    1.0f / swf::Twips::TWIPS_PER_PIXEL,
    swf::Twips::ZERO, swf::Twips::ZERO
};

inline const Matrix Matrix::PIXELS_TO_TWIPS{
    static_cast<float>(swf::Twips::TWIPS_PER_PIXEL), 0.0f, 0.0f,
    static_cast<float>(swf::Twips::TWIPS_PER_PIXEL),
    swf::Twips::ZERO, swf::Twips::ZERO
};

/// Implements IEEE-754 "Round to nearest, ties to even" rounding.
/// This is the rounding method used by Flash for transform operations.
/// Also clamps out-of-range values and NaN to INT32_MIN.
inline int32_t round_to_i32(float f) {
    if (std::isfinite(f)) {
        if (f < 2147483648.0f) {
            // Round ties to even
            return static_cast<int32_t>(std::nearbyint(f));
        } else {
            // Out-of-range clamps to MIN
            return INT32_MIN;
        }
    } else {
        // NaN/Infinity goes to 0
        return 0;
    }
}

} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_MATRIX_H
