// C++ translation of render/wgpu/src/context3d/shader_pair.rs
// Shader pair for AGAL to Naga compilation

#ifndef RUFFLE_RENDER_WGPU_CONTEXT3D_SHADER_PAIR_H
#define RUFFLE_RENDER_WGPU_CONTEXT3D_SHADER_PAIR_H

#include <vector>
#include <array>
#include <optional>
#include <memory>
#include <string>

// Forward declarations
namespace ruffle {
namespace render_wgpu {
struct Descriptors;
}
namespace render {
    class ShaderModule;
}
}

// External dependencies
#include <wgpu/wgpu.h>
#include <naga_agal/sampler_config.h>
#include <naga_agal/vertex_attribute_format.h>
#include <lru/lru.h>
#include <cell/ref_mut.h>

namespace ruffle {
namespace render_wgpu {
namespace context3d {

/// Maximum number of vertex attributes
constexpr size_t MAX_VERTEX_ATTRIBUTES = 16;

/// Compiled shader program containing vertex and fragment modules
struct CompiledShaderProgram {
    wgpu::ShaderModule vertex_module;
    wgpu::ShaderModule fragment_module;
    wgpu::BindGroupLayout bind_group_layout;
};

/// Information about a texture used in a shader
enum class ShaderTextureInfo {
    D2,    // 2D texture
    Cube,  // Cube map texture
};

/// Data required to compile a shader
///
/// This represents all the data that needs to be passed to
/// naga_agal::agal_to_naga to compile a shader. Used as cache key.
struct ShaderCompileData {
    std::array<naga_agal::SamplerConfig, 8> sampler_configs;
    std::array<std::optional<naga_agal::VertexAttributeFormat>, MAX_VERTEX_ATTRIBUTES> vertex_attributes;
    std::array<std::optional<ShaderTextureInfo>, 8> texture_infos;

    bool operator==(const ShaderCompileData& other) const = default;
};

/// Shader pair containing AGAL vertex and fragment bytecode
///
/// Caches compiled wgpu shader modules. The cache key represents all of the data
/// that we need to pass to naga_agal::agal_to_naga to compile a shader.
class ShaderPairAgal : public ruffle::render::ShaderModule {
public:
    /// Create a new shader pair from AGAL bytecode
    /// @param vertex_bytecode Vertex shader bytecode
    /// @param fragment_bytecode Fragment shader bytecode
    ShaderPairAgal(std::vector<uint8_t> vertex_bytecode, std::vector<uint8_t> fragment_bytecode);

    /// Get the fragment sampler configurations
    /// @return Array of 8 optional sampler configurations
    const std::array<std::optional<naga_agal::SamplerConfig>, 8>& fragment_sampler_configs() const;

    /// Compile the shader for the given data
    /// @param descriptors WebGPU descriptors
    /// @param data Shader compile data
    /// @return Reference to the compiled shader program
    cell::RefMut<CompiledShaderProgram> compile(
        const Descriptors* descriptors,
        const ShaderCompileData& data);

private:
    /// Vertex shader bytecode
    std::vector<uint8_t> vertex_bytecode;

    /// Fragment shader bytecode
    std::vector<uint8_t> fragment_bytecode;

    /// Fragment sampler configurations (extracted from bytecode)
    std::array<std::optional<naga_agal::SamplerConfig>, 8> fragment_sampler_configs;

    /// Cache of compiled shader programs
    /// TODO - figure out a good size for this cache
    std::unique_ptr<lru::LruCache<ShaderCompileData, CompiledShaderProgram>> compiled_cache;
};

} // namespace context3d
} // namespace render_wgpu
} // namespace ruffle

#endif // RUFFLE_RENDER_WGPU_CONTEXT3D_SHADER_PAIR_H
