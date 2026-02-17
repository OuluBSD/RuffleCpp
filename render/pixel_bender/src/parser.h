// C++ translation of render/pixel_bender/src/parser.rs
// Pixel Bender bytecode parsing code
// This is heavily based on https://github.com/jamesward/pbjas and https://github.com/HaxeFoundation/format/tree/master/format/pbj

#ifndef RUFFLE_RENDER_PIXEL_BENDER_PARSER_H
#define RUFFLE_RENDER_PIXEL_BENDER_PARSER_H

#include <string>
#include <vector>
#include <array>
#include <memory>
#include <cstdint>
#include <variant>
#include <optional>
#include <stdexcept>
#include <iostream>

namespace ruffle {
namespace render {
namespace pixel_bender {
namespace parser {

/// The name of a special parameter, which gets automatically filled in with the coordinates
/// of the pixel being processed.
inline constexpr const char* OUT_COORD_NAME = "_OutCoord";

// Forward declarations
struct PixelBenderReg;
struct PixelBenderMetadata;
struct PixelBenderShader;

/// Pixel Bender parsing error types
enum class PixelBenderParsingError {
    InvalidConditionalKind,
    IncompatibleRegisterKinds,
    UnsupportedRegisterKinds,
    MissingOutputParameter,
    IoError,
    Utf8Error,
    UnknownOpcode,
    UnexpectedMetaType,
    UnexpectedParamType
};

/// Exception class for Pixel Bender parsing errors
class PixelBenderParseException : public std::runtime_error {
public:
    explicit PixelBenderParseException(PixelBenderParsingError err)
        : std::runtime_error("Pixel Bender parsing error")
        , error_type_(err) {}
    
    PixelBenderParseException(PixelBenderParsingError err, const std::string& msg)
        : std::runtime_error("Pixel Bender parsing error: " + msg)
        , error_type_(err) {}
    
    PixelBenderParsingError error_type() const { return error_type_; }
    
private:
    PixelBenderParsingError error_type_;
};

/// Pixel Bender type opcodes
enum class PixelBenderTypeOpcode : uint8_t {
    TFloat = 0x1,
    TFloat2 = 0x2,
    TFloat3 = 0x3,
    TFloat4 = 0x4,
    TFloat2x2 = 0x5,
    TFloat3x3 = 0x6,
    TFloat4x4 = 0x7,
    TInt = 0x8,
    TInt2 = 0x9,
    TInt3 = 0xA,
    TInt4 = 0xB,
    TString = 0xC,
    TBool = 0xD,
    TBool2 = 0xE,
    TBool3 = 0xF,
    TBool4 = 0x10,
};

/// Pixel Bender type variant holding actual values
struct PixelBenderType {
    using FloatValue = float;
    using Float2Value = std::array<float, 2>;
    using Float3Value = std::array<float, 3>;
    using Float4Value = std::array<float, 4>;
    using Float2x2Value = std::array<float, 4>;
    using Float3x3Value = std::array<float, 9>;
    using Float4x4Value = std::array<float, 16>;
    using IntValue = int16_t;
    using Int2Value = std::array<int16_t, 2>;
    using Int3Value = std::array<int16_t, 3>;
    using Int4Value = std::array<int16_t, 4>;
    using StringValue = std::string;
    using BoolValue = int16_t;
    using Bool2Value = std::array<int16_t, 2>;
    using Bool3Value = std::array<int16_t, 3>;
    using Bool4Value = std::array<int16_t, 4>;
    
    std::variant<
        FloatValue, Float2Value, Float3Value, Float4Value,
        Float2x2Value, Float3x3Value, Float4x4Value,
        IntValue, Int2Value, Int3Value, Int4Value,
        StringValue,
        BoolValue, Bool2Value, Bool3Value, Bool4Value
    > value;
    
    PixelBenderTypeOpcode type_opcode;
    
    PixelBenderType() : type_opcode(PixelBenderTypeOpcode::TFloat) {}
    
