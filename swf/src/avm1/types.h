// C++ translation of swf/src/avm1/types.rs
// AVM1 action types

#ifndef RUFFLE_SWF_AVM1_TYPES_H
#define RUFFLE_SWF_AVM1_TYPES_H

#include <vector>
#include <string>
#include <cstdint>
#include <optional>
#include <variant>
#include <memory>

// Forward declarations
namespace ruffle {
namespace swf {
    class SwfStr;

    namespace avm1 {
        namespace opcode {
            enum class OpCode : uint8_t;
        }
    }
}
}

namespace ruffle {
namespace swf {
namespace avm1 {
namespace types {

/// Constant pool for AVM1 actions
struct ConstantPool {
    std::vector<std::string> strings;

    bool operator==(const ConstantPool& other) const = default;
};

/// DefineFunction action
struct DefineFunction {
    std::string name;
    std::vector<std::string> params;
    std::vector<uint8_t> actions;

    bool operator==(const DefineFunction& other) const = default;
};

/// Function parameter with optional register index
struct FunctionParam {
    std::string name;
    std::optional<uint8_t> register_index;

    bool operator==(const FunctionParam& other) const = default;
};

/// DefineFunction2 action with flags
struct DefineFunction2 {
    std::string name;
    uint8_t register_count;
    std::vector<FunctionParam> params;
    uint16_t flags;
    std::vector<uint8_t> actions;

    bool operator==(const DefineFunction2& other) const = default;

    /// Convert from DefineFunction
    static DefineFunction2 from_define_function(const DefineFunction& function) {
        DefineFunction2 result;
        result.name = function.name;
        result.register_count = 0;
        result.actions = function.actions;
        result.flags = 0;

        for (const auto& param : function.params) {
            FunctionParam fp;
            fp.name = param;
            fp.register_index = std::nullopt;
            result.params.push_back(fp);
        }

        return result;
    }
};

/// Function flags for DefineFunction2
struct FunctionFlags {
    static constexpr uint16_t PRELOAD_THIS = 1 << 0;
    static constexpr uint16_t SUPPRESS_THIS = 1 << 1;
    static constexpr uint16_t PRELOAD_ARGUMENTS = 1 << 2;
    static constexpr uint16_t SUPPRESS_ARGUMENTS = 1 << 3;
    static constexpr uint16_t PRELOAD_SUPER = 1 << 4;
    static constexpr uint16_t SUPPRESS_SUPER = 1 << 5;
    static constexpr uint16_t PRELOAD_ROOT = 1 << 6;
    static constexpr uint16_t PRELOAD_PARENT = 1 << 7;
    static constexpr uint16_t PRELOAD_GLOBAL = 1 << 8;
};

/// GetUrl action
struct GetUrl {
    std::string url;
    std::string target;

    bool operator==(const GetUrl& other) const = default;
};

/// SendVarsMethod enumeration
enum class SendVarsMethod : uint8_t {
    None = 0,
    Get = 1,
    Post = 2,
};

/// GetUrl2 flags
struct GetUrl2Flags {
    static constexpr uint8_t METHOD_NONE = 0;
    static constexpr uint8_t METHOD_GET = 1;
    static constexpr uint8_t METHOD_POST = 2;
    static constexpr uint8_t METHOD_MASK = 3;
    static constexpr uint8_t LOAD_TARGET = 1 << 6;
    static constexpr uint8_t LOAD_VARIABLES = 1 << 7;
};

/// GetUrl2 action with flags
class GetUrl2 {
private:
    uint8_t flags_;

public:
    explicit GetUrl2(uint8_t flags = 0) : flags_(flags) {}

    /// Returns the flags for an AVM1 `loadMovie` call.
    static GetUrl2 for_load_movie(SendVarsMethod method) {
        GetUrl2 result(GET_URL_FLAGS_LOAD_TARGET);
        result.set_send_vars_method(method);
        return result;
    }

    /// Returns the flags for an AVM1 `getURL` call.
    static GetUrl2 for_get_url(SendVarsMethod method) {
        GetUrl2 result(0);
        result.set_send_vars_method(method);
        return result;
    }

    /// Returns the flags for an AVM1 `loadVariables` or `LoadVars.load` call.
    static GetUrl2 for_load_vars(SendVarsMethod method) {
        GetUrl2 result(GET_URL_FLAGS_LOAD_VARIABLES);
        result.set_send_vars_method(method);
        return result;
    }

