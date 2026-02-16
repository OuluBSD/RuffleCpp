// C++ translation of core/src/avm2/object/function_object.rs
//! Function object implementation for AVM2

#ifndef RUFFLE_CORE_AVM2_OBJECT_FUNCTION_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_FUNCTION_OBJECT_H

#include <memory>
#include <cstdint>
#include <optional>
#include <format>
#include <string>

// Forward declarations
namespace ruffle {
namespace core {
    class WString;
    template<typename GC> class UpdateContext;

    namespace avm2 {
        template<typename T> class Activation;
        template<typename T> class ScriptObject;
        template<typename T> class ScriptObjectData;
        template<typename T> class TObject;
        template<typename T> class Gc;
        template<typename T> class GcWeak;
        template<typename T> class ClassObject;
        template<typename T> class Object;
        template<typename T> class Error;
        template<typename T> class Value;
        template<typename T> class ScopeChain;
        template<typename T> class AvmString;
        template<typename T> class BoundMethod;
        template<typename T> class Method;
        template<typename T> class FunctionArgs;
        template<typename T> class Mutation;
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Weak reference to FunctionObject data
template<typename GC>
class FunctionObjectWeak;

/// Data structure for FunctionObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class FunctionObjectData {
public:
    FunctionObjectData() = default;

    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    /// Executable code
    BoundMethod<GC> exec;

    /// Attached prototype (note: not the same thing as base object's proto)
    std::optional<Object<GC>> prototype;

    // Friend classes for accessing private members
    friend class FunctionObject<GC>;
    friend class FunctionObjectWeak<GC>;
};

/// FunctionObject - An Object which can be called to execute its function code
///
/// Represents a function in ActionScript 3. Can be called and constructed.
template<typename GC>
class FunctionObject {
public:
    using DataType = FunctionObjectData<GC>;
    using WeakType = FunctionObjectWeak<GC>;

    FunctionObject() = default;
    explicit FunctionObject(Gc<GC, DataType> data) : data_(data) {}

    /// Construct a function from an ABC method and the current closure scope.
    ///
    /// This associated constructor will also create and initialize an empty
    /// Object prototype for the function. The given receiver, if supplied,
    /// will override any user-specified this parameter.
    ///
    /// It is the caller's responsibility to ensure that the receiver passed
    /// to this method is not Value::Null or Value::Undefined.
    ///
    /// @param context The update context
    /// @param method The ABC method
    /// @param scope The scope chain
    /// @param receiver Optional receiver to override this parameter
    /// @param bound_superclass_object Optional bound superclass object
    /// @return New FunctionObject
    static FunctionObject from_method(
        UpdateContext<GC>* context,
        Method<GC> method,
        ScopeChain<GC> scope,
        std::optional<Value<GC>> receiver,
        std::optional<ClassObject<GC>> bound_superclass_object
    );

    /// Call the function
    ///
    /// @param activation The current activation context
    /// @param receiver The this value for the call
    /// @param arguments The arguments to pass
    /// @return The result value or error
    Value<GC> call(
        Activation<GC>* activation,
        Value<GC> receiver,
        FunctionArgs<GC> arguments
    );

    /// Construct an instance using this function as a constructor
    ///
    /// @param activation The current activation context
    /// @param arguments The arguments to pass
    /// @return The constructed object or error
    Object<GC> construct(
        Activation<GC>* activation,
        FunctionArgs<GC> arguments
    );

    /// Get the prototype
    /// @return The prototype object, or nullopt if not set
    std::optional<Object<GC>> prototype() const {
        return data_->prototype;
    }

    /// Set the prototype
    /// @param proto The new prototype
    /// @param mc The mutation context for GC barrier
    void set_prototype(std::optional<Object<GC>> proto, Mutation<GC>* mc);

    /// Get the executable method
    /// @return Reference to the bound method
    const BoundMethod<GC>& executable() const {
        return data_->exec;
    }

    /// Get the base script object data (for TObject trait)
    Gc<GC, ScriptObjectData<GC>> gc_base() const {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base;
    }

