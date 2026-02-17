// C++ translation of core/src/avm2/bytearray.rs
//! ByteArray storage and operations

#ifndef RUFFLE_CORE_AVM2_BYTEARRAY_H
#define RUFFLE_CORE_AVM2_BYTEARRAY_H

#include <cstdint>
#include <cstddef>
#include <vector>
#include <cell>
#include <optional>
#include <variant>
#include <string>
#include <cstring>
#include <algorithm>
#include <limits>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC>
    class AvmString;
    template<typename GC>
    class Activation;
    template<typename GC>
    class Error;
    template<typename GC>
    class ByteArrayObject;
    class UpdateContext;

    class WStr;

    template<typename T>
    struct FromWStr;

    namespace gc_arena {
        template<typename T>
        class Gc;
        class Mutation;
    }
}
}

namespace flate2 {
    enum class Compression {
        none,
        fast,
        best,
        default_compression
    };

    namespace read {
        template<typename T>
        class ZlibDecoder;
        template<typename T>
        class DeflateDecoder;
    }

    template<typename T>
    class ZlibEncoder;
    template<typename T>
    class DeflateEncoder;
}

#ifdef LZMA_FEATURE
namespace lzma_rs {
    // LZMA compression/decompression functions
}
#endif

namespace ruffle {
namespace core {
namespace avm2 {

using gc_arena::Gc;
using gc_arena::Mutation;

/// Endianness for ByteArray read/write operations
enum class Endian {
    Big,
    Little
};

/// Compression algorithms supported by ByteArray
enum class CompressionAlgorithm {
    Zlib,
    Deflate,
    Lzma
};

/// ByteArray-specific errors
enum class ByteArrayError {
    EndOfFile,
    IndexOutOfBounds
};

/// Object encoding for AMF serialization
enum class ObjectEncoding {
    Amf0 = 0,
    Amf3 = 3
};

/// Convert ByteArrayError to AVM Error
template<typename GC>
Error<GC> to_avm_error(ByteArrayError err, Activation<GC>* activation) {
    switch (err) {
        case ByteArrayError::EndOfFile:
            return make_error_2030(activation);
        case ByteArrayError::IndexOutOfBounds:
            return make_error_2006(activation);
    }
}

/// Storage backend for ByteArray
class ByteArrayStorage {
private:
    /// Underlying ByteArray
    std::vector<uint8_t> bytes_;

    /// The current position to read/write from
    size_t position_;

    /// This represents what endian to use while reading/writing data.
    Endian endian_;

    /// The encoding used when serializing/deserializing using readObject/writeObject
    ObjectEncoding object_encoding_;

public:
    /// Create a new ByteArrayStorage
    explicit ByteArrayStorage(UpdateContext* context)
        : position_(0)
        , endian_(Endian::Big)
        , object_encoding_(context->avm2.default_bytearray_encoding)
    {}

    /// Create a new ByteArrayStorage using an already existing vector
    ByteArrayStorage(UpdateContext* context, std::vector<uint8_t> bytes)
        : bytes_(std::move(bytes))
        , position_(0)
        , endian_(Endian::Big)
        , object_encoding_(context->avm2.default_bytearray_encoding)
    {}

    /// Write bytes at the next position in the ByteArray, growing if needed.
    std::optional<ByteArrayError> write_bytes(const std::vector<uint8_t>& buf) {
        auto result = write_at(buf, position_);
        if (!result.has_value()) {
            position_ += buf.size();
        }
        return result;
    }

    /// Write bytes within the ByteArray at the current position
    std::optional<ByteArrayError> write_bytes_within(size_t start, size_t amount) {
        auto result = write_at_within(start, amount, position_);
        if (!result.has_value()) {
            position_ += amount;
        }
        return result;
    }

    /// Reads any amount of bytes from the current position in the ByteArray
    std::variant<const uint8_t*, ByteArrayError> read_bytes(size_t amount) {
        auto result = read_at(amount, position_);
        if (auto* data = std::get_if<const uint8_t*>(&result)) {
            position_ += amount;
        }
        return result;
    }