    /// The HTTP method used for sending data.
    SendVarsMethod send_vars_method() const {
        switch (flags_ & METHOD_MASK) {
            case METHOD_NONE: return SendVarsMethod::None;
            case METHOD_GET: return SendVarsMethod::Get;
            case METHOD_POST: return SendVarsMethod::Post;
            default: return SendVarsMethod::None;
        }
    }

    /// Sets the HTTP method used for sending data.
    void set_send_vars_method(SendVarsMethod method) {
        flags_ &= ~METHOD_MASK;
        flags_ |= static_cast<uint8_t>(method);
    }

    /// Whether this action will load a movie or image into a display object.
    bool is_target_sprite() const {
        return (flags_ & LOAD_TARGET) != 0;
    }

    /// Whether this action will load variables into an ActionScript object.
    bool is_load_vars() const {
        return (flags_ & LOAD_VARIABLES) != 0;
    }

    uint8_t flags() const { return flags_; }

    bool operator==(const GetUrl2& other) const = default;

private:
    static constexpr uint8_t METHOD_NONE = 0;
    static constexpr uint8_t METHOD_GET = 1;
    static constexpr uint8_t METHOD_POST = 2;
    static constexpr uint8_t METHOD_MASK = 3;
    static constexpr uint8_t LOAD_TARGET = 1 << 6;
    static constexpr uint8_t LOAD_VARIABLES = 1 << 7;
};

/// GotoFrame action
struct GotoFrame {
    uint16_t frame;

    bool operator==(const GotoFrame& other) const = default;
};

/// GotoFrame2 action
struct GotoFrame2 {
    bool set_playing;
    uint16_t scene_offset;

    bool operator==(const GotoFrame2& other) const = default;
};

/// GotoLabel action
struct GotoLabel {
    std::string label;

    bool operator==(const GotoLabel& other) const = default;
};

/// If action
struct If {
    int16_t offset;

    bool operator==(const If& other) const = default;
};

/// Jump action
struct Jump {
    int16_t offset;

    bool operator==(const Jump& other) const = default;
};

/// Value types for Push action
class Value {
public:
    /// Value variant types
    enum class Type {
        Undefined,
        Null,
        Bool,
        Int,
        Float,
        Double,
        Str,
        Register,
        ConstantPool,
    };

private:
    Type type_;
    bool bool_value_;
    int32_t int_value_;
    float float_value_;
    double double_value_;
    std::string str_value_;
    uint8_t register_value_;
    uint16_t constant_pool_value_;

public:
    static Value undefined() {
        Value v;
        v.type_ = Type::Undefined;
        return v;
    }

    static Value null() {
        Value v;
        v.type_ = Type::Null;
        return v;
    }

    static Value boolean(bool value) {
        Value v;
        v.type_ = Type::Bool;
        v.bool_value_ = value;
        return v;
    }

    static Value integer(int32_t value) {
        Value v;
        v.type_ = Type::Int;
        v.int_value_ = value;
        return v;
    }

    static Value float_(float value) {
        Value v;
        v.type_ = Type::Float;
        v.float_value_ = value;
        return v;
    }

    static Value double_(double value) {
        Value v;
        v.type_ = Type::Double;
        v.double_value_ = value;
        return v;
    }

    static Value string(std::string value) {
        Value v;
        v.type_ = Type::Str;
        v.str_value_ = std::move(value);
        return v;
    }

    static Value register_(uint8_t value) {
        Value v;
        v.type_ = Type::Register;
        v.register_value_ = value;
        return v;
    }

    static Value constant_pool(uint16_t value) {
        Value v;
        v.type_ = Type::ConstantPool;
        v.constant_pool_value_ = value;
        return v;
    }

    Type type() const { return type_; }

    bool bool_val() const { return bool_value_; }
    int32_t int_val() const { return int_value_; }
    float float_val() const { return float_value_; }
    double double_val() const { return double_value_; }
    const std::string& str_val() const { return str_value_; }
    uint8_t register_val() const { return register_value_; }
    uint16_t constant_pool_val() const { return constant_pool_value_; }

