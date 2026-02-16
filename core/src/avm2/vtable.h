// C++ translation of core/src/avm2/vtable.rs
//! Virtual method table for AVM2 classes

#ifndef RUFFLE_CORE_AVM2_VTABLE_H
#define RUFFLE_CORE_AVM2_VTABLE_H

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <optional>
#include <memory>

// Forward declarations
namespace ruffle {
namespace core {
    class UpdateContext;
    class AvmString;
    class StringContext;

    namespace avm2 {
        template<typename T> class Activation;
        template<typename T> class Value;
        template<typename T> class Error;
        template<typename T> class Class;
        template<typename T> class ClassObject;
        template<typename T> class Method;
        template<typename T> class FunctionObject;
        template<typename T> class Multiname;
        template<typename T> class Namespace;
        template<typename T> class QName;
        template<typename T> class ScopeChain;
        template<typename T> class Property;
        template<typename T> class PropertyMap;
        template<typename T> class PropertyClass;
        template<typename T> class Trait;
        template<typename T> class Metadata;

        namespace metadata {
            template<typename GC>
            class Metadata;
        }
        using metadata::Metadata;
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Bound method with class scope information
template<typename GC>
class ClassBoundMethod {
public:
    ClassBoundMethod()
        : super_class_obj_(nullptr)
        , scope_(nullptr)
        , method_(nullptr)
    {}

    ClassBoundMethod(
        std::optional<ClassObject<GC>> super_class_obj,
        ScopeChain<GC> scope,
        Method<GC> method
    )
        : super_class_obj_(super_class_obj)
        , scope_(std::make_shared<ScopeChain<GC>>(scope))
        , method_(method)
    {}

    std::optional<ClassObject<GC>> super_class_obj() const { return super_class_obj_; }
    Method<GC> method() const { return method_; }

    /// Get the scope chain (must exist)
    ScopeChain<GC> scope() const {
        return scope_ ? *scope_ : ScopeChain<GC>();
    }

    /// Set the scope
    void set_scope(ScopeChain<GC> scope) {
        scope_ = std::make_shared<ScopeChain<GC>>(scope);
    }

private:
    std::optional<ClassObject<GC>> super_class_obj_;
    std::shared_ptr<ScopeChain<GC>> scope_;
    Method<GC> method_;
};

/// Data for VTable
template<typename GC>
class VTableData {
public:
    std::optional<ScopeChain<GC>> scope;
    std::optional<Namespace<GC>> protected_namespace;
    PropertyMap<GC, Property<GC>> resolved_traits;

    /// Metadata tables indexed by slot_id and disp_id
    std::unordered_map<uint32_t, std::vector<Metadata<GC>>> slot_metadata_table;
    std::unordered_map<uint32_t, std::vector<Metadata<GC>>> disp_metadata_table;

    /// PropertyClass for each slot, indexed by slot_id
    std::vector<PropertyClass<GC>> slot_classes;

    /// Method table indexed by disp_id
    std::vector<ClassBoundMethod<GC>> method_table;

    /// Default values for slots
    std::vector<std::optional<Value<GC>>> default_slots;

    VTableData() = default;
};

/// Virtual method table for AVM2 classes
///
/// Builds a new vtable by calculating the flattened list of instance traits
/// that a class maintains.
template<typename GC>
class VTable {
public:
    /// Create an empty vtable
    static VTable empty();

    /// Build a new vtable for a class
    ///
    /// \param defining_class_def The class defining this vtable
    /// \param super_class_obj The superclass object, if any
    /// \param scope The scope chain for this class
    /// \param superclass_vtable The vtable of the superclass, if any
    static VTable create(
        Class<GC> defining_class_def,
        std::optional<ClassObject<GC>> super_class_obj,
        std::optional<ScopeChain<GC>> scope,
        std::optional<VTable> superclass_vtable
    );

    /// Like create, but also copies properties from the defining class' interfaces
    static VTable create_with_interface_properties(
        Class<GC> defining_class_def,
        std::optional<ClassObject<GC>> super_class_obj,
        std::optional<ScopeChain<GC>> scope,
        std::optional<VTable> superclass_vtable,
        UpdateContext* context
    );

    /// Get the resolved traits property map
    const PropertyMap<GC, Property<GC>>* resolved_traits() const {
        return &data_->resolved_traits;
    }

    /// Get metadata for a slot
    std::optional<std::vector<Metadata<GC>>> get_metadata_for_slot(uint32_t slot_id) const;

    /// Get metadata for a dispatch ID
    std::optional<std::vector<Metadata<GC>>> get_metadata_for_disp(uint32_t disp_id) const;

    /// Get the class name for a slot
    AvmString slot_class_name(StringContext* context, uint32_t slot_id) const;

    /// Get a trait by name
    std::optional<Property<GC>> get_trait(const Multiname<GC>* name) const;

    /// Get a trait with its namespace
    std::optional<std::pair<Namespace<GC>, Property<GC>>> get_trait_with_ns(
        const Multiname<GC>* name
    ) const;

    /// Coerce a value to the type of a slot
    Value<GC> coerce_trait_value(
        uint32_t slot_id,
        Value<GC> value,
        Activation<GC>* activation
    );

    /// Check if a trait exists
    bool has_trait(const Multiname<GC>* name) const;

    /// Get a method by dispatch ID
    std::optional<Method<GC>> get_method(uint32_t disp_id) const;

    /// Get the full bound method by dispatch ID
    const ClassBoundMethod<GC>* get_full_method(uint32_t disp_id) const;

    /// Get the default slots array
    const std::vector<std::optional<Value<GC>>>& default_slots() const {
        return data_->default_slots;
    }

    /// Get the class for a slot
    std::optional<PropertyClass<GC>> slot_class(uint32_t slot_id) const;

    /// Set the class for a slot
    void set_slot_class(uint32_t slot_id, PropertyClass<GC> value);

    /// Replace all method scopes with a new scope
    void replace_scopes_with(ScopeChain<GC> new_scope);

    /// Get public properties
    std::vector<std::pair<AvmString, Property<GC>>> public_properties() const;

    /// Create a bound method for a receiver
    std::optional<FunctionObject<GC>> make_bound_method(
        UpdateContext* context,
        Value<GC> receiver,
        uint32_t disp_id
    );

    /// Bind a method to a receiver
    static FunctionObject<GC> bind_method(
        UpdateContext* context,
        Value<GC> receiver,
        const ClassBoundMethod<GC>* method
    );

private:
    explicit VTable(std::shared_ptr<VTableData<GC>> data)
        : data_(data) {}

    /// Initialize the vtable from class traits
    static std::shared_ptr<VTableData<GC>> init_vtable(
        Class<GC> defining_class_def,
        std::optional<ClassObject<GC>> super_class_obj,
        std::optional<ScopeChain<GC>> scope,
        std::optional<VTable> superclass_vtable
    );

    /// Copy interface properties to the vtable
    static void copy_interface_properties(
        VTableData<GC>* data,
        Class<GC> class_,
        UpdateContext* context
    );

    std::shared_ptr<VTableData<GC>> data_;
};

/// Convert a trait to its default value
template<typename GC>
Value<GC> trait_to_default_value(const Trait<GC>* trait_data);

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_VTABLE_H
