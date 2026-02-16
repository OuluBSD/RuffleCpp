// C++ translation of render/naga-agal/tests/wgsl.rs
// Test utilities for converting AGAL shaders to WGSL

#ifndef RUFFLE_RENDER_NAGA_AGAL_TESTS_WGSL_H
#define RUFFLE_RENDER_NAGA_AGAL_TESTS_WGSL_H

#include <string>
#include <vector>
#include <array>
#include <cstdint>

// Forward declarations
namespace naga {
    class Module;
    namespace valid {
        class Validator;
        enum class ValidationFlags : uint32_t;
        enum class Capabilities : uint32_t;
    }
    namespace back {
    namespace wgsl {
        class Writer;
        enum class WriterFlags : uint32_t;
    }
    }
}

namespace naga_agal {
    enum class VertexAttributeFormat;
    
    /// Convert AGAL shader to Naga module
    /// @param shader The AGAL shader bytes
    /// @param attrs The vertex attribute formats (8 slots)
    /// @param constants The shader constants
    /// @return The Naga module
    Module* agal_to_naga(
        const std::vector<uint8_t>& shader,
        const std::array<std::optional<VertexAttributeFormat>, 8>& attrs,
        const std::array<void*, 8>& constants
    );
}

namespace ruffle {
namespace render {
namespace naga_agal {
namespace tests {

/// Shader type enumeration
enum class ShaderType {
    Vertex,
    Fragment,
};

/// Convert a Naga module to WGSL string
/// @param module The Naga module to convert
/// @return The WGSL shader source code
std::string to_wgsl(const naga::Module* module);

/// Test a shader by converting AGAL to WGSL and comparing with snapshot
/// This is a macro in Rust, implemented as a function template in C++
/// @param shader The AGAL shader bytes
/// @param attrs The vertex attribute formats
/// @param shader_type The type of shader (vertex or fragment)
void test_shader(
    const std::vector<uint8_t>& shader,
    const std::array<std::optional<naga_agal::VertexAttributeFormat>, 8>& attrs,
    ShaderType shader_type
);

} // namespace tests
} // namespace naga_agal
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_NAGA_AGAL_TESTS_WGSL_H
