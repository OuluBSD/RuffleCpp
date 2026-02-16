// C++ translation of flv/src/header.rs
// FLV header parsing

#ifndef RUFFLE_FLV_HEADER_H
#define RUFFLE_FLV_HEADER_H

#include <cstdint>
#include <optional>

namespace ruffle {
namespace flv {

// Forward declaration - Error defined in error.h
namespace error {
    enum class Error;
}

// Forward declaration - FlvReader defined in reader.h
namespace reader {
    class FlvReader;
}

/// Flags indicating what type of data is present in the FLV file
enum class TypeFlags : uint8_t {
    None = 0,
    HasAudio = 0b0000_0001,
    HasVideo = 0b0000_0100,
};

/// Implement bitwise OR for TypeFlags
inline TypeFlags operator|(TypeFlags a, TypeFlags b) {
    return static_cast<TypeFlags>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

/// Implement bitwise AND for TypeFlags
inline TypeFlags operator&(TypeFlags a, TypeFlags b) {
    return static_cast<TypeFlags>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}

/// Check if a flag is set
inline bool has_flag(TypeFlags flags, TypeFlags flag) {
    return (flags & flag) != TypeFlags::None;
}

/// FLV file header
struct Header {
    /// FLV version number
    uint8_t version;

    /// Flags indicating what type of data is present
    TypeFlags type_flags;

    /// Offset to the start of the data tags
    uint32_t data_offset;

    /// Parse an FLV header.
    ///
    /// The header must, at a minimum, contain the FLV magic, version number,
    /// valid type flags, and a valid offset into the data. The reader will
    /// seek to the start of the data tags if successful or retain its prior
    /// position otherwise.
    ///
    /// Returns std::nullopt on error
    static std::optional<Header> parse(reader::FlvReader& reader);
};

} // namespace flv
} // namespace ruffle

#endif // RUFFLE_FLV_HEADER_H
