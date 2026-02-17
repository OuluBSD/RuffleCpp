// C++ translation of core/src/avm2/function.rs
//! AVM2 Function and BoundMethod representation

#ifndef RUFFLE_CORE_AVM2_FUNCTION_H
#define RUFFLE_CORE_AVM2_FUNCTION_H

#include <memory>
#include <vector>
#include <cstdint>
#include <optional>
#include <span>
#include <format>
#include <string>
#include <cell>

// Forward declarations
namespace ruffle {
namespace core {
    class WString;
    template<typename GC> class UpdateContext;
    template<typename GC> class Gc;
    template<typename GC> class GcWeak;

    namespace avm2 {
        template<typename T> class Activation;
        template<typename T> class Method;
        template<typename T> class ParamConfig;
        template<typename T> class Multiname;
        template<typename T> class ClassObject;
        template<typename T> class FunctionObject;
        template<typename T> class Object;
        template<typename T> class Error;
        template<typename T> class Value;
        template<typename T> class ScopeChain;

        enum class MethodKind {
            Native,
            Bytecode
        };
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Represents argument storage for function calls
template<typename GC>
class FunctionArgs {
public:
    FunctionArgs() : data_(nullptr), size_(0), is_cell_(false) {}

    static FunctionArgs empty() {
        return FunctionArgs();
    }

    static FunctionArgs from_slice(const std::span<Value<GC>> args) {
        FunctionArgs result;
        result.data_ = const_cast<Value<GC>*>(args.data());
        result.size_ = args.size();
        result.is_cell_ = false;
        return result;
    }

    static FunctionArgs from_cell_slice(std::span<std::cell::Value<GC>> args) {
        FunctionArgs result;
        result.data_cell_ = args.data();
        result.size_ = args.size();
        result.is_cell_ = true;
        return result;
    }

    Value<GC> get_at(size_t index) const {
        if (is_cell_) {
            return data_cell_[index].get();
        }
        return data_[index];
    }

    size_t len() const { return size_; }

    bool empty() const { return size_ == 0; }

private:
    union {
        Value<GC>* data_;
        std::cell::Value<GC>* data_cell_;
    };
    size_t size_;
    bool is_cell_;
};

/// Iterator for FunctionArgs
template<typename GC>
class FunctionArgsIter {
public:
    FunctionArgsIter(const FunctionArgs<GC>& args, size_t start = 0)
        : args_(args), index_(start) {}

    bool has_next() const { return index_ < args_.len(); }

    Value<GC> next() {
        if (index_ < args_.len()) {
            return args_.get_at(index_++);
        }
        return Value<GC>::undefined();
    }

private:
    const FunctionArgs<GC>& args_;
    size_t index_;
};

/// Represents a bound method
template<typename GC>
class BoundMethod {
public:
    BoundMethod() = default;

    BoundMethod(
        Method<GC> method,
        ScopeChain<GC> scope,
        std::optional<Value<GC>> bound_receiver,
        std::optional<ClassObject<GC>> bound_superclass
    ) : method_(method),
        scope_(scope),
        bound_receiver_(bound_receiver),
        bound_superclass_(bound_superclass) {}

    /// Create a BoundMethod from a method
    static BoundMethod from_method(
        Method<GC> method,
        ScopeChain<GC> scope,
        std::optional<Value<GC>> receiver,
        std::optional<ClassObject<GC>> superclass
    ) {
        return BoundMethod(method, scope, receiver, superclass);
    }

    /// Execute the bound method
    ///
    /// @param unbound_receiver The receiver provided by the caller
    /// @param arguments The function arguments
    /// @param activation The current activation
    /// @param callee Optional callee function object
    /// @return Result value or error
    Value<GC> exec(
        Value<GC> unbound_receiver,
        const FunctionArgs<GC>& arguments,
        Activation<GC>* activation,
        std::optional<FunctionObject<GC>> callee
    );

    /// Get the underlying method
    Method<GC> as_method() const { return method_; }

    /// Get debug full name
    WString debug_full_name() const;

    /// Get the method signature
    const std::vector<ParamConfig<GC>>& signature() const {
        return method_->signature();
    }

    /// Check if the method is variadic
    bool is_variadic() const {
        return method_->is_variadic();
    }

