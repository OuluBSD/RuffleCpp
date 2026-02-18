// C++ translation of swf/src/avm1/write.rs
// AVM1 action writer

#ifndef RUFFLE_SWF_AVM1_WRITE_H
#define RUFFLE_SWF_AVM1_WRITE_H

#include <cstdint>
#include <cstddef>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include <bit>

// Forward declarations
namespace ruffle {
namespace swf {
class SwfStr;
}
}

namespace ruffle {
namespace swf {
namespace avm1 {

// Forward declarations for types used in write.rs
namespace opcode {
    enum class OpCode : uint8_t;
}

namespace types {

// Value types for AVM1
struct Value;
struct Action;
struct ConstantPool;
struct DefineFunction;
struct DefineFunction2;
struct DefineFunctionParam;
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
struct TryFlags;
struct CatchVar;
struct WaitForFrame;
struct WaitForFrame2;
struct With;
struct Unknown;

} // namespace types

// Trait for SWF write extensions
template<typename W>
concept SwfWriteExt = requires(W w, uint8_t u8, uint16_t u16, uint32_t u32, uint64_t u64,
                                int8_t i8, int16_t i16, int32_t i32, float f32, double f64,
                                const SwfStr* s) {
    { w.write_u8(u8) } -> std::same_as<std::invoke_result_t<decltype(&W::write_u8), W, uint8_t>>;
    { w.write_u16(u16) } -> std::same_as<std::invoke_result_t<decltype(&W::write_u16), W, uint16_t>>;
    { w.write_u32(u32) } -> std::same_as<std::invoke_result_t<decltype(&W::write_u32), W, uint32_t>>;
    { w.write_u64(u64) } -> std::same_as<std::invoke_result_t<decltype(&W::write_u64), W, uint64_t>>;
    { w.write_i8(i8) } -> std::same_as<std::invoke_result_t<decltype(&W::write_i8), W, int8_t>>;
    { w.write_i16(i16) } -> std::same_as<std::invoke_result_t<decltype(&W::write_i16), W, int16_t>>;
    { w.write_i32(i32) } -> std::same_as<std::invoke_result_t<decltype(&W::write_i32), W, int32_t>>;
    { w.write_f32(f32) } -> std::same_as<std::invoke_result_t<decltype(&W::write_f32), W, float>>;
    { w.write_f64(f64) } -> std::same_as<std::invoke_result_t<decltype(&W::write_f64), W, double>>;
    { w.write_string(s) } -> std::same_as<std::invoke_result_t<decltype(&W::write_string), W, const SwfStr*>>;
};

/// Writer for AVM1 actions
/// Writes AVM1 bytecode actions to an output stream
template<typename W>
class Writer {
private:
    W* output;
    uint8_t version;

    /// Flash weirdly stores f64 as two LE 32-bit chunks.
    /// First word is the hi-word, second word is the lo-word.
    std::invoke_result_t<decltype(&W::write_u64), W*, uint64_t> write_f64_me(double n) {
        return output->write_u64(std::rotl(std::bit_cast<uint64_t>(n), 32));
    }

    /// Writes an action that has no payload.
    std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t> 
    write_small_action(opcode::OpCode opcode) {
        return write_action_header(opcode, 0);
    }

    std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
    write_constant_pool(const types::ConstantPool* action);

    std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
    write_define_function(const types::DefineFunction* action);

    std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
    write_define_function_2(const types::DefineFunction2* action);

    std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
    write_get_url(const types::GetUrl* action);

    std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
    write_get_url_2(types::GetUrl2 action);

    std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
    write_goto_frame(types::GotoFrame action);

    std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
    write_goto_frame_2(types::GotoFrame2 action);

    std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
    write_goto_label(const types::GotoLabel* action);

    std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
    write_if(types::If action);

    std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
    write_jump(types::Jump action);

    std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
    write_push(const types::Push* action);

    std::invoke_result_t<decltype(&W::write_u8), W*, uint8_t>
    write_push_value(const types::Value* value);

    std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
    write_set_target(const types::SetTarget* action);

    std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
    write_store_register(types::StoreRegister action);

    std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
    write_try(const types::Try* action);

    std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
    write_wait_for_frame(types::WaitForFrame action);

    std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
    write_wait_for_frame_2(types::WaitForFrame2 action);

    std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
    write_with(const types::With* action);

    std::invoke_result_t<decltype(&W::write_opcode_and_length), W*, uint8_t, size_t>
    write_unknown(const types::Unknown* action);

public:
    Writer(W* output, uint8_t version) : output(output), version(version) {}