    bool operator==(const Value& other) const {
        if (type_ != other.type_) return false;
        switch (type_) {
            case Type::Undefined: return true;
            case Type::Null: return true;
            case Type::Bool: return bool_value_ == other.bool_value_;
            case Type::Int: return int_value_ == other.int_value_;
            case Type::Float: return float_value_ == other.float_value_;
            case Type::Double: return double_value_ == other.double_value_;
            case Type::Str: return str_value_ == other.str_value_;
            case Type::Register: return register_value_ == other.register_value_;
            case Type::ConstantPool: return constant_pool_value_ == other.constant_pool_value_;
        }
        return false;
    }
};

/// Push action
struct Push {
    std::vector<Value> values;

    bool operator==(const Push& other) const = default;
};

/// SetTarget action
struct SetTarget {
    std::string target;

    bool operator==(const SetTarget& other) const = default;
};

/// StoreRegister action
struct StoreRegister {
    uint8_t register_;

    bool operator==(const StoreRegister& other) const = default;
};

/// Catch variable for Try action
class CatchVar {
public:
    enum class Type {
        Var,
        Register,
    };

private:
    Type type_;
    std::string var_value_;
    uint8_t register_value_;

public:
    static CatchVar var(std::string value) {
        CatchVar v;
        v.type_ = Type::Var;
        v.var_value_ = std::move(value);
        return v;
    }

    static CatchVar register_(uint8_t value) {
        CatchVar v;
        v.type_ = Type::Register;
        v.register_value_ = value;
        return v;
    }

    Type type() const { return type_; }
    const std::string& var_val() const { return var_value_; }
    uint8_t register_val() const { return register_value_; }

    bool operator==(const CatchVar& other) const {
        if (type_ != other.type_) return false;
        switch (type_) {
            case Type::Var: return var_value_ == other.var_value_;
            case Type::Register: return register_value_ == other.register_value_;
        }
        return false;
    }
};

/// Try flags
struct TryFlags {
    static constexpr uint8_t CATCH_BLOCK = 1 << 0;
    static constexpr uint8_t FINALLY_BLOCK = 1 << 1;
    static constexpr uint8_t CATCH_IN_REGISTER = 1 << 2;
};

/// Try action
struct Try {
    std::vector<uint8_t> try_body;
    std::optional<std::pair<CatchVar, std::vector<uint8_t>>> catch_body;
    std::optional<std::vector<uint8_t>> finally_body;

    bool operator==(const Try& other) const = default;
};

/// WaitForFrame action
struct WaitForFrame {
    uint16_t frame;
    uint8_t num_actions_to_skip;

    bool operator==(const WaitForFrame& other) const = default;
};

/// WaitForFrame2 action
struct WaitForFrame2 {
    uint8_t num_actions_to_skip;

    bool operator==(const WaitForFrame2& other) const = default;
};

/// With action
struct With {
    std::vector<uint8_t> actions;

    bool operator==(const With& other) const = default;
};

/// Unknown action
struct Unknown {
    uint8_t opcode;
    std::vector<uint8_t> data;

    bool operator==(const Unknown& other) const = default;
};

/// AVM1 Action enumeration
/// Each variant represents a different AVM1 bytecode action
class Action {
public:
    /// Action variant types
    enum class Type {
        Add,
        Add2,
        And,
        AsciiToChar,
        BitAnd,
        BitLShift,
        BitOr,
        BitRShift,
        BitURShift,
        BitXor,
        Call,
        CallFunction,
        CallMethod,
        CastOp,
        CharToAscii,
        CloneSprite,
        ConstantPool,
        Decrement,
        DefineFunction,
        DefineFunction2,
        DefineLocal,
        DefineLocal2,
        Delete,
        Delete2,
        Divide,
        End,
        EndDrag,
        Enumerate,
        Enumerate2,
        Equals,
        Equals2,
        Extends,
        GetMember,
        GetProperty,
        GetTime,
        GetUrl,
        GetUrl2,
        GetVariable,
        GotoFrame,
        GotoFrame2,
        GotoLabel,
        Greater,
        If,
        ImplementsOp,
        Increment,
        InitArray,
        InitObject,
        InstanceOf,
        Jump,
        Less,
        Less2,
        MBAsciiToChar,
        MBCharToAscii,
        MBStringExtract,
        MBStringLength,
        Modulo,
        Multiply,
        NewMethod,
        NewObject,
        NextFrame,
        Not,
        Or,
        Play,
        Pop,
        PreviousFrame,
        Push,
        PushDuplicate,
        RandomNumber,
        RemoveSprite,
        Return,
        SetMember,
        SetProperty,
        SetTarget,
        SetTarget2,
        SetVariable,
        StackSwap,
        StartDrag,
        Stop,
        StopSounds,
        StoreRegister,
        StrictEquals,
        StringAdd,
        StringEquals,
        StringExtract,
        StringGreater,
        StringLength,
        StringLess,
        Subtract,
        TargetPath,
        Throw,
        ToInteger,
        ToNumber,
        ToString,
        ToggleQuality,
        Trace,
        Try,
        TypeOf,
        WaitForFrame,
        WaitForFrame2,
        With,
        Unknown,
    };

private:
    Type type_;

