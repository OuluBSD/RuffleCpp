// C++ translation of core/src/avm1/globals/external_interface.rs
// flash.external.ExternalInterface object

#ifndef RUFFLE_CORE_AVM1_GLOBALS_EXTERNAL_INTERFACE_H
#define RUFFLE_CORE_AVM1_GLOBALS_EXTERNAL_INTERFACE_H

#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Error;
    template<typename T> class NativeObject;
    template<typename T> class Object;
    template<typename T> class Value;
    struct DeclContext;
    struct StaticDeclarations;
    template<typename T> class SystemClass;
    namespace parameters {
        enum class UndefinedAs;
    }
}
namespace avm_string {
    template<typename T> class AvmString;
}
namespace string {
    template<typename T> class WStr;
    template<typename T> class WString;
}
namespace external {
    class Callback;
    class ExternalInterface;
    template<typename T> class Value;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace external_interface {

/// Property declarations for ExternalInterface static object
extern const StaticDeclarations OBJECT_DECLS;

/// Create the ExternalInterface class
///
/// @param context The declaration context for creating the class
/// @param super_proto The prototype object for the superclass
/// @return A SystemClass containing the created ExternalInterface class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// Implements ExternalInterface.available property getter
///
/// @param activation The current activation context
/// @param this_obj The ExternalInterface object (unused)
/// @param args Arguments (unused)
/// @return true if ExternalInterface is available
template<typename GC>
Value<GC> get_available(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ExternalInterface.addCallback method
///
/// Registers an ActionScript function to be callable from JavaScript.
///
/// @param activation The current activation context
/// @param this_obj The ExternalInterface object (unused)
/// @param args Arguments (name, thisObject, method)
/// @return true if callback was registered successfully
template<typename GC>
Value<GC> add_callback(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ExternalInterface.call method
///
/// Calls a JavaScript function from ActionScript.
///
/// @param activation The current activation context
/// @param this_obj The ExternalInterface object (unused)
/// @param args Arguments (functionName, ...arguments)
/// @return The result from JavaScript or null
template<typename GC>
Value<GC> call(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ExternalInterface._initJS method (stub)
///
/// @param activation The current activation context
/// @param this_obj The ExternalInterface object (unused)
/// @param args Arguments (unused)
/// @return Value::Undefined
template<typename GC>
Value<GC> init_js(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ExternalInterface._objectID method (stub)
///
/// @param activation The current activation context
/// @param this_obj The ExternalInterface object (unused)
/// @param args Arguments (unused)
/// @return Value::Undefined
template<typename GC>
Value<GC> object_id(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ExternalInterface._addCallback method (stub)
///
/// @param activation The current activation context
/// @param this_obj The ExternalInterface object (unused)
/// @param args Arguments (unused)
/// @return Value::Undefined
template<typename GC>
Value<GC> add_callback2(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ExternalInterface._evalJS method (stub)
///
/// @param activation The current activation context
/// @param this_obj The ExternalInterface object (unused)
/// @param args Arguments (unused)
/// @return Value::Undefined
template<typename GC>
Value<GC> eval_js(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ExternalInterface._callOut method (stub)
///
/// @param activation The current activation context
/// @param this_obj The ExternalInterface object (unused)
/// @param args Arguments (unused)
/// @return Value::Undefined
template<typename GC>
Value<GC> call_out(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ExternalInterface._escapeXML method
///
/// Escapes special XML characters in a string.
///
/// @param activation The current activation context
/// @param this_obj The ExternalInterface object (unused)
/// @param args Arguments (string)
/// @return The escaped string or null
template<typename GC>
Value<GC> escape_xml(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ExternalInterface._unescapeXML method
///
/// Unescapes XML entities in a string.
///
/// @param activation The current activation context
/// @param this_obj The ExternalInterface object (unused)
/// @param args Arguments (string)
/// @return The unescaped string or null
template<typename GC>
Value<GC> unescape_xml(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ExternalInterface._jsQuoteString method
///
/// Escapes double quotes in a string for JavaScript.
///
/// @param activation The current activation context
/// @param this_obj The ExternalInterface object (unused)
/// @param args Arguments (string)
/// @return The quoted string or null
template<typename GC>
Value<GC> js_quote_string(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ExternalInterface._useSetReturnValueHack method (stub)
///
/// @param activation The current activation context
/// @param this_obj The ExternalInterface object (unused)
/// @param args Arguments (unused)
/// @return Value::Undefined
template<typename GC>
Value<GC> use_set_return_value_hack(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ExternalInterface._callIn method (stub)
///
/// @param activation The current activation context
/// @param this_obj The ExternalInterface object (unused)
/// @param args Arguments (unused)
/// @return Value::Undefined
template<typename GC>
Value<GC> call_in(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ExternalInterface._arrayToXML method
///
/// Converts an ActionScript array to XML format.
///
/// @param activation The current activation context
/// @param this_obj The ExternalInterface object (unused)
/// @param args Arguments (array)
/// @return XML string representation of the array
template<typename GC>
Value<GC> array_to_xml(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ExternalInterface._argumentsToXML method
///
/// Converts ActionScript arguments to XML format.
///
/// @param activation The current activation context
/// @param this_obj The ExternalInterface object (unused)
/// @param args Arguments (arguments object)
/// @return XML string representation of the arguments
template<typename GC>
Value<GC> arguments_to_xml(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ExternalInterface._objectToXML method
///
/// Converts an ActionScript object to XML format.
///
/// @param activation The current activation context
/// @param this_obj The ExternalInterface object (unused)
/// @param args Arguments (object)
/// @return XML string representation of the object
template<typename GC>
Value<GC> object_to_xml(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ExternalInterface._toXML method
///
/// Converts any ActionScript value to XML format.
///
/// @param activation The current activation context
/// @param this_obj The ExternalInterface object (unused)
/// @param args Arguments (value)
/// @return XML string representation of the value
template<typename GC>
Value<GC> to_xml(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ExternalInterface._objectToAS method (stub)
///
/// @param activation The current activation context
/// @param this_obj The ExternalInterface object (unused)
/// @param args Arguments (unused)
/// @return Empty object
template<typename GC>
Value<GC> object_to_as(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ExternalInterface._arrayToAS method (stub)
///
/// @param activation The current activation context
/// @param this_obj The ExternalInterface object (unused)
/// @param args Arguments (unused)
/// @return Empty array
template<typename GC>
Value<GC> array_to_as(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ExternalInterface._toAS method
///
/// Converts XML back to ActionScript value.
///
/// @param activation The current activation context
/// @param this_obj The ExternalInterface object (unused)
/// @param args Arguments (XML node)
/// @return The converted ActionScript value
template<typename GC>
Value<GC> to_as(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ExternalInterface._argumentsToAS method (stub)
///
/// @param activation The current activation context
/// @param this_obj The ExternalInterface object (unused)
/// @param args Arguments (unused)
/// @return Empty array
template<typename GC>
Value<GC> arguments_to_as(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ExternalInterface._arrayToJS method (stub)
///
/// @param activation The current activation context
/// @param this_obj The ExternalInterface object (unused)
/// @param args Arguments (unused)
/// @return Value::Undefined
template<typename GC>
Value<GC> array_to_js(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ExternalInterface._objectToJS method (stub)
///
/// @param activation The current activation context
/// @param this_obj The ExternalInterface object (unused)
/// @param args Arguments (unused)
/// @return Value::Undefined
template<typename GC>
Value<GC> object_to_js(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ExternalInterface._toJS method (stub)
///
/// @param activation The current activation context
/// @param this_obj The ExternalInterface object (unused)
/// @param args Arguments (unused)
/// @return Value::Undefined
template<typename GC>
Value<GC> to_js(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Internal helper: Escape XML characters in a string
///
/// @param activation The current activation context
/// @param arg The string to escape
/// @return The escaped string value
template<typename GC>
Value<GC> escape_xml_inner(
    Activation<GC>* activation,
    const string::WStr<GC>* arg
);

/// Internal helper: Unescape XML entities in a string
///
/// @param activation The current activation context
/// @param arg The string to unescape
/// @return The unescaped string value
template<typename GC>
Value<GC> unescape_xml_inner(
    Activation<GC>* activation,
    const string::WStr<GC>* arg
);

/// Internal helper: Convert array to XML string
///
/// @param activation The current activation context
/// @param arg The array object
/// @return XML string representation
template<typename GC>
string::WString<GC> array_to_xml_inner(
    Activation<GC>* activation,
    Object<GC>* arg
);

/// Internal helper: Convert arguments to XML string
///
/// @param activation The current activation context
/// @param arg The arguments object
/// @return XML string representation
template<typename GC>
string::WString<GC> arguments_to_xml_inner(
    Activation<GC>* activation,
    Object<GC>* arg
);

/// Internal helper: Convert object to XML string
///
/// @param activation The current activation context
/// @param arg The object
/// @return XML string representation
template<typename GC>
string::WString<GC> object_to_xml_inner(
    Activation<GC>* activation,
    Object<GC>* arg
);

/// Internal helper: Convert value to XML string
///
/// @param activation The current activation context
/// @param arg The value to convert
/// @return XML string representation
template<typename GC>
string::WString<GC> to_xml_inner(
    Activation<GC>* activation,
    Value<GC> arg
);

} // namespace external_interface
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_EXTERNAL_INTERFACE_H
