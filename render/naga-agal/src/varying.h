// C++ translation of render/naga-agal/src/varying.rs
// Naga AGAL Varying Registers

#ifndef RUFFLE_RENDER_NAGA_AGAL_VARYING_H
#define RUFFLE_RENDER_NAGA_AGAL_VARYING_H

#include <vector>
#include <optional>
#include <cstdint>

// Forward declarations for naga types
namespace naga {
    template<typename T>
    using Handle = uint32_t;  // Simplified handle type
    
    struct Expression;
    struct FunctionArgument;
    struct LocalVariable;
    struct StructMember;
    struct Type;
    struct TypeInner;
    struct Binding;
    enum class Interpolation;
    struct Span;
}

namespace ruffle {
namespace render {
namespace naga_agal {

/// Error type for Naga builder
class Error {
    // Error implementation
};

template<typename T>
using Result = std::expected<T, Error>;

/// Shader type enumeration
enum class ShaderType {
    Vertex,
    Fragment
};

/// Varying register structure
struct VaryingRegister {
    naga::Handle<naga::Expression> expr_local_variable;
    std::optional<size_t> output_struct_index;
};

/// Varying registers container
class VaryingRegisters {
public:
    // Whenever we write to a varying register in a vertex shader
    // or read from a varying register in a fragment shader
    // (for the first time), we store the created `Expression` here.
    // See `get_varying_pointer`
    std::vector<std::optional<VaryingRegister>> varying_pointers;
};

/// Naga builder context (simplified)
class NagaBuilder {
private:
    struct ShaderConfig {
        ShaderType shader_type;
    } shader_config;
    
    VaryingRegisters varying_registers;
    
    // Simplified function structure
    struct Function {
        std::vector<naga::LocalVariable> local_variables;
        std::vector<naga::Expression> expressions;
        std::vector<naga::FunctionArgument> arguments;
    } func;
    
    naga::Handle<naga::Type> vec4f;
    naga::Handle<naga::Type> return_type;
    naga::Handle<naga::Expression> dest;

public:
    /// Get varying pointer by index
    Result<naga::Handle<naga::Expression>> get_varying_pointer(size_t index);

    /// Builds the final output struct expression, using the 'main' output (a position or color)
    /// and any varying registers that were written to (if this is a vertex shader)
    Result<naga::Handle<naga::Expression>> build_output_expr(naga::Handle<naga::Type> return_ty);

private:
    /// Helper to evaluate expression
    naga::Handle<naga::Expression> evaluate_expr(const naga::Expression& expr);
    
    /// Helper to emit varying load
    Result<naga::Handle<naga::Expression>> emit_varying_load(size_t index);
};

} // namespace naga_agal
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_NAGA_AGAL_VARYING_H