    // Storage for each action type
    std::optional<ConstantPool> constant_pool_;
    std::optional<DefineFunction> define_function_;
    std::optional<DefineFunction2> define_function2_;
    std::optional<GetUrl> get_url_;
    std::optional<GetUrl2> get_url2_;
    std::optional<GotoFrame> goto_frame_;
    std::optional<GotoFrame2> goto_frame2_;
    std::optional<GotoLabel> goto_label_;
    std::optional<If> if_;
    std::optional<Jump> jump_;
    std::optional<Push> push_;
    std::optional<SetTarget> set_target_;
    std::optional<StoreRegister> store_register_;
    std::optional<Try> try_;
    std::optional<WaitForFrame> wait_for_frame_;
    std::optional<WaitForFrame2> wait_for_frame2_;
    std::optional<With> with_;
    std::optional<Unknown> unknown_;

public:
    // Static factory methods for each action type
    static Action add() { return Action(Type::Add); }
    static Action add2() { return Action(Type::Add2); }
    static Action and_() { return Action(Type::And); }
    static Action ascii_to_char() { return Action(Type::AsciiToChar); }
    static Action bit_and() { return Action(Type::BitAnd); }
    static Action bit_lshift() { return Action(Type::BitLShift); }
    static Action bit_or() { return Action(Type::BitOr); }
    static Action bit_rshift() { return Action(Type::BitRShift); }
    static Action bit_urshift() { return Action(Type::BitURShift); }
    static Action bit_xor() { return Action(Type::BitXor); }
    static Action call() { return Action(Type::Call); }
    static Action call_function() { return Action(Type::CallFunction); }
    static Action call_method() { return Action(Type::CallMethod); }
    static Action cast_op() { return Action(Type::CastOp); }
    static Action char_to_ascii() { return Action(Type::CharToAscii); }
    static Action clone_sprite() { return Action(Type::CloneSprite); }
    static Action constant_pool(ConstantPool value) {
        Action a(Type::ConstantPool);
        a.constant_pool_ = std::move(value);
        return a;
    }
    static Action decrement() { return Action(Type::Decrement); }
    static Action define_function(DefineFunction value) {
        Action a(Type::DefineFunction);
        a.define_function_ = std::move(value);
        return a;
    }
    static Action define_function2(DefineFunction2 value) {
        Action a(Type::DefineFunction2);
        a.define_function2_ = std::move(value);
        return a;
    }
    static Action define_local() { return Action(Type::DefineLocal); }
    static Action define_local2() { return Action(Type::DefineLocal2); }
    static Action delete_() { return Action(Type::Delete); }
    static Action delete2() { return Action(Type::Delete2); }
    static Action divide() { return Action(Type::Divide); }
    static Action end() { return Action(Type::End); }
    static Action end_drag() { return Action(Type::EndDrag); }
    static Action enumerate() { return Action(Type::Enumerate); }
    static Action enumerate2() { return Action(Type::Enumerate2); }
    static Action equals() { return Action(Type::Equals); }
    static Action equals2() { return Action(Type::Equals2); }
    static Action extends() { return Action(Type::Extends); }
    static Action get_member() { return Action(Type::GetMember); }
    static Action get_property() { return Action(Type::GetProperty); }
    static Action get_time() { return Action(Type::GetTime); }
    static Action get_url(GetUrl value) {
        Action a(Type::GetUrl);
        a.get_url_ = std::move(value);
        return a;
    }
    static Action get_url2(GetUrl2 value) {
        Action a(Type::GetUrl2);
        a.get_url2_ = std::move(value);
        return a;
    }
    static Action get_variable() { return Action(Type::GetVariable); }
    static Action goto_frame(GotoFrame value) {
        Action a(Type::GotoFrame);
        a.goto_frame_ = std::move(value);
        return a;
    }
    static Action goto_frame2(GotoFrame2 value) {
        Action a(Type::GotoFrame2);
        a.goto_frame2_ = std::move(value);
        return a;
    }
    static Action goto_label(GotoLabel value) {
        Action a(Type::GotoLabel);
        a.goto_label_ = std::move(value);
        return a;
    }
    static Action greater() { return Action(Type::Greater); }
    static Action if_(If value) {
        Action a(Type::If);
        a.if_ = std::move(value);
        return a;
    }
    static Action implements_op() { return Action(Type::ImplementsOp); }
    static Action increment() { return Action(Type::Increment); }
    static Action init_array() { return Action(Type::InitArray); }
    static Action init_object() { return Action(Type::InitObject); }
    static Action instance_of() { return Action(Type::InstanceOf); }
    static Action jump(Jump value) {
        Action a(Type::Jump);
        a.jump_ = std::move(value);
        return a;
    }
    static Action less() { return Action(Type::Less); }
    static Action less2() { return Action(Type::Less2); }
    static Action mb_ascii_to_char() { return Action(Type::MBAsciiToChar); }
    static Action mb_char_to_ascii() { return Action(Type::MBCharToAscii); }
    static Action mb_string_extract() { return Action(Type::MBStringExtract); }
    static Action mb_string_length() { return Action(Type::MBStringLength); }
    static Action modulo() { return Action(Type::Modulo); }
    static Action multiply() { return Action(Type::Multiply); }
    static Action new_method() { return Action(Type::NewMethod); }
    static Action new_object() { return Action(Type::NewObject); }
    static Action next_frame() { return Action(Type::NextFrame); }
    static Action not_() { return Action(Type::Not); }
    static Action or_() { return Action(Type::Or); }
    static Action play() { return Action(Type::Play); }
    static Action pop() { return Action(Type::Pop); }
    static Action previous_frame() { return Action(Type::PreviousFrame); }
    static Action push(Push value) {
        Action a(Type::Push);
        a.push_ = std::move(value);
        return a;
    }
    static Action push_duplicate() { return Action(Type::PushDuplicate); }
    static Action random_number() { return Action(Type::RandomNumber); }
    static Action remove_sprite() { return Action(Type::RemoveSprite); }
    static Action return_() { return Action(Type::Return); }
    static Action set_member() { return Action(Type::SetMember); }
    static Action set_property() { return Action(Type::SetProperty); }
    static Action set_target(SetTarget value) {
        Action a(Type::SetTarget);
        a.set_target_ = std::move(value);
        return a;
    }
    static Action set_target2() { return Action(Type::SetTarget2); }
    static Action set_variable() { return Action(Type::SetVariable); }
    static Action stack_swap() { return Action(Type::StackSwap); }
    static Action start_drag() { return Action(Type::StartDrag); }
    static Action stop() { return Action(Type::Stop); }
    static Action stop_sounds() { return Action(Type::StopSounds); }
    static Action store_register(StoreRegister value) {
        Action a(Type::StoreRegister);
        a.store_register_ = std::move(value);
        return a;
    }
    static Action strict_equals() { return Action(Type::StrictEquals); }
    static Action string_add() { return Action(Type::StringAdd); }
    static Action string_equals() { return Action(Type::StringEquals); }
    static Action string_extract() { return Action(Type::StringExtract); }
    static Action string_greater() { return Action(Type::StringGreater); }
    static Action string_length() { return Action(Type::StringLength); }
    static Action string_less() { return Action(Type::StringLess); }
    static Action subtract() { return Action(Type::Subtract); }
    static Action target_path() { return Action(Type::TargetPath); }
    static Action throw_() { return Action(Type::Throw); }
    static Action to_integer() { return Action(Type::ToInteger); }
    static Action to_number() { return Action(Type::ToNumber); }
    static Action to_string() { return Action(Type::ToString); }
    static Action toggle_quality() { return Action(Type::ToggleQuality); }
    static Action trace() { return Action(Type::Trace); }
    static Action try_(Try value) {
        Action a(Type::Try);
        a.try_ = std::move(value);
        return a;
    }
    static Action type_of() { return Action(Type::TypeOf); }
    static Action wait_for_frame(WaitForFrame value) {
        Action a(Type::WaitForFrame);
        a.wait_for_frame_ = std::move(value);
        return a;
    }
    static Action wait_for_frame2(WaitForFrame2 value) {
        Action a(Type::WaitForFrame2);
        a.wait_for_frame2_ = std::move(value);
        return a;
    }
    static Action with_(With value) {
        Action a(Type::With);
        a.with_ = std::move(value);
        return a;
    }
    static Action unknown(Unknown value) {
        Action a(Type::Unknown);
        a.unknown_ = std::move(value);
        return a;
    }

