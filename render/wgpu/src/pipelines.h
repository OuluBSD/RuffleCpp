// C++ translation of render/wgpu/src/pipelines.rs
// Render pipelines for wgpu

#ifndef RUFFLE_RENDER_WGPU_PIPELINES_H
#define RUFFLE_RENDER_WGPU_PIPELINES_H

#include <cstdint>
#include <cstddef>
#include <array>
#include <memory>

// Forward declarations
namespace wgpu {
    class Device;
    class RenderPipeline;
    class PipelineLayout;
    class ShaderModule;
    class BindGroupLayout;
    struct VertexBufferLayout;
    struct TextureFormat;
    struct BlendState;
    struct DepthStencilState;
    struct StencilFaceState;
    struct PushConstantRange;
    enum class PrimitiveTopology;
    enum class VertexStepMode;
    enum class VertexFormat;
    enum class TextureFormat : uint32_t;
    struct VertexAttribute;
    struct VertexState;
    struct FragmentState;
    struct PrimitiveState;
    struct MultisampleState;
    struct ColorTargetState;
    struct PipelineCompilationOptions;
    struct PipelineLayoutDescriptor;
    struct RenderPipelineDescriptor;
}

namespace ruffle {
namespace render {
namespace wgpu {

// Forward declarations
struct BindLayouts;
struct Shaders;
struct PosVertex;
struct PosColorVertex;

/// Mask state for stencil operations
///
/// Represents the current state of mask rendering.
enum class MaskState : uint8_t {
    NoMask = 0,
    DrawMaskStencil = 1,
    DrawMaskedContent = 2,
    ClearMaskStencil = 3,
    LENGTH = 4
};

/// Convert MaskState from/to usize
inline MaskState mask_state_from_usize(size_t value) {
    return static_cast<MaskState>(value);
}

inline size_t mask_state_to_usize(MaskState state) {
    return static_cast<size_t>(state);
}

/// Vertex buffer layout for position-only vertices
inline constexpr std::array<wgpu::VertexBufferLayout, 1> VERTEX_BUFFERS_DESCRIPTION_POS = {{
    wgpu::VertexBufferLayout{
        .array_stride = sizeof(PosVertex),
        .step_mode = wgpu::VertexStepMode::Vertex,
        .attributes = {
            wgpu::VertexAttribute{.format = wgpu::VertexFormat::Float32x2, .offset = 0, .shader_location = 0}
        }
    }
}};

/// Vertex buffer layout for position+color vertices
inline constexpr std::array<wgpu::VertexBufferLayout, 1> VERTEX_BUFFERS_DESCRIPTION_COLOR = {{
    wgpu::VertexBufferLayout{
        .array_stride = sizeof(PosColorVertex),
        .step_mode = wgpu::VertexStepMode::Vertex,
        .attributes = {
            wgpu::VertexAttribute{.format = wgpu::VertexFormat::Float32x2, .offset = 0, .shader_location = 0},
            wgpu::VertexAttribute{.format = wgpu::VertexFormat::Float32x4, .offset = 8, .shader_location = 1}
        }
    }
}};

/// Shape pipeline with mask state variants
///
/// Contains render pipelines for each mask state plus a stencilless variant.
class ShapePipeline {
private:
    /// Pipelines for each mask state
    std::array<wgpu::RenderPipeline*, MaskState::LENGTH> pipelines_;

    /// Stencilless pipeline (no stencil buffer)
    wgpu::RenderPipeline* stencilless_;

public:
    /// Default constructor
    ShapePipeline() = default;

    /// Get pipeline for a mask state
    ///
    /// \param mask_state The mask state
    /// \return The render pipeline
    wgpu::RenderPipeline* pipeline_for(MaskState mask_state) const {
        return pipelines_[mask_state_to_usize(mask_state)];
    }

    /// Get the stencilless pipeline
    ///
    /// \return The stencilless render pipeline
    wgpu::RenderPipeline* stencilless_pipeline() const {
        return stencilless_;
    }

    /// Build a shape pipeline
    ///
    /// \param stencilless The stencilless pipeline
    /// \param f Function to create pipelines for each mask state
    /// \return The constructed ShapePipeline
    static ShapePipeline build(
        wgpu::RenderPipeline* stencilless,
        std::function<wgpu::RenderPipeline*(MaskState)> f
    );
};

/// Render pipelines collection
///
/// Contains all the render pipelines needed for shape rendering.
class Pipelines {
public:
    /// Color pipelines (with mask states)
    ShapePipeline color;

