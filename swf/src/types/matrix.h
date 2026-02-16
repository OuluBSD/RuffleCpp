// C++ translation of swf/src/types/matrix.rs
// Matrix type for SWF display object transformations

#ifndef RUFFLE_SWF_TYPES_MATRIX_H
#define RUFFLE_SWF_TYPES_MATRIX_H

#include <cstdint>

// Forward declarations
namespace ruffle {
namespace swf {
    class Fixed16;
    class Twips;
    template<typename T> struct Point;
}
}

namespace ruffle {
namespace swf {
namespace types {

/// The transformation matrix used by Flash display objects.
///
/// The matrix is a 2x3 affine transformation matrix. A point (x, y) is transformed by the matrix
/// in the following way:
/// ```
///  [a c tx] *  [x] = [a*x + c*y + tx]
///  [b d ty]    [y]   [b*x + d*y + ty]
///  [0 0 1 ]    [1]   [1             ]
/// ```
///
/// The SWF format uses 16.16 format for `a`, `b`, `c`, `d`. Twips are used for `tx` and `ty`.
/// This means that objects in Flash can only move in units of twips, or 1/20 pixels.
struct Matrix {
    /// The matrix element at [0, 0]. Labeled ScaleX in SWF19.
    Fixed16 a;

    /// The matrix element at [1, 0]. Labeled RotateSkew0 in SWF19.
    Fixed16 b;

    /// The matrix element at [0, 1]. Labeled RotateSkew1 in SWF19.
    Fixed16 c;

    /// The matrix element at [1, 1]. Labeled ScaleY in SWF19.
    Fixed16 d;

    /// The X translation in twips. Labeled TranslateX in SWF19.
    Twips tx;

    /// The Y translation in twips. Labeled TranslateY in SWF19.
    Twips ty;

    /// Default constructor - creates identity matrix
    Matrix();

    /// Constructor with all elements
    ///
    /// \param a_ The [0, 0] element (ScaleX)
    /// \param b_ The [1, 0] element (RotateSkew0)
    /// \param c_ The [0, 1] element (RotateSkew1)
    /// \param d_ The [1, 1] element (ScaleY)
    /// \param tx_ The X translation in twips
    /// \param ty_ The Y translation in twips
    constexpr Matrix(Fixed16 a_, Fixed16 b_, Fixed16 c_, Fixed16 d_, Twips tx_, Twips ty_)
        : a(a_), b(b_), c(c_), d(d_), tx(tx_), ty(ty_) {}

    /// The identity matrix.
    ///
    /// Transforming an object by this matrix has no effect.
    static const Matrix IDENTITY;

    /// Returns a scale matrix.
    ///
    /// \param scale_x The X scale factor
    /// \param scale_y The Y scale factor
    /// \return A new scale matrix
    [[nodiscard]] static constexpr Matrix scale(Fixed16 scale_x, Fixed16 scale_y);

    /// Returns a rotation matrix that rotates by angle radians.
    ///
    /// \param angle The rotation angle in radians
    /// \return A new rotation matrix
    [[nodiscard]] static Matrix rotate(float angle);

    /// Returns a translation matrix.
    ///
    /// \param x The X translation in twips
    /// \param y The Y translation in twips
    /// \return A new translation matrix
    [[nodiscard]] static constexpr Matrix translate(Twips x, Twips y);

    /// Inverts the matrix.
    ///
    /// If the matrix is not invertible, the resulting matrix will be invalid.
    void invert();

    /// Equality comparison
    bool operator==(const Matrix& other) const {
        return a == other.a && b == other.b && c == other.c &&
               d == other.d && tx == other.tx && ty == other.ty;
    }

    /// Inequality comparison
    bool operator!=(const Matrix& other) const {
        return !(*this == other);
    }

    /// Matrix multiplication
    Matrix operator*(const Matrix& rhs) const;

    /// Matrix multiply-assign
    Matrix& operator*=(const Matrix& rhs);

    /// Transform a point by this matrix
    Point<Twips> operator*(const Point<Twips>& point) const;
};

} // namespace types
} // namespace swf
} // namespace ruffle

#endif // RUFFLE_SWF_TYPES_MATRIX_H
