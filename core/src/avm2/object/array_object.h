// C++ translation of core/src/avm2/object/array_object.rs
//! Array-structured objects

#ifndef RUFFLE_CORE_AVM2_OBJECT_ARRAY_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_ARRAY_OBJECT_H

#include <cstdint>
#include <vector>
#include <optional>
#include <memory>
#include <string>
#include <format>
#include <cell>
#include <mutex>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC>
    class AvmString;
    template<typename GC>
    class WStr;
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
    class UpdateContext;
    template<typename GC>
    class Mutation;

    namespace avm2 {
        template<typename GC>
        class ArrayStorage;
    }

    namespace gc_arena {
        template<typename T>
        class Gc;
        template<typename T>
        class GcWeak;
        class Mutation;
        template<typename T>
        class RefLock;
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {

using gc_arena::Gc;
using gc_arena::GcWeak;
using gc_arena::Mutation;
using gc_arena::RefLock;

/// Weak reference to ArrayObject data
template<typename GC>
class ArrayObjectWeak;

/// Data structure for ArrayObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class ArrayObjectData {
public:
    ArrayObjectData() = default;

    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    // Array-structured properties
    RefLock<ArrayStorage<GC>> array;

    // Friend classes for accessing private members
    friend class ArrayObject<GC>;
    friend class ArrayObjectWeak<GC>;
};

/// A class instance allocator that allocates array objects.
/// @param class_object The class object for Array
/// @param activation The current activation
/// @return New ArrayObject
template<typename GC>
Object<GC> array_allocator(
    ClassObject<GC> class_object,
    Activation<GC>* activation
);

/// An Object which stores numerical properties in an array.
template<typename GC>
class ArrayObject {
public:
    using DataType = ArrayObjectData<GC>;
    using WeakType = ArrayObjectWeak<GC>;

    ArrayObject() = default;
    explicit ArrayObject(Gc<GC, DataType> data) : data_(data) {}

    /// Get the base script object data (for TObject trait)
    Gc<GC, ScriptObjectData<GC>> gc_base() const {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base;
    }

    /// Construct an empty array.
    static ArrayObject<GC> empty(UpdateContext<GC>* context);

    /// Build an array object from storage.
    /// This will produce an instance of the system Array class.
    static ArrayObject<GC> from_storage(
        UpdateContext<GC>* context,
        ArrayStorage<GC> array
    );

    /// Create array for prototype
    static Object<GC> for_prototype(
        UpdateContext<GC>* context,
        ClassObject<GC> array_class
    );

    /// Try to parse a string as an array index
    static std::optional<size_t> as_array_index(const WStr<GC>& local_name);

    /// Set an element at the given index
    void set_element(Mutation<GC>* mc, size_t index, Value<GC> value);

    /// Get the array storage (immutable reference)
    const ArrayStorage<GC>& storage() const {
        return data_->array.get();
    }

    /// Get the array storage (mutable reference)
    ArrayStorage<GC>& storage_mut(Mutation<GC>* mc);

    // TObject trait implementations
    Value<GC> get_property_local(
        const Multiname<GC>& name,
        Activation<GC>* activation
    );

    Value<GC> get_index_property(size_t index) const;

    void set_index_property(
        Activation<GC>* activation,
        size_t index,
        Value<GC> value
    );

    void set_property_local(
        const Multiname<GC>& name,
        Value<GC> value,
        Activation<GC>* activation
    );

    void init_property_local(
        const Multiname<GC>& name,
        Value<GC> value,
        Activation<GC>* activation
    );

    bool delete_property_local(
        Activation<GC>* activation,
        const Multiname<GC>& name
    );

    bool has_own_property(const Multiname<GC>& name) const;

    uint32_t get_next_enumerant(
        uint32_t last_index,
        Activation<GC>* activation
    );

    Value<GC> get_enumerant_name(
        uint32_t index,
        Activation<GC>* activation
    );

    bool property_is_enumerable(AvmString<GC> name) const;

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to ArrayObject
template<typename GC>
class ArrayObjectWeak {
public:
    ArrayObjectWeak() = default;
    explicit ArrayObjectWeak(GcWeak<GC, ArrayObjectData<GC>> data)
        : data_(data) {}

private:
    GcWeak<GC, ArrayObjectData<GC>> data_;
};

// Template method implementations

template<typename GC>
Object<GC> array_allocator(
    ClassObject<GC> class_object,
    Activation<GC>* activation
) {
    auto base = ScriptObjectData<GC>::new_instance(class_object);

    auto data = Gc<GC, ArrayObjectData<GC>>::create(
        activation->gc(),
        ArrayObjectData<GC>{
            base,
            RefLock<ArrayStorage<GC>>(ArrayStorage<GC>::new(0))
        }
    );

    return Object<GC>::from_array_object(ArrayObject<GC>(data));
}

template<typename GC>
ArrayObject<GC> ArrayObject<GC>::empty(UpdateContext<GC>* context) {
    return from_storage(context, ArrayStorage<GC>::new(0));
}

template<typename GC>
ArrayObject<GC> ArrayObject<GC>::from_storage(
    UpdateContext<GC>* context,
    ArrayStorage<GC> array
) {
    auto class_object = context->avm2.classes().array;
    auto base = ScriptObjectData<GC>::new_instance(class_object);

    return ArrayObject<GC>(Gc<GC, ArrayObjectData<GC>>::create(
        context->gc(),
        ArrayObjectData<GC>{
            base,
            RefLock<ArrayStorage<GC>>(array)
        }
    ));
}

template<typename GC>
Object<GC> ArrayObject<GC>::for_prototype(
    UpdateContext<GC>* context,
    ClassObject<GC> array_class
) {
    auto object_class = context->avm2.classes().object;
    auto base = ScriptObjectData<GC>::custom_new(
        array_class.inner_class_definition(),
        object_class.prototype(),
        array_class.instance_vtable()
    );

    return ArrayObject<GC>(Gc<GC, ArrayObjectData<GC>>::create(
        context->gc(),
        ArrayObjectData<GC>{
            base,
            RefLock<ArrayStorage<GC>>(ArrayStorage<GC>::new(0))
        }
    )).into();
}

template<typename GC>
std::optional<size_t> ArrayObject<GC>::as_array_index(const WStr<GC>& local_name) {
    // TODO: this should use a custom implementation instead of parse()
    // FP is much stricter here, only allowing pure natural numbers without sign or leading zeros
    try {
        uint32_t val = std::stoul(local_name.to_utf8_lossy());
        if (val != UINT32_MAX) {
            return static_cast<size_t>(val);
        }
    } catch (...) {
        // Not a valid number
    }
    return std::nullopt;
}

template<typename GC>
void ArrayObject<GC>::set_element(Mutation<GC>* mc, size_t index, Value<GC> value) {
    data_->array.get().set(index, value);
}

template<typename GC>
ArrayStorage<GC>& ArrayObject<GC>::storage_mut(Mutation<GC>* mc) {
    return data_->array.get();
}

template<typename GC>
Value<GC> ArrayObject<GC>::get_property_local(
    const Multiname<GC>& name,
    Activation<GC>* activation
) {
    if (name.valid_dynamic_name()) {
        if (auto local_name = name.local_name()) {
            if (auto index = as_array_index(*local_name)) {
                if (auto result = get_index_property(*index)) {
                    return result;
                }
            }
        }
    }

    return gc_base().get_property_local(name, activation);
}

template<typename GC>
Value<GC> ArrayObject<GC>::get_index_property(size_t index) const {
    return data_->array.get().get(index);
}

template<typename GC>
void ArrayObject<GC>::set_index_property(
    Activation<GC>* activation,
    size_t index,
    Value<GC> value
) {
    set_element(activation->gc(), index, value);
}

template<typename GC>
void ArrayObject<GC>::set_property_local(
    const Multiname<GC>& name,
    Value<GC> value,
    Activation<GC>* activation
) {
    auto mc = activation->gc();

    if (name.valid_dynamic_name()) {
        if (auto local_name = name.local_name()) {
            if (auto index = as_array_index(*local_name)) {
                set_element(mc, *index, value);
                return;
            }
        }
    }

    gc_base().set_property_local(name, value, activation);
}

template<typename GC>
void ArrayObject<GC>::init_property_local(
    const Multiname<GC>& name,
    Value<GC> value,
    Activation<GC>* activation
) {
    auto mc = activation->gc();

    if (name.valid_dynamic_name()) {
        if (auto local_name = name.local_name()) {
            if (auto index = as_array_index(*local_name)) {
                set_element(mc, *index, value);
                return;
            }
        }
    }

    gc_base().init_property_local(name, value, activation);
}

template<typename GC>
bool ArrayObject<GC>::delete_property_local(
    Activation<GC>* activation,
    const Multiname<GC>& name
) {
    auto mc = activation->gc();

    if (name.valid_dynamic_name()) {
        if (auto local_name = name.local_name()) {
            if (auto index = as_array_index(*local_name)) {
                data_->array.get().delete_element(*index);
                return true;
            }
        }
    }

    return gc_base().delete_property_local(mc, name);
}

template<typename GC>
bool ArrayObject<GC>::has_own_property(const Multiname<GC>& name) const {
    if (name.valid_dynamic_name()) {
        if (auto local_name = name.local_name()) {
            if (auto index = as_array_index(*local_name)) {
                return data_->array.get().get(*index).has_value();
            }
        }
    }

    return gc_base().has_own_property(name);
}

template<typename GC>
uint32_t ArrayObject<GC>::get_next_enumerant(
    uint32_t last_index,
    Activation<GC>* activation
) {
    const auto& array = data_->array.get();
    const uint32_t array_length = static_cast<uint32_t>(array.length());

    // Array enumeration skips over holes.
    if (auto index = array.get_next_enumerant(static_cast<size_t>(last_index))) {
        return static_cast<uint32_t>(*index);
    }

    uint32_t adjusted_last_index = std::max(last_index, array_length);

    // After enumerating all of the 'normal' array entries,
    // we enumerate all of the local properties stored on the ScriptObject.
    uint32_t index = gc_base().get_next_enumerant(adjusted_last_index - array_length);
    if (index != 0) {
        return index + array_length;
    }

    return 0;
}

template<typename GC>
Value<GC> ArrayObject<GC>::get_enumerant_name(
    uint32_t index,
    Activation<GC>* activation
) {
    const uint32_t arr_len = static_cast<uint32_t>(data_->array.get().length());
    if (arr_len >= index) {
        if (index > 0) {
            return Value<GC>::number(static_cast<double>(index - 1));
        }
        return Value<GC>::null();
    } else {
        return gc_base().get_enumerant_name(index - arr_len).value_or(Value<GC>::null());
    }
}

template<typename GC>
bool ArrayObject<GC>::property_is_enumerable(AvmString<GC> name) const {
    auto index = as_array_index(name);
    if (index.has_value()) {
        return index.value() < data_->array.get().length();
    }
    return gc_base().property_is_enumerable(name);
}

} // namespace avm2
} // namespace core
} // namespace ruffle

// Debug formatter specialization
template<typename GC>
struct std::formatter<ruffle::core::avm2::ArrayObject<GC>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const ruffle::core::avm2::ArrayObject<GC>& obj, FormatContext& ctx) {
        return std::format_to(ctx.out(), "ArrayObject(ptr={:p})",
            static_cast<const void*>(obj.data_.get()));
    }
};

#endif // RUFFLE_CORE_AVM2_OBJECT_ARRAY_OBJECT_H
