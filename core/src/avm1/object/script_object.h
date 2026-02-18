// C++ translation of core/src/avm1/object/script_object.rs
//! ScriptObject implementation for AVM1

#ifndef RUFFLE_CORE_AVM1_OBJECT_SCRIPT_OBJECT_H
#define RUFFLE_CORE_AVM1_OBJECT_SCRIPT_OBJECT_H

#include <cstdint>
#include <optional>
#include <vector>
#include <memory>
#include <functional>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Error;
    template<typename T> class Object;
    template<typename T> class ObjectWeak;
    template<typename T> class ObjectHandle;
    template<typename T> class Value;
    template<typename T> class Property;
    template<typename T> class PropertyMap;
    template<typename T> class Watcher;
    template<typename T> class NativeObject;
    template<typename T> class SuperObject;
    template<typename T> class FunctionObject;
    enum class Attribute : uint8_t;

    struct ExecutionName;
    enum class ExecutionReason;
}
namespace string {
    template<typename T> class AvmString;
}
namespace display_object {
    template<typename T> class DisplayObject;
}
template<typename GC> class UpdateContext;
template<typename GC> class Mutation;
template<typename GC> class DynamicRoot;
}
}

namespace ruffle {
namespace core {
namespace avm1 {

/// Weak reference to Object data
template<typename GC>
class ObjectWeak {
public:
    ObjectWeak() = default;

    /// Upgrade the weak reference to a strong reference
    std::optional<Object<GC>> upgrade(Mutation<GC>* mc);

private:
    friend class Object<GC>;
    explicit ObjectWeak(GcWeak<GC, RefLock<ObjectData<GC>>> data) : data_(data) {}
    GcWeak<GC, RefLock<ObjectData<GC>>> data_;
};

/// Handle for stashing Object across GC mutations
class ObjectHandle {
public:
    /// Stash an object reference
    template<typename GC>
    static ObjectHandle stash(const UpdateContext<GC>* context, Object<GC> obj);

    /// Fetch the stashed object
    template<typename GC>
    Object<GC> fetch(const UpdateContext<GC>* context) const;

private:
    DynamicRoot handle_;
};

/// Data structure for Object
/// Uses RefLock for interior mutability
template<typename GC>
class ObjectData {
public:
    ObjectData() = default;

    /// Native object type
    NativeObject<GC> native;

    /// Property storage
    PropertyMap<GC, Property<GC>> properties;

    /// Interface list (for instanceof checks)
    std::optional<std::vector<Object<GC>>> interfaces;

    /// Property watchers
    PropertyMap<GC, Watcher<GC>> watchers;

    // Friend classes for accessing private members
    friend class Object<GC>;
    friend class ObjectWeak<GC>;
};

/// Object - The base object type for AVM1
///
/// Represents an ActionScript 1/2 object with properties, methods,
/// and prototype chain support.
template<typename GC>
class Object {
public:
    using DataType = ObjectData<GC>;
    using WeakType = ObjectWeak<GC>;
    using HandleType = ObjectHandle;

    Object() = default;
    explicit Object(Gc<GC, RefLock<DataType>> data) : data_(data) {}

    /// Get a weak reference to this object
    ObjectWeak<GC> as_weak() const {
        return ObjectWeak<GC>(Gc::downgrade(data_));
    }

    /// Create a new object with optional prototype
    ///
    /// @param context The string context
    /// @param proto Optional prototype value
    /// @return New Object
    static Object new_object(
        const StringContext<GC>* context,
        std::optional<Value<GC>> proto = std::nullopt
    );

    /// Create a new object with native type
    ///
    /// @param context The string context
    /// @param proto Optional prototype value
    /// @param native The native object type
    /// @return New Object
    static Object new_with_native(
        const StringContext<GC>* context,
        std::optional<Value<GC>> proto,
        NativeObject<GC> native
    );

    /// Create an object without assigning any __proto__ property
    ///
    /// @param gc_context The mutation context
    /// @return New Object
    static Object new_without_proto(Mutation<GC>* gc_context);

    /// Get the value of a data property, ignoring attributes
    ///
    /// Doesn't look up the prototype chain and ignores virtual properties.
    ///
    /// @param name The property name
    /// @param activation The current activation
    /// @return The property value or Undefined
    Value<GC> get_data(
        AvmString<GC> name,
        Activation<GC>* activation
    ) const;

    /// Set a data property, ignoring attributes
    ///
    /// Doesn't look up the prototype chain and ignores virtual properties,
    /// but still might call watchers.
    ///
    /// @param name The property name
    /// @param value The new value
    /// @param activation The current activation
    void set_data(
        AvmString<GC> name,
        Value<GC> value,
        Activation<GC>* activation
    );

    /// Get own enumerable properties
    ///
    /// @return Vector of (name, value) pairs
    std::vector<std::pair<AvmString<GC>, Value<GC>>> own_properties() const;