    // Helper methods to access values
    float as_float() const { return std::get<FloatValue>(value); }
    Float2Value as_float2() const { return std::get<Float2Value>(value); }
    Float3Value as_float3() const { return std::get<Float3Value>(value); }
    Float4Value as_float4() const { return std::get<Float4Value>(value); }
    Float2x2Value as_float2x2() const { return std::get<Float2x2Value>(value); }
    Float3x3Value as_float3x3() const { return std::get<Float3x3Value>(value); }
    Float4x4Value as_float4x4() const { return std::get<Float4x4Value>(value); }
    int16_t as_int() const { return std::get<IntValue>(value); }
    Int2Value as_int2() const { return std::get<Int2Value>(value); }
    Int3Value as_int3() const { return std::get<Int3Value>(value); }
    Int4Value as_int4() const { return std::get<Int4Value>(value); }
    const std::string& as_string() const { return std::get<StringValue>(value); }
    int16_t as_bool() const { return std::get<BoolValue>(value); }
    Bool2Value as_bool2() const { return std::get<Bool2Value>(value); }
    Bool3Value as_bool3() const { return std::get<Bool3Value>(value); }
    Bool4Value as_bool4() const { return std::get<Bool4Value>(value); }
};

/// Pixel Bender register channel
enum class PixelBenderRegChannel : uint8_t {
    R = 0,
    G = 1,
    B = 2,
    A = 3,
    M2x2 = 4,
    M3x3 = 5,
    M4x4 = 6,
};

/// Pixel Bender register kind
enum class PixelBenderRegKind : uint8_t {
    Float,
    Int,
};

/// Pixel Bender parameter qualifier
enum class PixelBenderParamQualifier : uint8_t {
    Input = 1,
    Output = 2,
};

/// Pixel Bender opcodes
enum class Opcode : uint8_t {
    Nop = 0x0,
    Add = 0x1,
    Sub = 0x2,
    Mul = 0x3,
    Rcp = 0x4,
    Div = 0x5,
    Atan2 = 0x6,
    Pow = 0x7,
    Mod = 0x8,
    Min = 0x9,
    Max = 0xA,
    Step = 0xB,
    Sin = 0xC,
    Cos = 0xD,
    Tan = 0xE,
    Asin = 0xF,
    Acos = 0x10,
    Atan = 0x11,
    Exp = 0x12,
    Exp2 = 0x13,
    Log = 0x14,
    Log2 = 0x15,
    Sqrt = 0x16,
    RSqrt = 0x17,
    Abs = 0x18,
    Sign = 0x19,
    Floor = 0x1A,
    Ceil = 0x1B,
    Fract = 0x1C,
    Mov = 0x1D,
    FloatToInt = 0x1E,
    IntToFloat = 0x1F,
    MatMatMul = 0x20,
    VecMatMul = 0x21,
    MatVecMul = 0x22,
    Normalize = 0x23,
    Length = 0x24,
    Distance = 0x25,
    DotProduct = 0x26,
    CrossProduct = 0x27,
    Equal = 0x28,
    NotEqual = 0x29,
    LessThan = 0x2A,
    LessThanEqual = 0x2B,
    LogicalNot = 0x2C,
    LogicalAnd = 0x2D,
    LogicalOr = 0x2E,
    LogicalXor = 0x2F,
    SampleNearest = 0x30,
    SampleLinear = 0x31,
    LoadIntOrFloat = 0x32,
    Select = 0x33,
    If = 0x34,
    Else = 0x35,
    EndIf = 0x36,
    FloatToBool = 0x37,
    BoolToFloat = 0x38,
    IntToBool = 0x39,
    BoolToInt = 0x3A,
    VectorEqual = 0x3B,
    VectorNotEqual = 0x3C,
    BoolAny = 0x3D,
    BoolAll = 0x3E,
    PBJMeta1 = 0xA0,
    PBJParam = 0xA1,
    PBJMeta2 = 0xA2,
    PBJParamTexture = 0xA3,
    Name = 0xA4,
    Version = 0xA5,
};

/// Pixel Bender register
struct PixelBenderReg {
    uint32_t index;
    std::vector<PixelBenderRegChannel> channels;
    PixelBenderRegKind kind;
    
    PixelBenderReg() : index(0), kind(PixelBenderRegKind::Float) {}
    
