// C++ translation of render/pixel_bender/src/tests.rs
// Pixel bender parser tests

#ifndef RUFFLE_RENDER_PIXEL_BENDER_TESTS_H
#define RUFFLE_RENDER_PIXEL_BENDER_TESTS_H

#include <vector>
#include <string>
#include <cstdint>
#include <variant>

namespace ruffle {
namespace render {
namespace pixel_bender {

// Forward declarations from parser module
enum class Opcode {
    Mov,
    Add,
    Sub,
    Mul,
    Div,
    Rcp,
    // ... other opcodes
};

enum class PixelBenderRegKind {
    Float,
    Int,
    Bool
};

struct PixelBenderRegChannel {
    enum Value { R, G, B, A };
    
    static std::vector<Value> RGBA() {
        return {R, G, B, A};
    }
};

struct PixelBenderReg {
    int index;
    std::vector<PixelBenderRegChannel::Value> channels;
    PixelBenderRegKind kind;
};

enum class PixelBenderParamQualifier {
    Input,
    Output,
    Normal
};

enum class PixelBenderTypeOpcode {
    TFloat,
    TFloat2,
    TFloat3,
    TFloat4,
    TInt,
    TInt2,
    TInt3,
    TInt4,
    TBool,
    // ... other types
};

struct PixelBenderMetadata {
    std::string key;
    std::variant<double, std::string, int> value;
    bool is_meta2;
};

struct PixelBenderParamNormal {
    PixelBenderParamQualifier qualifier;
    PixelBenderTypeOpcode param_type;
    PixelBenderReg reg;
    std::string name;
    std::vector<PixelBenderMetadata> metadata;
};

struct PixelBenderParamTexture {
    int index;
    int channels;
    std::string name;
};

using PixelBenderParam = std::variant<PixelBenderParamNormal, PixelBenderParamTexture>;

struct OperationNormal {
    Opcode opcode;
    PixelBenderReg dst;
    PixelBenderReg src;
};

struct OperationSampleNearest {
    PixelBenderReg dst;
    PixelBenderReg src;
    int tf;
};

struct OperationLoadFloat {
    PixelBenderReg dst;
    double val;
};

using Operation = std::variant<OperationNormal, OperationSampleNearest, OperationLoadFloat>;

struct PixelBenderMetadataEntry {
    std::string key;
    std::variant<double, std::string, int> value;
    bool is_meta2;
};

/// Pixel bender shader structure
struct PixelBenderShader {
    std::string name;
    int version;
    std::vector<PixelBenderParam> params;
    std::vector<PixelBenderMetadataEntry> metadata;
    std::vector<Operation> operations;
};

/// Parse a pixel bender shader from bytes
/// @param data The shader bytecode
/// @param is_meta2 Whether this is meta2 format
/// @return The parsed shader, or std::nullopt on failure
std::optional<PixelBenderShader> parse_shader(const std::vector<uint8_t>& data, bool is_meta2);

/// Test: Simple shader parsing
/// Tests parsing of a basic "DoNothing" shader that passes through pixels unchanged
void test_simple_shader();

} // namespace pixel_bender
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_PIXEL_BENDER_TESTS_H