    Type type() const { return type_; }

    // Accessors
    const ConstantPool* constant_pool() const { return constant_pool_ ? &*constant_pool_ : nullptr; }
    const DefineFunction* define_function() const { return define_function_ ? &*define_function_ : nullptr; }
    const DefineFunction2* define_function2() const { return define_function2_ ? &*define_function2_ : nullptr; }
    const GetUrl* get_url() const { return get_url_ ? &*get_url_ : nullptr; }
    const GetUrl2* get_url2() const { return get_url2_ ? &*get_url2_ : nullptr; }
    const GotoFrame* goto_frame() const { return goto_frame_ ? &*goto_frame_ : nullptr; }
    const GotoFrame2* goto_frame2() const { return goto_frame2_ ? &*goto_frame2_ : nullptr; }
    const GotoLabel* goto_label() const { return goto_label_ ? &*goto_label_ : nullptr; }
    const If* if_action() const { return if_ ? &*if_ : nullptr; }
    const Jump* jump() const { return jump_ ? &*jump_ : nullptr; }
    const Push* push() const { return push_ ? &*push_ : nullptr; }
    const SetTarget* set_target() const { return set_target_ ? &*set_target_ : nullptr; }
    const StoreRegister* store_register() const { return store_register_ ? &*store_register_ : nullptr; }
    const Try* try_action() const { return try_ ? &*try_ : nullptr; }
    const WaitForFrame* wait_for_frame() const { return wait_for_frame_ ? &*wait_for_frame_ : nullptr; }
    const WaitForFrame2* wait_for_frame2() const { return wait_for_frame2_ ? &*wait_for_frame2_ : nullptr; }
    const With* with_action() const { return with_ ? &*with_ : nullptr; }
    const Unknown* unknown() const { return unknown_ ? &*unknown_ : nullptr; }

