// C++ translation of render/wgpu/src/context3d/current_pipeline.rs
// Current pipeline state management for WGPU Context3D

#ifndef RUFFLE_RENDER_WGPU_CONTEXT3D_CURRENT_PIPELINE_H
#define RUFFLE_RENDER_WGPU_CONTEXT3D_CURRENT_PIPELINE_H

#include <cstdint>
#include <cstddef>
#include <array>
#include <optional>
#include <memory>
#include <vector>
#include <cell>

// Forward declarations
namespace naga {
    struct Module;
    namespace valid {
        struct Capabilities;
        struct ValidationFlags;
        class Validator;
    }
    namespace back {
        namespace wgsl {
            class Writer;
            struct WriterFlags;
        }
    }
}

namespace naga_agal {
    struct Filter;
    struct SamplerConfig;
    struct Wrapping;
    struct Mipmap;
    enum class VertexAttributeFormat;
    constexpr size_t TEXTURE_START_BIND_INDEX = 0;
    constexpr size_t TEXTURE_SAMPLER_START_BIND_INDEX = 0;
    constexpr const char* SHADER_ENTRY_POINT = "main";
}

namespace ruffle {
namespace render {
namespace backend {
    enum class Context3DTextureFilter;
    enum class Context3DTriangleFace;
    enum class Context3DVertexBufferFormat;
    enum class Context3DWrapMode;
    class Texture;
}
}
}

namespace wgpu {
    class Device;
    class Buffer;
    class TextureView;
    class Sampler;
    class BindGroupLayout;
    class PipelineLayout;
    class RenderPipeline;
    class BindGroup;
    class BindGroupEntry;
    class BindingResource;
    class BufferBinding;
    struct BufferDescriptor;
    struct RenderPipelineDescriptor;
    struct PipelineLayoutDescriptor;
    struct BindGroupDescriptor;
    struct VertexState;
    struct FragmentState;
    struct ColorTargetState;
    struct BlendState;
    struct BlendComponent;
    struct PrimitiveState;
    struct DepthStencilState;
    struct StencilFaceState;
    struct MultisampleState;
    struct VertexAttribute;
    struct VertexBufferLayout;
    enum class BufferUsages;
    enum class TextureFormat;
    enum class CompareFunction;
    enum class ColorWrites;
    enum class Face;
    enum class VertexStepMode;
    enum class VertexFormat;
    enum class AddressMode;
    enum class FrontFace;
    struct Color;
}

namespace ruffle {
namespace render {
namespace wgpu {

// Forward declarations
class Descriptors;
class VertexBufferWrapper;
struct ShaderCompileData;
enum class ShaderTextureInfo;
struct WgpuSamplerConfig;

namespace bitmaps {
    using WgpuSamplerConfig = ruffle::render::wgpu::WgpuSamplerConfig;
}

namespace context3d {
namespace shader_pair {
    using ShaderCompileData = ruffle::render::wgpu::ShaderCompileData;
    using ShaderTextureInfo = ruffle::render::wgpu::ShaderTextureInfo;
}
}

constexpr size_t MAX_VERTEX_ATTRIBUTES = 16;

// Vertex attribute information
struct VertexAttributeInfo {
    std::shared_ptr<VertexBufferWrapper> buffer;
    Context3DVertexBufferFormat format;
    uint32_t offset_in_32bit_units;
};

// Bound texture data
struct BoundTextureData {
    /// This is used to allow us to remove a bound texture when
    /// it's used with `setRenderToTexture`. The actual shader binding
    /// uses `view`
    std::shared_ptr<ruffle::render::backend::Texture> id;
    wgpu::TextureView view;
    bool cube;
};

/// Current pipeline state for WGPU Context3D
/// 
/// The flash Context3D API is similar to OpenGL - it has many methods
/// which modify the current state (`setVertexBufferAt`, `setCulling`, etc.)
/// These methods can be called at any time.
///
/// In WGPU, this state is associated by a `RenderPipeline` object,
/// which needs to be rebuilt whenever the state changes.
///
/// To match up these APIs, we store the current state in `CurrentPipeline`.
/// Whenever a state-changing `Context3DCommand` is executed, we mark the `CurrentPipeline`
/// as dirty. When a `wgpu::RenderPipeline` is actually needed by `drawTriangles`,
/// we build a new `wgpu::RenderPipeline` from the `CurrentPipeline` state (if it's dirty).
///
/// The `CurrentPipeline` state (including the compiled `wgpu::RenderPipeline`) is stored
/// in `WgpuContext3D`, and is re-used across calls to `present`. Due to lifetime issues,
/// we don't actually store the `wgpu::RenderPipeline` in `CurrentPipeline` - it's
/// instead stored in `WgpuContext3D`.
class CurrentPipeline {
private:
    std::shared_ptr<ShaderPairAgal> shaders;
    Context3DTriangleFace culling;
    std::array<std::optional<BoundTextureData>, 8> bound_textures;
    wgpu::Buffer vertex_shader_uniforms;
    wgpu::Buffer fragment_shader_uniforms;
    bool has_depth_texture;
    wgpu::ColorWrites color_mask;
    bool depth_mask;
    wgpu::CompareFunction pass_compare_mode;
    wgpu::BlendComponent color_component;
    wgpu::BlendComponent alpha_component;
    uint32_t sample_count;
    wgpu::TextureFormat target_format;
    mutable std::cell::cell<bool> dirty;
    std::array<naga_agal::SamplerConfig, 8> sampler_configs;

