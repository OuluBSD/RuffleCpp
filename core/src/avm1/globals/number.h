// C++ translation of core/src/avm1/globals/number.rs
// Number object for AVM1

#ifndef RUFFLE_CORE_AVM1_GLOBALS_NUMBER_H
#define RUFFLE_CORE_AVM1_GLOBALS_NUMBER_H

#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Error;
    template<typename T> class Object;
    template<typename T> class Value;
    struct DeclContext;
    struct StaticDeclarations;
    template<typename T> class SystemClass;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace number {

/// Property declarations for Number prototype
extern const StaticDeclarations PROTO_DECLS;

/// Property declarations for Number object (static properties)
extern const StaticDeclarations OBJECT_DECLS;

/// Create the Number class
///
/// \param context The declaration context for creating the class
/// \param super_proto The prototype object for the superclass
/// \return A SystemClass containing the created Number class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// Number constructor
///
/// Creates a Number object from the provided value.
///
/// \param activation The current activation context
/// \param this_obj The Number object being constructed
/// \param args Arguments for the constructor (value to convert to number)
/// \param arg_count Number of arguments
/// \return Value::Undefined
template<typename GC>
Value<GC> constructor(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Number function
///
/// Converts the provided value to a number and returns it.
///
/// \param activation The current activation context
/// \param this_obj The this object (unused for function call)
/// \param args Arguments (value to convert to number)
/// \param arg_count Number of arguments
/// \return The converted number value
template<typename GC>
Value<GC> function(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Number.toString method
///
/// Converts the number to a string representation.
/// Supports radix parameter (2-36) for base conversion.
///
/// \param activation The current activation context
/// \param this_obj The Number object
/// \param args Arguments (optional radix for base conversion)
/// \param arg_count Number of arguments
/// \return String representation of the number
template<typename GC>
Value<GC> to_string(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Number.valueOf method
///
/// Returns the primitive number value.
///
/// \param activation The current activation context
/// \param this_obj The Number object
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return The primitive number value, or Value::Undefined if not a number
template<typename GC>
Value<GC> value_of(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

} // namespace number
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_NUMBER_H
