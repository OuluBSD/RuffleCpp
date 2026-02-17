// C++ translation of core/src/avm2/object/bytearray_object.rs
//! ByteArray object implementation

#ifndef RUFFLE_CORE_AVM2_OBJECT_BYTEARRAY_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_BYTEARRAY_OBJECT_H

#include <memory>
#include <vector>
#include <optional>
#include <format>
#include <cell>
#include <cstdint>

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
    class ScriptObjectData;
    template<typename GC>
    class TObject;
    template<typename GC>
    class ClassObject;
    template<typename GC>
    class Object;
    template<typename GC>
    class ArrayObject;
    class UpdateContext;
    class Character;

    namespace gc_arena {
        template<typename T>
        class Gc;
        template<typename T>
        class GcWeak;
    }

    namespace avm2 {
        namespace bytearray {
            class ByteArrayStorage;
        }
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {

using gc_arena::Gc;
using gc_arena::GcWeak;
using bytearray::ByteArrayStorage;

/// Weak reference to ByteArrayObject data
template<typename GC>
class ByteArrayObjectWeak;

/// Data structure for ByteArrayObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class ByteArrayObjectData {
public:
    ByteArrayObjectData() = default;

    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    /// ByteArray storage
    mutable std::cell::RefCell<ByteArrayStorage> storage;

    // Friend classes for accessing private members
    friend class ByteArrayObject<GC>;
    friend class ByteArrayObjectWeak<GC>;
};

/// A class instance allocator that allocates ByteArray objects.
///
/// This allocator handles both regular ByteArray instances and subclasses
/// that may be associated with binary data characters.
///
/// @param class_object The class object for ByteArray
/// @param activation The current activation
/// @return New ByteArrayObject
template<typename GC>
Object<GC> byte_array_allocator(
    ClassObject<GC> class_object,
    Activation<GC>* activation
);

/// ByteArrayObject - Object representation for ByteArray
template<typename GC>
class ByteArrayObject {
public:
    using DataType = ByteArrayObjectData<GC>;
    using WeakType = ByteArrayObjectWeak<GC>;

    ByteArrayObject() = default;
    explicit ByteArrayObject(Gc<GC, DataType> data) : data_(data) {}

    /// Create a ByteArrayObject from existing storage
    /// @param context The update context
    /// @param bytes The ByteArrayStorage to use
    /// @return New ByteArrayObject
    static ByteArrayObject from_storage(
        UpdateContext<GC>* context,
        ByteArrayStorage bytes
    );

    /// Get the base script object data (for TObject trait)
    Gc<GC, ScriptObjectData<GC>> gc_base() const {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base;
    }

    /// Get immutable reference to storage
    /// @return Ref to ByteArrayStorage
    std::cell::Ref<ByteArrayStorage> storage() const {
        return data_->storage.borrow();
    }

    /// Get mutable reference to storage
    /// @return RefMut to ByteArrayStorage
    std::cell::RefMut<ByteArrayStorage> storage_mut() const {
        return data_->storage.borrow_mut();
    }

    /// Set an element at the given index
    /// @param activation The current activation
    /// @param index The index to set
    /// @param value The value to set (coerced to u8)
    /// @return Success or error
    Result<void, Error<GC>> set_element(
        Activation<GC>* activation,
        size_t index,
        Value<GC> value
    );

    /// Get a property via array index
    /// @param index The array index
    /// @return The value at the index, or Undefined if out of bounds
    std::optional<Value<GC>> get_index_property(size_t index) const;

    /// Set a property via array index
    /// @param activation The current activation
    /// @param index The array index
    /// @param value The value to set
    /// @return Success or error
    std::optional<Result<void, Error<GC>>> set_index_property(
        Activation<GC>* activation,
        size_t index,
        Value<GC> value
    );

    /// Get a property, handling array index access
    /// @param name The property name
    /// @param activation The current activation
    /// @return The property value or error
    Result<Value<GC>, Error<GC>> get_property_local(
        const Multiname<GC>& name,
        Activation<GC>* activation
    );

    /// Set a property, handling array index access
    /// @param name The property name
    /// @param value The value to set
    /// @param activation The current activation
    /// @return Success or error
    Result<void, Error<GC>> set_property_local(
        const Multiname<GC>& name,
        Value<GC> value,
        Activation<GC>* activation
    );

    /// Initialize a property, handling array index access
    /// @param name The property name
    /// @param value The value to set
    /// @param activation The current activation
    /// @return Success or error
    Result<void, Error<GC>> init_property_local(
        const Multiname<GC>& name,
        Value<GC> value,
        Activation<GC>* activation
    );

    /// Check if object has own property, including array indices
    /// @param name The property name
    /// @return true if the property exists
    bool has_own_property(const Multiname<GC>& name) const;

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to ByteArrayObject
template<typename GC>
class ByteArrayObjectWeak {
public:
    ByteArrayObjectWeak() = default;
    explicit ByteArrayObjectWeak(GcWeak<GC, ByteArrayObjectData<GC>> data)
        : data_(data) {}

private:
    GcWeak<GC, ByteArrayObjectData<GC>> data_;
};

// Template method implementations

template<typename GC>
Object<GC> byte_array_allocator(
    ClassObject<GC> class_object,
    Activation<GC>* activation
) {
    auto storage = std::optional<ByteArrayStorage>{};

    // Check if this class is associated with a binary data character
    if (auto class_symbol = activation->context->library->avm2_class_registry()->class_symbol(
            class_object.inner_class_definition())) {
        auto [movie, id] = class_symbol.value();
        if (auto lib = activation->context->library.library_for_movie(movie)) {
            if (auto character = lib->character_by_id(id)) {
                if (auto* binary_data = std::get_if<Character::BinaryData>(&character.value())) {
                    storage = ByteArrayStorage::from_vec(
                        activation->context,
                        binary_data->to_vec()
                    );
                }
            }
        }
    }

    auto storage_value = storage.value_or_else([&]() {
        return ByteArrayStorage::new(activation->context);
    });

    auto base = ScriptObjectData<GC>::new_instance(class_object);

    auto data = Gc<GC, ByteArrayObjectData<GC>>::create(
        activation->gc(),
        ByteArrayObjectData<GC>{
            base,
            std::cell::RefCell<ByteArrayStorage>::new(storage_value)
        }
    );

    return Object<GC>::from_bytearray_object(ByteArrayObject<GC>(data));
}

template<typename GC>
ByteArrayObject<GC> ByteArrayObject<GC>::from_storage(
    UpdateContext<GC>* context,
    ByteArrayStorage bytes
) {
    auto class_object = context->avm2()->classes()->bytearray;
    auto base = ScriptObjectData<GC>::new_instance(class_object);

    auto data = Gc<GC, DataType>::create(
        context->gc(),
        DataType{
            base,
            std::cell::RefCell<ByteArrayStorage>::new(bytes)
        }
    );

    return ByteArrayObject<GC>(data);
}

template<typename GC>
Result<void, Error<GC>> ByteArrayObject<GC>::set_element(
    Activation<GC>* activation,
    size_t index,
    Value<GC> value
) {
    auto storage_ref = data_->storage.borrow_mut();
    storage_ref.set(index, value.coerce_to_u32(activation)?);
    return {};
}

template<typename GC>
std::optional<Value<GC>> ByteArrayObject<GC>::get_index_property(size_t index) const {
    // ByteArrays never forward to base even for out-of-bounds access.
    auto storage_ref = data_->storage.borrow();
    if (auto val = storage_ref.get(index)) {
        return Value<GC>::from_integer(static_cast<int32_t>(val.value()));
    }
    return Value<GC>::undefined();
}

template<typename GC>
std::optional<Result<void, Error<GC>>> ByteArrayObject<GC>::set_index_property(
    Activation<GC>* activation,
    size_t index,
    Value<GC> value
) {
    // ByteArrays never forward to base even for out-of-bounds access.
    return std::make_optional(set_element(activation, index, value));
}

template<typename GC>
Result<Value<GC>, Error<GC>> ByteArrayObject<GC>::get_property_local(
    const Multiname<GC>& name,
    Activation<GC>* activation
) {
    if (name.valid_dynamic_name()) {
        if (auto local_name = name.local_name()) {
            if (auto index = ArrayObject<GC>::as_array_index(local_name.value())) {
                return get_index_property(index.value()).value_or(Value<GC>::undefined());
            }
        }
    }

    return data_->base.get_property_local(name, activation);
}

template<typename GC>
Result<void, Error<GC>> ByteArrayObject<GC>::set_property_local(
    const Multiname<GC>& name,
    Value<GC> value,
    Activation<GC>* activation
) {
    if (name.valid_dynamic_name()) {
        if (auto local_name = name.local_name()) {
            if (auto index = ArrayObject<GC>::as_array_index(local_name.value())) {
                return set_element(activation, index.value(), value);
            }
        }
    }

    return data_->base.set_property_local(name, value, activation);
}

template<typename GC>
Result<void, Error<GC>> ByteArrayObject<GC>::init_property_local(
    const Multiname<GC>& name,
    Value<GC> value,
    Activation<GC>* activation
) {
    if (name.valid_dynamic_name()) {
        if (auto local_name = name.local_name()) {
            if (auto index = ArrayObject<GC>::as_array_index(local_name.value())) {
                return set_element(activation, index.value(), value);
            }
        }
    }

    return data_->base.init_property_local(name, value, activation);
}

template<typename GC>
bool ByteArrayObject<GC>::has_own_property(const Multiname<GC>& name) const {
    if (name.valid_dynamic_name()) {
        if (auto local_name = name.local_name()) {
            if (auto index = ArrayObject<GC>::as_array_index(local_name.value())) {
                auto storage_ref = data_->storage.borrow();
                return storage_ref.get(index.value()).has_value();
            }
        }
    }

    return data_->base.has_own_property(name);
}

} // namespace avm2
} // namespace core
} // namespace ruffle

// Debug formatter specialization
template<typename GC>
struct std::formatter<ruffle::core::avm2::ByteArrayObject<GC>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const ruffle::core::avm2::ByteArrayObject<GC>& obj, FormatContext& ctx) {
        return std::format_to(ctx.out(), "ByteArrayObject(ptr={:p})",
            static_cast<const void*>(obj.data_.get()));
    }
};

#endif // RUFFLE_CORE_AVM2_OBJECT_BYTEARRAY_OBJECT_H
