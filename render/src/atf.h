// C++ translation of render/src/atf.rs
// ATF (Adobe Texture Format) parsing

#ifndef RUFFLE_RENDER_ATF_H
#define RUFFLE_RENDER_ATF_H

#include <cstdint>
#include <vector>
#include <memory>
#include <variant>
#include <stdexcept>

namespace ruffle {
namespace render {

/// ATF texture format enumeration
///
/// Adobe Texture Format supports various compression and encoding formats.
enum class ATFFormat : uint8_t {
    RGB888 = 0,
    RGBA8888 = 1,
    Compressed = 2,
    RawCompressed = 3,
    CompressedAlpha = 4,
    RawCompressedAlpha = 5,
    CompressedLossy = 0x0C,
    CompressedLossyAlpha = 0x0D,
};

/// ATF texture data variants
///
/// Different ATF formats store texture data in different ways.
/// This variant type represents all possible data formats.
struct ATFTextureDataUnknown {
    std::vector<uint8_t> data;
};

struct ATFTextureDataJpegXR {
    std::vector<uint8_t> data;
};

struct ATFTextureDataCompressedAlpha {
    std::vector<uint8_t> jpegxr_alpha;
    std::vector<uint8_t> dxt1_alpha_compressed;
    std::vector<uint8_t> jpegxr_bgr;
    std::vector<uint8_t> dxt5_rgb_compressed;
};

struct ATFTextureDataCompressedRawAlpha {
    std::vector<uint8_t> dxt5;
    std::vector<uint8_t> pvrtc;
    std::vector<uint8_t> etc1;
    std::vector<uint8_t> etc2;
};

using ATFTextureData = std::variant<
    ATFTextureDataUnknown,
    ATFTextureDataJpegXR,
    ATFTextureDataCompressedAlpha,
    ATFTextureDataCompressedRawAlpha
>;

/// Parsed ATF texture
///
/// Represents a fully parsed Adobe Texture Format file.
/// Contains texture dimensions, format, and mip level data.
class ATFTexture {
public:
    /// Texture width in pixels
    uint32_t width;

    /// Texture height in pixels
    uint32_t height;

    /// True if this is a cubemap texture (6 faces)
    bool cubemap;

    /// The pixel format of the texture
    ATFFormat format;

    /// Number of mip levels
    uint8_t mip_count;

    /// Texture data organized by face and mip level
    /// Outer vector: faces (1 for 2D, 6 for cubemap)
    /// Inner vector: mip levels
    std::vector<std::vector<ATFTextureData>> face_mip_data;

    /// Default constructor
    ATFTexture()
        : width(0)
        , height(0)
        , cubemap(false)
        , format(ATFFormat::RGB888)
        , mip_count(0) {}

    /// Parse an ATF texture from bytes
    ///
    /// Parses the ATF file format and extracts texture data.
    /// Based on https://github.com/openfl/openfl/blob/develop/src/openfl/display3D/_internal/ATFReader.hx
    ///
    /// \param bytes Pointer to the ATF file data
    /// \param size Size of the data in bytes
    /// \return Parsed ATFTexture
    /// \throws std::runtime_error if the file is invalid
    static ATFTexture from_bytes(const uint8_t* bytes, size_t size);

private:
    /// Read a 24-bit unsigned integer (little-endian)
    ///
    /// \param data Pointer to data
    /// \param offset Offset into data
    /// \return 24-bit value as 32-bit integer
    static uint32_t read_uint24(const uint8_t* data, size_t& offset);
};

/// Exception thrown when ATF parsing fails
class ATFError : public std::runtime_error {
public:
    explicit ATFError(const std::string& message)
        : std::runtime_error(message) {}
};

} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_ATF_H