    /// Same as `read_bytes`, but:
    /// - cuts the result at the first null byte to recreate a bug in FP
    /// - strips off an optional UTF8 BOM at the beginning
    std::variant<const uint8_t*, ByteArrayError> read_utf_bytes(size_t amount) {
        auto result = read_bytes(amount);
        if (auto* data = std::get_if<const uint8_t*>(&result)) {
            // Strip UTF8 BOM if present
            if (amount >= 3 && (*data)[0] == 0xEF && (*data)[1] == 0xBB && (*data)[2] == 0xBF) {
                *data += 3;
            }
            // Cut at first null byte
            for (size_t i = 0; i < amount; ++i) {
                if ((*data)[i] == 0) {
                    return *data;
                }
            }
        }
        return result;
    }

    /// Reads any amount of bytes at any offset in the ByteArray
    std::variant<const uint8_t*, ByteArrayError> read_at(size_t amount, size_t offset) const {
        if (offset + amount > bytes_.size()) {
            return ByteArrayError::EndOfFile;
        }
        return &bytes_[offset];
    }

    /// Write bytes at any offset in the ByteArray
    /// Will automatically grow the ByteArray to fit the new buffer
    std::optional<ByteArrayError> write_at(const std::vector<uint8_t>& buf, size_t offset) {
        if (offset + buf.size() > std::numeric_limits<uint32_t>::max()) {
            return ByteArrayError::IndexOutOfBounds;
        }

        size_t new_len = offset + buf.size();
        if (bytes_.size() < new_len) {
            set_length(new_len);
        }
        std::memcpy(&bytes_[offset], buf.data(), buf.size());
        return std::nullopt;
    }

    /// Write bytes at any offset in the ByteArray
    /// Will return an error if the new buffer does not fit the ByteArray
    std::optional<ByteArrayError> write_at_nongrowing(const std::vector<uint8_t>& buf, size_t offset) {
        if (offset + buf.size() > bytes_.size()) {
            return ByteArrayError::IndexOutOfBounds;
        }
        std::memcpy(&bytes_[offset], buf.data(), buf.size());
        return std::nullopt;
    }

    /// Write bytes at any offset in the ByteArray from within the current ByteArray using a memmove.
    /// Will automatically grow the ByteArray to fit the new buffer
    std::optional<ByteArrayError> write_at_within(size_t start, size_t amount, size_t offset) {
        // First verify that reading from `start` to `amount` is valid
        size_t end = start + amount;
        if (end > bytes_.size()) {
            return ByteArrayError::EndOfFile;
        }

        // Second we resize our underlying buffer to ensure that writing `amnt` from `offset` is valid.
        if (offset + amount > std::numeric_limits<uint32_t>::max()) {
            return ByteArrayError::IndexOutOfBounds;
        }

        size_t new_len = offset + amount;
        if (bytes_.size() < new_len) {
            set_length(new_len);
        }

        // Use memmove for overlapping regions
        std::memmove(&bytes_[offset], &bytes_[start], amount);
        return std::nullopt;
    }

    /// Compress the ByteArray into a temporary buffer.
    std::vector<uint8_t> compress(CompressionAlgorithm algorithm) {
        std::vector<uint8_t> buffer;

        switch (algorithm) {
            case CompressionAlgorithm::Zlib: {
                // Note: some content is sensitive to compression type (as it's visible in the header)
                auto encoder = flate2::ZlibEncoder(&bytes_, flate2::Compression::best);
                // encoder.read_to_end(&mut buffer)
                break;
            }
            case CompressionAlgorithm::Deflate: {
                auto encoder = flate2::DeflateEncoder(&bytes_, flate2::Compression::best);
                // encoder.read_to_end(&mut buffer)
                break;
            }
#ifdef LZMA_FEATURE
            case CompressionAlgorithm::Lzma: {
                // lzma_rs::lzma_compress(&mut &*self.bytes, &mut buffer)
                break;
            }
#endif
        }

        return buffer;
    }

