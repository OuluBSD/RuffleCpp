// C++ translation of render/src/matrix3d.rs
// The transformation matrix for 3D used by Flash display objects

#ifndef RUFFLE_RENDER_MATRIX3D_H
#define RUFFLE_RENDER_MATRIX3D_H

#include <array>
#include <cstdint>

// Forward declarations
namespace ruffle {
namespace render {
    struct Matrix;
}
namespace swf {
    struct Twips;
}
}

namespace ruffle {
namespace render {

/// The transformation matrix for 3D used by Flash display objects
struct Matrix3D {
    /// 4x4 matrix elements
    std::array<double, 16> raw_data;

    /// Default constructor - identity matrix
    Matrix3D() : raw_data{
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    } {}

    /// Create from 2D Matrix
    static Matrix3D from_matrix(const Matrix& matrix);

    /// Convert to 2D Matrix
    Matrix to_matrix() const;

    /// Copy constructor
    Matrix3D(const Matrix3D&) = default;

    /// Copy assignment
    Matrix3D& operator=(const Matrix3D&) = default;

    /// Equality comparison
    bool operator==(const Matrix3D& other) const {
        return raw_data == other.raw_data;
    }
};

} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_MATRIX3D_H
