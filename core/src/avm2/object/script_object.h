// C++ translation of core/src/avm2/object/script_object.rs
//! Default AVM2 object implementation

#ifndef RUFFLE_CORE_AVM2_OBJECT_SCRIPT_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_SCRIPT_OBJECT_H

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
    class Activation;
    template<typename GC>
    class Value;
    template<typename GC>
    class Error;
    template<typename GC>
    class Multiname;
    template<typename GC>
    class QName;
    template<typename GC>
    class Class;
    template<typename GC>
    class ClassObject;
    template<typename GC>
    class FunctionObject;
    template<typename GC>
    class Object;
    template<typename GC>
    class TObject;
    template<typename GC>
    class VTable;
    template<typename GC>
    class ArrayObject;
    template<typename GC>
    class ArrayStorage;
    class UpdateContext;

    namespace gc_arena {
        template<typename T>
        class Gc;
        template<typename T>
        class GcWeak;
        template<typename T>
        class Rootable;
        class Mutation;
        template<typename T>
        class DynamicRoot;
    }

    namespace avm2 {
        template<typename GC>
        class DynamicMap;
        template<typename GC>
        class DynamicKey;
        template<typename GC>
        class Lock;
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {

using gc_arena::Gc;
using gc_arena::GcWeak;
using gc_arena::Mutation;
using gc_arena::DynamicRoot;
using gc_arena::Rootable;

/// A class instance allocator that allocates ScriptObjects.
template<typename GC>
Object<GC> scriptobject_allocator(
    ClassObject<GC> class_object,
    Activation<GC>* activation
);

/// Default implementation of avm2::Object.
template<typename GC>
class ScriptObject {
private:
    Gc<GC, ScriptObjectData<GC>> data_;

public:
    explicit ScriptObject(Gc<GC, ScriptObjectData<GC>> data)
        : data_(data)
    {}

    Gc<GC, ScriptObjectData<GC>> gc_base() const {
        return data_;
    }

    /// Creates an instance of the Object class, exactly as if new Object()
    /// were called, but without going through any construction or call
    /// machinery (since it's unnecessary for the Object class).
    static Object<GC> new_object(UpdateContext<GC>* context);

    /// Construct an instance with a possibly-none class and proto chain.
    /// NOTE: this is a low-level function.
    /// This should *not* be used unless you really need
    /// to do something low-level, weird or lazily initialize the object.
    /// You shouldn't let scripts observe this weirdness.
    ///
    /// The proper way to create a normal empty ScriptObject (AS "Object") is to call
    /// ScriptObject::new_object(activation.context).
    ///
    /// Calling custom_object(mc, object_class, object_class.prototype()) is
    /// technically also equivalent, but not recommended outside VM initialization code
    static Object<GC> custom_object(
        Mutation<GC>* mc,
        Class<GC> class_,
        std::optional<Object<GC>> proto,
        VTable<GC> vtable
    );
};

/// Weak version of ScriptObject
template<typename GC>
class ScriptObjectWeak {
private:
    GcWeak<GC, ScriptObjectData<GC>> data_;

public:
    explicit ScriptObjectWeak(GcWeak<GC, ScriptObjectData<GC>> data)
        : data_(data)
    {}
};

/// Handle for ScriptObject that can be stashed across GC mutations
template<typename GC>
class ScriptObjectHandle {
private:
    DynamicRoot<Rootable<ScriptObjectData<GC>>> root_;

public:
    /// Stash a ScriptObject for later retrieval
    static ScriptObjectHandle stash(
        UpdateContext<GC>* context,
        ScriptObject<GC> obj
    ) {
        return ScriptObjectHandle(context->dynamic_root.stash(context->gc(), obj.data_));
    }

    /// Fetch a previously stashed ScriptObject
    ScriptObject<GC> fetch(UpdateContext<GC>* context) const {
        return ScriptObject<GC>(context->dynamic_root.fetch(&root_));
    }
};

/// Base data common to all TObject implementations.
///
/// Host implementations of TObject should embed ScriptObjectData and
/// forward any trait method implementations it does not overwrite to this
/// struct.
template<typename GC>
class ScriptObjectData {
private:
    /// Values stored on this object.
    DynamicMap<GC, DynamicKey<GC>, Value<GC>> values_;

    /// Slots stored on this object.
    std::vector<Lock<GC, Value<GC>>> slots_;

    /// Methods stored on this object.
    std::vector<std::optional<FunctionObject<GC>>> bound_methods_;

    /// Implicit prototype of this script object.
    std::optional<Object<GC>> proto_;

    /// The Class that this is an instance of.
    Class<GC> instance_class_;

    /// The table used for non-dynamic property lookups.
    VTable<GC> vtable_;

public:
    /// Create new object data of a given class.
    /// This is a low-level function used to implement things like object allocators.
    static ScriptObjectData new_instance(ClassObject<GC> instance_of) {
        return custom_new(
            instance_of.inner_class_definition(),
            Some(instance_of.prototype()),
            instance_of.instance_vtable()
        );
    }

    /// Create new custom object data of a given possibly-none class and prototype.
    /// This is a low-level function used to implement things like object allocators.
    /// This should *not* be used, unless you really need
    /// to do something weird or lazily initialize the object.
    /// You shouldn't let scripts observe this weirdness.
    static ScriptObjectData custom_new(
        Class<GC> instance_class,
        std::optional<Object<GC>> proto,
        VTable<GC> vtable
    ) {
        const auto& default_slots = vtable.default_slots();

        // We use iter and collect rather than setting elements of a Box<[]>
        // or pushing to a Vec for better performance
        std::vector<Lock<GC, Value<GC>>> slots;
        slots.reserve(default_slots.size());

        for (const auto& value : default_slots) {
            if (value.has_value()) {
                slots.emplace_back(Lock<GC, Value<GC>>::new(*value));
            } else {
                // FIXME this case throws a VerifyError during vtable
                // construction in Flash Player
                slots.emplace_back(Lock<GC, Value<GC>>::new(Value<GC>::undefined()));
            }
        }

        return ScriptObjectData(
            DynamicMap<GC, DynamicKey<GC>, Value<GC>>(),
            std::move(slots),
            std::vector<std::optional<FunctionObject<GC>>>(),
            proto,
            instance_class,
            vtable
        );
    }

    // Getters
    const DynamicMap<GC, DynamicKey<GC>, Value<GC>>& values() const { return values_; }
    DynamicMap<GC, DynamicKey<GC>, Value<GC>>& values_mut() { return values_; }
    const std::vector<Lock<GC, Value<GC>>>& slots() const { return slots_; }
    const std::vector<std::optional<FunctionObject<GC>>>& bound_methods() const { return bound_methods_; }
    std::optional<Object<GC>> proto() const { return proto_; }
    Class<GC> instance_class() const { return instance_class_; }
    VTable<GC> vtable() const { return vtable_; }

    // Setters
    void set_proto(Mutation<GC>* mc, Object<GC> proto) { proto_ = proto; }
    void set_vtable(Mutation<GC>* mc, VTable<GC> vtable) {
        // Make sure both vtables have the same number of slots
        // assert(vtable_.default_slots().size() == vtable.default_slots().size());
        vtable_ = vtable;
    }

    bool is_sealed() const {
        return instance_class_.is_sealed();
    }

    QName<GC> class_name() const {
        return instance_class_.name();
    }
};

/// Wrapper for ScriptObjectData providing additional methods
template<typename GC>
class ScriptObjectWrapper {
private:
    Gc<GC, ScriptObjectData<GC>> data_;

public:
    explicit ScriptObjectWrapper(Gc<GC, ScriptObjectData<GC>> data)
        : data_(data)
    {}

    /// Retrieve the values stored directly on this ScriptObjectData.
    const DynamicMap<GC, DynamicKey<GC>, Value<GC>>& values() const {
        return data_->values();
    }

    DynamicMap<GC, DynamicKey<GC>, Value<GC>>& values_mut(Mutation<GC>* mc) {
        return data_->values_mut();
    }

    /// Get a property locally
    Result<Value<GC>, Error<GC>> get_property_local(
        const Multiname<GC>* multiname,
        Activation<GC>* activation
    );

    /// Set a property locally
    Result<void, Error<GC>> set_property_local(
        const Multiname<GC>* multiname,
        Value<GC> value,
        Activation<GC>* activation
    );

    /// Initialize a property locally
    Result<void, Error<GC>> init_property_local(
        const Multiname<GC>* multiname,
        Value<GC> value,
        Activation<GC>* activation
    );

    /// Delete a property locally
    bool delete_property_local(Mutation<GC>* mc, const Multiname<GC>* multiname);

    /// Get a slot by index
    Value<GC> get_slot(uint32_t id) const {
        const auto& slots = data_->slots();
        if (id >= slots.size()) {
            // Slot index out of bounds - should panic in debug
            return Value<GC>::undefined();
        }
        return slots[id].get();
    }

    /// Set a slot by index
    void set_slot(uint32_t id, Value<GC> value, Mutation<GC>* mc) {
        auto& slots = const_cast<std::vector<Lock<GC, Value<GC>>>&>(data_->slots());
        if (id >= slots.size()) {
            // Slot index out of bounds - should panic in debug
            return;
        }
        slots[id].set(value);
    }

    /// Retrieve a bound method from the method table.
    std::optional<FunctionObject<GC>> get_bound_method(uint32_t id) const {
        const auto& bound_methods = data_->bound_methods();
        if (id >= bound_methods.size()) {
            return std::nullopt;
        }
        return bound_methods[id];
    }

    /// Check if object has own dynamic property
    bool has_own_dynamic_property(const Multiname<GC>* name) const {
        if (name->valid_dynamic_name()) {
            if (auto local_name = name->local_name()) {
                DynamicKey<GC> key = maybe_int_property(*local_name);
                return data_->values().contains_key(&key);
            }
        }
        return false;
    }

    /// Check if object has own property
    bool has_own_property(const Multiname<GC>* name) const {
        return data_->vtable().has_trait(name) || has_own_dynamic_property(name);
    }

    /// Get next enumerant
    uint32_t get_next_enumerant(uint32_t last_index) const {
        const auto& values = data_->values();
        auto next = values.next(last_index);
        return next.has_value() ? static_cast<uint32_t>(*next) : 0;
    }

    /// Get enumerant name
    std::optional<Value<GC>> get_enumerant_name(uint32_t index) const {
        const auto& values = data_->values();
        auto key = values.key_at(index);
        if (!key.has_value()) {
            return std::nullopt;
        }

        return std::visit([&](auto&& arg) -> Value<GC> {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, AvmString<GC>>) {
                return Value<GC>::string(*arg);
            } else if constexpr (std::is_same_v<T, Object<GC>>) {
                return Value<GC>::object(*arg);
            } else if constexpr (std::is_same_v<T, uint32_t>) {
                return Value<GC>::number(static_cast<double>(*arg));
            }
            return Value<GC>::undefined();
        }, *key);
    }

    /// Check if property is enumerable
    bool property_is_enumerable(AvmString<GC> name) const {
        DynamicKey<GC> key = maybe_int_property(name);
        auto prop = data_->values().get(&key);
        return prop.has_value() && prop->enumerable;
    }

    /// Set local property is enumerable
    void set_local_property_is_enumerable(
        Mutation<GC>* mc,
        AvmString<GC> name,
        bool is_enumerable
    ) {
        DynamicKey<GC> key = maybe_int_property(name);
        data_->values_mut().set_enumerable(&key, is_enumerable);
    }

    /// Install a method into the object.
    void install_bound_method(
        Mutation<GC>* mc,
        uint32_t disp_id,
        FunctionObject<GC> function
    ) {
        auto& bound_methods = const_cast<std::vector<std::optional<FunctionObject<GC>>>&>(
            data_->bound_methods()
        );

        if (bound_methods.size() <= disp_id) {
            bound_methods.resize(disp_id + 1, std::nullopt);
        }

        bound_methods[disp_id] = function;
    }
};

/// Convert a property name to a DynamicKey, handling integer property names
template<typename GC>
DynamicKey<GC> maybe_int_property(AvmString<GC> name) {
    // TODO: this should use a custom implementation, not parse()
    // FP is much stricter here, only allowing pure natural numbers without sign or leading zeros
    try {
        uint32_t val = std::stoul(name.to_utf8_lossy());
        return DynamicKey<GC>::uint(val);
    } catch (...) {
        return DynamicKey<GC>::string(name);
    }
}

/// General-purpose function for looking up dynamic properties on an object.
///
/// This is used in ScriptObject::get_property_local, Value::get_property,
/// Value::call_property, and VectorObject::get_property_local.
///
/// This method returns std::nullopt when the property is found on neither
/// the local values nor the prototype.
template<typename GC>
Result<std::optional<Value<GC>>, Error<GC>> get_dynamic_property(
    Activation<GC>* activation,
    const Multiname<GC>* multiname,
    const DynamicMap<GC, DynamicKey<GC>, Value<GC>>* local_values,
    std::optional<Object<GC>> prototype,
    Class<GC> instance_class
);

// Debug formatter
template<typename GC>
struct std::formatter<ScriptObject<GC>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const ScriptObject<GC>& obj, FormatContext& ctx) {
        ScriptObjectWrapper<GC> wrapper(obj.gc_base());
        return std::format_to(ctx.out(), "ScriptObject(name={}, ptr={:p})",
            wrapper.class_name().to_utf8_lossy(),
            static_cast<void*>(obj.gc_base().as_ptr()));
    }
};

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_OBJECT_SCRIPT_OBJECT_H