    /// Get the return type
    std::optional<Gc<GC, Multiname<GC>>> return_type() const {
        return method_->return_type();
    }

private:
    Method<GC> method_;
    ScopeChain<GC> scope_;
    std::optional<Value<GC>> bound_receiver_;
    std::optional<ClassObject<GC>> bound_superclass_;
};

/// Execute a method
///
/// The function will either be called directly if it is a Rust builtin, or
/// executed on the same AVM2 instance as the activation passed in here.
///
/// @param method The method to execute
/// @param scope The scope chain
/// @param receiver The receiver value
/// @param bound_superclass Optional bound superclass
/// @param arguments The function arguments
/// @param activation The current activation
/// @param callee Optional callee function object
/// @return Result value or error
template<typename GC>
Value<GC> exec(
    Method<GC> method,
    ScopeChain<GC> scope,
    Value<GC> receiver,
    std::optional<ClassObject<GC>> bound_superclass,
    const FunctionArgs<GC>& arguments,
    Activation<GC>* activation,
    std::optional<FunctionObject<GC>> callee
);

/// Display function name for debugging
template<typename GC>
void display_function(WString* output, Method<GC> method);

// Template method implementations

template<typename GC>
Value<GC> BoundMethod<GC>::exec(
    Value<GC> unbound_receiver,
    const FunctionArgs<GC>& arguments,
    Activation<GC>* activation,
    std::optional<FunctionObject<GC>> callee
) {
    Value<GC> receiver;

    if (bound_receiver_.has_value()) {
        receiver = bound_receiver_.value();
    } else if (unbound_receiver.is_null() || unbound_receiver.is_undefined()) {
        // Use global scope as receiver
        auto global_scope = scope_.get(0);
        if (global_scope.has_value()) {
            receiver = global_scope->values();
        } else {
            receiver = Value<GC>::undefined();
        }
    } else {
        receiver = unbound_receiver;
    }

    return exec<GC>(
        method_,
        scope_,
        receiver,
        bound_superclass_,
        arguments,
        activation,
        callee
    );
}

template<typename GC>
WString BoundMethod<GC>::debug_full_name() const {
    WString output;
    display_function(&output, method_);
    return output;
}

template<typename GC>
Value<GC> exec(
    Method<GC> method,
    ScopeChain<GC> scope,
    Value<GC> receiver,
    std::optional<ClassObject<GC>> bound_superclass,
    const FunctionArgs<GC>& arguments,
    Activation<GC>* activation,
    std::optional<FunctionObject<GC>> callee
) {
    // This is a simplified translation - the full implementation would need
    // proper handling of native vs bytecode methods, stack frames, etc.

    auto caller_dxns = activation->default_xml_namespace();
    auto caller_domain = activation->caller_domain();
    auto caller_movie = activation->caller_movie();

    // In real implementation, this would branch on method_kind()
    // and handle Native vs Bytecode methods differently

    // For native methods:
    // - Create activation from builtin
    // - Resolve method info
    // - Check argument count
    // - Resolve parameters
    // - Call native_method function pointer

    // For bytecode methods:
    // - Get stack frame
    // - Create activation from method
    // - Run actions
    // - Cleanup

    // Placeholder - real implementation would be much more complex
    return Value<GC>::undefined();
}

template<typename GC>
void display_function(WString* output, Method<GC> method) {
    // Get bound class if any
    auto bound_class = method.bound_class();

    if (bound_class.has_value()) {
        auto name = bound_class->name().to_qualified_name_no_mc();
        output->push_str(name);
    }

    // Display method name based on trait
    if (bound_class.has_value()) {
        // Check if this is an instance initializer
        if (bound_class->instance_init() == method) {
            if (bound_class->is_c_class()) {
                output->push_utf8(u"cinit");
            }
            // Otherwise do nothing for instance initializers
        } else {
            // Find the trait for this method
            // This would iterate through bound_class.traits() in real implementation

            // For now, just show method name if available
            if (!method.method_name().empty()) {
                output->push_char(u'/');
                output->push_utf8(method.method_name());
            }
        }
    } else if (method.is_function() && !method.method_name().empty()) {
        output->push_utf8(u"Function/");
        output->push_utf8(method.method_name());
    } else {
        output->push_utf8(u"MethodInfo-");
        output->push_utf8(std::to_string(method.abc_method_index()));
    }

    output->push_utf8(u"()");
}

} // namespace avm2
} // namespace core
} // namespace ruffle

// Debug formatter specialization
template<typename GC>
struct std::formatter<ruffle::core::avm2::BoundMethod<GC>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const ruffle::core::avm2::BoundMethod<GC>& method, FormatContext& ctx) {
        return std::format_to(ctx.out(), "BoundMethod(name={}, ptr={})",
            method.debug_full_name().to_utf8_lossy(),
            static_cast<const void*>(&method));
    }
};

#endif // RUFFLE_CORE_AVM2_FUNCTION_H
