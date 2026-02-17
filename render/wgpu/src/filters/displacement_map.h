// C++ translation of render/wgpu/src/filters/displacement_map.rs
// WGPU Displacement Map Filter

#ifndef RUFFLE_RENDER_WGPU_FILTERS_DISPLACEMENT_MAP_H
#define RUFFLE_RENDER_WGPU_FILTERS_DISPLACEMENT_MAP_H

#include <cstdint>
#include <memory>
#include <array>

// Forward declarations for wgpu types
namespace wgpu {
    class Device;
    class Buffer;
    class BindGroupLayout;
    class PipelineLayout;
    class RenderPipeline;
    class CommandEncoder;
    class TextureView;
    class Sampler;
    class BindGroup;
    
    struct BufferDescriptor;
    struct BindGroupLayoutDescriptor;
    struct PipelineLayoutDescriptor;
    struct RenderPipelineDescriptor;
    struct BindGroupDescriptor;
    struct BindGroupEntry;
    struct RenderPassDescriptor;
    struct Extent3d;
    struct Color;
    
    enum class ShaderStages;
    enum class BindingType;
    enum class TextureSampleType;
    enum class TextureViewDimension;
    enum class SamplerBindingType;
    enum class BufferBindingType;
    enum class BufferUsage;
    enum class PrimitiveTopology;
    enum class FrontFace;
    enum class PolygonMode;
    enum class IndexFormat;
    
    template<typename T>
    struct DynamicMapping;
    
    using BufferSize = uint64_t;
}

namespace wgpu {
namespace util {
    class StagingBelt;
}
}

namespace ruffle {
namespace render {
namespace filters {
    struct DisplacementMapFilter;
    enum class DisplacementMapFilterMode;
}
}
}

namespace ruffle {
namespace render {
namespace wgpu {

// Forward declarations
class Descriptors;
class TexturePool;

namespace surface {
namespace target {
    class CommandTarget;
}
}

struct FilterSource;
struct FilterVertex;

/// Displacement map uniform structure
struct DisplacementMapUniform {
    std::array<float, 4> color;
    uint32_t components;  // 00000000 00000000 XXXXXXXX YYYYYYYY
    uint32_t mode;        // 0 wrap, 1 clamp, 2 ignore, 3 color
    float scale_x;
    float scale_y;
    float source_width;
    float source_height;
    float map_width;
    float map_height;
    float offset_x;
    float offset_y;
    float viewscale_x;
    float viewscale_y;
};

/// Displacement map filter for WGPU
class DisplacementMapFilter {
private:
    wgpu::BindGroupLayout bind_group_layout;
    wgpu::PipelineLayout pipeline_layout;
    wgpu::Buffer vertex_buffer;
    wgpu::Buffer uniform_buffer;
    wgpu::BufferSize vertices_size;
    wgpu::BufferSize uniform_size;
    
    // Sample count map for pipelines (simplified)
    std::unordered_map<uint32_t, std::shared_ptr<wgpu::RenderPipeline>> pipelines;

public:
    /// Create displacement map filter
    explicit DisplacementMapFilter(const wgpu::Device& device);

    /// Get or create pipeline for sample count
    const wgpu::RenderPipeline& get_pipeline(
        const Descriptors& descriptors, 
        uint32_t msaa_sample_count
    );

    /// Apply displacement map filter
    std::optional<surface::target::CommandTarget> apply(
        const Descriptors& descriptors,
        TexturePool& texture_pool,
        wgpu::CommandEncoder& draw_encoder,
        wgpu::util::StagingBelt& staging_belt,
        const FilterSource& source,
        const filters::DisplacementMapFilter& filter
    );

private:
    /// Helper to get texture from bitmap handle
    static const void* as_texture(const void* bitmap_handle);
};

} // namespace wgpu
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_WGPU_FILTERS_DISPLACEMENT_MAP_H
