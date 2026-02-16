// C++ translation of swf/src/avm1/opcode.rs
// AVM1 OpCode definitions

#ifndef RUFFLE_SWF_AVM1_OPCODE_H
#define RUFFLE_SWF_AVM1_OPCODE_H

#include <cstdint>
#include <string>
#include <optional>

namespace ruffle {
namespace swf {
namespace avm1 {
namespace opcode {

/// AVM1 OpCode enumeration
/// Each opcode represents a specific ActionScript 1.0/2.0 bytecode instruction
enum class OpCode : uint8_t {
    End = 0x00,

    NextFrame = 0x04,
    PreviousFrame = 0x05,
    Play = 0x06,
    Stop = 0x07,
    ToggleQuality = 0x08,
    StopSounds = 0x09,
    Add = 0x0A,
    Subtract = 0x0B,
    Multiply = 0x0C,
    Divide = 0x0D,
    Equals = 0x0E,
    Less = 0x0F,
    And = 0x10,
    Or = 0x11,
    Not = 0x12,
    StringEquals = 0x13,
    StringLength = 0x14,
    StringExtract = 0x15,

    Pop = 0x17,
    ToInteger = 0x18,

    GetVariable = 0x1C,
    SetVariable = 0x1D,

    SetTarget2 = 0x20,
    StringAdd = 0x21,
    GetProperty = 0x22,
    SetProperty = 0x23,
    CloneSprite = 0x24,
    RemoveSprite = 0x25,
    Trace = 0x26,
    StartDrag = 0x27,
    EndDrag = 0x28,
    StringLess = 0x29,
    Throw = 0x2A,
    CastOp = 0x2B,
    ImplementsOp = 0x2C,

    RandomNumber = 0x30,
    MBStringLength = 0x31,
    CharToAscii = 0x32,
    AsciiToChar = 0x33,
    GetTime = 0x34,
    MBStringExtract = 0x35,
    MBCharToAscii = 0x36,
    MBAsciiToChar = 0x37,

    Delete = 0x3A,
    Delete2 = 0x3B,
    DefineLocal = 0x3C,
    CallFunction = 0x3D,
    Return = 0x3E,
    Modulo = 0x3F,
    NewObject = 0x40,
    DefineLocal2 = 0x41,
    InitArray = 0x42,
    InitObject = 0x43,
    TypeOf = 0x44,
    TargetPath = 0x45,
    Enumerate = 0x46,
    Add2 = 0x47,
    Less2 = 0x48,
    Equals2 = 0x49,
    ToNumber = 0x4A,
    ToString = 0x4B,
    PushDuplicate = 0x4C,
    StackSwap = 0x4D,
    GetMember = 0x4E,
    SetMember = 0x4F,
    Increment = 0x50,
    Decrement = 0x51,
    CallMethod = 0x52,
    NewMethod = 0x53,
    InstanceOf = 0x54,
    Enumerate2 = 0x55,

    BitAnd = 0x60,
    BitOr = 0x61,
    BitXor = 0x62,
    BitLShift = 0x63,
    BitRShift = 0x64,
    BitURShift = 0x65,
    StrictEquals = 0x66,
    Greater = 0x67,
    StringGreater = 0x68,
    Extends = 0x69,

    GotoFrame = 0x81,

    GetUrl = 0x83,

    StoreRegister = 0x87,
    ConstantPool = 0x88,

    WaitForFrame = 0x8A,
    SetTarget = 0x8B,
    GotoLabel = 0x8C,
    WaitForFrame2 = 0x8D,
    DefineFunction2 = 0x8E,
    Try = 0x8F,

    With = 0x94,

    Push = 0x96,

