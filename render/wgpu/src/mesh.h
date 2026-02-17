// C++ translation of render/wgpu/src/mesh.rs
// Mesh rendering for wgpu

#ifndef RUFFLE_RENDER_WGPU_MESH_H
#define RUFFLE_RENDER_WGPU_MESH_H

#include <cstdint>
#include <cstddef>
#include <vector>
#include <array>
#include <optional>
#include <memory>
#include <variant>

// Forward declarations
namespace wgpu {
    class Buffer;
    class Device;
    class TextureView;
    class BindGroup;
    class BindGroupLayout;
    class Sampler;
    struct BufferAddress;
}

namespace swf {
    using CharacterId = uint16_t;
    enum class GradientInterpolation : uint8_t;
}

namespace ruffle {
namespace render {
    class BitmapSource;
    namespace backend {
        class RenderBackend;
        struct ShapeHandle;
        class ShapeHandleImpl;
    }
    namespace tessellator {
        struct Bitmap;
        struct Draw;
        struct Gradient;
        enum class DrawType;
    }
}
}

namespace ruffle {
namespace render {
namespace wgpu {

// Forward declarations
struct Descriptors;
struct TextureTransforms;
struct GradientUniforms;
struct BufferBuilder;

/// Gradient size for texture generation
inline constexpr size_t GRADIENT_SIZE = 256;

/// Mesh shape data
///
/// Contains the vertex and index buffers for a rendered shape.
class Mesh : public ruffle::render::backend::ShapeHandleImpl {
public:
    /// Draw commands for the mesh
    std::vector<class Draw> draws;

    /// Vertex buffer
    wgpu::Buffer* vertex_buffer;

    /// Index buffer
    wgpu::Buffer* index_buffer;

    /// Default constructor
    Mesh() = default;

    /// Destructor
    ~Mesh();
};

/// Get mesh from shape handle
///
/// \param handle The shape handle
/// \return Pointer to the mesh
inline const Mesh* as_mesh(const ruffle::render::backend::ShapeHandle* handle) {
    return static_cast<const Mesh*>(handle->impl());
}

/// Pending draw type
///
/// Represents the type of draw operation before finalization.
enum class PendingDrawType {
    /// Solid color draw
    Color,

    /// Gradient fill draw
    Gradient,

    /// Bitmap fill draw
    Bitmap
};

/// Pending draw
///
/// Represents a draw operation that hasn't been finalized yet.
struct PendingDraw {
    /// The draw type
    PendingDrawType draw_type;

    /// Vertex buffer range
    wgpu::BufferAddress vertices_start;
    wgpu::BufferAddress vertices_end;

    /// Index buffer range
    wgpu::BufferAddress indices_start;
    wgpu::BufferAddress indices_end;

    /// Number of indices
    uint32_t num_indices;

    /// Number of mask indices
    uint32_t num_mask_indices;

    /// Gradient index (for gradient draws)
    size_t gradient_index;

    /// Texture transforms index (for gradient/bitmap draws)
    wgpu::BufferAddress texture_transforms_index;

    /// Texture view (for bitmap draws)
    wgpu::TextureView* texture_view;

    /// Is texture repeating (for bitmap draws)
    bool is_repeating;

    /// Is texture smoothed (for bitmap draws)
    bool is_smoothed;

    /// Bind group label (for debug)
    std::optional<std::string> bind_group_label;

    /// Create a new pending draw from tessellator data
    ///
    /// \param backend The render backend
    /// \param source The bitmap source
    /// \param draw The tessellator draw
    /// \param shape_id The shape character ID
    /// \param draw_id The draw index
    /// \param uniform_buffer The uniform buffer builder
    /// \param vertex_buffer The vertex buffer builder
    /// \param index_buffer The index buffer builder
    /// \return Optional pending draw (nullopt if bitmap not found)
    static std::optional<PendingDraw> create(
        void* backend,
        const ruffle::render::BitmapSource* source,
        const ruffle::render::tessellator::Draw& draw,
        swf::CharacterId shape_id,
        size_t draw_id,
        BufferBuilder* uniform_buffer,
        BufferBuilder* vertex_buffer,
        BufferBuilder* index_buffer
    );

