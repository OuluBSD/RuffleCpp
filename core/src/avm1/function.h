// C++ translation of core/src/avm1/function.rs
//! Code relating to executable functions + calling conventions.

#ifndef RUFFLE_CORE_AVM1_FUNCTION_H
#define RUFFLE_CORE_AVM1_FUNCTION_H

#include <vector>
#include <optional>
#include <string>
#include <cstdint>
#include <memory>
#include <functional>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename GC>
    class Activation;
    template<typename GC>
    class Value;
    template<typename GC>
    class Error;
    template<typename GC>
    class Object;
    template<typename GC>
    class Scope;
    template<typename GC>
    class AvmString;

    enum class NativeObject;

    namespace object {
        template<typename GC>
        class SuperObject;
    }

    namespace object_reference {
        template<typename GC>
        class MovieClipReference;
    }

    namespace property {
        enum class Attribute;
    }
}

namespace display_object {
    template<typename GC>
    class TDisplayObject;
}

namespace string {
    class StringContext;
    class SwfStr;
}

namespace tag_utils {
    struct SwfSlice;
}

namespace gc_arena {
    template<typename T>
    class Gc;
    class Mutation;
    template<typename T>
    class Collect;
}
}
}

namespace swf {
namespace avm1 {
namespace types {
    struct FunctionFlags;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {

using gc_arena::Gc;
using gc_arena::Mutation;

/// Represents a function defined in Ruffle's code.
template<typename GC>
using NativeFunction = std::function<Value<GC>(
    Activation<GC>*,
    Object<GC>,
    const std::vector<Value<GC>>&
)>;

/// Represents a function defined in Ruffle's code, compatible with ASnative.
template<typename GC>
using TableNativeFunction = std::function<Value<GC>(
    Activation<GC>*,
    Object<GC>,
    const std::vector<Value<GC>>&,
    uint16_t
)>;

/// Indicates the reason for an execution
enum class ExecutionReason {
    /// This execution is a "normal" function call from ActionScript bytecode.
    FunctionCall,

    /// This execution is a "normal" constructor call from ActionScript bytecode.
    ConstructorCall,

    /// This execution is a "special" internal function call from the player,
    /// such as getters, setters, `toString`, or event handlers.
    Special,
};

/// Parameter definition for AVM1 functions
template<typename GC>
struct Param {
    /// The register the argument will be preloaded into.
    std::optional<uint8_t> register_index;

    /// The name of the parameter.
    AvmString<GC> name;
};

/// Represents a function defined in the AVM1 runtime, either through
/// `DefineFunction` or `DefineFunction2`.
template<typename GC>
class Avm1Function {
public:
    /// Construct a function from a DefineFunction2 action.
    static Avm1Function<GC> from_swf_function(
        Mutation* gc_context,
        uint8_t swf_version,
        const tag_utils::SwfSlice& actions,
        const swf::avm1::types::DefineFunction2& swf_function,
        Gc<GC, Scope<GC>> scope,
        Gc<GC, std::vector<Value<GC>>> constant_pool,
        object_reference::MovieClipReference<GC> base_clip
    );

    uint8_t swf_version() const { return swf_version_; }
    std::optional<AvmString<GC>> name() const { return name_; }
    Gc<GC, Scope<GC>> scope() const { return scope_; }
    uint8_t register_count() const { return register_count_; }

    /// Execute the given function bytecode.
    Value<GC> exec(
        const std::string& name,
        Activation<GC>* activation,
        Value<GC> this_val,
        uint8_t depth,
        const std::vector<Value<GC>>& args,
        ExecutionReason reason,
        Object<GC> callee
    );

private:
    /// The file format version of the SWF that generated this function.
    uint8_t swf_version_;

    /// A reference to the underlying SWF data.
    tag_utils::SwfSlice data_;

    /// The name of the function, if not anonymous.
    std::optional<AvmString<GC>> name_;

    /// The number of registers to allocate for this function's private register set.
    uint8_t register_count_;

    /// The parameters of the function.
    std::vector<Param<GC>> params_;

    /// The scope the function was born into.
    Gc<GC, Scope<GC>> scope_;

    /// The constant pool the function executes with.
    Gc<GC, std::vector<Value<GC>>> constant_pool_;

    /// The base movie clip that the function was defined on.
    object_reference::MovieClipReference<GC> base_clip_;

    /// The flags that define the preloaded registers of the function.
    swf::avm1::types::FunctionFlags flags_;