    /// Write an action to the output stream
    std::invoke_result_t<decltype(&W::write_action), W*, const types::Action*>
    write_action(const types::Action* action);

    /// Write action header with opcode and length
    std::invoke_result_t<decltype(&W::write_opcode_and_length), W*, uint8_t, size_t>
    write_action_header(opcode::OpCode opcode, size_t length) {
        output->write_u8(static_cast<uint8_t>(opcode));
        // Opcodes less than 0x80 must have length 0
        if (opcode < opcode::OpCode(0x80)) {
            // Assertion would go here in debug builds
        }
        if (static_cast<uint8_t>(opcode) >= 0x80) {
            return output->write_u16(static_cast<uint16_t>(length));
        }
        return {};
    }

    /// Write opcode and length
    std::invoke_result_t<decltype(&W::write_opcode_and_length), W*, uint8_t, size_t>
    write_opcode_and_length(uint8_t opcode, size_t length) {
        output->write_u8(opcode);
        if (opcode >= 0x80) {
            return output->write_u16(static_cast<uint16_t>(length));
        }
        return {};
    }
};

// Template method implementations
template<typename W>
std::invoke_result_t<decltype(&W::write_action), W*, const types::Action*>
Writer<W>::write_action(const types::Action* action) {
    // This would match on the Action enum and call the appropriate write method
    // Implementation depends on the Action enum structure
    return {};
}

template<typename W>
std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
Writer<W>::write_constant_pool(const types::ConstantPool* action) {
    // Implementation would calculate length and write constant pool
    return {};
}

template<typename W>
std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
Writer<W>::write_define_function(const types::DefineFunction* action) {
    // Implementation would write DefineFunction action
    return {};
}

template<typename W>
std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
Writer<W>::write_define_function_2(const types::DefineFunction2* action) {
    // Implementation would write DefineFunction2 action
    return {};
}

template<typename W>
std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
Writer<W>::write_get_url(const types::GetUrl* action) {
    // Implementation would write GetUrl action
    return {};
}

template<typename W>
std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
Writer<W>::write_get_url_2(types::GetUrl2 action) {
    // Implementation would write GetUrl2 action
    return {};
}

template<typename W>
std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
Writer<W>::write_goto_frame(types::GotoFrame action) {
    // Implementation would write GotoFrame action
    return {};
}

template<typename W>
std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
Writer<W>::write_goto_frame_2(types::GotoFrame2 action) {
    // Implementation would write GotoFrame2 action
    return {};
}

template<typename W>
std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
Writer<W>::write_goto_label(const types::GotoLabel* action) {
    // Implementation would write GotoLabel action
    return {};
}

template<typename W>
std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
Writer<W>::write_if(types::If action) {
    // Implementation would write If action
    return {};
}

template<typename W>
std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
Writer<W>::write_jump(types::Jump action) {
    // Implementation would write Jump action
    return {};
}

template<typename W>
std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
Writer<W>::write_push(const types::Push* action) {
    // Implementation would write Push action
    return {};
}

template<typename W>
std::invoke_result_t<decltype(&W::write_u8), W*, uint8_t>
Writer<W>::write_push_value(const types::Value* value) {
    // Implementation would write push value
    return {};
}

template<typename W>
std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
Writer<W>::write_set_target(const types::SetTarget* action) {
    // Implementation would write SetTarget action
    return {};
}

template<typename W>
std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
Writer<W>::write_store_register(types::StoreRegister action) {
    // Implementation would write StoreRegister action
    return {};
}

template<typename W>
std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
Writer<W>::write_try(const types::Try* action) {
    // Implementation would write Try action
    return {};
}

template<typename W>
std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
Writer<W>::write_wait_for_frame(types::WaitForFrame action) {
    // Implementation would write WaitForFrame action
    return {};
}

template<typename W>
std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
Writer<W>::write_wait_for_frame_2(types::WaitForFrame2 action) {
    // Implementation would write WaitForFrame2 action
    return {};
}

template<typename W>
std::invoke_result_t<decltype(&W::write_action_header), W*, opcode::OpCode, size_t>
Writer<W>::write_with(const types::With* action) {
    // Implementation would write With action
    return {};
}

template<typename W>
std::invoke_result_t<decltype(&W::write_opcode_and_length), W*, uint8_t, size_t>
Writer<W>::write_unknown(const types::Unknown* action) {
    // Implementation would write Unknown action
    return {};
}

} // namespace avm1
} // namespace swf
} // namespace ruffle

#endif // RUFFLE_SWF_AVM1_WRITE_H
