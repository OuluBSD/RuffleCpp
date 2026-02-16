// C++ translation of render/wgpu/src/filters/bevel.rs
// Bevel filter implementation for WebGPU rendering

#ifndef RUFFLE_RENDER_WGPU_FILTERS_BEVEL_H
#define RUFFLE_RENDER_WGPU_FILTERS_BEVEL_H

#include <cstdint>
#include <array>
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
}
}

// External dependencies
#include <wgpu/wgpu.h>
#include <wgpu/util/staging_belt.h>
#include <swf/bevel_filter.h>
#include <sync/once_lock.h>
#include <bytemuck/bytemuck.h>

namespace ruffle {
namespace render_wgpu {
namespace filters {

/// Forward declarations
struct FilterSource;
struct FilterVertexWithDoubleBlur;
class BlurFilter;

/// Uniform data for bevel filter shader
///
/// Matches the BevelUniform struct in the WGSL shader.
struct BevelUniform {
    std::array<float, 4> highlight_color;
    std::array<float, 4> shadow_color;
    float strength;
    uint32_t bevel_type;       // 0 = outer, 1 = inner, 2 = full
    uint32_t knockout;         // Boolean flag as u32 for alignment
    uint32_t composite_source; // Boolean flag for undocumented flash feature

    bool operator==(const BevelUniform& other) const = default;
};

/// Bevel filter for WebGPU rendering
///
/// Applies a bevel effect to bitmap data using a blur-based approach.
class BevelFilter {
public:
    /// Create a new bevel filter
    /// @param device The WebGPU device
    explicit BevelFilter(wgpu::Device* device);

    /// Apply the bevel filter to a source texture
    /// @param descriptors WebGPU descriptors
    /// @param texture_pool Texture pool for temporary textures
    /// @param draw_encoder Command encoder for recording draw commands
    /// @param staging_belt Staging belt for buffer uploads
    /// @param source Source texture and vertices
    /// @param filter Bevel filter parameters from SWF
    /// @param blur_filter Blur filter for the bevel effect
    /// @return Command target with bevel effect applied
    surface::target::CommandTarget apply(
        const Descriptors* descriptors,
        TexturePool* texture_pool,
        wgpu::CommandEncoder* draw_encoder,
        wgpu::util::StagingBelt* staging_belt,
        const FilterSource* source,
        const swf::BevelFilter& filter,
        const BlurFilter* blur_filter) const;

private:
    /// Get or create the render pipeline for the given sample count
    /// @param descriptors WebGPU descriptors
    /// @param msaa_sample_count MSAA sample count
    /// @return The render pipeline for bevel filtering
    const wgpu::RenderPipeline* pipeline(
        const Descriptors* descriptors,
        uint32_t msaa_sample_count) const;

    // Pipeline cache for different sample counts
    utils::SampleCountMap<std::unique_ptr<sync::OnceLock<wgpu::RenderPipeline>>> pipeline_cache;

    // Pipeline layout
    wgpu::PipelineLayout pipeline_layout;

    // Vertex buffer for filter quads
    wgpu::Buffer vertex_buffer;

    // Uniform buffer for bevel parameters
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

#endif // RUFFLE_RENDER_WGPU_FILTERS_BEVEL_H