    static constexpr uint64_t AGAL_NUM_VERTEX_CONSTANTS = 128;
    static constexpr uint64_t AGAL_NUM_FRAGMENT_CONSTANTS = 28;
    static constexpr uint64_t AGAL_FLOATS_PER_REGISTER = 4;

    static constexpr uint64_t VERTEX_SHADER_UNIFORMS_BUFFER_SIZE =
        AGAL_NUM_VERTEX_CONSTANTS * AGAL_FLOATS_PER_REGISTER * sizeof(float);
    static constexpr uint64_t FRAGMENT_SHADER_UNIFORMS_BUFFER_SIZE =
        AGAL_NUM_FRAGMENT_CONSTANTS * AGAL_FLOATS_PER_REGISTER * sizeof(float);

public:
    /// Create a new CurrentPipeline
    CurrentPipeline(const Descriptors* descriptors);

    /// Set the shaders for this pipeline
    void set_shaders(std::optional<std::shared_ptr<ShaderPairAgal>> shaders);

    /// Update texture at a specific index
    void update_texture_at(size_t index, std::optional<BoundTextureData> texture);

    /// Remove a texture from all bound slots
    void remove_texture(const std::shared_ptr<ruffle::render::backend::Texture>& texture);

    /// Update vertex buffer at a specific index
    void update_vertex_buffer_at(size_t index);

    /// Update color mask
    void update_color_mask(wgpu::ColorWrites color_mask);

    /// Update depth settings
    void update_depth(bool depth_mask, wgpu::CompareFunction pass_compare_mode);

    /// Update has_depth_texture flag
    void update_has_depth_texture(bool has_depth_texture);

    /// Update sample count
    void update_sample_count(uint32_t sample_count);

    /// Update target format
    void update_target_format(wgpu::TextureFormat format);

    /// If the pipeline is dirty, recompiles it and returns freshly compiled pipeline
    /// Otherwise, returns std::nullopt.
    std::optional<std::pair<wgpu::RenderPipeline, wgpu::BindGroup>> rebuild_pipeline(
        const Descriptors* descriptors,
        const std::array<std::optional<VertexAttributeInfo>, MAX_VERTEX_ATTRIBUTES>& vertex_attributes
    ) const;

    /// Set culling mode
    void set_culling(Context3DTriangleFace face);

    /// Update blend factors
    void update_blend_factors(
        wgpu::BlendComponent color_component,
        wgpu::BlendComponent alpha_component
    );

    /// Update sampler state at a specific index
    void update_sampler_state_at(
        size_t sampler,
        ruffle::render::backend::Context3DWrapMode wrap,
        ruffle::render::backend::Context3DTextureFilter filter
    );

    /// Check if pipeline is dirty
    bool is_dirty() const { return dirty.get(); }

    /// Get bound textures
    const std::array<std::optional<BoundTextureData>, 8>& get_bound_textures() const {
        return bound_textures;
    }

    /// Get sampler configs
    const std::array<naga_agal::SamplerConfig, 8>& get_sampler_configs() const {
        return sampler_configs;
    }

    /// Get shaders
    const std::optional<std::shared_ptr<ShaderPairAgal>>& get_shaders() const {
        return shaders;
    }

    /// Get culling mode
    Context3DTriangleFace get_culling() const { return culling; }

    /// Get color mask
    wgpu::ColorWrites get_color_mask() const { return color_mask; }

    /// Get depth mask
    bool get_depth_mask() const { return depth_mask; }

    /// Get pass compare mode
    wgpu::CompareFunction get_pass_compare_mode() const { return pass_compare_mode; }

    /// Get sample count
    uint32_t get_sample_count() const { return sample_count; }

    /// Get target format
    wgpu::TextureFormat get_target_format() const { return target_format; }

    /// Get vertex shader uniforms buffer
    const wgpu::Buffer& get_vertex_shader_uniforms() const { return vertex_shader_uniforms; }

    /// Get fragment shader uniforms buffer
    const wgpu::Buffer& get_fragment_shader_uniforms() const { return fragment_shader_uniforms; }

    /// Check if has depth texture
    bool has_depth_texture() const { return has_depth_texture; }
};

// Helper function to convert naga module to WGSL (useful for debugging shader issues)
[[maybe_unused]]
static std::string to_wgsl(const naga::Module* module) {
    std::string out;
    
    // Validation would go here
    // naga::valid::Validator validator(ValidationFlags::all(), Capabilities::all());
    // auto module_info = validator.validate(module);
    
    // Writing would go here
    // naga::back::wgsl::Writer writer(&out, naga::back::wgsl::WriterFlags::EXPLICIT_TYPES);
    // writer.write(module, &module_info);
    
    return out;
}

} // namespace wgpu
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_WGPU_CONTEXT3D_CURRENT_PIPELINE_H