    /// Decompress the ByteArray into a temporary buffer.
    std::optional<std::vector<uint8_t>> decompress(CompressionAlgorithm algorithm) {
        std::vector<uint8_t> buffer;

        switch (algorithm) {
            case CompressionAlgorithm::Zlib: {
                auto decoder = flate2::read::ZlibDecoder(&bytes_);
                // decoder.read_to_end(&mut buffer)
                break;
            }
            case CompressionAlgorithm::Deflate: {
                auto decoder = flate2::read::DeflateDecoder(&bytes_);
                // decoder.read_to_end(&mut buffer)
                break;
            }
#ifdef LZMA_FEATURE
            case CompressionAlgorithm::Lzma: {
                // lzma_rs::lzma_decompress(&mut &*self.bytes, &mut buffer)
                break;
            }
#endif
        }

        if (buffer.empty()) {
            return std::nullopt;
        }
        return buffer;
    }

    /// Read a UTF string (length-prefixed)
    std::variant<const uint8_t*, ByteArrayError> read_utf() {
        auto len = read_unsigned_short();
        if (auto* error = std::get_if<ByteArrayError>(&len)) {
            return *error;
        }
        return read_utf_bytes(std::get<uint16_t>(len));
    }

    /// Write a boolean
    std::optional<ByteArrayError> write_boolean(bool val) {
        return write_bytes({val ? 1 : 0});
    }

    /// Read a boolean
    std::variant<bool, ByteArrayError> read_boolean() {
        auto result = read_bytes(1);
        if (auto* error = std::get_if<ByteArrayError>(&result)) {
            return *error;
        }
        auto* data = std::get<const uint8_t*>(result);
        return data[0] != 0;
    }

    /// Write a UTF String into the buffer, with its length as a prefix
    std::optional<ByteArrayError> write_utf(const std::string& utf_string) {
        if (utf_string.size() > std::numeric_limits<uint16_t>::max()) {
            return ByteArrayError::IndexOutOfBounds;
        }
        write_unsigned_short(static_cast<uint16_t>(utf_string.size()));
        return write_bytes(std::vector<uint8_t>(utf_string.begin(), utf_string.end()));
    }

    /// Clear the ByteArray
    void clear() {
        bytes_.clear();
        position_ = 0;
    }

    /// Shrink to fit
    void shrink_to_fit() {
        bytes_.shrink_to_fit();
    }

    /// Set the length of the ByteArray
    void set_length(size_t new_len) {
        bytes_.resize(new_len, 0);
        position_ = std::min(position_, new_len);
    }

    /// Get a byte at the specified position
    std::optional<uint8_t> get(size_t pos) const {
        if (pos >= bytes_.size()) {
            return std::nullopt;
        }
        return bytes_[pos];
    }

    /// Set a byte at the specified position
    void set(size_t item, uint8_t value) {
        if (bytes_.size() < item + 1) {
            bytes_.resize(item + 1, 0);
        }
        bytes_[item] = value;
    }

    /// Swap all data stored in this bytearray with the passed `Vec<u8>`.
    /// This method sets the bytearray's `position` to 0.
    void swap_storage_with(std::vector<uint8_t>& new_data) {
        position_ = 0;
        std::swap(bytes_, new_data);
    }

    /// Write a single byte at any offset in the bytearray, panicking if out of bounds.
    void set_nongrowing(size_t item, uint8_t value) {
        bytes_[item] = value;
    }

    /// Delete (zero) a byte at the specified position
    void delete_at(size_t item) {
        if (item < bytes_.size()) {
            bytes_[item] = 0;
        }
    }

    /// Get the underlying bytes
    const std::vector<uint8_t>& bytes() const {
        return bytes_;
    }

    /// Get mutable reference to underlying bytes
    std::vector<uint8_t>& bytes_mut() {
        return bytes_;
    }

    /// Get the length of the ByteArray
    size_t len() const {
        return bytes_.size();
    }

    /// Get the current position
    size_t position() const {
        return position_;
    }

    /// Set the current position
    void set_position(size_t pos) {
        position_ = pos;
    }

    /// Get the endianness
    Endian endian() const {
        return endian_;
    }

    /// Set the endianness
    void set_endian(Endian new_endian) {
        endian_ = new_endian;
    }

    /// Get the object encoding
    ObjectEncoding object_encoding() const {
        return object_encoding_;
    }

    /// Set the object encoding
    void set_object_encoding(ObjectEncoding new_object_encoding) {
        object_encoding_ = new_object_encoding;
    }

