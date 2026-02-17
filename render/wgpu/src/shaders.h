// C++ translation of render/wgpu/src/shaders.rs
// WGPU Shaders module

#ifndef RUFFLE_RENDER_WGPU_SHADERS_H
#define RUFFLE_RENDER_WGPU_SHADERS_H

#include <string_view>
#include <unordered_map>
#include <memory>

// Forward declarations for wgpu types
namespace wgpu {
    class Device;
    class ShaderModule;
}

namespace ruffle {
namespace render {
namespace wgpu {
namespace blend {
    enum class ComplexBlend;
}
}
}
}

namespace ruffle {
namespace render {
namespace wgpu {

// Shader filter common WGSL code (from ruffle_render::shader_source)
// This would be included from the shader_source module
extern const char* SHADER_FILTER_COMMON;

/// Shaders container for WGPU rendering
class Shaders {
public:
    std::shared_ptr<wgpu::ShaderModule> color_shader;
    
    /// This has a pipeline-overridable `bool` constant, `late_saturate`,
    /// with a default of `false`. It switches to performing saturation
    /// after re-multiplying the alpha, rather than before. This is used
    /// for the Stage3D `bitmap_opaque` pipeline, which needs to be able to
    /// avoid changing initially-in-range rgb values (regadless of whether
    /// dividing by the alpha value would produce an out-of-range value).
    std::shared_ptr<wgpu::ShaderModule> bitmap_shader;
    
    std::shared_ptr<wgpu::ShaderModule> gradient_shader;
    std::shared_ptr<wgpu::ShaderModule> copy_srgb_shader;
    std::shared_ptr<wgpu::ShaderModule> copy_shader;
    std::shared_ptr<wgpu::ShaderModule> alpha_mask_shader;
    
    std::unordered_map<blend::ComplexBlend, std::shared_ptr<wgpu::ShaderModule>> blend_shaders;
    
    std::shared_ptr<wgpu::ShaderModule> color_matrix_filter;
    std::shared_ptr<wgpu::ShaderModule> blur_filter;
    std::shared_ptr<wgpu::ShaderModule> glow_filter;
    std::shared_ptr<wgpu::ShaderModule> bevel_filter;
    std::shared_ptr<wgpu::ShaderModule> displacement_map_filter;

    /// Create shaders from device
    static Shaders create(const wgpu::Device& device);

private:
    /// Helper to create a shader module
    static std::shared_ptr<wgpu::ShaderModule> make_shader(
        const wgpu::Device& device, 
        const char* name, 
        const char* source
    );
    
    /// Helper to create a filter shader module
    static std::shared_ptr<wgpu::ShaderModule> make_filter_shader(
        const wgpu::Device& device, 
        const char* name, 
        const char* source
    );
};

} // namespace wgpu
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_WGPU_SHADERS_H