    Jump = 0x99,
    GetUrl2 = 0x9A,
    DefineFunction = 0x9B,
    If = 0x9D,
    Call = 0x9E,
    GotoFrame2 = 0x9F,
};

/// OpCode utility functions
namespace op_code_utils {

/// Convert a u8 value to OpCode
/// @param n The numeric opcode value
/// @return Optional OpCode if valid, nullopt otherwise
inline std::optional<OpCode> from_u8(uint8_t n) {
    switch (n) {
        case 0x00: return OpCode::End;
        case 0x04: return OpCode::NextFrame;
        case 0x05: return OpCode::PreviousFrame;
        case 0x06: return OpCode::Play;
        case 0x07: return OpCode::Stop;
        case 0x08: return OpCode::ToggleQuality;
        case 0x09: return OpCode::StopSounds;
        case 0x0A: return OpCode::Add;
        case 0x0B: return OpCode::Subtract;
        case 0x0C: return OpCode::Multiply;
        case 0x0D: return OpCode::Divide;
        case 0x0E: return OpCode::Equals;
        case 0x0F: return OpCode::Less;
        case 0x10: return OpCode::And;
        case 0x11: return OpCode::Or;
        case 0x12: return OpCode::Not;
        case 0x13: return OpCode::StringEquals;
        case 0x14: return OpCode::StringLength;
        case 0x15: return OpCode::StringExtract;
        case 0x17: return OpCode::Pop;
        case 0x18: return OpCode::ToInteger;
        case 0x1C: return OpCode::GetVariable;
        case 0x1D: return OpCode::SetVariable;
        case 0x20: return OpCode::SetTarget2;
        case 0x21: return OpCode::StringAdd;
        case 0x22: return OpCode::GetProperty;
        case 0x23: return OpCode::SetProperty;
        case 0x24: return OpCode::CloneSprite;
        case 0x25: return OpCode::RemoveSprite;
        case 0x26: return OpCode::Trace;
        case 0x27: return OpCode::StartDrag;
        case 0x28: return OpCode::EndDrag;
        case 0x29: return OpCode::StringLess;
        case 0x2A: return OpCode::Throw;
        case 0x2B: return OpCode::CastOp;
        case 0x2C: return OpCode::ImplementsOp;
        case 0x30: return OpCode::RandomNumber;
        case 0x31: return OpCode::MBStringLength;
        case 0x32: return OpCode::CharToAscii;
        case 0x33: return OpCode::AsciiToChar;
        case 0x34: return OpCode::GetTime;
        case 0x35: return OpCode::MBStringExtract;
        case 0x36: return OpCode::MBCharToAscii;
        case 0x37: return OpCode::MBAsciiToChar;
        case 0x3A: return OpCode::Delete;
        case 0x3B: return OpCode::Delete2;
        case 0x3C: return OpCode::DefineLocal;
        case 0x3D: return OpCode::CallFunction;
        case 0x3E: return OpCode::Return;
        case 0x3F: return OpCode::Modulo;
        case 0x40: return OpCode::NewObject;
        case 0x41: return OpCode::DefineLocal2;
        case 0x42: return OpCode::InitArray;
        case 0x43: return OpCode::InitObject;
        case 0x44: return OpCode::TypeOf;
        case 0x45: return OpCode::TargetPath;
        case 0x46: return OpCode::Enumerate;
        case 0x47: return OpCode::Add2;
        case 0x48: return OpCode::Less2;
        case 0x49: return OpCode::Equals2;
        case 0x4A: return OpCode::ToNumber;
        case 0x4B: return OpCode::ToString;
        case 0x4C: return OpCode::PushDuplicate;
        case 0x4D: return OpCode::StackSwap;
        case 0x4E: return OpCode::GetMember;
        case 0x4F: return OpCode::SetMember;
        case 0x50: return OpCode::Increment;
        case 0x51: return OpCode::Decrement;
        case 0x52: return OpCode::CallMethod;
        case 0x53: return OpCode::NewMethod;
        case 0x54: return OpCode::InstanceOf;
        case 0x55: return OpCode::Enumerate2;
        case 0x60: return OpCode::BitAnd;
        case 0x61: return OpCode::BitOr;
        case 0x62: return OpCode::BitXor;
        case 0x63: return OpCode::BitLShift;
        case 0x64: return OpCode::BitRShift;
        case 0x65: return OpCode::BitURShift;
        case 0x66: return OpCode::StrictEquals;
        case 0x67: return OpCode::Greater;
        case 0x68: return OpCode::StringGreater;
        case 0x69: return OpCode::Extends;
        case 0x81: return OpCode::GotoFrame;
        case 0x83: return OpCode::GetUrl;
        case 0x87: return OpCode::StoreRegister;
        case 0x88: return OpCode::ConstantPool;
        case 0x8A: return OpCode::WaitForFrame;
        case 0x8B: return OpCode::SetTarget;
        case 0x8C: return OpCode::GotoLabel;
        case 0x8D: return OpCode::WaitForFrame2;
        case 0x8E: return OpCode::DefineFunction2;
        case 0x8F: return OpCode::Try;
        case 0x94: return OpCode::With;
        case 0x96: return OpCode::Push;
        case 0x99: return OpCode::Jump;
        case 0x9A: return OpCode::GetUrl2;
        case 0x9B: return OpCode::DefineFunction;
        case 0x9D: return OpCode::If;
        case 0x9E: return OpCode::Call;
        case 0x9F: return OpCode::GotoFrame2;
        default: return std::nullopt;
    }
}

/// Format an opcode as a string
/// @param opcode The numeric opcode value
/// @return String representation of the opcode
inline std::string format(uint8_t opcode) {
    if (auto op = from_u8(opcode)) {
        switch (*op) {
            case OpCode::End: return "End";
            case OpCode::NextFrame: return "NextFrame";
            case OpCode::PreviousFrame: return "PreviousFrame";
            case OpCode::Play: return "Play";
            case OpCode::Stop: return "Stop";
            case OpCode::ToggleQuality: return "ToggleQuality";
            case OpCode::StopSounds: return "StopSounds";
            case OpCode::Add: return "Add";
            case OpCode::Subtract: return "Subtract";
            case OpCode::Multiply: return "Multiply";
            case OpCode::Divide: return "Divide";
            case OpCode::Equals: return "Equals";
            case OpCode::Less: return "Less";
            case OpCode::And: return "And";
            case OpCode::Or: return "Or";
            case OpCode::Not: return "Not";
            case OpCode::StringEquals: return "StringEquals";
            case OpCode::StringLength: return "StringLength";
            case OpCode::StringExtract: return "StringExtract";
            case OpCode::Pop: return "Pop";
            case OpCode::ToInteger: return "ToInteger";
            case OpCode::GetVariable: return "GetVariable";
            case OpCode::SetVariable: return "SetVariable";
            case OpCode::SetTarget2: return "SetTarget2";
            case OpCode::StringAdd: return "StringAdd";
            case OpCode::GetProperty: return "GetProperty";
            case OpCode::SetProperty: return "SetProperty";
            case OpCode::CloneSprite: return "CloneSprite";
            case OpCode::RemoveSprite: return "RemoveSprite";
            case OpCode::Trace: return "Trace";
            case OpCode::StartDrag: return "StartDrag";
            case OpCode::EndDrag: return "EndDrag";
            case OpCode::StringLess: return "StringLess";
            case OpCode::Throw: return "Throw";
            case OpCode::CastOp: return "CastOp";
            case OpCode::ImplementsOp: return "ImplementsOp";
            case OpCode::RandomNumber: return "RandomNumber";
            case OpCode::MBStringLength: return "MBStringLength";
            case OpCode::CharToAscii: return "CharToAscii";
            case OpCode::AsciiToChar: return "AsciiToChar";
            case OpCode::GetTime: return "GetTime";
            case OpCode::MBStringExtract: return "MBStringExtract";
            case OpCode::MBCharToAscii: return "MBCharToAscii";
            case OpCode::MBAsciiToChar: return "MBAsciiToChar";
            case OpCode::Delete: return "Delete";
            case OpCode::Delete2: return "Delete2";
            case OpCode::DefineLocal: return "DefineLocal";
            case OpCode::CallFunction: return "CallFunction";
            case OpCode::Return: return "Return";
            case OpCode::Modulo: return "Modulo";
            case OpCode::NewObject: return "NewObject";
            case OpCode::DefineLocal2: return "DefineLocal2";
            case OpCode::InitArray: return "InitArray";
            case OpCode::InitObject: return "InitObject";
            case OpCode::TypeOf: return "TypeOf";
            case OpCode::TargetPath: return "TargetPath";
            case OpCode::Enumerate: return "Enumerate";
            case OpCode::Add2: return "Add2";
            case OpCode::Less2: return "Less2";
            case OpCode::Equals2: return "Equals2";
            case OpCode::ToNumber: return "ToNumber";
            case OpCode::ToString: return "ToString";
            case OpCode::PushDuplicate: return "PushDuplicate";
            case OpCode::StackSwap: return "StackSwap";
            case OpCode::GetMember: return "GetMember";
            case OpCode::SetMember: return "SetMember";
            case OpCode::Increment: return "Increment";
            case OpCode::Decrement: return "Decrement";
            case OpCode::CallMethod: return "CallMethod";
            case OpCode::NewMethod: return "NewMethod";
            case OpCode::InstanceOf: return "InstanceOf";
            case OpCode::Enumerate2: return "Enumerate2";
            case OpCode::BitAnd: return "BitAnd";
            case OpCode::BitOr: return "BitOr";
            case OpCode::BitXor: return "BitXor";
            case OpCode::BitLShift: return "BitLShift";
            case OpCode::BitRShift: return "BitRShift";
            case OpCode::BitURShift: return "BitURShift";
            case OpCode::StrictEquals: return "StrictEquals";
            case OpCode::Greater: return "Greater";
            case OpCode::StringGreater: return "StringGreater";
            case OpCode::Extends: return "Extends";
            case OpCode::GotoFrame: return "GotoFrame";
            case OpCode::GetUrl: return "GetUrl";
            case OpCode::StoreRegister: return "StoreRegister";
            case OpCode::ConstantPool: return "ConstantPool";
            case OpCode::WaitForFrame: return "WaitForFrame";
            case OpCode::SetTarget: return "SetTarget";
            case OpCode::GotoLabel: return "GotoLabel";
            case OpCode::WaitForFrame2: return "WaitForFrame2";
            case OpCode::DefineFunction2: return "DefineFunction2";
            case OpCode::Try: return "Try";
            case OpCode::With: return "With";
            case OpCode::Push: return "Push";
            case OpCode::Jump: return "Jump";
            case OpCode::GetUrl2: return "GetUrl2";
            case OpCode::DefineFunction: return "DefineFunction";
            case OpCode::If: return "If";
            case OpCode::Call: return "Call";
            case OpCode::GotoFrame2: return "GotoFrame2";
        }
    }
    return "Unknown(" + std::to_string(opcode) + ")";
}

} // namespace op_code_utils

} // namespace opcode
} // namespace avm1
} // namespace swf
} // namespace ruffle

#endif // RUFFLE_SWF_AVM1_OPCODE_H
