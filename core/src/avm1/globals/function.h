// C++ translation of core/src/avm1/globals/function.rs
// Function prototype and class implementation

#ifndef RUFFLE_CORE_AVM1_GLOBALS_FUNCTION_H
#define RUFFLE_CORE_AVM1_GLOBALS_FUNCTION_H

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
namespace function {

/// Property declarations for Function prototype
extern const StaticDeclarations PROTO_DECLS;

/// Create the Function class
///
/// Since Object and Function are so heavily intertwined, this function does
/// not allocate an object to store either proto. Instead, they must be provided
/// through the DeclContext.
///
/// \param context The declaration context for creating the class
/// \return A SystemClass containing the created Function class
template<typename GC>
SystemClass<GC> create_class(DeclContext<GC>* context);

/// Implements new Function()
///
/// Function constructor - returns the first argument if provided,
/// otherwise returns the this object.
///
/// \param activation The current activation context
/// \param this_obj The object being constructed
/// \param args Arguments:
///   - args[0]: optional function body or value
/// \return The first argument or this object
template<typename GC>
Value<GC> constructor(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Function()
///
/// Function called as a function (not constructor).
/// Returns the first argument if provided, otherwise creates
/// a prototypeless bare object.
///
/// \param activation The current activation context
/// \param this_obj The function object
/// \param args Arguments:
///   - args[0]: optional function body or value
/// \return The first argument or a new bare object
template<typename GC>
Value<GC> function_call(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Function.prototype.call
///
/// Calls the function with a specified this value and arguments.
///
/// \param activation The current activation context
/// \param func The function object to call
/// \param args Arguments:
///   - args[0]: this value for the function call
///   - args[1..]: arguments to pass to the function
/// \return The result of the function call
template<typename GC>
Value<GC> call(
    Activation<GC>* activation,
    Object<GC>* func,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Function.prototype.apply
///
/// Calls the function with a specified this value and arguments
/// provided as an array-like object.
///
/// \param activation The current activation context
/// \param func The function object to call
/// \param args Arguments:
///   - args[0]: this value for the function call
///   - args[1]: array-like object containing arguments
/// \return The result of the function call
template<typename GC>
Value<GC> apply(
    Activation<GC>* activation,
    Object<GC>* func,
    const Value<GC>* args,
    size_t arg_count
);

} // namespace function
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_FUNCTION_H
