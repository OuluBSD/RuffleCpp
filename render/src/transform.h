// C++ translation of render/src/transform.rs
// Transform and TransformStack for DisplayObject transformations

#ifndef RUFFLE_RENDER_TRANSFORM_H
#define RUFFLE_RENDER_TRANSFORM_H

#include <vector>
#include <optional>
#include <cstdint>

// Forward declarations
namespace ruffle {
namespace render {
    struct Matrix;
    struct PerspectiveProjection;
}
namespace swf {
    struct ColorTransform;
}
}

namespace ruffle {
namespace render {

/// Represents the transform for a DisplayObject.
///
/// This includes both the transformation matrix and the color transform.
/// This is used to track the cumulative transformation state when rendering
/// nested display objects.
struct Transform {
    /// The transformation matrix
    Matrix matrix;

    /// The color transform
    swf::ColorTransform color_transform;

    /// Optional perspective projection for 3D rendering
    std::optional<PerspectiveProjection> perspective_projection;

    /// Default constructor - creates identity transform
    Transform();

    /// Constructor with matrix and color transform
    Transform(const Matrix& mat, const swf::ColorTransform& color);

    /// Constructor with all fields
    Transform(
        const Matrix& mat,
        const swf::ColorTransform& color,
        const std::optional<PerspectiveProjection>& perspective
    );

    /// Copy constructor
    Transform(const Transform&) = default;

    /// Copy assignment
    Transform& operator=(const Transform&) = default;

    /// Equality comparison
    bool operator==(const Transform& other) const;

    /// Inequality comparison
    bool operator!=(const Transform& other) const {
        return !(*this == other);
    }
};

/// A stack of transforms for tracking nested display object transformations.
///
/// This is used during rendering to maintain the cumulative transformation
/// state as we traverse the display list hierarchy.
class TransformStack {
private:
    std::vector<Transform> stack_;

public:
    /// Default constructor - creates stack with identity transform
    TransformStack();

    /// Push a transform onto the stack
    ///
    /// The new transform is composed with the current top of stack:
    /// - Matrices are multiplied
    /// - Color transforms are multiplied
    /// - Perspective projections are NOT merged (TODO)
    ///
    /// \param transform The transform to push
    void push(const Transform& transform);

    /// Pop the top transform from the stack
    ///
    /// \throws std::logic_error if stack would become empty
    void pop();

    /// Get the current (top) transform
    ///
    /// \return The current transform
    Transform transform() const;

    /// Get the current matrix
    ///
    /// \return The current matrix
    const Matrix& matrix() const;

    /// Get the current color transform
    ///
    /// \return The current color transform
    const swf::ColorTransform& color_transform() const;

    /// Check if stack has only the base transform
    ///
    /// \return true if stack size is 1
    bool is_empty() const { return stack_.size() == 1; }

    /// Get the current stack depth
    ///
    /// \return Number of transforms on the stack
    size_t depth() const { return stack_.size(); }
};

} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_TRANSFORM_H