    // Helper methods
    void load_this(Activation<GC>* frame, Value<GC> this_val, uint8_t* preload_r);
    void load_arguments(Activation<GC>* frame, const std::vector<Value<GC>>& args,
                       std::optional<Object<GC>> caller, uint8_t* preload_r);
    void load_super(Activation<GC>* frame, std::optional<Object<GC>> this_obj,
                    uint8_t depth, uint8_t* preload_r);
    void load_root(Activation<GC>* frame, uint8_t* preload_r);
    void load_parent(Activation<GC>* frame, uint8_t* preload_r);
    void load_global(Activation<GC>* frame, uint8_t* preload_r);
};

/// Represents a function that can be defined in the Ruffle runtime or by the
/// AVM1 bytecode itself.
template<typename GC>
class FunctionObject {
public:
    /// Builds a new function object.
    Object<GC> build(
        const string::StringContext& context,
        Object<GC> fn_proto,
        std::optional<Object<GC>> prototype
    );

    /// A function that does nothing.
    static FunctionObject<GC> empty();

    /// A function with AVM1 bytecode.
    static FunctionObject<GC> bytecode(Gc<GC, Avm1Function<GC>> function);

    /// A function with a native executable.
    static FunctionObject<GC> native(NativeFunction<GC> function);

    /// A function with a native executable, compatible with ASnative.
    static FunctionObject<GC> table_native(TableNativeFunction<GC> native, uint16_t index);

    /// A native constructor.
    static FunctionObject<GC> constructor(
        NativeFunction<GC> constructor,
        std::optional<NativeFunction<GC>> function
    );

    /// Execute the given code.
    Value<GC> exec(
        const std::string& name,
        Activation<GC>* activation,
        Value<GC> this_val,
        uint8_t depth,
        const std::vector<Value<GC>>& args,
        ExecutionReason reason,
        Object<GC> callee
    );

    /// Execute the given code as a constructor.
    Value<GC> exec_constructor(
        const std::string& name,
        Activation<GC>* activation,
        Value<GC> this_val,
        uint8_t depth,
        const std::vector<Value<GC>>& args,
        ExecutionReason reason,
        Object<GC> callee
    );

    /// Call the function.
    Value<GC> call(
        const std::string& name,
        Activation<GC>* activation,
        Object<GC> callee,
        Value<GC> this_val,
        const std::vector<Value<GC>>& args
    );

    /// Construct on an existing object.
    void construct_on_existing(
        Activation<GC>* activation,
        Object<GC> callee,
        Object<GC> this_obj,
        const std::vector<Value<GC>>& args
    );

    /// Construct a new object.
    Value<GC> construct(
        Activation<GC>* activation,
        Object<GC> callee,
        const std::vector<Value<GC>>& args
    );

private:
    /// Internal enum for executable type
    enum class ExecutableType {
        Native,
        TableNative,
        Action
    };

    struct Executable {
        ExecutableType type;
        NativeFunction<GC> native_fn;
        TableNativeFunction<GC> table_native_fn;
        uint16_t table_index;
        Gc<GC, Avm1Function<GC>> action_fn;

        static Executable native(NativeFunction<GC> fn) {
            return Executable{ExecutableType::Native, fn, nullptr, 0, nullptr};
        }

        static Executable table_native(TableNativeFunction<GC> fn, uint16_t index) {
            return Executable{ExecutableType::TableNative, nullptr, fn, index, nullptr};
        }

        static Executable action(Gc<GC, Avm1Function<GC>> fn) {
            return Executable{ExecutableType::Action, nullptr, nullptr, 0, fn};
        }
    };

    /// The code that will be invoked when this object is called.
    Executable function_;

    /// The code that will be invoked when this object is constructed.
    std::optional<NativeFunction<GC>> constructor_;

