// C++ translation of core/src/avm2/object/proxy_object.rs
//! Object representation for `Proxy`.

#ifndef RUFFLE_CORE_AVM2_OBJECT_PROXY_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_PROXY_OBJECT_H

#include <memory>
#include <vector>
#include <optional>
#include <format>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC>
    class AvmString;
    template<typename GC>
    class Activation;
    template<typename GC>
    class Value;
    template<typename GC>
    class Error;
    template<typename GC>
    class Multiname;
    template<typename GC>
    class ScriptObjectData;
    template<typename GC>
    class TObject;
    template<typename GC>
    class ClassObject;
    template<typename GC>
    class Object;
    template<typename GC>
    class QNameObject;

    namespace function {
        template<typename GC>
        struct FunctionArgs;
    }

    namespace gc_arena {
        template<typename T>
        class Gc;
        template<typename T>
        class GcWeak;
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {

using gc_arena::Gc;
using gc_arena::GcWeak;
using function::FunctionArgs;

/// Weak reference to ProxyObject data
template<typename GC>
class ProxyObjectWeak;

/// Data structure for ProxyObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class ProxyObjectData {
public:
    ProxyObjectData() = default;

    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    // Friend classes for accessing private members
    friend class ProxyObject<GC>;
    friend class ProxyObjectWeak<GC>;
};

/// A class instance allocator that allocates Proxy objects.
/// @param class_object The class object for Proxy
/// @param activation The current activation
/// @return New ProxyObject
template<typename GC>
Object<GC> proxy_allocator(
    ClassObject<GC> class_object,
    Activation<GC>* activation
);

/// ProxyObject - Object representation for Proxy
template<typename GC>
class ProxyObject {
public:
    using DataType = ProxyObjectData<GC>;
    using WeakType = ProxyObjectWeak<GC>;

    ProxyObject() = default;
    explicit ProxyObject(Gc<GC, DataType> data) : data_(data) {}

    /// Get the base script object data (for TObject trait)
    Gc<GC, ScriptObjectData<GC>> gc_base() const {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base;
    }

    /// Get a property via Proxy's getProperty trap
    /// @param multiname The property name
    /// @param activation The current activation
    /// @return The property value or error
    Result<Value<GC>, Error<GC>> get_property_local(
        const Multiname<GC>& multiname,
        Activation<GC>* activation
    );

    /// Set a property via Proxy's setProperty trap
    /// @param multiname The property name
    /// @param value The value to set
    /// @param activation The current activation
    /// @return Success or error
    Result<void, Error<GC>> set_property_local(
        const Multiname<GC>& multiname,
        Value<GC> value,
        Activation<GC>* activation
    );

    /// Call a property via Proxy's callProperty trap
    /// @param multiname The property name
    /// @param arguments The function arguments
    /// @param activation The current activation
    /// @return The call result or error
    Result<Value<GC>, Error<GC>> call_property_local(
        const Multiname<GC>& multiname,
        FunctionArgs<GC> arguments,
        Activation<GC>* activation
    );

    /// Delete a property via Proxy's deleteProperty trap
    /// @param activation The current activation
    /// @param multiname The property name
    /// @return true if deleted, false otherwise
    Result<bool, Error<GC>> delete_property_local(
        Activation<GC>* activation,
        const Multiname<GC>& multiname
    );

    /// Check if property exists via Proxy's hasProperty trap (using 'in' operator)
    /// @param activation The current activation
    /// @param name The property name
    /// @return true if property exists
    Result<bool, Error<GC>> has_property_via_in(
        Activation<GC>* activation,
        const Multiname<GC>& name
    );

    /// Check if own property exists via Proxy's hasProperty trap
    /// @param name The property name as string
    /// @param activation The current activation
    /// @return true if property exists
    Result<bool, Error<GC>> has_own_property_string(
        AvmString<GC> name,
        Activation<GC>* activation
    );

    /// Get next enumerant via Proxy's nextNameIndex trap
    /// @param last_index The last index returned
    /// @param activation The current activation
    /// @return The next index, or 0 if done
    Result<uint32_t, Error<GC>> get_next_enumerant(
        uint32_t last_index,
        Activation<GC>* activation
    );

    /// Get enumerant name via Proxy's nextName trap
    /// @param index The enumerant index
    /// @param activation The current activation
    /// @return The enumerant name
    Result<Value<GC>, Error<GC>> get_enumerant_name(
        uint32_t index,
        Activation<GC>* activation
    );

    /// Get enumerant value via Proxy's nextValue trap
    /// @param index The enumerant index
    /// @param activation The current activation
    /// @return The enumerant value
    Result<Value<GC>, Error<GC>> get_enumerant_value(
        uint32_t index,
        Activation<GC>* activation
    );

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to ProxyObject
template<typename GC>
class ProxyObjectWeak {
public:
    ProxyObjectWeak() = default;
    explicit ProxyObjectWeak(GcWeak<GC, ProxyObjectData<GC>> data)
        : data_(data) {}

private:
    GcWeak<GC, ProxyObjectData<GC>> data_;
};

// Template method implementations

template<typename GC>
Object<GC> proxy_allocator(
    ClassObject<GC> class_object,
    Activation<GC>* activation
) {
    auto base = ScriptObjectData<GC>::new_instance(class_object);

    auto data = Gc<GC, ProxyObjectData<GC>>::create(
        activation->gc(),
        ProxyObjectData<GC>{
            base
        }
    );

    return Object<GC>::from_proxy_object(ProxyObject<GC>(data));
}

template<typename GC>
Result<Value<GC>, Error<GC>> ProxyObject<GC>::get_property_local(
    const Multiname<GC>& multiname,
    Activation<GC>* activation
) {
    auto self_val = Value<GC>::from_object(Object<GC>::from_proxy_object(*this));

    auto qname = QNameObject<GC>::from_name(activation, multiname);
    return self_val.call_method(
        globals::methods::flash_utils_proxy::GET_PROPERTY,
        {Value<GC>::from_object(Object<GC>::from_qname_object(qname))},
        activation
    );
}

template<typename GC>
Result<void, Error<GC>> ProxyObject<GC>::set_property_local(
    const Multiname<GC>& multiname,
    Value<GC> value,
    Activation<GC>* activation
) {
    auto self_val = Value<GC>::from_object(Object<GC>::from_proxy_object(*this));

    auto qname = QNameObject<GC>::from_name(activation, multiname);
    self_val.call_method(
        globals::methods::flash_utils_proxy::SET_PROPERTY,
        {
            Value<GC>::from_object(Object<GC>::from_qname_object(qname)),
            value
        },
        activation
    )?;

    return {};
}

template<typename GC>
Result<Value<GC>, Error<GC>> ProxyObject<GC>::call_property_local(
    const Multiname<GC>& multiname,
    FunctionArgs<GC> arguments,
    Activation<GC>* activation
) {
    auto self_val = Value<GC>::from_object(Object<GC>::from_proxy_object(*this));

    auto qname = QNameObject<GC>::from_name(activation, multiname);

    std::vector<Value<GC>> args;
    args.reserve(1 + arguments.len());
    args.push_back(Value<GC>::from_object(Object<GC>::from_qname_object(qname)));
    for (const auto& arg : arguments.to_slice()) {
        args.push_back(arg);
    }

    return self_val.call_method(
        globals::methods::flash_utils_proxy::CALL_PROPERTY,
        args,
        activation
    );
}

template<typename GC>
Result<bool, Error<GC>> ProxyObject<GC>::delete_property_local(
    Activation<GC>* activation,
    const Multiname<GC>& multiname
) {
    auto self_val = Value<GC>::from_object(Object<GC>::from_proxy_object(*this));

    auto qname = QNameObject<GC>::from_name(activation, multiname);

    auto result = self_val.call_method(
        globals::methods::flash_utils_proxy::DELETE_PROPERTY,
        {Value<GC>::from_object(Object<GC>::from_qname_object(qname))},
        activation
    )?;

    return result.coerce_to_boolean();
}

template<typename GC>
Result<bool, Error<GC>> ProxyObject<GC>::has_property_via_in(
    Activation<GC>* activation,
    const Multiname<GC>& name
) {
    auto self_val = Value<GC>::from_object(Object<GC>::from_proxy_object(*this));

    auto local_name = name.local_name().value_or(AvmString<GC>::from_utf8(activation->gc(), "*"));

    auto result = self_val.call_method(
        globals::methods::flash_utils_proxy::HAS_PROPERTY,
        {Value<GC>::from_string(local_name)},
        activation
    )?;

    return result.coerce_to_boolean();
}

template<typename GC>
Result<bool, Error<GC>> ProxyObject<GC>::has_own_property_string(
    AvmString<GC> name,
    Activation<GC>* activation
) {
    auto self_val = Value<GC>::from_object(Object<GC>::from_proxy_object(*this));

    auto result = self_val.call_method(
        globals::methods::flash_utils_proxy::HAS_PROPERTY,
        {Value<GC>::from_string(name)},
        activation
    )?;

    return result.coerce_to_boolean();
}

template<typename GC>
Result<uint32_t, Error<GC>> ProxyObject<GC>::get_next_enumerant(
    uint32_t last_index,
    Activation<GC>* activation
) {
    auto self_val = Value<GC>::from_object(Object<GC>::from_proxy_object(*this));

    auto result = self_val.call_method(
        globals::methods::flash_utils_proxy::NEXT_NAME_INDEX,
        {Value<GC>::from_uint(last_index)},
        activation
    )?;

    return result.coerce_to_u32(activation);
}

template<typename GC>
Result<Value<GC>, Error<GC>> ProxyObject<GC>::get_enumerant_name(
    uint32_t index,
    Activation<GC>* activation
) {
    auto self_val = Value<GC>::from_object(Object<GC>::from_proxy_object(*this));

    return self_val.call_method(
        globals::methods::flash_utils_proxy::NEXT_NAME,
        {Value<GC>::from_uint(index)},
        activation
    );
}

template<typename GC>
Result<Value<GC>, Error<GC>> ProxyObject<GC>::get_enumerant_value(
    uint32_t index,
    Activation<GC>* activation
) {
    auto self_val = Value<GC>::from_object(Object<GC>::from_proxy_object(*this));

    return self_val.call_method(
        globals::methods::flash_utils_proxy::NEXT_VALUE,
        {Value<GC>::from_uint(index)},
        activation
    );
}

} // namespace avm2
} // namespace core
} // namespace ruffle

// Debug formatter specialization
template<typename GC>
struct std::formatter<ruffle::core::avm2::ProxyObject<GC>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const ruffle::core::avm2::ProxyObject<GC>& obj, FormatContext& ctx) {
        return std::format_to(ctx.out(), "ProxyObject(ptr={:p})",
            static_cast<const void*>(obj.data_.get()));
    }
};

#endif // RUFFLE_CORE_AVM2_OBJECT_PROXY_OBJECT_H