    /// Convert to string representation
    ///
    /// @param mc The mutation context
    /// @return String representation like "[object Function-<index>]"
    AvmString<GC> to_string(Mutation<GC>* mc) const;

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to FunctionObject
template<typename GC>
class FunctionObjectWeak {
public:
    FunctionObjectWeak() = default;
    explicit FunctionObjectWeak(GcWeak<GC, FunctionObjectData<GC>> data)
        : data_(data) {}

private:
    GcWeak<GC, FunctionObjectData<GC>> data_;
};

// Template method implementations

template<typename GC>
FunctionObject<GC> FunctionObject<GC>::from_method(
    UpdateContext<GC>* context,
    Method<GC> method,
    ScopeChain<GC> scope,
    std::optional<Value<GC>> receiver,
    std::optional<ClassObject<GC>> bound_superclass_object
) {
    auto fn_class = context->avm2()->classes()->function;
    auto exec = BoundMethod<GC>::from_method(method, scope, receiver, bound_superclass_object);

    auto es3_proto = ScriptObject<GC>::new_object(context);

    auto data = Gc<GC, DataType>::create(
        context->gc(),
        DataType{
            ScriptObjectData<GC>::create(fn_class),
            exec,
            std::make_optional(es3_proto)
        }
    );

    auto function_object = FunctionObject<GC>(data);

    // Set the constructor property on the prototype to point back to this function
    auto constructor_prop = AvmString<GC>::from_utf8(context->gc(), "constructor");
    es3_proto.set_dynamic_property(constructor_prop, Value<GC>::from_object(function_object), context->gc());
    es3_proto.set_local_property_is_enumerable(context->gc(), constructor_prop, false);

    return function_object;
}

template<typename GC>
Value<GC> FunctionObject<GC>::call(
    Activation<GC>* activation,
    Value<GC> receiver,
    FunctionArgs<GC> arguments
) {
    auto& exec = data_->exec;
    return exec.exec(receiver, arguments, activation, std::make_optional(*this));
}

template<typename GC>
Object<GC> FunctionObject<GC>::construct(
    Activation<GC>* activation,
    FunctionArgs<GC> arguments
) {
    auto& method = data_->exec.as_method();
    if (method.bound_class().has_value()) {
        // If the Method is class-bound, attempting to construct it throws
        // an error
        throw make_error_1064(activation, method);
    }

    auto object_class = activation->avm2()->classes()->object;

    auto proto = prototype().value_or(object_class.prototype());
    if (!prototype().has_value()) {
        set_prototype(std::make_optional(proto), activation->gc());
    }

    auto instance = ScriptObject<GC>::custom_object(
        activation->gc(),
        object_class.inner_class_definition(),
        std::make_optional(proto),
        object_class.instance_vtable()
    );

    auto result = call(activation, Value<GC>::from_object(instance), arguments);

    // If the constructor returns an object, use that instead of the created instance
    if (result.is_object()) {
        return result.as_object();
    }
    return instance;
}

template<typename GC>
void FunctionObject<GC>::set_prototype(std::optional<Object<GC>> proto, Mutation<GC>* mc) {
    data_->prototype = proto;
}

template<typename GC>
AvmString<GC> FunctionObject<GC>::to_string(Mutation<GC>* mc) const {
    auto& method = data_->exec.as_method();
    auto method_index = method.abc_method_index();

    return AvmString<GC>::from_utf8(mc, std::format("[object Function-{}]", method_index));
}

} // namespace avm2
} // namespace core
} // namespace ruffle

// Debug formatter specialization
template<typename GC>
struct std::formatter<ruffle::core::avm2::FunctionObject<GC>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const ruffle::core::avm2::FunctionObject<GC>& obj, FormatContext& ctx) {
        auto& method = obj.data_->exec.as_method();
        auto method_index = method.abc_method_index();
        return std::format_to(ctx.out(), "FunctionObject(ptr={}, name={})",
            static_cast<const void*>(obj.data_.get()),
            method_index);
    }
};

#endif // RUFFLE_CORE_AVM2_OBJECT_FUNCTION_OBJECT_H
