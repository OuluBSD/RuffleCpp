// C++ translation of core/src/avm2/globals/flash/external/external_interface.rs
// flash.external.ExternalInterface native methods

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_EXTERNAL_EXTERNAL_INTERFACE_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_EXTERNAL_EXTERNAL_INTERFACE_H

#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class AvmString;
    template<typename T> class FunctionObject;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace external {
namespace external_interface {

/// Implements flash.external.ExternalInterface.call
/// Calls a JavaScript function in the HTML page containing the SWF
///
/// @param activation The current activation context
/// @param this_value The this value (unused)
/// @param args Function arguments (name at index 0, followed by arguments to pass)
/// @return Result from JavaScript call
template<typename GC>
Value<GC> call(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.external.ExternalInterface.available getter
/// Returns true if ExternalInterface is available
///
/// @param activation The current activation context
/// @param this_value The this value (unused)
/// @param args Function arguments (unused)
/// @return Boolean indicating availability
template<typename GC>
Value<GC> get_available(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.external.ExternalInterface.addCallback
/// Registers a callback function for JavaScript to call
///
/// @param activation The current activation context
/// @param this_value The this value (unused)
/// @param args Function arguments (name at index 0, function at index 1)
/// @return Value::Undefined
template<typename GC>
Value<GC> add_callback(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.external.ExternalInterface.objectID getter
/// Returns the ID of the SWF's DOM object, or null if not available
///
/// @param activation The current activation context
/// @param this_value The this value (unused)
/// @param args Function arguments (unused)
/// @return String ID or null
template<typename GC>
Value<GC> get_object_id(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Helper function to check if ExternalInterface is available
/// Throws error 2067 if not available
///
/// @param activation The current activation context
/// @throws Error 2067 if ExternalInterface is not available
template<typename GC>
void check_available(Activation<GC>* activation);

} // namespace external_interface
} // namespace external
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_EXTERNAL_EXTERNAL_INTERFACE_H
