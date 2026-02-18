// C++ translation of core/src/avm2/globals/flash/utils/byte_array.rs
// ByteArray implementation for AVM2

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_UTILS_BYTE_ARRAY_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_UTILS_BYTE_ARRAY_H

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

// Forward declarations
namespace ruffle {
namespace core {
namespace string {
    class AvmString;
}
namespace avm2 {
    template<typename GC>
    class Activation;
    template<typename GC>
    class Value;
    template<typename GC>
    class Error;
    namespace bytearray {
        enum class Endian;
        enum class ObjectEncoding;
    }
}
}
}

namespace encoding_rs {
    class Encoding;
    extern const Encoding* UTF_8;
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace utils {
namespace byte_array {

/// Get the default object encoding for ByteArray
template<typename GC>
Value<GC> get_default_object_encoding(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Set the default object encoding for ByteArray
template<typename GC>
Value<GC> set_default_object_encoding(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Writes a single byte to the bytearray
template<typename GC>
Value<GC> write_byte(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Writes multiple bytes to the bytearray from another bytearray
template<typename GC>
Value<GC> write_bytes(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Reads bytes from the current bytearray into another bytearray
template<typename GC>
Value<GC> read_bytes(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Write a UTF-8 string to the bytearray
template<typename GC>
Value<GC> write_utf(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Read a UTF-8 string from the bytearray
template<typename GC>
Value<GC> read_utf(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Strip BOM from bytes and return as AvmString
template<typename GC>
string::AvmString strip_bom(
    Activation<GC>* activation,
    const std::vector<uint8_t>& bytes
);

/// Convert bytearray to string
template<typename GC>
Value<GC> to_string(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Clear the bytearray
template<typename GC>
Value<GC> clear(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Get the current position
template<typename GC>
Value<GC> get_position(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Set the current position
template<typename GC>
Value<GC> set_position(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Get bytes available for reading
template<typename GC>
Value<GC> get_bytes_available(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Get the length of the bytearray
template<typename GC>
Value<GC> get_length(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Set the length of the bytearray
template<typename GC>
Value<GC> set_length(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Get the endianness
template<typename GC>
Value<GC> get_endian(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Set the endianness
template<typename GC>
Value<GC> set_endian(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Read a short (16-bit signed integer)
template<typename GC>
Value<GC> read_short(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Read an unsigned short (16-bit unsigned integer)
template<typename GC>
Value<GC> read_unsigned_short(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Read a double (64-bit floating point)
template<typename GC>
Value<GC> read_double(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Read a float (32-bit floating point)
template<typename GC>
Value<GC> read_float(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Read an int (32-bit signed integer)
template<typename GC>
Value<GC> read_int(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Read an unsigned int (32-bit unsigned integer)
template<typename GC>
Value<GC> read_unsigned_int(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Read a boolean
template<typename GC>
Value<GC> read_boolean(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Read a byte (8-bit signed integer)
template<typename GC>
Value<GC> read_byte(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Read UTF-8 bytes as a string
template<typename GC>
Value<GC> read_utf_bytes(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Read an unsigned byte (8-bit unsigned integer)
template<typename GC>
Value<GC> read_unsigned_byte(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Write a float (32-bit floating point)
template<typename GC>
Value<GC> write_float(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Write a double (64-bit floating point)
template<typename GC>
Value<GC> write_double(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Write a boolean
template<typename GC>
Value<GC> write_boolean(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Write an int (32-bit signed integer)
template<typename GC>
Value<GC> write_int(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Write an unsigned int (32-bit unsigned integer)
template<typename GC>
Value<GC> write_unsigned_int(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Write a short (16-bit signed integer)
template<typename GC>
Value<GC> write_short(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Write a multi-byte string with specified charset
template<typename GC>
Value<GC> write_multi_byte(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Read a multi-byte string with specified charset
template<typename GC>
Value<GC> read_multi_byte(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Write UTF-8 bytes
template<typename GC>
Value<GC> write_utf_bytes(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Compress the bytearray
template<typename GC>
Value<GC> compress(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Uncompress the bytearray
template<typename GC>
Value<GC> uncompress(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Read an AMF object
template<typename GC>
Value<GC> read_object(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Write an AMF object
template<typename GC>
Value<GC> write_object(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Get the object encoding
template<typename GC>
Value<GC> get_object_encoding(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Set the object encoding
template<typename GC>
Value<GC> set_object_encoding(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace byte_array
} // namespace utils
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_UTILS_BYTE_ARRAY_H
