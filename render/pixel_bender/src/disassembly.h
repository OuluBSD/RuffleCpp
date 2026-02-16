// C++ translation of render/pixel_bender/src/disassembly.rs
// Pixel Bender disassembler for debugging purposes

#ifndef RUFFLE_RENDER_PIXEL_BENDER_DISASSEMBLY_H
#define RUFFLE_RENDER_PIXEL_BENDER_DISASSEMBLY_H

#include <string>
#include <vector>
#include <cstdint>

// Forward declarations
namespace ruffle {
namespace render {
namespace pixel_bender {
namespace parser {

// Forward declarations from parser module
enum class Opcode : uint8_t;
enum class PixelBenderTypeOpcode : uint8_t;
enum class PixelBenderRegKind : uint8_t;
enum class PixelBenderRegChannel : uint8_t;
enum class PixelBenderParamQualifier : uint8_t;

struct PixelBenderReg;
struct PixelBenderMetadata;
struct PixelBenderType;
struct PixelBenderParam;
struct PixelBenderShader;
struct Operation;

} // namespace parser

/// Pixel Bender disassembler for debugging purposes
///
/// Provides a human-readable string representation of Pixel Bender shaders
/// for debugging and inspection.
class PixelBenderShaderDisassembly {
public:
    /// Create a disassembler for the given shader
    /// @param shader Pointer to the shader to disassemble
    explicit PixelBenderShaderDisassembly(const parser::PixelBenderShader* shader);

    /// Convert the shader to a string representation
    /// @return String representation of the disassembled shader
    std::string to_string() const;

private:
    /// Pointer to the shader being disassembled
    const parser::PixelBenderShader* shader;

    /// Format metadata section
    /// @param f Output string
    /// @param metadata Vector of metadata entries
    void fmt_metadata(std::string& f, const std::vector<parser::PixelBenderMetadata>& metadata) const;

    /// Format parameters section
    /// @param f Output string
    void fmt_parameters(std::string& f) const;

    /// Format a register
    /// @param f Output string
    /// @param reg Register to format
    void fmt_reg(std::string& f, const parser::PixelBenderReg& reg) const;

    /// Format a type
    /// @param f Output string
    /// @param type Type to format
    void fmt_type(std::string& f, const parser::PixelBenderType& type) const;

    /// Format operations section
    /// @param f Output string
    void fmt_operations(std::string& f) const;

    /// Format a single operation
    /// @param f Output string
    /// @param op Operation to format
    /// @param prefix Current indentation prefix
    void fmt_operation(std::string& f, const parser::Operation& op, std::string& prefix) const;

    /// Increase indentation prefix
    /// @param prefix Prefix string to modify
    void prefix_inc(std::string& prefix) const;

    /// Decrease indentation prefix
    /// @param prefix Prefix string to modify
    void prefix_dec(std::string& prefix) const;

    /// Convert parameter qualifier to string
    /// @param qualifier Qualifier to convert
    /// @return String representation
    const char* qualifier_to_str(parser::PixelBenderParamQualifier qualifier) const;

    /// Convert type opcode to string
    /// @param type_opcode Type opcode to convert
    /// @return String representation
    const char* type_to_str(parser::PixelBenderTypeOpcode type_opcode) const;

    /// Convert register channel to string
    /// @param ch Channel to convert
    /// @return String representation
    const char* channel_to_str(parser::PixelBenderRegChannel ch) const;

    /// Convert opcode to string
    /// @param opcode Opcode to convert
    /// @return String representation
    const char* opcode_to_str(parser::Opcode opcode) const;
};

} // namespace pixel_bender
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_PIXEL_BENDER_DISASSEMBLY_H
