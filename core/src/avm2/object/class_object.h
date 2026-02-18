// C++ translation of core/src/avm2/object/class_object.rs
//! Class object implementation for AVM2

#ifndef RUFFLE_CORE_AVM2_OBJECT_CLASS_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_CLASS_OBJECT_H

#include <memory>
#include <cstdint>
#include <optional>
#include <unordered_map>

// Forward declarations
namespace ruffle {
namespace core {
    class WString;
    template<typename GC> class UpdateContext;
    template<typename GC> class Mutation;

    namespace avm2 {
        template<typename T> class Activation;
        template<typename T> class ScriptObject;
        template<typename T> class ScriptObjectData;
        template<typename T> class TObject;
        template<typename T> class Gc;
        template<typename T> class GcWeak;
        template<typename T> class Class;
        template<typename T> class Object;
        template<typename T> class Error;
        template<typename T> class Value;
        template<typename T> class ScopeChain;
        template<typename T> class AvmString;
        template<typename T> class VTable;
        template<typename T> class Multiname;
        template<typename T> class QName;
        template<typename T> class TranslationUnit;
        template<typename T> class FunctionArgs;
        template<typename T> class Method;
        template<typename T> class MethodAssociation;
        template<typename T> class Property;
        template<typename T> class Lock;
        template<typename T> class RefLock;
        template<typename T> class HasPrefixField;

        struct CustomConstructorFn;
        using AllocatorFn = void*;
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Weak reference to ClassObject data
template<typename GC>
class ClassObjectWeak;

/// Data structure for ClassObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class ClassObjectData {
public:
    ClassObjectData() = default;

    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    /// The class associated with this class object
    Class<GC> class_;

    /// The associated prototype. Should always be non-None after initialization
    Lock<GC, std::optional<Object<GC>>> prototype;

    /// The captured scope that all class traits will use
    ScopeChain<GC> class_scope;

    /// The captured scope that all instance traits will use
    Lock<GC, ScopeChain<GC>> instance_scope;

    /// The base class of this one. If None, this class has no parent
    std::optional<ClassObject<GC>> superclass_object;

    /// List of all applications of this class (only applicable if generic)
    RefLock<GC, std::unordered_map<std::optional<Class<GC>>, ClassObject<GC>>> applications;

    /// VTable used for instances of this class
    Lock<GC, VTable<GC>> instance_vtable;

    // Friend classes for accessing private members
    friend class ClassObject<GC>;
    friend class ClassObjectWeak<GC>;
};

/// ClassObject - An Object which represents a class in AVM2
///
/// Represents a class constructor and its associated metadata.
/// Can be called to construct instances of the class.
template<typename GC>
class ClassObject {
public:
    using DataType = ClassObjectData<GC>;
    using WeakType = ClassObjectWeak<GC>;

    ClassObject() = default;
    explicit ClassObject(Gc<GC, DataType> data) : data_(data) {}

    /// Construct a class from a Class definition
    ///
    /// This function returns the class constructor object, which should be
    /// used in all cases where the type needs to be referred to.
    ///
    /// @param activation The current activation
    /// @param class_def The class definition
    /// @param superclass_object Optional superclass object
    /// @return Result ClassObject or error
    static std::optional<ClassObject> from_class(
        Activation<GC>* activation,
        Class<GC> class_def,
        std::optional<ClassObject> superclass_object
    );

    /// Like from_class, but skips method binding, validation, and initializer
    static ClassObject from_class_partial(
        Activation<GC>* activation,
        Class<GC> class_def,
        std::optional<ClassObject> superclass_object
    );

    /// Allocate a class but do not properly construct it
    static ClassObject from_class_minimal(
        Activation<GC>* activation,
        Class<GC> class_def,
        std::optional<ClassObject> superclass_object
    );

    /// Allocate the prototype for this class
    Object<GC> allocate_prototype(
        UpdateContext<GC>* context,
        std::optional<ClassObject> superclass_object
    );

    /// Bind all the methods declared by the ClassObject to itself
    void bind_methods(Activation<GC>* activation);

    /// Validate signatures of the class
    void validate_class(Activation<GC>* activation);

    /// Run the class's initializer method
    void run_class_initializer(Activation<GC>* activation);

    /// Finish initialization of the class
    void into_finished_class(Activation<GC>* activation);

    /// Link this class to a prototype
    void link_prototype(UpdateContext<GC>* context, Object<GC> class_proto);

    /// Manually set the type of this Class
    void link_type(Mutation<GC>* gc_context, Object<GC> proto);

    /// Call the instance initializer
    Value<GC> call_init(
        Value<GC> receiver,
        FunctionArgs<GC> arguments,
        Activation<GC>* activation
    );

    /// Call a method defined in this class (for supercalls)
    Value<GC> call_super(
        Multiname<GC>* multiname,
        Object<GC> receiver,
        FunctionArgs<GC> arguments,
        Activation<GC>* activation
    );

    /// Call a getter defined in this class
    Value<GC> get_super(
        Multiname<GC>* multiname,
        Object<GC> receiver,
        Activation<GC>* activation
    );

    /// Call a setter defined in this class
    void set_super(
        Multiname<GC>* multiname,
        Value<GC> value,
        Object<GC> receiver,
        Activation<GC>* activation
    );

    /// Add an application of this generic class
    void add_application(
        Mutation<GC>* mc,
        std::optional<Class<GC>> param,
        ClassObject<GC> cls
    );

    /// Parametrize this class (does not check if class is generic)
    ClassObject<GC> parametrize(
        Mutation<GC>* mc,
        std::optional<Class<GC>> param
    );

    /// Get the inner class definition (i_class)
    Class<GC> inner_class_definition() const {
        return data_->class_;
    }

    /// Get the prototype object
    Object<GC> prototype() const {
        return data_->prototype.get().value_or(Object<GC>{});
    }

    /// Get the superclass object
    std::optional<ClassObject<GC>> superclass_object() const {
        return data_->superclass_object;
    }

    /// Get the instance scope chain
    ScopeChain<GC> instance_scope() const {
        return data_->instance_scope.get();
    }

    /// Get the class scope chain
    ScopeChain<GC> class_scope() const {
        return data_->class_scope;
    }

    /// Get the instance vtable
    VTable<GC> instance_vtable() const {
        return data_->instance_vtable.get();
    }

    /// Set the vtable
    void set_vtable(Mutation<GC>* mc, VTable<GC> vtable);

    /// Get the init method
    std::optional<Method<GC>> init_method() const;

    /// Get the base script object data
    ScriptObjectData<GC> base() const {
        return data_->base;
    }

    /// Initialize the instance vtable
    void init_instance_vtable(Activation<GC>* activation);

private:
    Gc<GC, DataType> data_;

    /// Call a method super helper
    Value<GC> call_method_super(
        Activation<GC>* activation,
        Object<GC> receiver,
        uint32_t disp_id,
        FunctionArgs<GC> arguments
    );
};

/// Weak reference to ClassObject
template<typename GC>
class ClassObjectWeak {
public:
    ClassObjectWeak() = default;
    explicit ClassObjectWeak(GcWeak<GC, ClassObjectData<GC>> data)
        : data_(data) {}

    /// Upgrade the weak reference to a strong reference
    std::optional<ClassObject<GC>> upgrade(Mutation<GC>* mc) const {
        if (auto strong = data_.upgrade(mc)) {
            return std::make_optional(ClassObject<GC>(strong));
        }
        return std::nullopt;
    }

private:
    GcWeak<GC, ClassObjectData<GC>> data_;
};

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_OBJECT_CLASS_OBJECT_H
