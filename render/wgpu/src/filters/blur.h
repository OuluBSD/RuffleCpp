// C++ translation of render/wgpu/src/filters/blur.rs
// Blur filter implementation for WebGPU rendering

#ifndef RUFFLE_RENDER_WGPU_FILTERS_BLUR_H
#define RUFFLE_RENDER_WGPU_FILTERS_BLUR_H

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
}
}

// External dependencies
#include <wgpu/wgpu.h>
#include <wgpu/util/staging_belt.h>
#include <swf/blur_filter.h>
#include <bytemuck/bytemuck.h>
#include <sync/once_lock.h>

namespace ruffle {
namespace render_wgpu {
namespace filters {

/// This is a 1:1 match of `struct Filter` in `blur.wgsl`. See that, and the usage below, for more info.
/// Since WebGL requires 16 byte struct size (alignment), some of these fields (namely m2 and last_weight)
/// are passed in precomputed, even though they are trivial to get (addition/multiplication by constant).
/// The struct would have to be padded with dummy data otherwise anyway - these are at least useful.
struct BlurUniform {
    float direction[2];
    float full_size;
    float m;
    float m2;
    float first_weight;
    float last_offset;
    float last_weight;
    
    bool operator==(const BlurUniform& other) const = default;
};

/// Blur filter for WebGPU rendering
class BlurFilter {
public:
    /// Create a new blur filter
    /// @param device The WebGPU device
    explicit BlurFilter(wgpu::Device* device);

    /// Apply the blur filter to a source texture
    /// @param descriptors WebGPU descriptors
    /// @param texture_pool Texture pool for temporary textures
    /// @param draw_encoder Command encoder for recording draw commands
    /// @param staging_belt Staging belt for buffer uploads
    /// @param source Source texture and vertices
    /// @param filter Blur filter parameters from SWF
    /// @return Optional command target with blurred result, or nullopt if no blur needed
    std::optional<surface::target::CommandTarget> apply(
        const Descriptors* descriptors,
        TexturePool* texture_pool,
        wgpu::CommandEncoder* draw_encoder,
        wgpu::util::StagingBelt* staging_belt,
        const FilterSource* source,
        const swf::BlurFilter& filter);

private:
    /// Get or create the render pipeline for the given sample count
    const wgpu::RenderPipeline* pipeline(
        const Descriptors* descriptors,
        uint32_t msaa_sample_count) const;

    /// Render with uniform buffers
    void render_with_uniform_buffers(
        const Descriptors* descriptors,
        wgpu::CommandEncoder* draw_encoder,
        const wgpu::RenderPipeline* pipeline,
        surface::target::CommandTarget* destination,
        const wgpu::TextureView* source,
        wgpu::BufferSlice vertices);

    // Pipeline cache for different sample counts
    utils::SampleCountMap<std::optional<wgpu::RenderPipeline>> pipelines;
    
    // Pipeline layout
    wgpu::PipelineLayout pipeline_layout;
    
    // Vertex buffer for filter quads
    wgpu::Buffer vertex_buffer;
    
    // Uniform buffer for blur parameters
    wgpu::Buffer uniform_buffer;
    
    // Bind group layout
    wgpu::BindGroupLayout bind_group_layout;
    
    // Buffer sizes
    wgpu::BufferSize vertices_size;
    wgpu::BufferSize uniform_size;
};

} // namespace filters
} // namespace render_wgpu
} // namespace ruffle

#endif // RUFFLE_RENDER_WGPU_FILTERS_BLUR_H
