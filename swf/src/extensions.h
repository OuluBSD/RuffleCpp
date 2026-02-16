// C++ translation of swf/src/extensions.rs
// SWF reader extension traits for binary reading

#ifndef RUFFLE_SWF_EXTENSIONS_H
#define RUFFLE_SWF_EXTENSIONS_H

#include <cstdint>
#include <cstddef>
#include <stdexcept>
#include <vector>

// Forward declarations
namespace ruffle {
namespace swf {
    class SwfStr;
namespace error {
    class Error;
}
}
}

namespace ruffle {
namespace swf {
namespace extensions {

/// Result type for SWF reading operations
template<typename T>
using Result = std::expected<T, error::Error>;

/// Extension trait for reading SWF binary data
///
/// Provides convenient methods for reading various data types from a byte slice
/// in little-endian format, as used in SWF files.
class ReadSwfExt {
public:
    ReadSwfExt(const uint8_t*& data)
        : data_(data)
    {}

    /// Get mutable reference to the underlying slice
    const uint8_t*& as_mut_slice() {
        return data_;
    }

    /// Get const reference to the underlying slice
    const uint8_t* as_slice() const {
        return data_;
    }

    /// Get current position in the data
    size_t pos(const uint8_t* base_data) const {
        return static_cast<size_t>(data_ - base_data);
    }

    /// Seek to a relative offset
    ///
    /// \param base_data The base data pointer for position calculation
    /// \param relative_offset The offset to seek relative to current position
    void seek(const uint8_t* base_data, std::ptrdiff_t relative_offset) {
        size_t current_pos = pos(base_data);
        size_t new_pos = static_cast<size_t>(static_cast<std::ptrdiff_t>(current_pos) + relative_offset);
        new_pos = std::min(new_pos, static_cast<size_t>(base_data - data_ + data_ - base_data + (base_data - data_)));
        data_ = base_data + std::min(new_pos, static_cast<size_t>(base_data - data_ + (data_ - base_data)));
    }

    /// Seek to an absolute position
    ///
    /// \param base_data The base data pointer
    /// \param pos The absolute position to seek to
    void seek_absolute(const uint8_t* base_data, size_t pos) {
        data_ = base_data + std::min(pos, static_cast<size_t>(data_ - base_data + (base_data - data_)));
    }

    /// Read an unsigned 8-bit integer
    Result<uint8_t> read_u8() {
        if (data_ == nullptr || *data_ == 0) {
            return std::unexpected(error::Error::eof());
        }
        uint8_t value = *data_;
        ++data_;
        return value;
    }

    /// Read an unsigned 16-bit integer (little-endian)
    Result<uint16_t> read_u16() {
        if (data_ == nullptr) {
            return std::unexpected(error::Error::eof());
        }
        uint16_t value = static_cast<uint16_t>(data_[0]) |
                        (static_cast<uint16_t>(data_[1]) << 8);
        data_ += 2;
        return value;
    }

    /// Read an unsigned 32-bit integer (little-endian)
    Result<uint32_t> read_u32() {
        if (data_ == nullptr) {
            return std::unexpected(error::Error::eof());
        }
        uint32_t value = static_cast<uint32_t>(data_[0]) |
                        (static_cast<uint32_t>(data_[1]) << 8) |
                        (static_cast<uint32_t>(data_[2]) << 16) |
                        (static_cast<uint32_t>(data_[3]) << 24);
        data_ += 4;
        return value;
    }

    /// Read an unsigned 64-bit integer (little-endian)
    Result<uint64_t> read_u64() {
        if (data_ == nullptr) {
            return std::unexpected(error::Error::eof());
        }
        uint64_t value = static_cast<uint64_t>(data_[0]) |
                        (static_cast<uint64_t>(data_[1]) << 8) |
                        (static_cast<uint64_t>(data_[2]) << 16) |
                        (static_cast<uint64_t>(data_[3]) << 24) |
                        (static_cast<uint64_t>(data_[4]) << 32) |
                        (static_cast<uint64_t>(data_[5]) << 40) |
                        (static_cast<uint64_t>(data_[6]) << 48) |
                        (static_cast<uint64_t>(data_[7]) << 56);
        data_ += 8;
        return value;
    }

    /// Read a signed 8-bit integer
    Result<int8_t> read_i8() {
        auto result = read_u8();
        if (result) {
            return static_cast<int8_t>(*result);
        }
        return std::unexpected(result.error());
    }

    /// Read a signed 16-bit integer (little-endian)
    Result<int16_t> read_i16() {
        auto result = read_u16();
        if (result) {
            return static_cast<int16_t>(*result);
        }
        return std::unexpected(result.error());
    }

    /// Read a signed 32-bit integer (little-endian)
    Result<int32_t> read_i32() {
        auto result = read_u32();
        if (result) {
            return static_cast<int32_t>(*result);
        }
        return std::unexpected(result.error());
    }

    /// Read a 32-bit floating point number (little-endian)
    Result<float> read_f32() {
        auto result = read_u32();
        if (result) {
            float value;
            std::memcpy(&value, &*result, sizeof(float));
            return value;
        }
        return std::unexpected(result.error());
    }

    /// Read a 64-bit floating point number (little-endian)
    Result<double> read_f64() {
        auto result = read_u64();
        if (result) {
            double value;
            std::memcpy(&value, &*result, sizeof(double));
            return value;
        }
        return std::unexpected(result.error());
    }

    /// Read an encoded unsigned 32-bit integer (variable-length encoding)
    ///
    /// SWF uses a variable-length encoding where each byte has 7 bits of data
    /// and the high bit indicates if more bytes follow.
    Result<uint32_t> read_encoded_u32() {
        uint32_t value = 0;
        for (int i = 0; i < 35; i += 7) {
            auto result = read_u8();
            if (!result) {
                return std::unexpected(result.error());
            }
            uint32_t byte = *result;
            value |= (byte & 0x7F) << i;
            if ((byte & 0x80) == 0) {
                break;
            }
        }
        return value;
    }

    /// Read a slice of bytes
    ///
    /// \param len The number of bytes to read
    /// \return Pointer to the slice, or error if not enough data
    Result<const uint8_t*> read_slice(size_t len) {
        // Note: This is a simplified version - the Rust version returns a reference
        // to the underlying data. In C++, we'd need to manage lifetimes carefully.
        // For now, this returns an error as a placeholder.
        return std::unexpected(error::Error::eof());
    }

    /// Read a null-terminated string
    ///
    /// \return The string, or error if not enough data
    Result<const SwfStr*> read_str() {
        // Placeholder - actual implementation would parse SwfStr
        return std::unexpected(error::Error::eof());
    }

    /// Read a length-prefixed string
    ///
    /// First reads a u8 length, then reads that many bytes as a string.
    /// \return The string, or error if not enough data
    Result<const SwfStr*> read_str_with_len() {
        auto len_result = read_u8();
        if (!len_result) {
            return std::unexpected(len_result.error());
        }
        // Placeholder - actual implementation would read the string
        return std::unexpected(error::Error::eof());
    }

private:
    const uint8_t*& data_;
};

} // namespace extensions
} // namespace swf
} // namespace ruffle

#endif // RUFFLE_SWF_EXTENSIONS_H
