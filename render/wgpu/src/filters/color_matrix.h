// C++ translation of render/wgpu/src/filters/color_matrix.rs
//! Color matrix filter implementation for WebGPU rendering

#ifndef RUFFLE_RENDER_WGPU_FILTERS_COLOR_MATRIX_H
#define RUFFLE_RENDER_WGPU_FILTERS_COLOR_MATRIX_H

#include <cstdint>
#include <array>
#include <optional>
#include <memory>

// Forward declarations
namespace ruffle {
namespace render_wgpu {

struct Descriptors;
class TexturePool;

namespace surface {
namespace target {
    class CommandTarget;
}
}

namespace utils {
    template<typename T>
    class SampleCountMap;
}

namespace filters {
    struct FilterSource;
    struct FilterVertex;
}

} // namespace render_wgpu
} // namespace ruffle

// External dependencies
namespace wgpu {
    class Device;
    class BindGroupLayout;
    class PipelineLayout;
    class Buffer;
    class RenderPipeline;
    class CommandEncoder;
    class TextureView;
    class BindGroup;
    struct BufferSize;
    namespace util {
        class StagingBelt;
    }
}

namespace swf {
    struct ColorMatrixFilter;
}

namespace ruffle {
namespace render_wgpu {
namespace filters {

/// Color matrix filter for WebGPU rendering
/// Applies a 4x5 color transformation matrix to textures
class ColorMatrixFilter {
public:
    /// Create a new color matrix filter
    /// @param device The WebGPU device
    explicit ColorMatrixFilter(wgpu::Device* device);

    /// Apply the color matrix filter to a source texture
    /// @param descriptors WebGPU descriptors
    /// @param texture_pool Texture pool for temporary textures
    /// @param draw_encoder Command encoder for recording draw commands
    /// @param staging_belt Staging belt for buffer uploads
    /// @param source Source texture and vertices
    /// @param filter Color matrix filter parameters from SWF
    /// @return Command target with filtered result
    CommandTarget apply(
        const Descriptors* descriptors,
        TexturePool* texture_pool,
        wgpu::CommandEncoder* draw_encoder,
        wgpu::util::StagingBelt* staging_belt,
        const FilterSource* source,
        const swf::ColorMatrixFilter& filter);

private:
    /// Get or create the render pipeline for the given sample count
    /// @param descriptors WebGPU descriptors
    /// @param msaa_sample_count The MSAA sample count
    /// @return The render pipeline
    const wgpu::RenderPipeline* pipeline(
        const Descriptors* descriptors,
        uint32_t msaa_sample_count) const;

    // Pipeline cache for different sample counts
    utils::SampleCountMap<std::optional<wgpu::RenderPipeline>> pipelines_;

    // Pipeline layout
    wgpu::PipelineLayout pipeline_layout_;

    // Vertex buffer for filter quads
    wgpu::Buffer vertex_buffer_;

    // Uniform buffer for color matrix (20 floats = 4x5 matrix)
    wgpu::Buffer uniform_buffer_;

    // Bind group layout
    wgpu::BindGroupLayout bind_group_layout_;

    // Buffer sizes
    wgpu::BufferSize vertices_size_;
    wgpu::BufferSize uniform_size_;
};

} // namespace filters
} // namespace render_wgpu
} // namespace ruffle

#endif // RUFFLE_RENDER_WGPU_FILTERS_COLOR_MATRIX_H
