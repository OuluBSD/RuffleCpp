// C++ translation of swf/src/avm1/read.rs
//! AVM1 bytecode reader

#ifndef RUFFLE_SWF_AVM1_READ_H
#define RUFFLE_SWF_AVM1_READ_H

#include <vector>
#include <string>
#include <cstdint>
#include <optional>
#include <variant>

namespace ruffle {
namespace swf {
namespace avm1 {

// Forward declarations
struct Action;
struct ConstantPool;
struct DefineFunction;
struct DefineFunction2;
struct FunctionParam;
struct GetUrl;
struct GetUrl2;
struct GotoFrame;
struct GotoFrame2;
struct GotoLabel;
struct If;
struct Jump;
struct Push;
struct SetTarget;
struct StoreRegister;
struct Try;
struct WaitForFrame;
struct WaitForFrame2;
struct With;
struct Unknown;

/// Value types for AVM1 Push action
struct Value {
    using DataType = std::variant<
        std::monostate,      // Null
        std::monostate,      // Undefined
        std::string,         // String
        float,               // Float
        double,              // Double
        int32_t,             // Int
        uint8_t,             // Register
        bool,                // Bool
        uint8_t,             // ConstantPool (8-bit index)
        uint16_t             // ConstantPool (16-bit index)
    >;
    
    DataType data;
    
    static Value null() { return Value{std::monostate{}}; }
    static Value undefined() { return Value{std::monostate{}}; }
    static Value str(const std::string& s) { return Value{s}; }
    static Value float_(float f) { return Value{f}; }
    static Value double_(double d) { return Value{d}; }
    static Value int_(int32_t i) { return Value{i}; }
    static Value reg(uint8_t r) { return Value{r}; }
    static Value bool_(bool b) { return Value{b}; }
    static Value constant8(uint8_t i) { return Value{i}; }
    static Value constant16(uint16_t i) { return Value{i}; }
};

/// AVM1 bytecode reader
class Reader {
public:
    /// Create a new reader
    ///
    /// \param input Input byte slice
    /// \param version SWF version
    explicit Reader(const uint8_t* input, size_t length, uint8_t version);

    /// Seek to a position in the data
    ///
    /// \param data Data slice to seek in
    /// \param jump_offset Offset to jump (can be negative)
    void seek(const uint8_t* data, int16_t jump_offset);

    /// Get reference to remaining input
    const uint8_t* get_ref() const;

    /// Get mutable reference to remaining input
    uint8_t*& get_mut();

    /// Read a single action
    ///
    /// \return The parsed action or error
    std::optional<Action> read_action();

    /// Read opcode and length
    ///
    /// \return Pair of (opcode, length)
    std::pair<uint8_t, size_t> read_opcode_and_length();

private:
    /// Read an operation by opcode
    ///
    /// \param opcode The opcode to read
    /// \param length In-out length parameter
    /// \return The parsed action
    std::optional<Action> read_op(uint8_t opcode, size_t* length);

    /// Read constant pool
    ConstantPool read_constant_pool();

    /// Read DefineFunction action
    DefineFunction read_define_function(size_t* action_length);

    /// Read DefineFunction2 action
    DefineFunction2 read_define_function_2(size_t* action_length);

    /// Read GetUrl action
    GetUrl read_get_url();

    /// Read GetUrl2 action
    GetUrl2 read_get_url_2();

    /// Read GotoFrame action
    GotoFrame read_goto_frame();

    /// Read GotoFrame2 action
    GotoFrame2 read_goto_frame_2();

    /// Read GotoLabel action
    GotoLabel read_goto_label();

    /// Read If action
    If read_if();

    /// Read Jump action
    Jump read_jump();

    /// Read Push action
    Push read_push(size_t length);

    /// Read SetTarget action
    SetTarget read_set_target();

    /// Read StoreRegister action
    StoreRegister read_store_register();

    /// Read Try action
    Try read_try(size_t* length);

    /// Read WaitForFrame action
    WaitForFrame read_wait_for_frame();

    /// Read WaitForFrame2 action
    WaitForFrame2 read_wait_for_frame_2();

    /// Read With action
    With read_with(size_t* action_length);

    /// Read unknown action
    Unknown read_unknown_action(uint8_t opcode, size_t length);

    /// Read a string from input
    std::string read_str();

    /// Read a slice of bytes
    const uint8_t* read_slice(size_t length);

    /// Read uint8_t
    uint8_t read_u8();

    /// Read uint16_t (little endian)
    uint16_t read_u16();

    /// Read int16_t (little endian)
    int16_t read_i16();

    /// Read int32_t (little endian)
    int32_t read_i32();

    /// Read float (little endian)
    float read_f32();

    /// Read double (Flash weird format - two LE 32-bit chunks)
    double read_f64_me();

    const uint8_t* input_;
    const uint8_t* end_;
    uint8_t version_;
};

} // namespace avm1
} // namespace swf
} // namespace ruffle

#endif // RUFFLE_SWF_AVM1_READ_H
