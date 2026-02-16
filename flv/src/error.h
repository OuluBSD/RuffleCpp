// C++ translation of flv/src/error.rs
// FLV parser error types

#ifndef RUFFLE_FLV_ERROR_H
#define RUFFLE_FLV_ERROR_H

#include <string>
#include <variant>
#include <system_error>

namespace ruffle {
namespace flv {

/// FLV parser error types
///
/// Mirrors the Rust Error enum from flv/src/error.rs
class Error {
public:
    /// Error variant types
    enum class Kind {
        EndOfData,                    // The FLV parser ran out of data
        PointerTooBig,                // FLV length exceeds maximum memory size
        WrongMagic,                   // Invalid FLV header signature
        UnknownValueType,             // Script data block with unknown value type
        ShortAudioBlock,              // Audio data block too short
        UnknownAudioFormatType,       // Unknown audio format type (u8)
        UnknownAudioRate,             // Unknown audio sample rate (u8)
        UnknownAudioSampleSize,       // Unknown audio sample size (u8)
        UnknownAudioChannelCount,     // Unknown audio channel count (u8)
        UnknownAacPacketType,         // Unknown AAC packet type (u8)
        ShortVideoBlock,              // Video data block too short
        UnknownVideoFrameType,        // Unknown video frame type (u8)
        UnknownVideoCodec,            // Unknown video codec (u8)
        UnknownVideoCommandType,      // Unknown video command frame type (u8)
        UnknownAvcPacketType,         // Unknown AVC packet type (u8)
        UnknownTagType,               // Unknown tag type (u8)
        IoError,                      // IO error (std::errc, message)
    };

private:
    Kind kind_;
    std::string message_;

    // Additional data for specific error types
    std::variant<
        std::monostate,              // For simple errors
        uint8_t,                     // For Unknown* errors with u8 value
        std::pair<std::errc, std::string>  // For IoError
    > data_;

public:
    /// Create a simple error
    explicit Error(Kind kind);

    /// Create an error with a u8 value
    Error(Kind kind, uint8_t value);

    /// Create an IO error
    Error(std::errc io_error, const std::string& message);

    /// Get the error kind
    [[nodiscard]] Kind kind() const { return kind_; }

    /// Get the error message
    [[nodiscard]] const std::string& message() const { return message_; }

    /// Get the u8 value for Unknown* errors
    [[nodiscard]] std::optional<uint8_t> value() const;

    /// Get full error description
    [[nodiscard]] std::string to_string() const;

    /// Equality comparison
    bool operator==(const Error& other) const;

    /// Inequality comparison
    bool operator!=(const Error& other) const {
        return !(*this == other);
    }
};

/// Stream output operator for Error
std::ostream& operator<<(std::ostream& os, const Error& error);

} // namespace flv
} // namespace ruffle

#endif // RUFFLE_FLV_ERROR_H
