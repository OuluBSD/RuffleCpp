// C++ translation of core/src/avm1/globals/object.rs
// Object prototype

#ifndef RUFFLE_CORE_AVM1_GLOBALS_OBJECT_H
#define RUFFLE_CORE_AVM1_GLOBALS_OBJECT_H

#include <cstdint>
#include <memory>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Error;
    template<typename T> class DisplayObject;
    template<typename T> class Object;
    template<typename T> class Value;
    struct DeclContext;
    struct StaticDeclarations;
    template<typename T> class SystemClass;
    namespace property {
        enum class Attribute : uint8_t;
    }
}
}
namespace string {
    template<typename T> class AvmString;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace object {

/// Property declarations for Object prototype
extern const StaticDeclarations PROTO_DECLS;

/// Property declarations for Object object (static properties)
extern const StaticDeclarations OBJECT_DECLS;

/// Create the Object class
///
/// Since Object and Function are so heavily intertwined, this function does
/// not allocate an object to store either proto. Instead, they must be provided
/// through the DeclContext.
///
/// \param context The declaration context for creating the class
/// \return A SystemClass containing the created Object class
template<typename GC>
SystemClass<GC> create_class(DeclContext<GC>* context);

/// Object constructor
///
/// Implements the Object constructor.
///
/// \param activation The current activation context
/// \param this_obj The Object being constructed
/// \param args Arguments for the constructor
/// \param arg_count Number of arguments
/// \return The constructed object or coerced value
template<typename GC>
Value<GC> constructor(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Object function
///
/// Implements the Object function (called without new).
///
/// \param activation The current activation context
/// \param this_obj The this object (unused for function call)
/// \param args Arguments for the function
/// \param arg_count Number of arguments
/// \return A new object or coerced value
template<typename GC>
Value<GC> function(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Object.prototype.addProperty
///
/// Adds a property with getter and setter functions to an object.
///
/// \param activation The current activation context
/// \param this_obj The Object to add the property to
/// \param args Arguments (name, getter, setter)
/// \param arg_count Number of arguments
/// \return true if property was added, false otherwise
template<typename GC>
Value<GC> add_property(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Object.prototype.hasOwnProperty
///
/// Checks if the object has an own property with the given name.
///
/// \param activation The current activation context
/// \param this_obj The Object to check
/// \param args Arguments (property name)
/// \param arg_count Number of arguments
/// \return true if the object has the property, false otherwise
template<typename GC>
Value<GC> has_own_property(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Object.prototype.toString
///
/// Returns a string representation of the object.
///
/// \param activation The current activation context
/// \param this_obj The Object
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return String representation ("[object Object]" or "[type Function]")
template<typename GC>
Value<GC> to_string(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Object.prototype.isPropertyEnumerable
///
/// Checks if a property is enumerable.
///
/// \param activation The current activation context
/// \param this_obj The Object to check
/// \param args Arguments (property name)
/// \param arg_count Number of arguments
/// \return true if the property is enumerable, false otherwise
template<typename GC>
Value<GC> is_property_enumerable(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Object.prototype.isPrototypeOf
///
/// Checks if this object is in the prototype chain of another object.
///
/// \param activation The current activation context
/// \param this_obj The Object to check
/// \param args Arguments (object to check against)
/// \param arg_count Number of arguments
/// \return true if this object is in the prototype chain, false otherwise
template<typename GC>
Value<GC> is_prototype_of(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Object.prototype.valueOf
///
/// Returns the primitive value of the object.
///
/// \param activation The current activation context
/// \param this_obj The Object
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return The object itself
template<typename GC>
Value<GC> value_of(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Object.registerClass
///
/// Registers a constructor function for a given class name.
///
/// \param activation The current activation context
/// \param this_obj The Object (unused)
/// \param args Arguments (class name, constructor function)
/// \param arg_count Number of arguments
/// \return true if registration succeeded, false otherwise
template<typename GC>
Value<GC> register_class(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Object.prototype.watch
///
/// Sets a watch callback on a property.
///
/// \param activation The current activation context
/// \param this_obj The Object to watch
/// \param args Arguments (property name, callback, user data)
/// \param arg_count Number of arguments
/// \return true if watch was set, false otherwise
template<typename GC>
Value<GC> watch(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Object.prototype.unwatch
///
/// Removes a watch callback from a property.
///
/// \param activation The current activation context
/// \param this_obj The Object to unwatch
/// \param args Arguments (property name)
/// \param arg_count Number of arguments
/// \return true if watch was removed, false otherwise
template<typename GC>
Value<GC> unwatch(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ASSetPropFlags
///
/// An undocumented function that allows ActionScript 2.0 classes to
/// declare the property flags of a given property.
///
/// \param activation The current activation context
/// \param this_obj Unused
/// \param args Arguments (object, property list, set flags, clear flags)
/// \param arg_count Number of arguments
/// \return Value::Undefined
template<typename GC>
Value<GC> as_set_prop_flags(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements toLocaleString
///
/// Returns a locale-specific string representation.
///
/// \param activation The current activation context
/// \param this_obj The Object
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return String representation
template<typename GC>
Value<GC> to_locale_string(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Method indices for Object
namespace method {
    constexpr uint16_t WATCH = 0;
    constexpr uint16_t UNWATCH = 1;
    constexpr uint16_t ADD_PROPERTY = 2;
    constexpr uint16_t VALUE_OF = 3;
    constexpr uint16_t TO_STRING = 4;
    constexpr uint16_t HAS_OWN_PROPERTY = 5;
    constexpr uint16_t IS_PROTOTYPE_OF = 6;
    constexpr uint16_t IS_PROPERTY_ENUMERABLE = 7;
    constexpr uint16_t REGISTER_CLASS = 8;
    constexpr uint16_t CONSTRUCTOR = 9;
}

} // namespace object
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_OBJECT_H