    /// Retrieve a named, non-virtual property exclusively
    ///
    /// This function should not inspect prototype chains.
    ///
    /// @param name The property name
    /// @param activation The current activation
    /// @return The property value or nullopt
    std::optional<Value<GC>> get_local_stored(
        AvmString<GC> name,
        Activation<GC>* activation
    ) const;

    /// Set a named property on the object
    ///
    /// @param name The property name
    /// @param value The new value
    /// @param activation The current activation
    /// @param this The this value for setters
    void set_local(
        AvmString<GC> name,
        Value<GC> value,
        Activation<GC>* activation,
        Object<GC> this_obj
    );

    /// Call the underlying object
    ///
    /// @param name The execution name
    /// @param activation The current activation
    /// @param this The this value
    /// @param args The arguments
    /// @return The result value or error
    Value<GC> call(
        ExecutionName<GC> name,
        Activation<GC>* activation,
        Value<GC> this_val,
        const std::vector<Value<GC>>& args
    ) const;

    /// Construct on an existing object
    ///
    /// @param activation The current activation
    /// @param this The object to construct on
    /// @param args The arguments
    void construct_on_existing(
        Activation<GC>* activation,
        Object<GC> this_obj,
        const std::vector<Value<GC>>& args
    ) const;

    /// Construct a new object
    ///
    /// @param activation The current activation
    /// @param args The arguments
    /// @return The constructed value
    Value<GC> construct(
        Activation<GC>* activation,
        const std::vector<Value<GC>>& args
    ) const;

    /// Retrieve a getter defined on this object
    ///
    /// @param name The property name
    /// @param activation The current activation
    /// @return The getter function or nullopt
    std::optional<Object<GC>> getter(
        AvmString<GC> name,
        Activation<GC>* activation
    ) const;

    /// Retrieve a setter defined on this object
    ///
    /// @param name The property name
    /// @param activation The current activation
    /// @return The setter function or nullopt
    std::optional<Object<GC>> setter(
        AvmString<GC> name,
        Activation<GC>* activation
    ) const;

    /// Delete a named property from the object
    ///
    /// @param activation The current activation
    /// @param name The property name
    /// @return True if deleted, false otherwise
    bool delete_property(Activation<GC>* activation, AvmString<GC> name);

    /// Define a virtual property
    ///
    /// A virtual property is a set of get/set functions that are called
    /// when a given named property is retrieved or stored.
    ///
    /// @param gc_context The mutation context
    /// @param name The property name
    /// @param getter The getter function
    /// @param setter Optional setter function
    /// @param attributes Property attributes
    void add_property(
        Mutation<GC>* gc_context,
        AvmString<GC> name,
        Object<GC> getter,
        std::optional<Object<GC>> setter,
        Attribute attributes
    );

    /// Define a virtual property with case sensitivity
    ///
    /// @param activation The current activation
    /// @param name The property name
    /// @param getter The getter function
    /// @param setter Optional setter function
    /// @param attributes Property attributes
    void add_property_with_case(
        Activation<GC>* activation,
        AvmString<GC> name,
        Object<GC> getter,
        std::optional<Object<GC>> setter,
        Attribute attributes
    );

    /// Call the watcher of a property
    ///
    /// @param activation The current activation
    /// @param name The property name
    /// @param value The new value (may be modified by watcher)
    /// @param this The this value
    void call_watcher(
        Activation<GC>* activation,
        AvmString<GC> name,
        Value<GC>& value,
        Object<GC> this_obj
    );

    /// Set a watcher on a property
    ///
    /// @param activation The current activation
    /// @param name The property name
    /// @param callback The watcher callback function
    /// @param user_data User data passed to the watcher
    void watch(
        Activation<GC>* activation,
        AvmString<GC> name,
        Object<GC> callback,
        Value<GC> user_data
    );

    /// Remove a watcher from a property
    ///
    /// @param activation The current activation
    /// @param name The property name
    /// @return True if a watcher was removed
    bool unwatch(Activation<GC>* activation, AvmString<GC> name);

    /// Define a value on an object
    ///
    /// Unlike setting a value, this replaces any existing virtual or
    /// built-in properties.
    ///
    /// @param gc_context The mutation context
    /// @param name The property name
    /// @param value The value
    /// @param attributes Property attributes
    void define_value(
        Mutation<GC>* gc_context,
        AvmString<GC> name,
        Value<GC> value,
        Attribute attributes
    );

    /// Set the attributes of a property
    ///
    /// @param gc_context The mutation context
    /// @param name Optional property name (nullopt for all properties)
    /// @param set_attributes Attributes to set
    /// @param clear_attributes Attributes to clear
    void set_attributes(
        Mutation<GC>* gc_context,
        std::optional<AvmString<GC>> name,
        Attribute set_attributes,
        Attribute clear_attributes
    );