    PixelBenderReg(uint32_t idx, std::vector<PixelBenderRegChannel> chs, PixelBenderRegKind k)
        : index(idx), channels(std::move(chs)), kind(k) {}
    
    /// Check if this register is scalar
    bool is_scalar() const {
        if (channels.size() != 1) return false;
        auto ch = channels[0];
        return ch != PixelBenderRegChannel::M2x2 &&
               ch != PixelBenderRegChannel::M3x3 &&
               ch != PixelBenderRegChannel::M4x4;
    }
};

/// Pixel Bender operation
struct Operation {
    enum class Type {
        Nop,
        Normal,
        LoadInt,
        LoadFloat,
        If,
        SampleNearest,
        SampleLinear,
        Else,
        EndIf,
        Select
    };
    
    Type type;
    
    // For Normal operations
    Opcode opcode;
    PixelBenderReg dst;
    PixelBenderReg src;
    
    // For LoadInt
    int32_t int_val;
    
    // For LoadFloat
    float float_val;
    
    // For Sample operations
    uint8_t texture_filter;
    
    // For Select
    PixelBenderReg condition;
    PixelBenderReg src1;
    PixelBenderReg src2;
    
    // Constructors for different operation types
    static Operation make_nop() {
        Operation op;
        op.type = Type::Nop;
        return op;
    }
    
    static Operation make_normal(Opcode op, PixelBenderReg d, PixelBenderReg s) {
        Operation result;
        result.type = Type::Normal;
        result.opcode = op;
        result.dst = std::move(d);
        result.src = std::move(s);
        return result;
    }
    
    static Operation make_load_int(PixelBenderReg d, int32_t v) {
        Operation result;
        result.type = Type::LoadInt;
        result.dst = std::move(d);
        result.int_val = v;
        return result;
    }
    
    static Operation make_load_float(PixelBenderReg d, float v) {
        Operation result;
        result.type = Type::LoadFloat;
        result.dst = std::move(d);
        result.float_val = v;
        return result;
    }
    
    static Operation make_if(PixelBenderReg s) {
        Operation result;
        result.type = Type::If;
        result.src = std::move(s);
        return result;
    }
    
    static Operation make_sample_nearest(PixelBenderReg d, PixelBenderReg s, uint8_t tf) {
        Operation result;
        result.type = Type::SampleNearest;
        result.dst = std::move(d);
        result.src = std::move(s);
        result.texture_filter = tf;
        return result;
    }
    
    static Operation make_sample_linear(PixelBenderReg d, PixelBenderReg s, uint8_t tf) {
        Operation result;
        result.type = Type::SampleLinear;
        result.dst = std::move(d);
        result.src = std::move(s);
        result.texture_filter = tf;
        return result;
    }
    
    static Operation make_else() {
        Operation op;
        op.type = Type::Else;
        return op;
    }
    
    static Operation make_endif() {
        Operation op;
        op.type = Type::EndIf;
        return op;
    }
    
    static Operation make_select(PixelBenderReg cond, PixelBenderReg s1, PixelBenderReg s2, PixelBenderReg d) {
        Operation result;
        result.type = Type::Select;
        result.condition = std::move(cond);
        result.src1 = std::move(s1);
        result.src2 = std::move(s2);
        result.dst = std::move(d);
        return result;
    }
};

/// Pixel Bender parameter
struct PixelBenderParam {
    enum class Type {
        Normal,
        Texture
    };
    
    Type type;
    
    // For Normal parameters
    PixelBenderParamQualifier qualifier;
    PixelBenderTypeOpcode param_type;
    PixelBenderReg reg;
    std::string name;
    std::vector<PixelBenderMetadata> metadata;
    
    // For Texture parameters
    uint8_t texture_index;
    uint8_t texture_channels;
    std::string texture_name;
    
    PixelBenderParam() : type(Type::Normal), qualifier(PixelBenderParamQualifier::Input) {}
};

/// Pixel Bender metadata
struct PixelBenderMetadata {
    std::string key;
    PixelBenderType value;
    bool is_meta2;
    
    PixelBenderMetadata() : is_meta2(false) {}
    
