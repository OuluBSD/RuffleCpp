// C++ translation of core/src/avm2/property.rs
//! Property data structures

#ifndef RUFFLE_CORE_AVM2_PROPERTY_H
#define RUFFLE_CORE_AVM2_PROPERTY_H

#include <cstdint>
#include <optional>
#include <variant>
#include <string>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename T> class Gc;
    template<typename T> class Mutation;
    namespace string {
        template<typename T> class AvmString;
        template<typename T> class StringContext;
    }
    namespace avm2 {
        template<typename T> class Activation;
        template<typename T> class Domain;
        template<typename T> class Multiname;
        template<typename T> class Value;
        template<typename T> class Error;
        template<typename T> class Class;
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Property data structures
///
/// Represents different kinds of properties in AVM2 classes:
/// - Virtual properties (getter/setter methods)
/// - Method properties (direct method references)
/// - Slot properties (storage slots)
/// - ConstSlot properties (constant storage slots)
template<typename GC>
class Property {
public:
    /// Virtual property with getter and/or setter
    struct Virtual {
        std::optional<uint32_t> get_disp_id;  // Getter dispatch ID
        std::optional<uint32_t> set_disp_id;  // Setter dispatch ID
    };

    /// Method property
    struct Method {
        uint32_t disp_id;  // Dispatch ID
    };

    /// Slot property (mutable storage)
    struct Slot {
        uint32_t slot_id;
    };

    /// ConstSlot property (constant storage)
    struct ConstSlot {
        uint32_t slot_id;
    };

private:
    std::variant<Virtual, Method, Slot, ConstSlot> data_;

public:
    /// Create a virtual property
    static Property virtual_prop(std::optional<uint32_t> get_id, std::optional<uint32_t> set_id) {
        Property prop;
        prop.data_ = Virtual{get_id, set_id};
        return prop;
    }

    /// Create a method property
    static Property method(uint32_t disp_id) {
        Property prop;
        prop.data_ = Method{disp_id};
        return prop;
    }

    /// Create a slot property
    static Property slot(uint32_t slot_id) {
        Property prop;
        prop.data_ = Slot{slot_id};
        return prop;
    }

    /// Create a const slot property
    static Property const_slot(uint32_t slot_id) {
        Property prop;
        prop.data_ = ConstSlot{slot_id};
        return prop;
    }

    /// Convenience constructors
    static Property new_method(uint32_t disp_id) {
        return method(disp_id);
    }

    static Property new_getter(uint32_t disp_id) {
        return virtual_prop(disp_id, std::nullopt);
    }

    static Property new_setter(uint32_t disp_id) {
        return virtual_prop(std::nullopt, disp_id);
    }

    static Property new_slot(uint32_t slot_id) {
        return slot(slot_id);
    }

    static Property new_const_slot(uint32_t slot_id) {
        return const_slot(slot_id);
    }

    /// Check if this is a virtual property
    [[nodiscard]] bool is_virtual() const {
        return std::holds_alternative<Virtual>(data_);
    }

    /// Check if this is a method property
    [[nodiscard]] bool is_method() const {
        return std::holds_alternative<Method>(data_);
    }

    /// Check if this is a slot property
    [[nodiscard]] bool is_slot() const {
        return std::holds_alternative<Slot>(data_);
    }

    /// Check if this is a const slot property
    [[nodiscard]] bool is_const_slot() const {
        return std::holds_alternative<ConstSlot>(data_);
    }

    /// Get the getter dispatch ID if this is a virtual property
    [[nodiscard]] std::optional<uint32_t> get_getter_id() const {
        if (auto* virt = std::get_if<Virtual>(&data_)) {
            return virt->get_disp_id;
        }
        return std::nullopt;
    }

    /// Get the setter dispatch ID if this is a virtual property
    [[nodiscard]] std::optional<uint32_t> get_setter_id() const {
        if (auto* virt = std::get_if<Virtual>(&data_)) {
            return virt->set_disp_id;
        }
        return std::nullopt;
    }

    /// Get the dispatch ID if this is a method property
    [[nodiscard]] std::optional<uint32_t> get_method_disp_id() const {
        if (auto* method = std::get_if<Method>(&data_)) {
            return method->disp_id;
        }
        return std::nullopt;
    }

    /// Get the slot ID if this is a slot or const slot property
    [[nodiscard]] std::optional<uint32_t> get_slot_id() const {
        if (auto* slot = std::get_if<Slot>(&data_)) {
            return slot->slot_id;
        }
        if (auto* const_slot = std::get_if<ConstSlot>(&data_)) {
            return const_slot->slot_id;
        }
        return std::nullopt;
    }
};

/// The type of a `Slot`/`ConstSlot` property, represented
/// as a lazily-resolved class. This also implements the
/// property-specific coercion logic applied when setting
/// or initializing a property.
///
/// The class resolution needs to be lazy, since we can have
/// a cycle of property type references between classes
/// (e.g. Class1 has `var prop1:Class2`, and Class2
/// has `var prop2:Class1`).
///
/// Additionally, property class resolution uses special
/// logic, different from normal "runtime" class resolution,
/// that allows private types to be referenced.
template<typename GC>
class PropertyClass {
public:
    /// The type `*`. This allows `Value::Undefined`, so it needs to
    /// be distinguished from the `Object` class
    struct AnyType {};

    /// A resolved class reference
    struct ClassType {
        Class<GC> class_ref;
    };

    /// A name that needs to be resolved
    struct NameType {
        Gc<GC, Multiname<GC>> name;
        Domain<GC> domain;
    };

private:
    std::variant<AnyType, ClassType, NameType> data_;

public:
    /// Create an Any type (represents `*`)
    static PropertyClass any() {
        PropertyClass pc;
        pc.data_ = AnyType{};
        return pc;
    }

    /// Create a Class type
    static PropertyClass from_class(Class<GC> class_ref) {
        PropertyClass pc;
        pc.data_ = ClassType{class_ref};
        return pc;
    }

    /// Create a Name type
    static PropertyClass from_name(Gc<GC, Multiname<GC>> name, Domain<GC> domain) {
        PropertyClass pc;
        pc.data_ = NameType{name, domain};
        return pc;
    }

    /// Create a PropertyClass from an optional name
    static PropertyClass name(std::optional<Gc<GC, Multiname<GC>>> name, Domain<GC> domain) {
        if (name.has_value()) {
            return from_name(name.value(), domain);
        }
        return any();
    }

    /// Returns `value` coerced to the type of this `PropertyClass`.
    /// The bool is `true` if this `PropertyClass` was just modified
    /// to cache a class resolution, and `false` if it was not modified.
    std::pair<Value<GC>, bool> coerce(
        Activation<GC>* activation,
        Value<GC> value
    );

    /// Get the resolved class, resolving the name if necessary
    std::optional<Class<GC>> get_class(Activation<GC>* activation);

    /// Get the name as an AvmString
    string::AvmString<GC> get_name(string::StringContext<GC>* context);

    /// Check if this is the Any type
    [[nodiscard]] bool is_any() const {
        return std::holds_alternative<AnyType>(data_);
    }

    /// Check if this is a resolved Class type
    [[nodiscard]] bool is_class() const {
        return std::holds_alternative<ClassType>(data_);
    }

    /// Check if this is a Name type (needs resolution)
    [[nodiscard]] bool is_name() const {
        return std::holds_alternative<NameType>(data_);
    }
};

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_PROPERTY_H
