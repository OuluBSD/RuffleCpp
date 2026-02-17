// C++ translation of core/src/avm2/method.rs
//! AVM2 methods

#ifndef RUFFLE_CORE_AVM2_METHOD_H
#define RUFFLE_CORE_AVM2_METHOD_H

#include <vector>
#include <optional>
#include <memory>
#include <cstdint>
#include <string>
#include <rc>
#include <sync/atomic>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC>
    class Activation;
    template<typename GC>
    class Value;
    template<typename GC>
    class Error;
    template<typename GC>
    class Multiname;
    template<typename GC>
    class Class;
    template<typename GC>
    class AvmString;

    namespace avm2 {
        template<typename GC>
        class TranslationUnit;
        template<typename GC>
        class VerifiedMethodInfo;
        template<typename GC>
        class Method;
        template<typename GC>
        class MethodAssociation;

        namespace script {
            template<typename T> class TranslationUnit;
        }
    }

    namespace tag_utils {
        template<typename T> class SwfMovie;
    }

    namespace gc_arena {
        template<typename T>
        class Gc;
        template<typename T>
        class OnceLock;
        class Mutation;
    }
}
}

namespace swf {
namespace avm2 {
namespace types {
    struct AbcFile;
    struct Method;
    struct MethodBody;
    struct MethodFlags;
    struct MethodParam;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {

using gc_arena::Gc;
using gc_arena::OnceLock;
using gc_arena::Mutation;

/// Represents a function defined in Ruffle's code.
///
/// Parameters are as follows:
///
///  * The AVM2 runtime
///  * The current `this` object
///  * The arguments this function was called with
///
/// Native functions are allowed to return a Value or an Error.
template<typename GC>
using NativeMethodImpl = Value<GC> (*)(
    Activation<GC>*,
    Value<GC>,
    const std::vector<Value<GC>>&
);

/// Configuration of a single parameter of a method,
/// with the parameter's type resolved.
template<typename GC>
struct ResolvedParamConfig {
    /// The type of the parameter.
    std::optional<Class<GC>> param_type;

    /// The default value for this parameter.
    std::optional<Value<GC>> default_value;
};

/// Configuration of a single parameter of a method.
template<typename GC>
struct ParamConfig {
    /// The name of the type of the parameter.
    std::optional<Gc<GC, Multiname<GC>>> param_type_name;

    /// The default value for this parameter.
    std::optional<Value<GC>> default_value;

    /// Create an optional parameter config
    static ParamConfig optional(
        std::optional<Gc<GC, Multiname<GC>>> param_type_name,
        Value<GC> default_value
    ) {
        return ParamConfig {
            .param_type_name = param_type_name,
            .default_value = default_value
        };
    }
};

/// Represents info for either a bytecode or native method
template<typename GC>
struct MethodKind {
    enum class Kind {
        Bytecode,
        Native
    };

    Kind kind;

    union Data {
        struct BytecodeData {
            OnceLock<VerifiedMethodInfo<GC>> verified_info;
        } bytecode;

        struct NativeData {
            NativeMethodImpl<GC> native_method;
            bool fast_call;
        } native;

        Data() {}
        ~Data() {}
    } data;

    static MethodKind bytecode() {
        MethodKind result;
        result.kind = Kind::Bytecode;
        return result;
    }

    static MethodKind native(NativeMethodImpl<GC> native_method, bool fast_call) {
        MethodKind result;
        result.kind = Kind::Native;
        result.data.native = {native_method, fast_call};
        return result;
    }
};

/// The info this method is associated with. This includes the bound Class and
/// whether the method should be run in interpreter mode. The association is
/// used to ensure that, for example, a SWF cannot use `newfunction` to create
/// a freestanding function for a method that is also class-bound, as this would
/// break the verifier/optimizer.
template<typename GC>
class MethodAssociation {
private:
    /// The class this method is bound to. Once set, this cannot be changed.
    /// The method may only be called with a receiver that is an instance of
    /// this class.
    std::optional<Class<GC>> bound_class_;

    /// Whether this method should be run in "interpreter mode" (as opposed to
    /// "JIT mode"). Most methods run in "JIT mode", except for class
    /// initializer and script initializer methods, which always run in
    /// "interpreter mode".
    bool is_interpreted_;

public:
    /// Create a freestanding method association
    static MethodAssociation freestanding() {
        return MethodAssociation {
            .bound_class_ = std::nullopt,
            .is_interpreted_ = false
        };
    }

    /// Create a class-bound method association
    static MethodAssociation classbound(Class<GC> bound_class, bool is_interpreted) {
        return MethodAssociation {
            .bound_class_ = bound_class,
            .is_interpreted_ = is_interpreted
        };
    }

    std::optional<Class<GC>> bound_class() const { return bound_class_; }
    bool is_interpreted() const { return is_interpreted_; }
};

/// Internal method data
template<typename GC>
struct MethodData {
    /// The translation unit this function was defined in.
    TranslationUnit<GC> txunit;

