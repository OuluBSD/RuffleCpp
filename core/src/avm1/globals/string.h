// C++ translation of core/src/avm1/globals/string.rs
// String class impl

#ifndef RUFFLE_CORE_AVM1_GLOBALS_STRING_H
#define RUFFLE_CORE_AVM1_GLOBALS_STRING_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Error;
    template<typename T> class NativeObject;
    template<typename T> class Object;
    template<typename T> class Value;
    template<typename T> class ArrayBuilder;
    struct DeclContext;
    struct StaticDeclarations;
    template<typename T> class SystemClass;
}
}
namespace string {
    template<typename T> class AvmString;
    template<typename T> class WString;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace string_class {

/// Property declarations for String prototype
extern const StaticDeclarations PROTO_DECLS;

/// Property declarations for String object (static properties)
extern const StaticDeclarations OBJECT_DECLS;

/// Create the String class
///
/// \param context The declaration context for creating the class
/// \param super_proto The prototype object for the superclass
/// \return A SystemClass containing the created String class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// String constructor
///
/// Creates a String object from the provided value.
///
/// \param activation The current activation context
/// \param this_obj The String object being constructed
/// \param args Arguments for the constructor (value to convert to string)
/// \param arg_count Number of arguments
/// \return Value::Undefined (populates this with the string value)
template<typename GC>
Value<GC> constructor(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// String function
///
/// Converts the provided value to a string and returns it.
///
/// \param activation The current activation context
/// \param this_obj The this object (unused for function call)
/// \param args Arguments (value to convert to string)
/// \param arg_count Number of arguments
/// \return The converted string value
template<typename GC>
Value<GC> function(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements String.prototype.charAt
///
/// Returns the character at the specified index.
///
/// \param activation The current activation context
/// \param this_obj The String object
/// \param args Arguments (index)
/// \param arg_count Number of arguments
/// \return The character at the specified index, or empty string if out of bounds
template<typename GC>
Value<GC> char_at(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements String.prototype.charCodeAt
///
/// Returns the character code at the specified index.
///
/// \param activation The current activation context
/// \param this_obj The String object
/// \param args Arguments (index)
/// \param arg_count Number of arguments
/// \return The character code at the specified index, or NaN if out of bounds
template<typename GC>
Value<GC> char_code_at(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements String.prototype.concat
///
/// Concatenates the provided strings to this string.
///
/// \param activation The current activation context
/// \param this_obj The String object
/// \param args Arguments (strings to concatenate)
/// \param arg_count Number of arguments
/// \return The concatenated string
template<typename GC>
Value<GC> concat(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements String.fromCharCode
///
/// Creates a string from the provided character codes.
///
/// \param activation The current activation context
/// \param this_obj The String constructor (unused)
/// \param args Arguments (character codes)
/// \param arg_count Number of arguments
/// \return The created string
template<typename GC>
Value<GC> from_char_code(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements String.prototype.indexOf
///
/// Returns the index of the first occurrence of the specified substring.
///
/// \param activation The current activation context
/// \param this_obj The String object
/// \param args Arguments (search string, optional start index)
/// \param arg_count Number of arguments
/// \return The index of the first occurrence, or -1 if not found
template<typename GC>
Value<GC> index_of(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements String.prototype.lastIndexOf
///
/// Returns the index of the last occurrence of the specified substring.
///
/// \param activation The current activation context
/// \param this_obj The String object
/// \param args Arguments (search string, optional start index)
/// \param arg_count Number of arguments
/// \return The index of the last occurrence, or -1 if not found
template<typename GC>
Value<GC> last_index_of(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements String.prototype.slice
///
/// Returns a substring from the specified start to end index.
///
/// \param activation The current activation context
/// \param this_obj The String object
/// \param args Arguments (start index, optional end index)
/// \param arg_count Number of arguments
/// \return The sliced substring
template<typename GC>
Value<GC> slice(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements String.prototype.split
///
/// Splits the string by the specified delimiter.
///
/// \param activation The current activation context
/// \param this_obj The String object
/// \param args Arguments (delimiter, optional limit)
/// \param arg_count Number of arguments
/// \return Array of substrings
template<typename GC>
Value<GC> split(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements String.prototype.substr
///
/// Returns a substring starting at the specified index with the specified length.
///
/// \param activation The current activation context
/// \param this_obj The String object
/// \param args Arguments (start index, optional length)
/// \param arg_count Number of arguments
/// \return The substring
template<typename GC>
Value<GC> substr(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements String.prototype.substring
///
/// Returns a substring between the specified start and end indices.
///
/// \param activation The current activation context
/// \param this_obj The String object
/// \param args Arguments (start index, optional end index)
/// \param arg_count Number of arguments
/// \return The substring
template<typename GC>
Value<GC> substring(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements String.prototype.toUpperCase
///
/// Returns the string converted to uppercase.
///
/// \param activation The current activation context
/// \param this_obj The String object
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return The uppercase string
template<typename GC>
Value<GC> to_upper_case(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements String.prototype.toLowerCase
///
/// Returns the string converted to lowercase.
///
/// \param activation The current activation context
/// \param this_obj The String object
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return The lowercase string
template<typename GC>
Value<GC> to_lower_case(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// String.toString / String.valueOf impl
///
/// Returns the string value of the String object.
///
/// \param activation The current activation context
/// \param this_obj The String object
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return The string value, or Value::Undefined if not a String object
template<typename GC>
Value<GC> to_string_value_of(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Normalizes an index parameter used in String functions such as substring.
/// The returned index will be within the range of [0, len].
///
/// \param i The index value
/// \param len The length of the string
/// \return The normalized index
size_t string_index(int32_t i, size_t len);

/// Normalizes a wrapping index parameter used in String functions such as slice.
/// Negative values will count backwards from len.
/// The returned index will be within the range of [0, len].
///
/// \param i The index value
/// \param len The length of the string
/// \return The normalized wrapping index
size_t string_wrapping_index(int32_t i, size_t len);

} // namespace string_class
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_STRING_H