    /// Finish the pending draw
    ///
    /// Converts the pending draw into a finalized draw.
    ///
    /// \param descriptors The render descriptors
    /// \param uniform_buffer The uniform buffer
    /// \param gradients The gradient data
    /// \return The finalized draw
    class Draw finish(
        Descriptors* descriptors,
        wgpu::Buffer* uniform_buffer,
        const std::vector<class CommonGradient>& gradients
    ) const;
};

/// Draw type
///
/// Represents the finalized type of draw operation.
enum class DrawType {
    /// Solid color draw
    Color,

    /// Gradient fill draw
    Gradient,

    /// Bitmap fill draw
    Bitmap
};

/// Bitmap binds
///
/// Contains the bind group for bitmap rendering.
struct BitmapBinds {
    /// The bind group
    wgpu::BindGroup* bind_group;

    /// Create bitmap binds
    ///
    /// \param device The wgpu device
    /// \param layout The bind group layout
    /// \param sampler The sampler
    /// \param uniform_buffer The uniform buffer
    /// \param texture_transforms The texture transforms offset
    /// \param texture_view The texture view
    /// \param label Debug label
    /// \return The bitmap binds
    static BitmapBinds create(
        wgpu::Device* device,
        wgpu::BindGroupLayout* layout,
        wgpu::Sampler* sampler,
        wgpu::Buffer* uniform_buffer,
        wgpu::BufferAddress texture_transforms,
        wgpu::TextureView* texture_view,
        std::optional<std::string> label
    );
};

/// Draw
///
/// Represents a finalized draw operation.
class Draw {
public:
    /// The draw type
    DrawType draw_type;

    /// Vertex buffer range
    wgpu::BufferAddress vertices_start;
    wgpu::BufferAddress vertices_end;

    /// Index buffer range
    wgpu::BufferAddress indices_start;
    wgpu::BufferAddress indices_end;

    /// Number of indices
    uint32_t num_indices;

    /// Number of mask indices
    uint32_t num_mask_indices;

    /// Bind group (for gradient/bitmap draws)
    wgpu::BindGroup* bind_group;

    /// Bitmap binds (for bitmap draws)
    std::optional<BitmapBinds> bitmap_binds;

    /// Default constructor
    Draw() = default;
};

/// Common gradient data
///
/// Contains the gradient texture and uniform buffer offset.
class CommonGradient {
public:
    /// Gradient texture view
    wgpu::TextureView* texture_view;

    /// Uniform buffer offset
    wgpu::BufferAddress buffer_offset;

    /// Default constructor
    CommonGradient() = default;

    /// Create a common gradient from gradient data
    ///
    /// \param descriptors The render descriptors
    /// \param gradient The gradient data
    /// \param uniform_buffers The uniform buffer builder
    /// \return The common gradient
    static CommonGradient create(
        Descriptors* descriptors,
        const ruffle::render::tessellator::Gradient& gradient,
        BufferBuilder* uniform_buffers
    );

private:
    /// Convert sRGB color to linear
    ///
    /// \param color The sRGB color component (0-255)
    /// \return The linear color value
    static float srgb_to_linear(float color);

    /// Linear interpolation
    ///
    /// \param a First value
    /// \param b Second value
    /// \param t Interpolation factor (0-1)
    /// \return Interpolated value
    static float lerp(float a, float b, float t);
};

/// Create texture transforms uniform
///
/// Creates a texture transform matrix from a 3x3 matrix.
///
/// \param matrix The 3x3 transformation matrix
/// \param buffer The buffer builder
/// \return The buffer offset
wgpu::BufferAddress create_texture_transforms(
    const std::array<std::array<float, 3>, 3>& matrix,
    BufferBuilder* buffer
);

} // namespace wgpu
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_WGPU_MESH_H
