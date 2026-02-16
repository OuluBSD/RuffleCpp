// C++ translation of core/src/avm2/globals/function.rs
//! Function builtin and prototype

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FUNCTION_H
#define RUFFLE_CORE_AVM2_GLOBALS_FUNCTION_H

#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class FunctionObject;
    template<typename T> class ClassObject;
    template<typename T> class ArrayObject;
    template<typename T> class ArrayStorage;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace function {

/// Create a dummy function using Function.createDummyFunction
/// The Function class must be stored properly in SystemClasses; otherwise, this method will panic
///
/// \param activation The current activation context
/// \return A dummy FunctionObject that returns Value::Undefined when called
template<typename GC>
FunctionObject<GC> create_dummy_function(Activation<GC>* activation);

} // namespace function

/// Implements Function's custom constructor
/// This is used when ActionScript manually calls 'new Function()',
/// which produces a dummy function that just returns Value::Undefined when called
///
/// \param activation The current activation context
/// \param args Arguments passed to the constructor (must be empty, otherwise error 1066)
/// \return A dummy FunctionObject, or error if arguments were provided
template<typename GC>
Value<GC> function_constructor(
    Activation<GC>* activation,
    const std::vector<Value<GC>>& args
);

/// Initialize the Function class
/// Set Function's prototype and register it in SystemClasses
/// This method is called from AS during builtins initialization
///
/// \param activation The current activation context
/// \param this_value The Function class object
/// \param args Unused arguments
/// \return Value::Undefined
template<typename GC>
Value<GC> _init_function_class(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Function.prototype.call
/// Calls the function with the specified this value and arguments
///
/// \param activation The current activation context
/// \param func_value The function to call (must be a FunctionObject)
/// \param args First argument is this value, rest are passed to the function
/// \return The result of calling the function
template<typename GC>
Value<GC> call(
    Activation<GC>* activation,
    Value<GC> func_value,
    const std::vector<Value<GC>>& args
);

/// Implements Function.prototype.apply
/// Calls the function with the specified this value and an array of arguments
///
/// \param activation The current activation context
/// \param func_value The function to call (must be a FunctionObject)
/// \param args First argument is this value, second is array of arguments (or null/undefined)
/// \return The result of calling the function
/// \throws Error 1116 if the second argument is not null, undefined, or an array
template<typename GC>
Value<GC> apply(
    Activation<GC>* activation,
    Value<GC> func_value,
    const std::vector<Value<GC>>& args
);

/// Implements Function.prototype.length getter
/// Returns the number of parameters the function expects
///
/// \param activation The current activation context
/// \param this_value The function object
/// \param args Unused arguments
/// \return The function's parameter count, or Undefined if not a function
template<typename GC>
Value<GC> get_length(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Function.prototype.prototype getter
/// Returns the function's prototype object
///
/// \param activation The current activation context
/// \param this_value The function object
/// \param args Unused arguments
/// \return The function's prototype, or Undefined if not a function or no prototype
template<typename GC>
Value<GC> get_prototype(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Function.prototype.prototype setter
/// Sets the function's prototype object
///
/// \param activation The current activation context
/// \param this_value The function object
/// \param args First argument is the new prototype value
/// \return Value::Undefined
template<typename GC>
Value<GC> set_prototype(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FUNCTION_H
