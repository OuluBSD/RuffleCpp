// C++ translation of render/wgpu/src/filters/glow.rs
// Glow filter implementation for WebGPU rendering

#ifndef RUFFLE_RENDER_WGPU_FILTERS_GLOW_H
#define RUFFLE_RENDER_WGPU_FILTERS_GLOW_H

#include <cstdint>
#include <array>
#include <optional>

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
#include <swf/glow_filter.h>

namespace ruffle {
namespace render_wgpu {
namespace filters {

// Forward declarations
struct FilterSource;
struct FilterVertexWithBlur;
class BlurFilter;

/// Uniform data for glow filter shader
/// Matches the GlowUniform struct in the WGSL shader
struct GlowUniform {
    std::array<float, 4> color;
    float strength;
    uint32_t inner;            // Boolean flag as u32 for alignment
    uint32_t knockout;         // Boolean flag as u32 for alignment
    uint32_t composite_source; // Boolean flag as u32 for alignment

    bool operator==(const GlowUniform& other) const = default;
};

/// Glow filter for WebGPU rendering
///
/// Applies a glow effect to bitmap data using a blur-based approach.
class GlowFilter {
public:
    /// Create a new glow filter
    /// @param device The WebGPU device
    explicit GlowFilter(wgpu::Device* device);

    /// Apply the glow filter to a source texture
    /// @param descriptors WebGPU descriptors
    /// @param texture_pool Texture pool for temporary textures
    /// @param draw_encoder Command encoder for recording draw commands
    /// @param staging_belt Staging belt for buffer uploads
    /// @param source Source texture and vertices
    /// @param filter Glow filter parameters from SWF
    /// @param blur_filter Blur filter for the glow effect
    /// @param blur_offset Offset for blur vertices
    /// @return Command target with glow effect applied
    surface::target::CommandTarget apply(
        const Descriptors* descriptors,
        TexturePool* texture_pool,
        wgpu::CommandEncoder* draw_encoder,
        wgpu::util::StagingBelt* staging_belt,
        const FilterSource* source,
        const swf::GlowFilter& filter,
        const BlurFilter* blur_filter,
        std::pair<float, float> blur_offset) const;

private:
    /// Get or create the render pipeline for the given sample count
    /// @param descriptors WebGPU descriptors
    /// @param msaa_sample_count MSAA sample count
    /// @return The render pipeline for glow filtering
    const wgpu::RenderPipeline* pipeline(
        const Descriptors* descriptors,
        uint32_t msaa_sample_count) const;

    // Pipeline cache for different sample counts
    utils::SampleCountMap<std::optional<wgpu::RenderPipeline>> pipeline_cache;

    // Pipeline layout
    wgpu::PipelineLayout pipeline_layout;

    // Vertex buffer for filter quads
    wgpu::Buffer vertex_buffer;

    // Uniform buffer for glow parameters
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

#endif // RUFFLE_RENDER_WGPU_FILTERS_GLOW_H
