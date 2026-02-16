// C++ translation of render/src/perspective_projection.rs
// Perspective projection for 3D rendering

#ifndef RUFFLE_RENDER_PERSPECTIVE_PROJECTION_H
#define RUFFLE_RENDER_PERSPECTIVE_PROJECTION_H

#include <cstdint>

// Forward declarations
namespace ruffle {
namespace render {
namespace matrix3d {
    class Matrix3D;
}
}
}

namespace ruffle {
namespace render {

/// Perspective projection settings for 3D rendering
struct PerspectiveProjection {
    /// Unit: degree. Must be greater than 0 and less than 180.
    double field_of_view;

    /// The center of the projection in (x, y).
    std::pair<double, double> center;

    /// Default constructor
    PerspectiveProjection()
        : field_of_view(55.0)
        , center(250.0, 250.0)
    {}

    /// Create perspective projection from focal length
    ///
    /// \param focal_length The focal length
    /// \param width The width of the viewport
    /// \return PerspectiveProjection with calculated field of view
    static PerspectiveProjection from_focal_length(double focal_length, double width) {
        constexpr double DEG2RAD = 3.14159265358979323846 / 180.0;
        double field_of_view = std::atan((width / 2.0) / focal_length) / DEG2RAD * 2.0;
        return PerspectiveProjection{field_of_view, {250.0, 250.0}};
    }

    /// Get the focal length for a given width
    ///
    /// \param width The width of the viewport
    /// \return The focal length
    float focal_length(float width) const {
        constexpr double DEG2RAD = 3.14159265358979323846 / 180.0;
        double rad = field_of_view * DEG2RAD;
        return static_cast<float>((width / 2.0) * std::tan((3.14159265358979323846 - rad) / 2.0));
    }

    /// Convert to Matrix3D
    ///
    /// \param width The width of the viewport
    /// \return Matrix3D representing this perspective projection
    matrix3d::Matrix3D to_matrix3d(float width) const;
};

} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_PERSPECTIVE_PROJECTION_H
