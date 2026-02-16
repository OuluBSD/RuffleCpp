// C++ translation of render/naga-agal/src/lib.rs
//! Naga AGAL shader compiler

#ifndef RUFFLE_RENDER_NAGA_AGAL_LIB_H
#define RUFFLE_RENDER_NAGA_AGAL_LIB_H

#include <cstdint>
#include <optional>
#include <string>
#include <vector>
#include <array>
#include <memory>

// Forward declarations
namespace naga {
    class Module;
}

namespace ruffle {
namespace render {
namespace naga_agal {

/// Shader entry point name
constexpr const char* SHADER_ENTRY_POINT = "main";

/// Maximum number of vertex attributes
constexpr size_t MAX_VERTEX_ATTRIBUTES = 8;

/// Maximum number of textures
constexpr size_t MAX_TEXTURES = 8;

/// Vertex attribute format
enum class VertexAttributeFormat {
    Float1,
    Float2,
    Float3,
    Float4,
    Bytes4,
};

/// Shader type
enum class ShaderType {
    Vertex,
    Fragment,
};

/// Error types for AGAL compilation
class Error {
public:
    enum class Kind {
        InvalidHeader,
        InvalidShaderType,
        MissingVertexAttributeData,
        Unimplemented,
        ReadError,
        InvalidOpcode,
        InvalidVersion,
        SamplerConfigMismatch,
    };

    Error(Kind kind) : kind_(kind) {}
    Error(Kind kind, const std::string& message) : kind_(kind), message_(message) {}
    Error(Kind kind, size_t index) : kind_(kind), index_(index) {}
    Error(Kind kind, uint8_t shader_type) : kind_(kind), shader_type_(shader_type) {}
    Error(Kind kind, size_t texture, const class SamplerConfig& old, const class SamplerConfig& new_)
        : kind_(kind), index_(texture) {}

    Kind kind() const { return kind_; }
    const std::string& message() const { return message_; }
    size_t index() const { return index_; }
    uint8_t shader_type() const { return shader_type_; }

private:
    Kind kind_;
    std::string message_;
    size_t index_;
    uint8_t shader_type_;
};

/// Sampler configuration
class SamplerConfig {
    // Forward declaration - full definition in types.h
};

/// Filter mode
enum class Filter {
    Nearest,
    Linear,
};

/// Mipmap mode
enum class Mipmap {
    None,
    Nearest,
    Linear,
};

/// Wrapping mode
enum class Wrapping {
    Clamp,
    Repeat,
};

/// Re-export types from types.h
#include "types.h"

/// Compile an Adobe AGAL shader to a Naga Module
///
/// The `vertex_attributes` parameter is only used when compiling
/// a vertex shader.
///
/// The returning Module can be passed directly to `wgpu`,
/// or compiled to a particular shader language using a `naga` backend.
///
/// The shader entrypoint is always named `main`.
///
/// # Vertex Shader
///
/// * Vertex attributes - AGAL supports up to 8 vertex attributes,
///   stored in `va0` to `va7`. You must provide the format of each attribute
///   in the corresponding entry in the `vertex_attributes` array.
///   Each attribute is mapped to the corresponding binding in the Naga shader
///   - for example, va3 will have binding id 3.
///
/// * Vertex output - An AGAL vertex shader has one main output (a vec4 position),
///   and 8 varying outputs. The main output is mapped to the Naga 'Position' output,
///   while each *used* varying register is mapped to a corresponding field in
///   the Naga output struct. For example, if a vertex shader uses varying registers
///   2 and 5, then the Naga output struct type will have two members, with binding ids 2 and 5.
///   If a shader does not write to a varying register, then it is not included in the
///   Naga output struct type.
///
/// * Program constants - An AGAL vertex shader has access to 128 program constants.
///   These are mapped to a single Naga uniform buffer, with a binding id of 0.
///   Each program constant is a vec4, and are stored in increasing order of register number.
///
/// # Fragment Shader
///
/// * Fragment input - An AGAL fragment shader can read from the 8 varying registers
///   set by the fragment shader. Each *used* varying register is mapped to a corresponding
///   binding in the Naga input type. For example, if a fragment shader uses varying registers
///   2 and 5, then the Naga input type will have two members, with binding ids 2 and 5.
///
/// * Program constants - An AGAL fragment shader has access to 28 program constants.
///   These are mapped to a single Naga uniform buffer, with a binding id of 1.
///
/// @param agal The AGAL shader bytecode
/// @param vertex_attributes Array of vertex attribute formats (used for vertex shaders)
/// @param sampler_configs Array of sampler configurations
/// @return The compiled Naga module or an error
std::optional<naga::Module> agal_to_naga(
    const std::vector<uint8_t>& agal,
    const std::array<std::optional<VertexAttributeFormat>, MAX_VERTEX_ATTRIBUTES>& vertex_attributes,
    const std::array<SamplerConfig, MAX_TEXTURES>& sampler_configs
);

/// Extract sampler configurations from AGAL bytecode
/// @param agal The AGAL shader bytecode
/// @return Array of sampler configurations or error
std::optional<std::array<SamplerConfig, MAX_TEXTURES>> extract_sampler_configs(
    const std::vector<uint8_t>& agal
);

} // namespace naga_agal
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_NAGA_AGAL_LIB_H
