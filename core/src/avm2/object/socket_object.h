// C++ translation of core/src/avm2/object/socket_object.rs
// Socket object representation for AVM2

#ifndef RUFFLE_CORE_AVM2_OBJECT_SOCKET_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_SOCKET_OBJECT_H

#include <cstdint>
#include <cstddef>
#include <deque>
#include <vector>
#include <optional>
#include <memory>
#include <array>
#include <fmt/format.h>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC>
    class Activation;
    template<typename GC>
    class ScriptObjectData;
    template<typename GC>
    class Gc;
    template<typename GC>
    class GcWeak;
    template<typename GC>
    class Class;
    namespace avm2 {
        template<typename GC>
        class Object;
        template<typename GC>
        class TObject;
        template<typename GC>
        class Error;
        namespace bytearray {
            enum class Endian;
            enum class ObjectEncoding;
            class ByteArrayError;
        }
    }
}
namespace socket {
    class SocketHandle;
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace object {

/// Weak reference to SocketObjectData
template<typename GC>
struct SocketObjectWeak {
    GcWeak<GC, SocketObjectData<GC>> weak;

    explicit SocketObjectWeak(GcWeak<GC, SocketObjectData<GC>> w)
        : weak(w) {}
};

/// Data structure for Socket objects
template<typename GC>
struct SocketObjectData {
    /// Base script object
    ScriptObjectData<GC> base;

    /// Socket handle (optional)
    std::optional<ruffle::socket::SocketHandle> handle;

    /// Byte order (default: Big endian)
    bytearray::Endian endian;

    /// Object encoding (default: AMF3)
    bytearray::ObjectEncoding object_encoding;

    /// Socket connection timeout in milliseconds
    uint32_t timeout;

    /// Read buffer
    std::deque<uint8_t> read_buffer;

    /// Write buffer
    std::vector<uint8_t> write_buffer;
};

/// Socket object for AVM2 socket operations
template<typename GC>
class SocketObject {
private:
    Gc<GC, SocketObjectData<GC>> data_;

public:
    /// Get the endianness
    bytearray::Endian endian() const {
        return data_->endian;
    }

    /// Set the endianness
    void set_endian(bytearray::Endian endian) {
        data_->endian = endian;
    }

    /// Get the object encoding
    bytearray::ObjectEncoding object_encoding() const {
        return data_->object_encoding;
    }

    /// Set the object encoding
    void set_object_encoding(bytearray::ObjectEncoding object_encoding) {
        data_->object_encoding = object_encoding;
    }

    /// Get the timeout in milliseconds
    uint32_t timeout() const {
        return data_->timeout;
    }

    /// Set the timeout in milliseconds (minimum 250ms)
    void set_timeout(uint32_t timeout) {
        // When a timeout of smaller than 250 milliseconds is provided,
        // we clamp it to 250 milliseconds.
        data_->timeout = std::max(uint32_t(250), timeout);
    }

    /// Get the socket handle
    std::optional<ruffle::socket::SocketHandle> handle() const {
        return data_->handle;
    }

    /// Set the socket handle, returns previous handle if any
    std::optional<ruffle::socket::SocketHandle> set_handle(
        ruffle::socket::SocketHandle handle
    ) {
        auto old = data_->handle;
        data_->handle = handle;
        return old;
    }

    /// Get mutable reference to read buffer
    std::deque<uint8_t>& read_buffer() {
        return data_->read_buffer;
    }

    /// Get mutable reference to write buffer
    std::vector<uint8_t>& write_buffer() {
        return data_->write_buffer;
    }

    /// Read bytes from the buffer
    /// @param amount Number of bytes to read
    /// @return Vector of bytes read
    /// @throws ByteArrayError if not enough bytes available
    std::vector<uint8_t> read_bytes(size_t amount) {
        auto& buf = data_->read_buffer;
        if (amount > buf.size()) {
            throw bytearray::ByteArrayError::EndOfFile;
        }

        std::vector<uint8_t> result(buf.begin(), buf.begin() + amount);
        buf.erase(buf.begin(), buf.begin() + amount);
        return result;
    }

    /// Write bytes to the buffer
    void write_bytes(const std::vector<uint8_t>& bytes) {
        data_->write_buffer.insert(
            data_->write_buffer.end(),
            bytes.begin(),
            bytes.end()
        );
    }

    /// Read a boolean value
    bool read_boolean() {
        auto bytes = read_bytes(1);
        return bytes[0] != 0;
    }

    /// Write a boolean value
    void write_boolean(bool val) {
        write_bytes({static_cast<uint8_t>(val ? 1 : 0)});
    }

    /// Read UTF bytes - cuts at first null byte and strips optional UTF8 BOM
    /// @param amount Number of bytes to read
    /// @return Vector of bytes (without BOM and truncated at null)
    std::vector<uint8_t> read_utf_bytes(size_t amount) {
        auto bytes = read_bytes(amount);

        // Strip UTF8 BOM if present
        const std::array<uint8_t, 3> bom = {0xEF, 0xBB, 0xBF};
        if (bytes.size() >= 3 &&
            bytes[0] == bom[0] && bytes[1] == bom[1] && bytes[2] == bom[2]) {
            bytes.erase(bytes.begin(), bytes.begin() + 3);
        }

        // Cut at first null byte
        auto null_it = std::find(bytes.begin(), bytes.end(), 0);
        if (null_it != bytes.end()) {
            bytes.erase(null_it, bytes.end());
        }

        return bytes;
    }

    /// Read a UTF string (length-prefixed)
    std::vector<uint8_t> read_utf() {
        uint16_t len = read_unsigned_short();
        return read_utf_bytes(len);
    }

    /// Write a UTF string with length prefix
    /// @param activation Current activation (for error handling)
    /// @param utf_string String to write
    /// @throws Error if string is too long
    void write_utf(Activation<GC>* activation, const std::string& utf_string) {
        if (utf_string.size() <= std::numeric_limits<uint16_t>::max()) {
            write_unsigned_short(static_cast<uint16_t>(utf_string.size()));
            write_bytes(std::vector<uint8_t>(utf_string.begin(), utf_string.end()));
        } else {
            throw make_error_2006(activation);
        }
    }

    // Write methods for different data types
    void write_float(float val) {
        auto bytes = to_bytes(val, data_->endian);
        write_bytes(bytes);
    }

    void write_double(double val) {
        auto bytes = to_bytes(val, data_->endian);
        write_bytes(bytes);
    }

    void write_int(int32_t val) {
        auto bytes = to_bytes(val, data_->endian);
        write_bytes(bytes);
    }

    void write_unsigned_int(uint32_t val) {
        auto bytes = to_bytes(val, data_->endian);
        write_bytes(bytes);
    }

    void write_short(int16_t val) {
        auto bytes = to_bytes(val, data_->endian);
        write_bytes(bytes);
    }

    void write_unsigned_short(uint16_t val) {
        auto bytes = to_bytes(val, data_->endian);
        write_bytes(bytes);
    }

    // Read methods for different data types
    float read_float() {
        auto bytes = read_bytes(4);
        return from_bytes<float>(bytes, data_->endian);
    }

    double read_double() {
        auto bytes = read_bytes(8);
        return from_bytes<double>(bytes, data_->endian);
    }

    int32_t read_int() {
        auto bytes = read_bytes(4);
        return from_bytes<int32_t>(bytes, data_->endian);
    }

    uint32_t read_unsigned_int() {
        auto bytes = read_bytes(4);
        return from_bytes<uint32_t>(bytes, data_->endian);
    }

    int16_t read_short() {
        auto bytes = read_bytes(2);
        return from_bytes<int16_t>(bytes, data_->endian);
    }

    uint16_t read_unsigned_short() {
        auto bytes = read_bytes(2);
        return from_bytes<uint16_t>(bytes, data_->endian);
    }

    int8_t read_byte() {
        auto bytes = read_bytes(1);
        return static_cast<int8_t>(bytes[0]);
    }

    uint8_t read_unsigned_byte() {
        auto bytes = read_bytes(1);
        return bytes[0];
    }

    /// Get the underlying GC data
    Gc<GC, SocketObjectData<GC>> gc_data() const {
        return data_;
    }

    /// Get weak reference
    SocketObjectWeak<GC> weak() const {
        return SocketObjectWeak<GC>(data_.weak());
    }

private:
    template<typename T>
    static std::vector<uint8_t> to_bytes(T val, bytearray::Endian endian) {
        std::vector<uint8_t> bytes(sizeof(T));
        if (endian == bytearray::Endian::Big) {
            for (size_t i = 0; i < sizeof(T); ++i) {
                bytes[i] = (val >> (8 * (sizeof(T) - 1 - i))) & 0xFF;
            }
        } else {
            for (size_t i = 0; i < sizeof(T); ++i) {
                bytes[i] = (val >> (8 * i)) & 0xFF;
            }
        }
        return bytes;
    }

    template<typename T>
    static T from_bytes(const std::vector<uint8_t>& bytes, bytearray::Endian endian) {
        T val = 0;
        if (endian == bytearray::Endian::Big) {
            for (size_t i = 0; i < sizeof(T); ++i) {
                val |= static_cast<T>(bytes[i]) << (8 * (sizeof(T) - 1 - i));
            }
        } else {
            for (size_t i = 0; i < sizeof(T); ++i) {
                val |= static_cast<T>(bytes[i]) << (8 * i);
            }
        }
        return val;
    }
};

/// Debug formatting for SocketObject
template<typename GC>
struct DebugFormatter<SocketObject<GC>> {
    static std::string format(const SocketObject<GC>&) {
        return "SocketObject";
    }
};

} // namespace object
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_OBJECT_SOCKET_OBJECT_H