    PixelBenderMetadata(std::string k, PixelBenderType v, bool m2)
        : key(std::move(k)), value(std::move(v)), is_meta2(m2) {}
};

/// Pixel Bender shader
struct PixelBenderShader {
    std::string name;
    int32_t version;
    std::vector<PixelBenderParam> params;
    std::vector<PixelBenderMetadata> metadata;
    std::vector<Operation> operations;
    
    PixelBenderShader() : version(0) {}
    
    /// Get the output register and its type
    std::optional<std::pair<const PixelBenderReg*, PixelBenderTypeOpcode>> output_reg() const {
        for (auto it = params.rbegin(); it != params.rend(); ++it) {
            const auto& param = *it;
            if (param.type == PixelBenderParam::Type::Normal &&
                param.qualifier == PixelBenderParamQualifier::Output) {
                return std::make_pair(&param.reg, param.param_type);
            }
        }
        return std::nullopt;
    }
    
    /// Get the number of output channels
    std::optional<size_t> output_channels() const {
        auto out = output_reg();
        if (out) {
            return out->first->channels.size();
        }
        return std::nullopt;
    }
};

/// Pixel Bender shader handle (shared pointer wrapper)
class PixelBenderShaderHandle {
public:
    explicit PixelBenderShaderHandle(std::shared_ptr<PixelBenderShader> shader)
        : shader_(std::move(shader)) {}
    
    const PixelBenderShader& shader() const { return *shader_; }
    PixelBenderShader& shader() { return *shader_; }
    
