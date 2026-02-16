// C++ translation of render/wgpu/src/filters/shader.rs
// Shader filter implementation for WebGPU rendering

#ifndef RUFFLE_RENDER_WGPU_FILTERS_SHADER_H
#define RUFFLE_RENDER_WGPU_FILTERS_SHADER_H

// Forward declarations
namespace ruffle {
namespace render {
    struct ShaderFilter;
}
namespace render_wgpu {
struct Descriptors;
class TexturePool;
namespace surface {
namespace target {
    class CommandTarget;
}
}
namespace pixel_bender_support {
    struct ImageInputTexture;
    struct PixelBenderShaderArgument;
}
}
}

// External dependencies
#include <wgpu/wgpu.h>
#include <ruffle_render/filters/shader_filter.h>

namespace ruffle {
namespace render_wgpu {
namespace filters {

/// Forward declarations
struct FilterSource;

/// Shader filter for WebGPU rendering
///
/// Applies a Pixel Bender shader as a filter effect.
/// All of the data is stored in the ShaderFilterArgs.
class ShaderFilter {
public:
    /// Create a new shader filter
    ShaderFilter() = default;

    /// Apply the shader filter to a source texture
    /// @param descriptors WebGPU descriptors
    /// @param texture_pool Texture pool for temporary textures
    /// @param draw_encoder Command encoder for recording draw commands
    /// @param source Source texture and vertices
    /// @param filter Shader filter parameters from SWF
    /// @return Command target with shader effect applied
    surface::target::CommandTarget apply(
        const Descriptors* descriptors,
        TexturePool* texture_pool,
        wgpu::CommandEncoder* draw_encoder,
        const FilterSource* source,
        ruffle::render::ShaderFilter filter) const;
};

} // namespace filters
} // namespace render_wgpu
} // namespace ruffle

#endif // RUFFLE_RENDER_WGPU_FILTERS_SHADER_H