    static void define_constructor_props(
        Activation<GC>* activation,
        Object<GC> this_obj,
        Value<GC> callee
    );
};

// ============================================================================
// Template Implementations
// ============================================================================

template<typename GC>
Avm1Function<GC> Avm1Function<GC>::from_swf_function(
    Mutation* gc_context,
    uint8_t swf_version,
    const tag_utils::SwfSlice& actions,
    const swf::avm1::types::DefineFunction2& swf_function,
    Gc<GC, Scope<GC>> scope,
    Gc<GC, std::vector<Value<GC>>> constant_pool,
    object_reference::MovieClipReference<GC> base_clip
) {
    Avm1Function<GC> func;
    func.swf_version_ = swf_version;
    func.data_ = actions;

    // Set name if not empty
    if (!swf_function.name.empty()) {
        // TODO: Decode SWF string properly
        func.name_ = std::optional<AvmString<GC>>();
    }

    func.register_count_ = swf_function.register_count;

    // Convert parameters
    for (const auto& param : swf_function.params) {
        Param<GC> p;
        p.register_index = param.register_index;
        // TODO: Decode parameter name
        func.params_.push_back(p);
    }

    func.scope_ = scope;
    func.constant_pool_ = constant_pool;
    func.base_clip_ = base_clip;
    func.flags_ = swf_function.flags;

    return func;
}

template<typename GC>
void Avm1Function<GC>::load_this(Activation<GC>* frame, Value<GC> this_val, uint8_t* preload_r) {
    bool preload = flags_.contains(FunctionFlags::PRELOAD_THIS);
    bool suppress = flags_.contains(FunctionFlags::SUPPRESS_THIS);

    if (preload) {
        auto this_to_set = suppress ? Value<GC>::undefined() : this_val;
        frame->set_local_register(*preload_r, this_to_set);
        (*preload_r)++;
    }
}

template<typename GC>
void Avm1Function<GC>::load_arguments(
    Activation<GC>* frame,
    const std::vector<Value<GC>>& args,
    std::optional<Object<GC>> caller,
    uint8_t* preload_r
) {
    bool preload = flags_.contains(FunctionFlags::PRELOAD_ARGUMENTS);
    bool suppress = flags_.contains(FunctionFlags::SUPPRESS_ARGUMENTS);

    if (suppress && !preload) {
        return;
    }

    // Create arguments array
    // TODO: Implement ArrayBuilder
    auto arguments = Value<GC>::undefined();  // Placeholder

    // Define callee property
    // arguments.define_value(frame->gc(), "callee", frame->callee, Attribute::DONT_ENUM);

    // Define caller property
    auto caller_val = caller.has_value() ? Value<GC>::from_object(*caller) : Value<GC>::null();
    // arguments.define_value(frame->gc(), "caller", caller_val, Attribute::DONT_ENUM);

    if (preload) {
        frame->set_local_register(*preload_r, arguments);
        (*preload_r)++;
    } else {
        frame->force_define_local("arguments", arguments);
    }
}

template<typename GC>
void Avm1Function<GC>::load_super(
    Activation<GC>* frame,
    std::optional<Object<GC>> this_obj,
    uint8_t depth,
    uint8_t* preload_r
) {
    bool preload = flags_.contains(FunctionFlags::PRELOAD_SUPER);
    bool suppress = flags_.contains(FunctionFlags::SUPPRESS_SUPER);

    std::optional<Value<GC>> zuper;
    if (this_obj.has_value() && !suppress) {
        // TODO: Create SuperObject
        // zuper = NativeObject::Super(SuperObject::new(*this_obj, depth));
    }

    if (preload) {
        auto super_val = zuper.has_value() ? *zuper : Value<GC>::undefined();
        frame->set_local_register(*preload_r, super_val);
        (*preload_r)++;
    } else if (zuper.has_value()) {
        frame->force_define_local("super", *zuper);
    }
}

template<typename GC>
void Avm1Function<GC>::load_root(Activation<GC>* frame, uint8_t* preload_r) {
    if (flags_.contains(FunctionFlags::PRELOAD_ROOT)) {
        auto root = frame->base_clip()->avm1_root()->object1_or_undef();
        frame->set_local_register(*preload_r, root);
        (*preload_r)++;
    }
}

template<typename GC>
void Avm1Function<GC>::load_parent(Activation<GC>* frame, uint8_t* preload_r) {
    if (flags_.contains(FunctionFlags::PRELOAD_PARENT)) {
        if (auto parent = frame->base_clip()->avm1_parent()) {
            frame->set_local_register(*preload_r, parent->object1_or_undef());
            (*preload_r)++;
        }
    }
}

template<typename GC>
void Avm1Function<GC>::load_global(Activation<GC>* frame, uint8_t* preload_r) {
    if (flags_.contains(FunctionFlags::PRELOAD_GLOBAL)) {
        auto global = frame->global_object();
        frame->set_local_register(*preload_r, Value<GC>::from_object(global));
        (*preload_r)++;
    }
}

template<typename GC>
Value<GC> Avm1Function<GC>::exec(
    const std::string& name,
    Activation<GC>* activation,
    Value<GC> this_val,
    uint8_t depth,
    const std::vector<Value<GC>>& args,
    ExecutionReason reason,
    Object<GC> callee
) {
    // Implementation would create a new activation frame and execute bytecode
    // This is a simplified placeholder
    return Value<GC>::undefined();
}

// FunctionObject implementations

template<typename GC>
Object<GC> FunctionObject<GC>::build(
    const string::StringContext& context,
    Object<GC> fn_proto,
    std::optional<Object<GC>> prototype
) {
    auto obj = Object<GC>::new_object(context, std::optional<Object<GC>>());
    // obj.set_native(context.gc(), NativeObject::Function(Gc::new(context.gc(), *this)));

    if (prototype.has_value()) {
        // prototype->define_value(context.gc(), "constructor", Value<GC>::from_object(obj), property::Attribute::DONT_ENUM);
        // obj.define_value(context.gc(), "prototype", Value<GC>::from_object(*prototype), property::Attribute::DONT_ENUM | property::Attribute::DONT_DELETE);
    }

    return obj;
}

template<typename GC>
FunctionObject<GC> FunctionObject<GC>::empty() {
    return FunctionObject<GC>{
        Executable::native([](Activation<GC>*, Object<GC>, const std::vector<Value<GC>>&) {
            return Value<GC>::undefined();
        }),
        std::nullopt
    };
}

template<typename GC>
FunctionObject<GC> FunctionObject<GC>::bytecode(Gc<GC, Avm1Function<GC>> function) {
    return FunctionObject<GC>{
        Executable::action(function),
        std::nullopt
    };
}

template<typename GC>
FunctionObject<GC> FunctionObject<GC>::native(NativeFunction<GC> function) {
    return FunctionObject<GC>{
        Executable::native(function),
        std::nullopt
    };
}

template<typename GC>
FunctionObject<GC> FunctionObject<GC>::table_native(TableNativeFunction<GC> native, uint16_t index) {
    return FunctionObject<GC>{
        Executable::table_native(native, index),
        std::nullopt
    };
}

template<typename GC>
FunctionObject<GC> FunctionObject<GC>::constructor(
    NativeFunction<GC> constructor,
    std::optional<NativeFunction<GC>> function
) {
    return FunctionObject<GC>{
        Executable::native(function.value_or([](Activation<GC>*, Object<GC>, const std::vector<Value<GC>>&) {
            return Value<GC>::undefined();
        })),
        constructor
    };
}

template<typename GC>
Value<GC> FunctionObject<GC>::exec(
    const std::string& name,
    Activation<GC>* activation,
    Value<GC> this_val,
    uint8_t depth,
    const std::vector<Value<GC>>& args,
    ExecutionReason reason,
    Object<GC> callee
) {
    switch (function_.type) {
        case ExecutableType::Native:
            return function_.native_fn(activation, this_val.coerce_to_object_or_bare(activation), args);
        case ExecutableType::TableNative:
            return function_.table_native_fn(activation, this_val.coerce_to_object_or_bare(activation), args, function_.table_index);
        case ExecutableType::Action:
            return function_.action_fn->exec(name, activation, this_val, depth, args, reason, callee);
    }
    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> FunctionObject<GC>::exec_constructor(
    const std::string& name,
    Activation<GC>* activation,
    Value<GC> this_val,
    uint8_t depth,
    const std::vector<Value<GC>>& args,
    ExecutionReason reason,
    Object<GC> callee
) {
    auto constr = constructor_.has_value()
        ? Executable::native(*constructor_)
        : function_;

    switch (constr.type) {
        case ExecutableType::Native:
            return constr.native_fn(activation, this_val.coerce_to_object_or_bare(activation), args);
        case ExecutableType::TableNative:
            return constr.table_native_fn(activation, this_val.coerce_to_object_or_bare(activation), args, constr.table_index);
        case ExecutableType::Action:
            return constr.action_fn->exec(name, activation, this_val, depth, args, reason, callee);
    }
    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> FunctionObject<GC>::call(
    const std::string& name,
    Activation<GC>* activation,
    Object<GC> callee,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    return exec(name, activation, this_val, 0, args, ExecutionReason::FunctionCall, callee);
}

template<typename GC>
void FunctionObject<GC>::construct_on_existing(
    Activation<GC>* activation,
    Object<GC> callee,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
) {
    define_constructor_props(activation, this_obj, Value<GC>::from_object(callee));

    // Always ignore the constructor's return value.
    exec_constructor("[ctor]", activation, Value<GC>::from_object(this_obj), 1,
                     args, ExecutionReason::ConstructorCall, callee);
}

template<typename GC>
Value<GC> FunctionObject<GC>::construct(
    Activation<GC>* activation,
    Object<GC> callee,
    const std::vector<Value<GC>>& args
) {
    auto prototype = callee->get("prototype", activation);
    auto this_obj = Object<GC>::new_object(activation->strings(), prototype.as_object());

    define_constructor_props(activation, this_obj, Value<GC>::from_object(callee));

    // Propagate the return value only for native constructors.
    bool propagate = constructor_.has_value();
    auto ret = exec_constructor("[ctor]", activation, Value<GC>::from_object(this_obj), 1,
                                args, ExecutionReason::ConstructorCall, callee);
    return propagate ? ret : Value<GC>::from_object(this_obj);
}

template<typename GC>
void FunctionObject<GC>::define_constructor_props(
    Activation<GC>* activation,
    Object<GC> this_obj,
    Value<GC> callee
) {
    // this_obj.define_value(activation->gc(), "__constructor__", callee, property::Attribute::DONT_ENUM);
    if (activation->swf_version() < 7) {
        // this_obj.define_value(activation->gc(), "constructor", callee, property::Attribute::DONT_ENUM);
    }
}

} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_FUNCTION_H
