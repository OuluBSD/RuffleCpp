// C++ translation of core/src/avm2/class.rs
// AVM2 classes

#ifndef RUFFLE_CORE_AVM2_CLASS_H
#define RUFFLE_CORE_AVM2_CLASS_H

#include <vector>
#include <optional>
#include <memory>
#include <cstdint>
#include <functional>
#include <string>
#include <unordered_map>
#include <bitset>

// Forward declarations
namespace ruffle {
namespace core {

template<typename GC>
class Mutation;

namespace avm2 {

template<typename GC>
class Activation;
template<typename GC>
class Value;
template<typename GC>
class Error;
template<typename GC>
class Multiname;
template<typename GC>
class Namespace;
template<typename GC>
class QName;
template<typename GC>
class Method;
template<typename GC>
class Trait;
template<typename GC>
class VTable;
template<typename GC>
class ClassObject;
template<typename GC>
class Object;

namespace method {
    template<typename GC>
    using NativeMethodImpl = std::function<Value<GC>(Activation<GC>*, const std::vector<Value<GC>>&)>;
}

/// Class attributes bitflags
enum class ClassAttributes : uint8_t {
    None = 0,
    Sealed = 1 << 0,      // Class is sealed, attempts to set/init dynamic properties will error
    Final = 1 << 1,       // Class is final, cannot be subclassed
    Interface = 1 << 2,   // Class is an interface
    Generic = 1 << 3      // Class accepts type parameters
};

/// Combine ClassAttributes flags
inline ClassAttributes operator|(ClassAttributes a, ClassAttributes b) {
    return static_cast<ClassAttributes>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

inline ClassAttributes operator&(ClassAttributes a, ClassAttributes b) {
    return static_cast<ClassAttributes>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}

inline ClassAttributes& operator|=(ClassAttributes& a, ClassAttributes b) {
    a = a | b;
    return a;
}

/// Builtin type identifiers
enum class BuiltinType {
    None,
    Object,
    Function,
    Class,
    Boolean,
    String,
    Number,
    Int,
    Uint,
    Array,
    // Add more as needed
};

/// A function that can be used to allocate instances of a class.
template<typename GC>
using AllocatorFn = std::function<Object<GC>(ClassObject<GC>, Activation<GC>*)>;

/// Allocator wrapper
template<typename GC>
struct Allocator {
    AllocatorFn<GC> fn;
    
    explicit Allocator(AllocatorFn<GC> f) : fn(std::move(f)) {}
};

/// A function that can be used to both allocate and construct an instance of a class.
template<typename GC>
using CustomConstructorFn = std::function<Value<GC>(Activation<GC>*, const std::vector<Value<GC>>&)>;

/// Custom constructor wrapper
template<typename GC>
struct CustomConstructor {
    CustomConstructorFn<GC> fn;
    
    explicit CustomConstructor(CustomConstructorFn<GC> f) : fn(std::move(f)) {}
};

/// Class link state
enum class ClassLinkType {
    Unlinked,
    LinkToInstance,
    LinkToClass
};

/// Forward declaration
template<typename GC>
class Class;

/// Class data
template<typename GC>
class ClassData {
private:
    /// The name of the class
    QName<GC> name_;
    
    /// The type parameter for this class (only supported for Vector)
    std::optional<std::optional<Class<GC>>> param_;
    
    /// This class's superclass, or None if it has no superclass
    std::optional<Class<GC>> super_class_;
    
    /// Attributes of the given class
    ClassAttributes attributes_;
    
    /// The namespace that protected traits of this class are stored into
    std::optional<Namespace<GC>> protected_namespace_;
    
    /// The list of interfaces this class directly implements
    std::vector<Class<GC>> direct_interfaces_;
    
    /// Interfaces implemented by this class (including from parent classes)
    std::optional<std::vector<Class<GC>>> all_interfaces_;
    
    /// The instance allocator for this class
    Allocator<GC> instance_allocator_;
    
    /// The instance initializer for this class
    std::optional<Method<GC>> instance_init_;
    
    /// Traits for a given class
    std::optional<std::vector<Trait<GC>>> traits_;
    
    /// The class' vtable
    std::optional<VTable<GC>> vtable_;
    
    /// The customization point for Class(args...) without new
    std::optional<typename method::NativeMethodImpl<GC>> call_handler_;
    
    /// The custom constructor for this class
    std::optional<CustomConstructor<GC>> custom_constructor_;
    
    /// The Class this Class is linked to
    ClassLinkType linked_class_type_;
    std::optional<Class<GC>> linked_class_;
    
    /// The special builtin class that this class represents
    BuiltinType builtin_type_;
    
    /// Maps a type parameter to the application of this class with that parameter
    std::unordered_map<size_t, Class<GC>> applications_;
    
    /// The ClassObjects for this class
    std::vector<ClassObject<GC>> class_objects_;
    
public:
    ClassData()
        : attributes_(ClassAttributes::None)
        , instance_allocator_(Allocator<GC>(nullptr))
        , linked_class_type_(ClassLinkType::Unlinked)
        , builtin_type_(BuiltinType::None) {}
    
    // Getters
    const QName<GC>& name() const { return name_; }
    const std::optional<std::optional<Class<GC>>>& param() const { return param_; }
    const std::optional<Class<GC>>& super_class() const { return super_class_; }
    ClassAttributes attributes() const { return attributes_; }
    const std::optional<Namespace<GC>>& protected_namespace() const { return protected_namespace_; }
    const std::vector<Class<GC>>& direct_interfaces() const { return direct_interfaces_; }
    const Allocator<GC>& instance_allocator() const { return instance_allocator_; }
    const std::optional<Method<GC>>& instance_init() const { return instance_init_; }
    const std::optional<std::vector<Trait<GC>>>& traits() const { return traits_; }
    const std::optional<VTable<GC>>& vtable() const { return vtable_; }
    const std::optional<typename method::NativeMethodImpl<GC>>& call_handler() const { return call_handler_; }
    const std::optional<CustomConstructor<GC>>& custom_constructor() const { return custom_constructor_; }
    BuiltinType builtin_type() const { return builtin_type_; }
    
    // Setters
    void set_name(QName<GC> name) { name_ = std::move(name); }
    void set_param(std::optional<std::optional<Class<GC>>> param) { param_ = std::move(param); }
    void set_super_class(std::optional<Class<GC>> super_class) { super_class_ = std::move(super_class); }
    void set_attributes(ClassAttributes attributes) { attributes_ = attributes; }
    void set_protected_namespace(std::optional<Namespace<GC>> ns) { protected_namespace_ = std::move(ns); }
    void set_direct_interfaces(std::vector<Class<GC>> interfaces) { direct_interfaces_ = std::move(interfaces); }
    void set_instance_allocator(Allocator<GC> allocator) { instance_allocator_ = std::move(allocator); }
    void set_instance_init(std::optional<Method<GC>> init) { instance_init_ = std::move(init); }
    void set_traits(std::optional<std::vector<Trait<GC>>> traits) { traits_ = std::move(traits); }
    void set_vtable(std::optional<VTable<GC>> vtable) { vtable_ = std::move(vtable); }
    void set_call_handler(std::optional<typename method::NativeMethodImpl<GC>> handler) { call_handler_ = std::move(handler); }
    void set_custom_constructor(std::optional<CustomConstructor<GC>> ctor) { custom_constructor_ = std::move(ctor); }
    void set_builtin_type(BuiltinType type) { builtin_type_ = type; }
    
    // Class object management
    void add_class_object(ClassObject<GC> class_object) {
        class_objects_.push_back(class_object);
    }
    
    const std::vector<ClassObject<GC>>& class_objects() const {
        return class_objects_;
    }
    
    std::optional<ClassObject<GC>> class_object() const {
        if (class_objects_.size() == 1) {
            return class_objects_[0];
        }
        return std::nullopt;
    }
    
    // Application management
    void add_application(Class<GC> param, Class<GC> cls) {
        applications_[reinterpret_cast<size_t>(param.as_ptr())] = cls;
    }
    
    std::optional<Class<GC>> get_application(Class<GC> param) const {
        auto it = applications_.find(reinterpret_cast<size_t>(param.as_ptr()));
        if (it != applications_.end()) {
            return it->second;
        }
        return std::nullopt;
    }
};

/// A loaded ABC Class which can be used to construct objects with
template<typename GC>
class Class {
private:
    std::shared_ptr<ClassData<GC>> data_;
    
public:
    Class() = default;
    explicit Class(std::shared_ptr<ClassData<GC>> data) : data_(std::move(data)) {}
    
    /// Get raw pointer for comparison
    const void* as_ptr() const {
        return data_.get();
    }
    
    /// Create an unlinked class from its name, superclass, and traits
    static Class custom_new(
        QName<GC> name,
        std::optional<Class<GC>> super_class,
        std::optional<Method<GC>> instance_init,
        std::vector<Trait<GC>> traits,
        Mutation<GC>* mc
    ) {
        auto data = std::make_shared<ClassData<GC>>();
        data->set_name(std::move(name));
        data->set_super_class(std::move(super_class));
        data->set_instance_init(std::move(instance_init));
        data->set_traits(std::move(traits));
        return Class(data);
    }
    
    /// Apply type parameters to an existing class
    static Class with_type_param(
        Class<GC> this_class,
        std::optional<Class<GC>> param
        // UpdateContext would be needed for full implementation
    );
    
    /// Set the attributes of the class
    void set_attributes(ClassAttributes attributes) {
        if (data_) {
            data_->set_attributes(attributes);
        }
    }
    
    /// Add a class object
    void add_class_object(Mutation<GC>* mc, ClassObject<GC> class_object) {
        if (data_) {
            data_->add_class_object(class_object);
        }
    }
    
    /// Get class objects
    const std::vector<ClassObject<GC>>& class_objects() const {
        static const std::vector<ClassObject<GC>> empty;
        return data_ ? data_->class_objects() : empty;
    }
    
    /// Get single class object if only one exists
    std::optional<ClassObject<GC>> class_object() const {
        return data_ ? data_->class_object() : std::nullopt;
    }
    
    /// Get the name of the class
    const QName<GC>& name() const {
        static QName<GC> empty;
        return data_ ? data_->name() : empty;
    }
    
    /// Get the type parameter
    const std::optional<std::optional<Class<GC>>>& param() const {
        static const std::optional<std::optional<Class<GC>>> empty;
        return data_ ? data_->param() : empty;
    }
    
    /// Get the superclass
    std::optional<Class<GC>> super_class() const {
        return data_ ? data_->super_class() : std::nullopt;
    }
    
    /// Get the attributes
    ClassAttributes attributes() const {
        return data_ ? data_->attributes() : ClassAttributes::None;
    }
    
    /// Get the protected namespace
    std::optional<Namespace<GC>> protected_namespace() const {
        return data_ ? data_->protected_namespace() : std::nullopt;
    }
    
    /// Get the direct interfaces
    const std::vector<Class<GC>>& direct_interfaces() const {
        static const std::vector<Class<GC>> empty;
        return data_ ? data_->direct_interfaces() : empty;
    }
    
    /// Get the instance allocator
    const Allocator<GC>& instance_allocator() const {
        static const Allocator<GC> empty(nullptr);
        return data_ ? data_->instance_allocator() : empty;
    }
    
    /// Get the instance init method
    std::optional<Method<GC>> instance_init() const {
        return data_ ? data_->instance_init() : std::nullopt;
    }
    
    /// Get the traits
    std::optional<std::vector<Trait<GC>>> traits() const {
        return data_ ? data_->traits() : std::nullopt;
    }
    
    /// Get the vtable
    std::optional<VTable<GC>> vtable() const {
        return data_ ? data_->vtable() : std::nullopt;
    }
    
    /// Get the call handler
    std::optional<typename method::NativeMethodImpl<GC>> call_handler() const {
        return data_ ? data_->call_handler() : std::nullopt;
    }
    
    /// Get the custom constructor
    std::optional<CustomConstructor<GC>> custom_constructor() const {
        return data_ ? data_->custom_constructor() : std::nullopt;
    }
    
    /// Get the builtin type
    BuiltinType builtin_type() const {
        return data_ ? data_->builtin_type() : BuiltinType::None;
    }
    
    /// Check if class is sealed
    bool is_sealed() const {
        return data_ && (data_->attributes() & ClassAttributes::Sealed) != ClassAttributes::None;
    }
    
    /// Check if class is final
    bool is_final() const {
        return data_ && (data_->attributes() & ClassAttributes::Final) != ClassAttributes::None;
    }
    
    /// Check if class is an interface
    bool is_interface() const {
        return data_ && (data_->attributes() & ClassAttributes::Interface) != ClassAttributes::None;
    }
    
    /// Check if class is generic
    bool is_generic() const {
        return data_ && (data_->attributes() & ClassAttributes::Generic) != ClassAttributes::None;
    }
    
    /// Equality operators
    bool operator==(const Class<GC>& other) const {
        return data_ == other.data_;
    }
    
    bool operator!=(const Class<GC>& other) const {
        return !(*this == other);
    }
    
    /// Check if valid
    explicit operator bool() const {
        return data_ != nullptr;
    }
};

/// Default implicit allocator
template<typename GC>
Object<GC> implicit_allocator(ClassObject<GC> class_obj, Activation<GC>* activation);

/// Script object allocator
template<typename GC>
Object<GC> scriptobject_allocator(ClassObject<GC> class_obj, Activation<GC>* activation);

/// Abstract class allocator for classes that are not constructible
template<typename GC>
Result<Object<GC>, Error<GC>> abstract_class_allocator(
    ClassObject<GC> class_object,
    Activation<GC>* activation
);

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_CLASS_H
