// C++ translation of flv/src/reader.rs
// FLV container reader for demuxing

#ifndef RUFFLE_FLV_READER_H
#define RUFFLE_FLV_READER_H

#include <cstdint>
#include <cstddef>
#include <vector>
#include <system_error>

// Forward declarations
namespace ruffle {
namespace flv {
namespace error {
    class Error;
}
}
}

namespace ruffle {
namespace flv {

/// A reader that allows demuxing an FLV container.
///
/// This reader works with a borrowed byte slice and maintains a position
/// within that slice. It provides methods for reading various data types
/// in big-endian format, as used in FLV files.
class FlvReader {
public:
    /// Construct a new FLV reader from a source buffer
    ///
    /// \param source Pointer to the source data
    /// \param size Size of the source data
    explicit FlvReader(const uint8_t* source, size_t size)
        : source_(source)
        , size_(size)
        , position_(0)
    {}

    /// Construct from source parts (reconstitute a broken-down reader)
    ///
    /// \param source Pointer to the source data
    /// \param size Size of the source data
    /// \param position The seek position within the data
    FlvReader(const uint8_t* source, size_t size, size_t position)
        : source_(source)
        , size_(size)
        , position_(position)
    {}

    /// Get the source buffer pointer
    const uint8_t* source() const {
        return source_;
    }

    /// Get the size of the source buffer
    size_t size() const {
        return size_;
    }

    /// Get the current position
    size_t position() const {
        return position_;
    }

    /// Set the current position
    void set_position(size_t pos) {
        position_ = pos;
    }

    /// Break down the reader into its source parts
    ///
    /// \return Tuple of (source pointer, size, position)
    std::tuple<const uint8_t*, size_t, size_t> into_parts() const {
        return {source_, size_, position_};
    }

    /// Read a certain number of bytes from the buffer
    ///
    /// This advances the buffer position. If the requested number of bytes
    /// are not available, EndOfData error is returned.
    ///
    /// \param count Number of bytes to read
    /// \return Pointer to the data, or error if not enough data
    std::expected<const uint8_t*, error::Error> read(size_t count) {
        size_t start = position_;
        size_t end = position_ + count;
        
        if (end > size_ || end < start) {  // Check for overflow
            return std::unexpected(error::Error::end_of_data());
        }

        position_ = end;
        return source_ + start;
    }

    /// Peek at bytes without advancing the position
    ///
    /// \param count Number of bytes to peek
    /// \return Pointer to the data, or error if not enough data
    std::expected<const uint8_t*, error::Error> peek(size_t count) {
        size_t end = position_ + count;
        
        if (end > size_ || end < position_) {  // Check for overflow
            return std::unexpected(error::Error::end_of_data());
        }

        return source_ + position_;
    }

    /// Read an unsigned 8-bit integer
    std::expected<uint8_t, error::Error> read_u8() {
        auto data = read(1);
        if (data) {
            return (*data)[0];
        }
        return std::unexpected(data.error());
    }

    /// Read an unsigned 16-bit integer (big-endian)
    std::expected<uint16_t, error::Error> read_u16() {
        auto data = read(2);
        if (data) {
            uint16_t value = (static_cast<uint16_t>((*data)[0]) << 8) |
                            static_cast<uint16_t>((*data)[1]);
            return value;
        }
        return std::unexpected(data.error());
    }

    /// Read a signed 16-bit integer (big-endian)
    std::expected<int16_t, error::Error> read_i16() {
        auto result = read_u16();
        if (result) {
            return static_cast<int16_t>(*result);
        }
        return std::unexpected(result.error());
    }

    /// Read an unsigned 24-bit integer (big-endian)
    std::expected<uint32_t, error::Error> read_u24() {
        auto data = read(3);
        if (data) {
            uint32_t value = (static_cast<uint32_t>((*data)[0]) << 16) |
                            (static_cast<uint32_t>((*data)[1]) << 8) |
                            static_cast<uint32_t>((*data)[2]);
            return value;
        }
        return std::unexpected(data.error());
    }

    /// Peek at an unsigned 24-bit integer (big-endian)
    std::expected<uint32_t, error::Error> peek_u24() {
        auto data = peek(3);
        if (data) {
            uint32_t value = (static_cast<uint32_t>((*data)[0]) << 16) |
                            (static_cast<uint32_t>((*data)[1]) << 8) |
                            static_cast<uint32_t>((*data)[2]);
            return value;
        }
        return std::unexpected(data.error());
    }

    /// Read an unsigned 32-bit integer (big-endian)
    std::expected<uint32_t, error::Error> read_u32() {
        auto data = read(4);
        if (data) {
            uint32_t value = (static_cast<uint32_t>((*data)[0]) << 24) |
                            (static_cast<uint32_t>((*data)[1]) << 16) |
                            (static_cast<uint32_t>((*data)[2]) << 8) |
                            static_cast<uint32_t>((*data)[3]);
            return value;
        }
        return std::unexpected(data.error());
    }

    /// Read a 64-bit floating point number (big-endian)
    std::expected<double, error::Error> read_f64() {
        auto data = read(8);
        if (data) {
            uint64_t bits = 0;
            for (int i = 0; i < 8; ++i) {
                bits = (bits << 8) | (*data)[i];
            }
            double value;
            std::memcpy(&value, &bits, sizeof(double));
            return value;
        }
        return std::unexpected(data.error());
    }

    /// Seek to a new position
    ///
    /// \param pos The new position (from start)
    /// \return The new position, or error if invalid
    std::expected<uint64_t, std::io::error_code> seek_start(uint64_t pos) {
        if (pos > size_) {
            return std::unexpected(std::make_error_code(std::errc::invalid_argument));
        }
        position_ = static_cast<size_t>(pos);
        return pos;
    }

    /// Seek relative to current position
    ///
    /// \param offset The offset from current position
    /// \return The new position, or error if invalid
    std::expected<uint64_t, std::io::error_code> seek_current(int64_t offset) {
        int64_t new_pos = static_cast<int64_t>(position_) + offset;
        if (new_pos < 0 || static_cast<size_t>(new_pos) > size_) {
            return std::unexpected(std::make_error_code(std::errc::invalid_argument));
        }
        position_ = static_cast<size_t>(new_pos);
        return static_cast<uint64_t>(new_pos);
    }

    /// Seek relative to end of data
    ///
    /// \param offset The offset from end (negative = before end)
    /// \return The new position, or error if invalid
    std::expected<uint64_t, std::io::error_code> seek_end(int64_t offset) {
        int64_t new_pos = static_cast<int64_t>(size_) - offset;
        if (new_pos < 0 || static_cast<size_t>(new_pos) > size_) {
            return std::unexpected(std::make_error_code(std::errc::invalid_argument));
        }
        position_ = static_cast<size_t>(new_pos);
        return static_cast<uint64_t>(new_pos);
    }

private:
    const uint8_t* source_;
    size_t size_;
    size_t position_;
};

} // namespace flv
} // namespace ruffle

#endif // RUFFLE_FLV_READER_H
