// C++ translation of core/src/avm2/globals/json.rs
// JSON implementation for AVM2

#ifndef RUFFLE_CORE_AVM2_GLOBALS_JSON_H
#define RUFFLE_CORE_AVM2_GLOBALS_JSON_H

#include <vector>
#include <string>
#include <optional>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class AvmString;
    template<typename T> class FunctionObject;
    template<typename T> class Object;
    template<typename T> class ArrayObject;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace json {

/// Replacer type for JSON.stringify
/// Can be either a function or a property list array
template<typename GC>
struct Replacer {
    enum class Type { Function, PropList };
    Type type;
    
    union {
        FunctionObject<GC>* function;
        ArrayObject<GC>* prop_list;
    };
    
    Replacer(FunctionObject<GC>* func) : type(Type::Function), function(func) {}
    Replacer(ArrayObject<GC>* arr) : type(Type::PropList), prop_list(arr) {}
};

/// JSON.parse implementation
/// Parses a JSON string and returns the corresponding AVM2 value
///
/// @param activation The current activation context
/// @param this_value The this value (unused)
/// @param args Function arguments (JSON string at index 0, optional reviver function at index 1)
/// @return Parsed AVM2 value
/// @throws Error 1132 if JSON parsing fails
template<typename GC>
Value<GC> parse(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// JSON.stringify implementation
/// Converts an AVM2 value to a JSON string
///
/// @param activation The current activation context
/// @param this_value The this value (unused)
/// @param args Function arguments (value at index 0, optional replacer at index 1, optional spaces at index 2)
/// @return JSON string representation
/// @throws Error 1131 if replacer is invalid
template<typename GC>
Value<GC> stringify(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace json
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_JSON_H
