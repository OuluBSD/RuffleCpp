// C++ translation of core/src/avm2/object/namespace_object.rs
//! Boxed namespaces object implementation for AVM2

#ifndef RUFFLE_CORE_AVM2_OBJECT_NAMESPACE_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_NAMESPACE_OBJECT_H

#include <memory>
#include <cstdint>
#include <optional>
#include <format>

// Forward declarations
namespace ruffle {
namespace core {
    class WString;
    template<typename GC> class UpdateContext;

    namespace avm2 {
        template<typename T> class Activation;
        template<typename T> class ScriptObjectData;
        template<typename T> class TObject;
        template<typename T> class Gc;
        template<typename T> class GcWeak;
        template<typename T> class ClassObject;
        template<typename T> class Object;
        template<typename T> class Error;
        template<typename T> class Value;
        template<typename T> class AvmString;
        template<typename T> class Namespace;
        template<typename T> class Mutation;
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Weak reference to NamespaceObject data
template<typename GC>
class NamespaceObjectWeak;

/// Data structure for NamespaceObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class NamespaceObjectData {
public:
    NamespaceObjectData() = default;

    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    /// The namespace name this object is associated with
    Namespace<GC> namespace_;

    /// The prefix that this namespace has been given
    std::optional<AvmString<GC>> prefix;

    // Friend classes for accessing private members
    friend class NamespaceObject<GC>;
    friend class NamespaceObjectWeak<GC>;
};

/// NamespaceObject - An Object which represents a boxed namespace name
///
/// Represents a Namespace instance in ActionScript 3
template<typename GC>
class NamespaceObject {
public:
    using DataType = NamespaceObjectData<GC>;
    using WeakType = NamespaceObjectWeak<GC>;

    NamespaceObject() = default;
    explicit NamespaceObject(Gc<GC, DataType> data) : data_(data) {}

    /// Create a NamespaceObject from namespace and prefix
    ///
    /// @param activation The current activation context
    /// @param namespace The namespace to box
    /// @param prefix Optional prefix string
    /// @return New NamespaceObject
    static NamespaceObject from_ns_and_prefix(
        Activation<GC>* activation,
        Namespace<GC> namespace_,
        std::optional<AvmString<GC>> prefix
    );

    /// Box a namespace into an object
    ///
    /// @param activation The current activation context
    /// @param namespace The namespace to box
    /// @return New NamespaceObject
    static NamespaceObject from_namespace(
        Activation<GC>* activation,
        Namespace<GC> namespace_
    );

    /// Get the namespace
    /// @return The namespace
    Namespace<GC> namespace_() const {
        return data_->namespace_;
    }

    /// Get the prefix
    /// @return The prefix if present, nullopt otherwise
    std::optional<AvmString<GC>> prefix() const {
        return data_->prefix;
    }

    /// Get the base script object data (for TObject trait)
    Gc<GC, ScriptObjectData<GC>> gc_base() const {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base;
    }

    /// Check if a property is enumerable
    ///
    /// @param name The property name
    /// @return true if the property is enumerable (prefix or uri)
    bool property_is_enumerable(AvmString<GC> name) const;

    /// Get the next enumerant index
    ///
    /// @param last_index The last enumerated index
    /// @param activation The current activation context
    /// @return The next index, or 0 if no more enumerants
    uint32_t get_next_enumerant(uint32_t last_index, Activation<GC>* activation) const;

    /// Get the enumerant value at the given index
    ///
    /// @param index The enumerant index (1=uri, 2=prefix)
    /// @param activation The current activation context
    /// @return The value at the index
    Value<GC> get_enumerant_value(uint32_t index, Activation<GC>* activation) const;

    /// Get the enumerant name at the given index
    ///
    /// @param index The enumerant index (1=uri, 2=prefix)
    /// @param activation The current activation context
    /// @return The name at the index
    Value<GC> get_enumerant_name(uint32_t index, Activation<GC>* activation) const;

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to NamespaceObject
template<typename GC>
class NamespaceObjectWeak {
public:
    NamespaceObjectWeak() = default;
    explicit NamespaceObjectWeak(GcWeak<GC, NamespaceObjectData<GC>> data)
        : data_(data) {}

private:
    GcWeak<GC, NamespaceObjectData<GC>> data_;
};

// Template method implementations

template<typename GC>
NamespaceObject<GC> NamespaceObject<GC>::from_ns_and_prefix(
    Activation<GC>* activation,
    Namespace<GC> namespace_,
    std::optional<AvmString<GC>> prefix
) {
    auto class_obj = activation->avm2()->classes()->namespace;
    auto base = ScriptObjectData<GC>::create(class_obj);
    auto data = Gc<GC, DataType>::create(
        activation->gc(),
        DataType{base, namespace_, prefix}
    );
    return NamespaceObject<GC>(data);
}

template<typename GC>
NamespaceObject<GC> NamespaceObject<GC>::from_namespace(
    Activation<GC>* activation,
    Namespace<GC> namespace_
) {
    auto class_obj = activation->avm2()->classes()->namespace;
    auto base = ScriptObjectData<GC>::create(class_obj);

    std::optional<AvmString<GC>> prefix;
    if (namespace_.as_uri(activation->strings()).empty()) {
        prefix = std::make_optional(AvmString<GC>::empty(activation->gc()));
    }

    auto data = Gc<GC, DataType>::create(
        activation->gc(),
        DataType{base, namespace_, prefix}
    );
    return NamespaceObject<GC>(data);
}

template<typename GC>
bool NamespaceObject<GC>::property_is_enumerable(AvmString<GC> name) const {
    return name == "prefix" || name == "uri";
}

template<typename GC>
uint32_t NamespaceObject<GC>::get_next_enumerant(uint32_t last_index, Activation<GC>* activation) const {
    return (last_index < 2) ? (last_index + 1) : 0;
}

template<typename GC>
Value<GC> NamespaceObject<GC>::get_enumerant_value(uint32_t index, Activation<GC>* activation) const {
    switch (index) {
        case 1:
            return Value<GC>::from_string(namespace_().as_uri(activation->strings()));
        case 2:
            return prefix().has_value() ? Value<GC>::from_string(prefix.value()) : Value<GC>::undefined();
        default:
            return Value<GC>::undefined();
    }
}

template<typename GC>
Value<GC> NamespaceObject<GC>::get_enumerant_name(uint32_t index, Activation<GC>* activation) const {
    switch (index) {
        case 1:
            return Value<GC>::from_string(AvmString<GC>::from_utf8(activation->gc(), "uri"));
        case 2:
            return Value<GC>::from_string(AvmString<GC>::from_utf8(activation->gc(), "prefix"));
        default:
            return Value<GC>::null();
    }
}

} // namespace avm2
} // namespace core
} // namespace ruffle

// Debug formatter specialization
template<typename GC>
struct std::formatter<ruffle::core::avm2::NamespaceObject<GC>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const ruffle::core::avm2::NamespaceObject<GC>& obj, FormatContext& ctx) {
        return std::format_to(ctx.out(), "NamespaceObject(ptr={})",
            static_cast<const void*>(obj.data_.get()));
    }
};

#endif // RUFFLE_CORE_AVM2_OBJECT_NAMESPACE_OBJECT_H