    /// Line pipelines (with mask states)
    ShapePipeline lines;

    /// Bitmap opaque pipeline (no blending)
    wgpu::RenderPipeline* bitmap_opaque;

    /// Bitmap opaque with dummy stencil (for use inside stencil passes)
    wgpu::RenderPipeline* bitmap_opaque_dummy_stencil;

    /// Bitmap pipelines with trivial blend modes
    std::array<ShapePipeline, 4> bitmap; // TrivialBlend::LENGTH = 4

    /// Gradient pipelines (with mask states)
    ShapePipeline gradients;

    /// Complex blend pipelines
    std::array<ShapePipeline, 9> complex_blends; // ComplexBlend::LENGTH = 9

    /// Alpha mask pipelines (with mask states)
    ShapePipeline alpha_mask;

    /// Default constructor
    Pipelines() = default;

    /// Create all pipelines
    ///
    /// \param device The wgpu device
    /// \param shaders The shader modules
    /// \param format The target texture format
    /// \param msaa_sample_count The MSAA sample count
    /// \param bind_layouts The bind group layouts
    /// \return Unique pointer to the pipelines
    static std::unique_ptr<Pipelines> create(
        wgpu::Device* device,
        Shaders* shaders,
        wgpu::TextureFormat format,
        uint32_t msaa_sample_count,
        BindLayouts* bind_layouts
    );
};

/// Create a render pipeline descriptor
///
/// Helper function to build a complete pipeline descriptor.
///
/// \param label Debug label
/// \param vertex_shader Vertex shader module
/// \param fragment_shader Fragment shader module
/// \param pipeline_layout Pipeline layout
/// \param depth_stencil_state Optional depth/stencil state
/// \param color_target_state Color target states
/// \param color_target_state_count Number of color targets
/// \param vertex_buffer_layout Vertex buffer layouts
/// \param vertex_buffer_layout_count Number of vertex buffer layouts
/// \param msaa_sample_count MSAA sample count
/// \param fragment_constants Fragment shader constants
/// \param fragment_constants_count Number of constants
/// \param primitive_topology Primitive topology
/// \return The pipeline descriptor
wgpu::RenderPipelineDescriptor create_pipeline_descriptor(
    const char* label,
    wgpu::ShaderModule* vertex_shader,
    wgpu::ShaderModule* fragment_shader,
    wgpu::PipelineLayout* pipeline_layout,
    wgpu::DepthStencilState* depth_stencil_state,
    wgpu::ColorTargetState* color_target_state,
    size_t color_target_state_count,
    wgpu::VertexBufferLayout* vertex_buffer_layout,
    size_t vertex_buffer_layout_count,
    uint32_t msaa_sample_count,
    const char** fragment_constant_names,
    double* fragment_constant_values,
    size_t fragment_constants_count,
    wgpu::PrimitiveTopology primitive_topology
);

/// Create a shape pipeline
///
/// Creates pipelines for all mask states plus stencilless.
///
/// \param name Pipeline name for debug labels
/// \param device The wgpu device
/// \param format The target texture format
/// \param shader The shader module
/// \param msaa_sample_count The MSAA sample count
/// \param vertex_buffers_layout Vertex buffer layouts
/// \param vertex_buffers_layout_count Number of layouts
/// \param bind_group_layouts Bind group layouts
/// \param bind_group_layouts_count Number of layouts
/// \param blend The blend state
/// \param push_constant_ranges Push constant ranges
/// \param push_constant_ranges_count Number of ranges
/// \param primitive_topology Primitive topology
/// \return The shape pipeline
ShapePipeline create_shape_pipeline(
    const char* name,
    wgpu::Device* device,
    wgpu::TextureFormat format,
    wgpu::ShaderModule* shader,
    uint32_t msaa_sample_count,
    wgpu::VertexBufferLayout* vertex_buffers_layout,
    size_t vertex_buffers_layout_count,
    wgpu::BindGroupLayout** bind_group_layouts,
    size_t bind_group_layouts_count,
    wgpu::BlendState blend,
    wgpu::PushConstantRange* push_constant_ranges,
    size_t push_constant_ranges_count,
    wgpu::PrimitiveTopology primitive_topology
);

} // namespace wgpu
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_WGPU_PIPELINES_H
