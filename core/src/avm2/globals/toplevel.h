// C++ translation of core/src/avm2/globals/toplevel.rs
//! Global scope built-ins for AVM2

#ifndef RUFFLE_CORE_AVM2_GLOBALS_TOPLEVEL_H
#define RUFFLE_CORE_AVM2_GLOBALS_TOPLEVEL_H

#include <cstdint>
#include <string>
#include <vector>
#include <span>

// Forward declarations
namespace ruffle {
namespace core {
    class WString;
    template<typename GC> class UpdateContext;

    namespace avm2 {
        template<typename T> class Activation;
        template<typename T> class Error;
        template<typename T> class Value;
        template<typename T> class AvmString;
        namespace parameters {
            template<typename T> class ParametersExt;
        }
    }
    namespace string {
        template<typename T> class WStr;
        template<typename T> class WString;
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace toplevel {

/// Implements the global trace() function
///
/// Outputs a message to the console or debug output.
///
/// @param activation The current activation context
/// @param this_val The this value (unused)
/// @param args The arguments to trace
/// @return Value::Undefined
template<typename GC>
Value<GC> trace(
    Activation<GC>* activation,
    Value<GC> this_val,
    std::span<Value<GC>> args
);

/// Implements the global isFinite() function
///
/// Checks if a value is a finite number.
///
/// @param activation The current activation context
/// @param this_val The this value (unused)
/// @param args The arguments (first arg is the value to check)
/// @return true if the value is finite, false otherwise
template<typename GC>
Value<GC> is_finite(
    Activation<GC>* activation,
    Value<GC> this_val,
    std::span<Value<GC>> args
);

/// Implements the global isNaN() function
///
/// Checks if a value is NaN (Not a Number).
///
/// @param activation The current activation context
/// @param this_val The this value (unused)
/// @param args The arguments (first arg is the value to check)
/// @return true if the value is NaN, false otherwise
template<typename GC>
Value<GC> is_na_n(
    Activation<GC>* activation,
    Value<GC> this_val,
    std::span<Value<GC>> args
);

/// Implements the global parseInt() function
///
/// Parses a string and returns an integer.
///
/// @param activation The current activation context
/// @param this_val The this value (unused)
/// @param args The arguments (string to parse, optional radix)
/// @return The parsed integer
template<typename GC>
Value<GC> parse_int(
    Activation<GC>* activation,
    Value<GC> this_val,
    std::span<Value<GC>> args
);

/// Implements the global parseFloat() function
///
/// Parses a string and returns a floating-point number.
///
/// @param activation The current activation context
/// @param this_val The this value (unused)
/// @param args The arguments (string to parse)
/// @return The parsed floating-point number, or NaN if parsing fails
template<typename GC>
Value<GC> parse_float(
    Activation<GC>* activation,
    Value<GC> this_val,
    std::span<Value<GC>> args
);

/// Implements the global isXMLName() function
///
/// Checks if a value is a valid XML name.
///
/// @param activation The current activation context
/// @param this_val The this value (unused)
/// @param args The arguments (value to check)
/// @return true if the value is a valid XML name, false otherwise
template<typename GC>
Value<GC> is_xml_name(
    Activation<GC>* activation,
    Value<GC> this_val,
    std::span<Value<GC>> args
);

/// Implements the global escape() function
///
/// Encodes a string by replacing certain characters with escape sequences.
///
/// @param activation The current activation context
/// @param this_val The this value (unused)
/// @param args The arguments (string to escape)
/// @return The escaped string
template<typename GC>
Value<GC> escape(
    Activation<GC>* activation,
    Value<GC> this_val,
    std::span<Value<GC>> args
);

/// Implements the global unescape() function
///
/// Decodes a string encoded with escape().
///
/// @param activation The current activation context
/// @param this_val The this value (unused)
/// @param args The arguments (string to unescape)
/// @return The unescaped string
template<typename GC>
Value<GC> unescape(
    Activation<GC>* activation,
    Value<GC> this_val,
    std::span<Value<GC>> args
);

/// Implements the global encodeURI() function
///
/// Encodes a URI string.
///
/// @param activation The current activation context
/// @param this_val The this value (unused)
/// @param args The arguments (URI to encode)
/// @return The encoded URI string
template<typename GC>
Value<GC> encode_uri(
    Activation<GC>* activation,
    Value<GC> this_val,
    std::span<Value<GC>> args
);

/// Implements the global encodeURIComponent() function
///
/// Encodes a URI component string.
///
/// @param activation The current activation context
/// @param this_val The this value (unused)
/// @param args The arguments (URI component to encode)
/// @return The encoded URI component string
template<typename GC>
Value<GC> encode_uri_component(
    Activation<GC>* activation,
    Value<GC> this_val,
    std::span<Value<GC>> args
);

/// Implements the global decodeURI() function
///
/// Decodes a URI string encoded with encodeURI().
///
/// @param activation The current activation context
/// @param this_val The this value (unused)
/// @param args The arguments (URI to decode)
/// @return The decoded URI string
template<typename GC>
Value<GC> decode_uri(
    Activation<GC>* activation,
    Value<GC> this_val,
    std::span<Value<GC>> args
);

/// Implements the global decodeURIComponent() function
///
/// Decodes a URI component string encoded with encodeURIComponent().
///
/// @param activation The current activation context
/// @param this_val The this value (unused)
/// @param args The arguments (URI component to decode)
/// @return The decoded URI component string
template<typename GC>
Value<GC> decode_uri_component(
    Activation<GC>* activation,
    Value<GC> this_val,
    std::span<Value<GC>> args
);

} // namespace toplevel
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_TOPLEVEL_H
