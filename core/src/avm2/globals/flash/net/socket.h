// C++ translation of core/src/avm2/globals/flash/net/socket.rs
// flash.net.Socket native methods

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_SOCKET_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_SOCKET_H

#include <cstdint>
#include <cstddef>
#include <vector>
#include <string>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename GC>
    class Activation;
    template<typename GC>
    class Value;
    template<typename GC>
    class Error;
    template<typename GC>
    class Object;
    template<typename GC>
    class AvmString;
    namespace bytearray {
        enum class Endian;
        enum class ObjectEncoding;
    }
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash_net_socket {

/// Connect the socket to a host and port.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args Arguments: [host: string, port: u32]
/// @return Undefined
template<typename GC>
Value<GC> connect(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Get the socket timeout in milliseconds.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args No arguments
/// @return The timeout value
template<typename GC>
Value<GC> get_timeout(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Set the socket timeout in milliseconds.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args Arguments: [timeout: u32]
/// @return Undefined
template<typename GC>
Value<GC> set_timeout(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Close the socket connection.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args No arguments
/// @return Undefined
template<typename GC>
Value<GC> close(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Get the number of bytes available to read.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args No arguments
/// @return The number of bytes available
template<typename GC>
Value<GC> get_bytes_available(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Get the socket endianness.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args No arguments
/// @return "bigEndian" or "littleEndian"
template<typename GC>
Value<GC> get_endian(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Set the socket endianness.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args Arguments: [endian: string ("bigEndian" or "littleEndian")]
/// @return Undefined
template<typename GC>
Value<GC> set_endian(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Check if the socket is connected.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args No arguments
/// @return true if connected, false otherwise
template<typename GC>
Value<GC> get_connected(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Get the object encoding (AMF version).
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args No arguments
/// @return 0 for AMF0, 3 for AMF3
template<typename GC>
Value<GC> get_object_encoding(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Set the object encoding (AMF version).
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args Arguments: [encoding: u32 (0 for AMF0, 3 for AMF3)]
/// @return Undefined
template<typename GC>
Value<GC> set_object_encoding(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Flush the write buffer (send data).
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args No arguments
/// @return Undefined
template<typename GC>
Value<GC> flush(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Read a boolean value from the socket.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args No arguments
/// @return The boolean value
template<typename GC>
Value<GC> read_boolean(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Read a signed byte from the socket.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args No arguments
/// @return The byte value
template<typename GC>
Value<GC> read_byte(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Read bytes from the socket into a ByteArray.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args Arguments: [bytes: ByteArray, offset: u32, length: u32]
/// @return Undefined
template<typename GC>
Value<GC> read_bytes(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Read a double-precision floating-point value from the socket.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args No arguments
/// @return The double value
template<typename GC>
Value<GC> read_double(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Read a single-precision floating-point value from the socket.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args No arguments
/// @return The float value
template<typename GC>
Value<GC> read_float(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Read a signed 32-bit integer from the socket.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args No arguments
/// @return The integer value
template<typename GC>
Value<GC> read_int(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Read a multi-byte string from the socket.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args Arguments: [length: u32, charset: string]
/// @return The decoded string
template<typename GC>
Value<GC> read_multi_byte(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Read an AMF-encoded object from the socket.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args No arguments
/// @return The deserialized object
template<typename GC>
Value<GC> read_object(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Read a signed 16-bit integer from the socket.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args No arguments
/// @return The short value
template<typename GC>
Value<GC> read_short(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Read an unsigned byte from the socket.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args No arguments
/// @return The unsigned byte value
template<typename GC>
Value<GC> read_unsigned_byte(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Read an unsigned 32-bit integer from the socket.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args No arguments
/// @return The unsigned integer value
template<typename GC>
Value<GC> read_unsigned_int(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Read an unsigned 16-bit integer from the socket.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args No arguments
/// @return The unsigned short value
template<typename GC>
Value<GC> read_unsigned_short(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Read a UTF-8 string from the socket.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args No arguments
/// @return The UTF-8 string
template<typename GC>
Value<GC> read_utf(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Read a UTF-8 string of specified length from the socket.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args Arguments: [length: u32]
/// @return The UTF-8 string
template<typename GC>
Value<GC> read_utf_bytes(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Write a boolean value to the socket.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args Arguments: [value: bool]
/// @return Undefined
template<typename GC>
Value<GC> write_boolean(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Write a byte to the socket.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args Arguments: [value: u32]
/// @return Undefined
template<typename GC>
Value<GC> write_byte(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Write bytes from a ByteArray to the socket.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args Arguments: [bytes: ByteArray, offset: u32, length: u32]
/// @return Undefined
template<typename GC>
Value<GC> write_bytes(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Write a double-precision floating-point value to the socket.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args Arguments: [value: Number]
/// @return Undefined
template<typename GC>
Value<GC> write_double(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Write a single-precision floating-point value to the socket.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args Arguments: [value: Number]
/// @return Undefined
template<typename GC>
Value<GC> write_float(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Write a signed 32-bit integer to the socket.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args Arguments: [value: i32]
/// @return Undefined
template<typename GC>
Value<GC> write_int(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Write a multi-byte string to the socket.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args Arguments: [value: string, charset: string]
/// @return Undefined
template<typename GC>
Value<GC> write_multi_byte(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Write an AMF-encoded object to the socket.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args Arguments: [object: *]
/// @return Undefined
template<typename GC>
Value<GC> write_object(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Write a signed 16-bit integer to the socket.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args Arguments: [value: i32]
/// @return Undefined
template<typename GC>
Value<GC> write_short(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Write an unsigned 32-bit integer to the socket.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args Arguments: [value: u32]
/// @return Undefined
template<typename GC>
Value<GC> write_unsigned_int(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Write a UTF-8 string to the socket.
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args Arguments: [value: string]
/// @return Undefined
template<typename GC>
Value<GC> write_utf(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Write a UTF-8 string to the socket (bytes only).
///
/// @param activation The current activation
/// @param this The Socket object
/// @param args Arguments: [value: string]
/// @return Undefined
template<typename GC>
Value<GC> write_utf_bytes(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

} // namespace flash_net_socket
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_SOCKET_H