    bool operator==(const PixelBenderShaderHandle& other) const {
        return shader_ == other.shader_;
    }
    
private:
    std::shared_ptr<PixelBenderShader> shader_;
};

/// Pixel Bender shader implementation trait
class PixelBenderShaderImpl {
public:
    virtual ~PixelBenderShaderImpl() = default;
    virtual const PixelBenderShader& parsed_shader() const = 0;
};

/// Parse Pixel Bender bytecode
/// @param data Byte array containing the shader bytecode
/// @param validate Whether to validate the shader during parsing
/// @return Parsed PixelBenderShader
/// @throws PixelBenderParseException on parsing errors
PixelBenderShader parse_shader(const std::vector<uint8_t>& data, bool validate);

/// Utility functions
inline PixelBenderParamQualifier param_qualifier_from_u8(uint8_t v) {
    return (v == 2) ? PixelBenderParamQualifier::Output : PixelBenderParamQualifier::Input;
}

/// Convert type opcode to string
inline const char* type_opcode_to_string(PixelBenderTypeOpcode opcode) {
    switch (opcode) {
        case PixelBenderTypeOpcode::TFloat: return "float";
        case PixelBenderTypeOpcode::TFloat2: return "float2";
        case PixelBenderTypeOpcode::TFloat3: return "float3";
        case PixelBenderTypeOpcode::TFloat4: return "float4";
        case PixelBenderTypeOpcode::TFloat2x2: return "matrix2x2";
        case PixelBenderTypeOpcode::TFloat3x3: return "matrix3x3";
        case PixelBenderTypeOpcode::TFloat4x4: return "matrix4x4";
        case PixelBenderTypeOpcode::TInt: return "int";
        case PixelBenderTypeOpcode::TInt2: return "int2";
        case PixelBenderTypeOpcode::TInt3: return "int3";
        case PixelBenderTypeOpcode::TInt4: return "int4";
        case PixelBenderTypeOpcode::TString: return "string";
        case PixelBenderTypeOpcode::TBool: return "bool";
        case PixelBenderTypeOpcode::TBool2: return "bool2";
        case PixelBenderTypeOpcode::TBool3: return "bool3";
        case PixelBenderTypeOpcode::TBool4: return "bool4";
        default: return "unknown";
    }
}

/// Convert opcode to string
inline const char* opcode_to_string(Opcode opcode) {
    switch (opcode) {
        case Opcode::Nop: return "nop";
        case Opcode::Add: return "add";
        case Opcode::Sub: return "sub";
        case Opcode::Mul: return "mul";
        case Opcode::Rcp: return "rcp";
        case Opcode::Div: return "div";
        case Opcode::Atan2: return "atan2";
        case Opcode::Pow: return "pow";
        case Opcode::Mod: return "mod";
        case Opcode::Min: return "min";
        case Opcode::Max: return "max";
        case Opcode::Step: return "step";
        case Opcode::Sin: return "sin";
        case Opcode::Cos: return "cos";
        case Opcode::Tan: return "tan";
        case Opcode::Asin: return "asin";
        case Opcode::Acos: return "acos";
        case Opcode::Atan: return "atan";
        case Opcode::Exp: return "exp";
        case Opcode::Exp2: return "exp2";
        case Opcode::Log: return "log";
        case Opcode::Log2: return "log2";
        case Opcode::Sqrt: return "sqrt";
        case Opcode::RSqrt: return "rsqrt";
        case Opcode::Abs: return "abs";
        case Opcode::Sign: return "sign";
        case Opcode::Floor: return "floor";
        case Opcode::Ceil: return "ceil";
        case Opcode::Fract: return "fract";
        case Opcode::Mov: return "mov";
        case Opcode::FloatToInt: return "floatToInt";
        case Opcode::IntToFloat: return "intToFloat";
        case Opcode::MatMatMul: return "matMatMul";
        case Opcode::VecMatMul: return "vecMatMul";
        case Opcode::MatVecMul: return "matVecMul";
        case Opcode::Normalize: return "normalize";
        case Opcode::Length: return "length";
        case Opcode::Distance: return "distance";
        case Opcode::DotProduct: return "dotProduct";
        case Opcode::CrossProduct: return "crossProduct";
        case Opcode::Equal: return "equal";
        case Opcode::NotEqual: return "notEqual";
        case Opcode::LessThan: return "lessThan";
        case Opcode::LessThanEqual: return "lessThanEqual";
        case Opcode::LogicalNot: return "logicalNot";
        case Opcode::LogicalAnd: return "logicalAnd";
        case Opcode::LogicalOr: return "logicalOr";
        case Opcode::LogicalXor: return "logicalXor";
        case Opcode::SampleNearest: return "sampleNearest";
        case Opcode::SampleLinear: return "sampleLinear";
        case Opcode::LoadIntOrFloat: return "loadIntOrFloat";
        case Opcode::Select: return "select";
        case Opcode::If: return "if";
        case Opcode::Else: return "else";
        case Opcode::EndIf: return "endif";
        case Opcode::FloatToBool: return "floatToBool";
        case Opcode::BoolToFloat: return "boolToFloat";
        case Opcode::IntToBool: return "intToBool";
        case Opcode::BoolToInt: return "boolToInt";
        case Opcode::VectorEqual: return "vectorEqual";
        case Opcode::VectorNotEqual: return "vectorNotEqual";
        case Opcode::BoolAny: return "boolAny";
        case Opcode::BoolAll: return "boolAll";
        case Opcode::PBJMeta1: return "pbjMeta1";
        case Opcode::PBJParam: return "pbjParam";
        case Opcode::PBJMeta2: return "pbjMeta2";
        case Opcode::PBJParamTexture: return "pbjParamTexture";
        case Opcode::Name: return "name";
        case Opcode::Version: return "version";
        default: return "unknown";
    }
}

/// Convert register channel to string
inline const char* channel_to_string(PixelBenderRegChannel ch) {
    switch (ch) {
        case PixelBenderRegChannel::R: return "r";
        case PixelBenderRegChannel::G: return "g";
        case PixelBenderRegChannel::B: return "b";
        case PixelBenderRegChannel::A: return "a";
        case PixelBenderRegChannel::M2x2: return "m2x2";
        case PixelBenderRegChannel::M3x3: return "m3x3";
        case PixelBenderRegChannel::M4x4: return "m4x4";
        default: return "unknown";
    }
}

/// Convert register kind to string
inline const char* reg_kind_to_string(PixelBenderRegKind kind) {
    return (kind == PixelBenderRegKind::Float) ? "float" : "int";
}

/// Convert parameter qualifier to string
inline const char* qualifier_to_string(PixelBenderParamQualifier qual) {
    return (qual == PixelBenderParamQualifier::Output) ? "output" : "input";
}

} // namespace parser
} // namespace pixel_bender
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_PIXEL_BENDER_PARSER_H