    /// The underlying ABC file of the above translation unit.
    std::shared_ptr<swf::avm2::types::AbcFile> abc;

    /// The ABC method this function uses.
    uint32_t abc_method;

    /// The ABC method body this function uses.
    std::optional<uint32_t> abc_method_body;

    MethodKind<GC> method_kind;

    /// The parameter signature of this method.
    std::vector<ParamConfig<GC>> signature;

    /// The return type of this method, or None if the method does not coerce
    /// its return value.
    std::optional<Gc<GC, Multiname<GC>>> return_type;

    /// The resolved signature and return type.
    OnceLock<ResolvedMethodInfo<GC>> resolved_info;

    /// The class that this method is bound to.
    OnceLock<MethodAssociation<GC>> association;

    /// Whether or not this method was declared as a free-standing function.
    bool is_function;

    /// Whether or not this method substitutes Undefined for missing arguments.
    bool is_unchecked;
};

/// The resolved parameters and return type of a method.
template<typename GC>
struct ResolvedMethodInfo {
    std::vector<ResolvedParamConfig<GC>> param_config;
    std::optional<Class<GC>> return_type;
};

/// Represents a reference to an AVM2 method and body.
template<typename GC>
class Method {
private:
    Gc<GC, MethodData<GC>> data_;

public:
    /// Construct a `Method` from an `AbcFile` and method index.
    static Method from_method_index(
        TranslationUnit<GC> txunit,
        uint32_t abc_method,
        bool is_function,
        Activation<GC>* activation
    );

    /// Get the underlying ABC file.
    std::shared_ptr<swf::avm2::types::AbcFile> abc() const {
        return data_->abc;
    }

    /// Get the underlying translation unit this method was defined in.
    TranslationUnit<GC> translation_unit() const {
        return data_->txunit;
    }

    uint32_t abc_method_index() const {
        return data_->abc_method;
    }

    /// Get a reference to the SwfMovie this method came from.
    std::shared_ptr<tag_utils::SwfMovie> owner_movie() const {
        return data_->txunit.movie();
    }

    /// Verify the method
    void verify(Activation<GC>* activation);

    /// Get the list of method params for this method.
    const std::vector<ParamConfig<GC>>& signature() const {
        return data_->signature;
    }

    const std::vector<ResolvedParamConfig<GC>>& resolved_param_config() const {
        return data_->resolved_info.get()->param_config;
    }

    std::optional<Class<GC>> resolved_return_type() const {
        return data_->resolved_info.get()->return_type;
    }

    const VerifiedMethodInfo<GC>* get_verified_info() const {
        if (data_->method_kind.kind == MethodKind<GC>::Kind::Bytecode) {
            return &data_->method_kind.data.bytecode.verified_info.get();
        }
        // Should not be called on native methods
        return nullptr;
    }

    /// Resolve the classes used in this method's signature and return type.
    void resolve_info(Activation<GC>* activation);

    /// Get the name of this method.
    std::string method_name() const;

    /// Determine if a given method is variadic.
    ///
    /// Variadic methods shove excess parameters into a final register.
    bool is_variadic() const;

    /// Check if this method needs `arguments`.
    bool needs_arguments_object() const;

    /// Check if this method sets the default XML namespace (uses dxns/dxnslate opcodes).
    bool sets_dxns() const;

    const MethodKind<GC>& method_kind() const {
        return data_->method_kind;
    }

    std::optional<Gc<GC, Multiname<GC>>> return_type() const {
        return data_->return_type;
    }

    /// Get the bound class that this method has been associated with, or
    /// `None` if the method is associated as freestanding.
    std::optional<Class<GC>> bound_class() const {
        if (auto assoc = data_->association.get()) {
            return assoc->bound_class();
        }
        return std::nullopt;
    }

    /// Returns true if this method should be run in "interpreter mode".
    bool is_interpreted() const {
        if (auto assoc = data_->association.get()) {
            return assoc->is_interpreted();
        }
        return false;
    }

    /// Get the `MethodAssociation` that this method is associated with.
    std::optional<MethodAssociation<GC>> method_association() const {
        return data_->association.get();
    }

    /// Mark this method as associated with the given `MethodAssociation`.
    void associate(
        Activation<GC>* activation,
        MethodAssociation<GC> new_association
    );

    /// Check that this method has been associated as bound to a class.
    void check_classbound(Activation<GC>* activation);

    bool is_function() const {
        return data_->is_function;
    }

    /// Determine if a given method is unchecked.
    ///
    /// A method is unchecked if both of the following are true:
    ///
    ///  * The method was declared as a free-standing function
    ///  * The function's parameters have no declared types or default values
    bool is_unchecked() const {
        return data_->is_unchecked;
    }

private:
    explicit Method(Gc<GC, MethodData<GC>> data) : data_(data) {}
};

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_METHOD_H
