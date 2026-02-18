// C++ translation of render/pixel_bender/src/assembly.rs
// PixelBender shader assembly parser and assembler

#ifndef RUFFLE_RENDER_PIXEL_BENDER_ASSEMBLY_H
#define RUFFLE_RENDER_PIXEL_BENDER_ASSEMBLY_H

#include "../parser.h"
#include "../../src/shader_source.h"

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <cstdint>
#include <ostream>

namespace ruffle {
namespace render {
namespace pixel_bender {
namespace assembly {

/// Error location in source (line, column)
struct PbasmErrorLocation {
    size_t line;
    size_t column;
    
    PbasmErrorLocation(size_t l, size_t c) : line(l), column(c) {}
};

/// PixelBender assembly error types
enum class PbasmErrorType {
    ParsingError,
    IoError,
    UnknownOpcode,
    UnknownType,
    TooFewArguments,
    TooManyArguments,
    WrongArgument,
    ErrorParsingInt,
    ErrorParsingFloat,
    ErrorParsingString,
    StringTooLong,
    BadRegisterIndex,
    BadRegisterChannels,
    MatrixRegisterNotAllowed,
    BadMatrixDstRegister,
    SwizzleNotAllowed
};

/// PixelBender assembly error
struct PbasmError {
    PbasmErrorType type;
    std::string message;
    std::optional<PbasmErrorLocation> location;
    
    PbasmError(PbasmErrorType t, std::string msg) 
        : type(t), message(std::move(msg)) {}
    
    PbasmError(PbasmErrorType t, std::string msg, PbasmErrorLocation loc)
        : type(t), message(std::move(msg)), location(loc) {}
};

/// Register size specification
enum class RegisterSize {
    None,
    Standard,    // 1-4 channels
    Matrix       // 2x2, 3x3, 4x4
};

/// Register type for parsing
enum class RegisterType {
    Parameter,
    Destination,
    Source
};

/// Common opcode table entry
enum class Opcode : uint8_t {
    Abs,
    Acos,
    Add,
    BoolAll,
    LogicalAnd,
    Any,
    Asin,
    Atan,
    Atan2,
    BoolToFloat,
    BoolToInt,
    Ceil,
    Cos,
    CrossProduct,
    Distance,
    Div,
    DotProduct,
    VectorEqual,
    Equal,
    Exp,
    Exp2,
    FloatToBool,
    FloatToInt,
    Floor,
    Fract,
    IntToBool,
    IntToFloat,
    LessThanEqual,
    Length,
    Log,
    Log2,
    LessThan,
    Max,
    Min,
    Mod,
    Mov,
    MatMatMul,
    MatVecMul,
    VecMatMul,
    Mul,
    VectorNotEqual,
    NotEqual,
    Normalize,
    LogicalNot,
    LogicalOr,
    Pow,
    Rcp,
    RSqrt,
    Sign,
    Sin,
    Sqrt,
    Step,
    Sub,
    Tan,
    LogicalXor,
    Version,
    Name,
    PBJParam,
    PBJParamTexture,
    PBJMeta1,
    PBJMeta2,
    SampleNearest,
    SampleLinear,
    LoadIntOrFloat,
    Nop,
    Select,
    If,
    Else,
    EndIf
};

/// PixelBender shader assembler
class PixelBenderShaderAssembly {
public:
    /// Construct assembler with input and output
    /// 
    /// @param input The assembly source code
    /// @param write Output stream for binary shader
    PixelBenderShaderAssembly(const std::string& input, std::ostream& write);
    
    /// Assemble the shader
    /// 
    /// @return Result indicating success or error
    std::optional<PbasmError> assemble();

private:
    std::string input_;
    std::ostream& write_;
    
    /// Assemble shader from parsed pairs
    std::optional<PbasmError> assemble_shader(const std::vector<ParsePair>& pairs);
    
    /// Assemble a single line/statement
    std::optional<PbasmError> assemble_line(const std::vector<ParsePair>& pairs);
    
    /// Assemble a statement (opcode + arguments)
    std::optional<PbasmError> assemble_statement(
        const std::string& opcode,
        std::vector<ParsePair>& arguments
    );
    
    /// Get next argument or error
    std::optional<ParsePair> next_arg(
        const std::string& opcode,
        std::vector<ParsePair>& arguments,
        PbasmErrorLocation loc
    );
    
    /// Check no more arguments remain
    std::optional<PbasmError> check_no_more_args(
        const std::string& opcode,
        std::vector<ParsePair>& arguments,
        PbasmErrorLocation loc
    );
    
    /// Parse argument as integer
    template<typename T>
    std::optional<T> argument_as_int(const ParsePair& argument);
    
    /// Parse argument as string
    std::optional<std::string> argument_as_string(const ParsePair& argument);
    
    /// Parse argument as type opcode
    std::optional<PixelBenderTypeOpcode> argument_as_type(const ParsePair& argument);
    
    /// Parse argument as typed value
    std::optional<std::pair<PixelBenderTypeOpcode, std::vector<uint8_t>>> 
    argument_as_typed_value(const ParsePair& argument);
    
    /// Parse argument as register
    std::optional<std::pair<RegisterSize, std::vector<uint8_t>>>
    argument_as_reg(
        const ParsePair& argument,
        RegisterType reg_type,
        bool allow_matrix
    );
    
    /// Write string to output with length prefix
    static void write_string(std::ostream& write, const std::string& str);
    
    /// Get literal integer value
    template<typename T>
    std::optional<T> get_literal_int(const ParsePair& literal);
    
    /// Get literal float value
    template<typename T>
    std::optional<T> get_literal_float(const ParsePair& literal);
    
    /// Get literal string value
    std::optional<std::string> get_literal_string(const ParsePair& literal);
    
    /// Unwrap pair to inner rule
    std::optional<ParsePair> unwrap_pair(
        const ParsePair& pair,
        ParseRule expected,
        std::optional<ParseRule> inner
    );
};

/// Common opcode lookup table
class CommonOpcodeTable {
public:
    /// Get opcode by name
    /// 
    /// @param name Opcode name (e.g., "add", "mov")
    /// @return Opcode enum value if found
    static std::optional<Opcode> get(const std::string& name);
    
private:
    static const std::unordered_map<std::string, Opcode> table_;
};

} // namespace assembly
} // namespace pixel_bender
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_PIXEL_BENDER_ASSEMBLY_H