    /// Get the number of bytes available for reading
    size_t bytes_available() const {
        if (position_ >= bytes_.size()) {
            return 0;
        }
        return bytes_.size() - position_;
    }

    // ========================================================================
    // Type-specific read/write methods
    // ========================================================================

    /// Write a float (4 bytes)
    std::optional<ByteArrayError> write_float(float val) {
        return write_endian_bytes(reinterpret_cast<const uint8_t*>(&val), 4);
    }

    /// Write a double (8 bytes)
    std::optional<ByteArrayError> write_double(double val) {
        return write_endian_bytes(reinterpret_cast<const uint8_t*>(&val), 8);
    }

    /// Write a signed 32-bit integer
    std::optional<ByteArrayError> write_int(int32_t val) {
        return write_endian_bytes(reinterpret_cast<const uint8_t*>(&val), 4);
    }

    /// Write an unsigned 32-bit integer
    std::optional<ByteArrayError> write_unsigned_int(uint32_t val) {
        return write_endian_bytes(reinterpret_cast<const uint8_t*>(&val), 4);
    }

    /// Write a signed 16-bit integer
    std::optional<ByteArrayError> write_short(int16_t val) {
        return write_endian_bytes(reinterpret_cast<const uint8_t*>(&val), 2);
    }

    /// Write an unsigned 16-bit integer
    std::optional<ByteArrayError> write_unsigned_short(uint16_t val) {
        return write_endian_bytes(reinterpret_cast<const uint8_t*>(&val), 2);
    }

    /// Write a signed 8-bit integer
    std::optional<ByteArrayError> write_byte(int8_t val) {
        return write_bytes({static_cast<uint8_t>(val)});
    }

    /// Write an unsigned 8-bit integer
    std::optional<ByteArrayError> write_unsigned_byte(uint8_t val) {
        return write_bytes({val});
    }

    /// Read a float (4 bytes)
    std::variant<float, ByteArrayError> read_float() {
        return read_endian_float<float>(4);
    }

    /// Read a double (8 bytes)
    std::variant<double, ByteArrayError> read_double() {
        return read_endian_float<double>(8);
    }

    /// Read a signed 32-bit integer
    std::variant<int32_t, ByteArrayError> read_int() {
        return read_endian_int<int32_t>(4);
    }

    /// Read an unsigned 32-bit integer
    std::variant<uint32_t, ByteArrayError> read_unsigned_int() {
        return read_endian_int<uint32_t>(4);
    }

    /// Read a signed 16-bit integer
    std::variant<int16_t, ByteArrayError> read_short() {
        return read_endian_int<int16_t>(2);
    }

    /// Read an unsigned 16-bit integer
    std::variant<uint16_t, ByteArrayError> read_unsigned_short() {
        return read_endian_int<uint16_t>(2);
    }

    /// Read a signed 8-bit integer
    std::variant<int8_t, ByteArrayError> read_byte() {
        auto result = read_bytes(1);
        if (auto* error = std::get_if<ByteArrayError>(&result)) {
            return *error;
        }
        return static_cast<int8_t>(*std::get<const uint8_t*>(result));
    }

    /// Read an unsigned 8-bit integer
    std::variant<uint8_t, ByteArrayError> read_unsigned_byte() {
        return read_bytes(1);
    }

    /// Read a float at offset
    std::variant<float, ByteArrayError> read_float_at(size_t offset) {
        return read_endian_float_at<float>(offset, 4);
    }

    /// Read a double at offset
    std::variant<double, ByteArrayError> read_double_at(size_t offset) {
        return read_endian_float_at<double>(offset, 8);
    }

    /// Read a signed 32-bit integer at offset
    std::variant<int32_t, ByteArrayError> read_int_at(size_t offset) {
        return read_endian_int_at<int32_t>(offset, 4);
    }

    /// Read an unsigned 32-bit integer at offset
    std::variant<uint32_t, ByteArrayError> read_unsigned_int_at(size_t offset) {
        return read_endian_int_at<uint32_t>(offset, 4);
    }

    /// Read a signed 16-bit integer at offset
    std::variant<int16_t, ByteArrayError> read_short_at(size_t offset) {
        return read_endian_int_at<int16_t>(offset, 2);
    }