    /// Get the __proto__ of this object
    ///
    /// @param activation The current activation
    /// @return The prototype value
    Value<GC> proto(Activation<GC>* activation) const;

    /// Get the prototype of this object (as if it was a function)
    ///
    /// @param activation The current activation
    /// @return The prototype value
    Value<GC> prototype(Activation<GC>* activation) const;

    /// Check if the object has a given named property
    ///
    /// @param activation The current activation
    /// @param name The property name
    /// @return True if property exists
    bool has_property(Activation<GC>* activation, AvmString<GC> name) const;

    /// Check if the object has a property on itself (not prototype)
    ///
    /// @param activation The current activation
    /// @param name The property name
    /// @return True if own property exists
    bool has_own_property(Activation<GC>* activation, AvmString<GC> name) const;

    /// Check if the object has a virtual property on itself
    ///
    /// @param activation The current activation
    /// @param name The property name
    /// @return True if own virtual property exists
    bool has_own_virtual(Activation<GC>* activation, AvmString<GC> name) const;

    /// Get a stored value from the object
    ///
    /// @param name The property name
    /// @param activation The current activation
    /// @return The property value or Undefined
    Value<GC> get_stored(
        AvmString<GC> name,
        Activation<GC>* activation
    ) const;

    /// Set a stored value on the object
    ///
    /// @param name The property name
    /// @param value The new value
    /// @param activation The current activation
    void set_stored(
        AvmString<GC> name,
        Value<GC> value,
        Activation<GC>* activation
    );

    /// Get the native object type
    ///
    /// @return Reference to the native object
    const NativeObject<GC>& native() const {
        return data_->native;
    }

    /// Get the native object type (non-super)
    ///
    /// @return Reference to the native object
    const NativeObject<GC>& native_no_super() const {
        return data_->native;
    }

    /// Get as SuperObject if applicable
    ///
    /// @return SuperObject or nullopt
    std::optional<SuperObject<GC>> as_super_object() const;

    /// Get the interfaces list
    ///
    /// @return Reference to interfaces vector or nullopt
    const std::optional<std::vector<Object<GC>>>& interfaces() const {
        return data_->interfaces;
    }

    /// Set the interfaces list
    ///
    /// @param gc_context The mutation context
    /// @param interfaces The new interfaces list
    void set_interfaces(
        Mutation<GC>* gc_context,
        std::optional<std::vector<Object<GC>>> interfaces
    );

    /// Get the length property (for arrays)
    ///
    /// @param activation The current activation
    /// @return The length value
    Value<GC> length(Activation<GC>* activation) const;

    /// Set the length property (for arrays)
    ///
    /// @param activation The current activation
    /// @param new_length The new length
    void set_length(Activation<GC>* activation, int32_t new_length);

    /// Delete an array element
    ///
    /// @param activation The current activation
    /// @param index The element index
    /// @return True if deleted
    bool delete_element(Activation<GC>* activation, int32_t index);

    /// Check if this object is an instance of another
    ///
    /// @param activation The current activation
    /// @param prototype The prototype to check against
    /// @return True if instance of
    bool is_instance_of(
        Activation<GC>* activation,
        Object<GC> prototype
    ) const;

    /// Get the property map
    ///
    /// @return Reference to the property map
    const PropertyMap<GC, Property<GC>>& properties() const {
        return data_->properties;
    }

    /// Get the property map (mutable)
    ///
    /// @param gc_context The mutation context
    /// @return Mutable reference to the property map
    PropertyMap<GC, Property<GC>>& properties_mut(Mutation<GC>* gc_context) {
        return data_->properties;
    }

private:
    Gc<GC, RefLock<DataType>> data_;

    /// Internal implementation of new_object
    static Object new_impl(
        const StringContext<GC>* context,
        std::optional<Value<GC>> proto,
        NativeObject<GC> native
    );

    /// Parse an array index from a property name
    static std::optional<int32_t> parse_array_index(AvmString<GC> name);
};

/// Watcher - A callback that is called when a property changes
template<typename GC>
class Watcher {
public:
    Watcher() = default;

    /// Create a new watcher
    ///
    /// @param callback The callback function
    /// @param user_data User data passed to the callback
    static Watcher create(Object<GC> callback, Value<GC> user_data);

    /// Call the watcher
    ///
    /// @param activation The current activation
    /// @param name The property name
    /// @param old_value The old value
    /// @param new_value The new value
    /// @param this The this value
    /// @return The result value (may override new_value)
    Value<GC> call(
        Activation<GC>* activation,
        AvmString<GC> name,
        Value<GC> old_value,
        Value<GC> new_value,
        Object<GC> this_obj
    ) const;

private:
    Object<GC> callback_;
    Value<GC> user_data_;
};

} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_OBJECT_SCRIPT_OBJECT_H