    bool operator==(const Action& other) const {
        if (type_ != other.type_) return false;
        // Compare the stored values based on type
        switch (type_) {
            case Type::ConstantPool: return constant_pool_ == other.constant_pool_;
            case Type::DefineFunction: return define_function_ == other.define_function_;
            case Type::DefineFunction2: return define_function2_ == other.define_function2_;
            case Type::GetUrl: return get_url_ == other.get_url_;
            case Type::GetUrl2: return get_url2_ == other.get_url2_;
            case Type::GotoFrame: return goto_frame_ == other.goto_frame_;
            case Type::GotoFrame2: return goto_frame2_ == other.goto_frame2_;
            case Type::GotoLabel: return goto_label_ == other.goto_label_;
            case Type::If: return if_ == other.if_;
            case Type::Jump: return jump_ == other.jump_;
            case Type::Push: return push_ == other.push_;
            case Type::SetTarget: return set_target_ == other.set_target_;
            case Type::StoreRegister: return store_register_ == other.store_register_;
            case Type::Try: return try_ == other.try_;
            case Type::WaitForFrame: return wait_for_frame_ == other.wait_for_frame_;
            case Type::WaitForFrame2: return wait_for_frame2_ == other.wait_for_frame2_;
            case Type::With: return with_ == other.with_;
            case Type::Unknown: return unknown_ == other.unknown_;
            default: return true;  // Simple variants with no data
        }
    }

private:
    explicit Action(Type type) : type_(type) {}
};

} // namespace types
} // namespace avm1
} // namespace swf
} // namespace ruffle

#endif // RUFFLE_SWF_AVM1_TYPES_H