    /// Read an unsigned 16-bit integer at offset
    std::variant<uint16_t, ByteArrayError> read_unsigned_short_at(size_t offset) {
        return read_endian_int_at<uint16_t>(offset, 2);
    }

    /// Read a signed 8-bit integer at offset
    std::variant<int8_t, ByteArrayError> read_byte_at(size_t offset) {
        auto result = read_at(1, offset);
        if (auto* error = std::get_if<ByteArrayError>(&result)) {
            return *error;
        }
        return static_cast<int8_t>(*std::get<const uint8_t*>(result));
    }

    /// Read an unsigned 8-bit integer at offset
    std::variant<uint8_t, ByteArrayError> read_unsigned_byte_at(size_t offset) {
        return read_at(1, offset);
    }

private:
    /// Helper to write bytes with endianness
    std::optional<ByteArrayError> write_endian_bytes(const uint8_t* data, size_t size) {
        std::vector<uint8_t> bytes(data, data + size);
        if (endian_ == Endian::Big) {
            std::reverse(bytes.begin(), bytes.end());
        }
        return write_bytes(bytes);
    }

    /// Helper to read float with endianness
    template<typename T>
    std::variant<T, ByteArrayError> read_endian_float(size_t size) {
        auto result = read_bytes(size);
        if (auto* error = std::get_if<ByteArrayError>(&result)) {
            return *error;
        }
        auto* data = std::get<const uint8_t*>(result);
        std::vector<uint8_t> bytes(data, data + size);
        if (endian_ == Endian::Big) {
            std::reverse(bytes.begin(), bytes.end());
        }
        T value;
        std::memcpy(&value, bytes.data(), size);
        return value;
    }

    /// Helper to read integer with endianness
    template<typename T>
    std::variant<T, ByteArrayError> read_endian_int(size_t size) {
        auto result = read_bytes(size);
        if (auto* error = std::get_if<ByteArrayError>(&result)) {
            return *error;
        }
        auto* data = std::get<const uint8_t*>(result);
        std::vector<uint8_t> bytes(data, data + size);
        if (endian_ == Endian::Big) {
            std::reverse(bytes.begin(), bytes.end());
        }
        T value = 0;
        for (size_t i = 0; i < size; ++i) {
            value |= static_cast<T>(bytes[i]) << (i * 8);
        }
        return value;
    }

    /// Helper to read float at offset with endianness
    template<typename T>
    std::variant<T, ByteArrayError> read_endian_float_at(size_t offset, size_t size) {
        auto result = read_at(size, offset);
        if (auto* error = std::get_if<ByteArrayError>(&result)) {
            return *error;
        }
        auto* data = std::get<const uint8_t*>(result);
        std::vector<uint8_t> bytes(data, data + size);
        if (endian_ == Endian::Big) {
            std::reverse(bytes.begin(), bytes.end());
        }
        T value;
        std::memcpy(&value, bytes.data(), size);
        return value;
    }

    /// Helper to read integer at offset with endianness
    template<typename T>
    std::variant<T, ByteArrayError> read_endian_int_at(size_t offset, size_t size) {
        auto result = read_at(size, offset);
        if (auto* error = std::get_if<ByteArrayError>(&result)) {
            return *error;
        }
        auto* data = std::get<const uint8_t*>(result);
        std::vector<uint8_t> bytes(data, data + size);
        if (endian_ == Endian::Big) {
            std::reverse(bytes.begin(), bytes.end());
        }
        T value = 0;
        for (size_t i = 0; i < size; ++i) {
            value |= static_cast<T>(bytes[i]) << (i * 8);
        }
        return value;
    }
};

/// FromWStr trait implementation for CompressionAlgorithm
template<>
struct FromWStr<CompressionAlgorithm> {
    using Err = void;

    static std::optional<CompressionAlgorithm> from_wstr(const WStr* s) {
        if (*s == *WStr::from_units(b"zlib")) {
            return CompressionAlgorithm::Zlib;
        } else if (*s == *WStr::from_units(b"deflate")) {
            return CompressionAlgorithm::Deflate;
        } else if (*s == *WStr::from_units(b"lzma")) {
            return CompressionAlgorithm::Lzma;
        }
        return std::nullopt;
    }
};

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_BYTEARRAY_H
