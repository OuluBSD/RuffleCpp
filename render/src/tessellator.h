// C++ translation of render/src/tessellator.rs
// Shape tessellation using lyon

#ifndef RUFFLE_RENDER_TESSELLATOR_H
#define RUFFLE_RENDER_TESSELLATOR_H

#include <vector>
#include <cstdint>
#include <memory>
#include <array>

// Forward declarations
namespace swf {
    struct Color;
    struct Fixed8;
    enum class GradientSpread;
    enum class GradientInterpolation;
    struct GradientRecord;
}

namespace ruffle {
namespace render {

// Forward declarations
class BitmapSource;

/// Type of gradient (linear, radial, focal)
enum class GradientType {
    Linear,
    Radial,
    Focal
};

/// A gradient definition
struct Gradient {
    GradientType gradient_type;
    swf::GradientSpread repeat_mode;
    swf::Fixed8 focal_point;
    swf::GradientInterpolation interpolation;
    std::vector<swf::GradientRecord> records;

    bool operator==(const Gradient& other) const = default;
};

/// A vertex in the tessellated mesh
struct Vertex {
    float x;
    float y;
    swf::Color color;
};

/// Bitmap fill information
struct Bitmap {
    std::array<std::array<float, 3>, 3> matrix;
    uint16_t bitmap_id;
    bool is_smoothed;
    bool is_repeating;
};

/// Type of draw operation
enum class DrawType {
    Color,
    Gradient {
        std::array<std::array<float, 3>, 3> matrix;
        size_t gradient_index;
    },
    Bitmap {
        Bitmap bitmap;
    }
};

/// A single draw command in the mesh
struct Draw {
    DrawType draw_type;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    uint32_t mask_index_count;
};

/// The tessellated mesh result
struct Mesh {
    std::vector<Draw> draws;
    std::vector<Gradient> gradients;
};

/// Shape tessellator using lyon
class ShapeTessellator {
public:
    ShapeTessellator();
    ~ShapeTessellator();

    /// Tessellate a shape into a mesh
    Mesh tessellate_shape(
        const struct DistilledShape& shape,
        const BitmapSource* bitmap_source
    );

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;

    void flush_draw(DrawType draw);
};

/// Convert SWF gradient to uniform gradient
Gradient swf_gradient_to_uniforms(
    GradientType gradient_type,
    const swf::Gradient& gradient,
    swf::Fixed8 focal_point
);

/// Convert SWF matrix to GL matrix
std::array<std::array<float, 3>, 3> swf_to_gl_matrix(const class Matrix& m);

/// Convert SWF bitmap matrix to GL matrix
std::array<std::array<float, 3>, 3> swf_bitmap_to_gl_matrix(
    const class Matrix& m,
    uint32_t bitmap_width,
    uint32_t bitmap_height
);

} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_TESSELLATOR_H
