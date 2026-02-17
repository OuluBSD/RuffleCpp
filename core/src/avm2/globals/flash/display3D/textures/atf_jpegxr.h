// C++ translation of core/src/avm2/globals/flash/display3D/textures/atf_jpegxr.rs
//! ATF JPEG-XR compressed texture upload implementation

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY3D_TEXTURES_ATF_JPEGXR_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY3D_TEXTURES_ATF_JPEGXR_H

#include <vector>
#include <cstdint>
#include <cstddef>
#include <optional>
#include <variant>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class Object;
    template<typename T> class TextureObject;
    namespace bytearray {
        class ByteArrayStorage;
    }
}
namespace render {
namespace atf {
    struct ATFTexture;
    enum class ATFTextureData;
}
}
}
}

// JPEG-XR decoder types (external library)
namespace jpegxr {
    enum class PixelFormat;
    class ImageDecode;
    struct PixelInfo;
}

// LZMA decompression (optional feature)
namespace lzma_rs {
    void lzma_decompress(std::vector<uint8_t>* input, std::vector<uint8_t>* output);
}

// Image processing (external library)
namespace image {
    enum class ImageFormat;
    class ImageReader;
    class DynamicImage;
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace display3D {
namespace textures {
namespace atf_jpegxr {

using bytearray::ByteArrayStorage;
using ruffle::core::render::atf::ATFTexture;
using ruffle::core::render::atf::ATFTextureData;

/// Internal helper to convert JPEG-XR to raw pixels
/// @param atf_width The expected width of the ATF texture
/// @param atf_height The expected height of the ATF texture
/// @param bytes The JPEG-XR encoded data
/// @return Decoded raw pixel data
std::vector<uint8_t> jpegxr_to_raw_pixels(
    uint32_t atf_width,
    uint32_t atf_height,
    std::vector<uint8_t> bytes
);

/// Internal helper to convert JPEG-XR to TIFF image
/// @param atf_width The expected width of the ATF texture
/// @param atf_height The expected height of the ATF texture
/// @param bytes The JPEG-XR encoded data
/// @return Tuple of (decoded image, pixel format)
std::pair<image::DynamicImage, jpegxr::PixelFormat> jpegxr_to_tiff(
    uint32_t atf_width,
    uint32_t atf_height,
    std::vector<uint8_t> bytes
);

/// Implements compressed texture upload from ByteArray
///
/// Handles ATF textures with JPEG-XR compression, including:
/// - Standard JPEG-XR compressed textures
/// - CompressedAlpha format with DXT1/DXT5 + JPEG-XR
/// - CompressedRawAlpha format with DXT5 fallback
///
/// @param activation The current AVM2 activation
/// @param texture The target texture object
/// @param data The ByteArray containing ATF data
/// @param byte_array_offset Offset in the ByteArray to read from
/// @param is_cube Whether this is a cube map texture
/// @return Success or error
template<typename GC>
Result<void, Error<GC>> do_compressed_upload(
    Activation<GC>* activation,
    TextureObject<GC> texture,
    Object<GC> data,
    size_t byte_array_offset,
    bool is_cube
);

} // namespace atf_jpegxr
} // namespace textures
} // namespace display3D
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

// Template method implementations

template<typename GC>
Result<void, Error<GC>> atf_jpegxr::do_compressed_upload(
    Activation<GC>* activation,
    TextureObject<GC> texture,
    Object<GC> data,
    size_t byte_array_offset,
    bool is_cube
) {
    auto bytearray = data.as_bytearray().expect("Expected ByteArray");
    const auto& bytes = ByteArrayStorage::bytes(&bytearray);
    
    if (byte_array_offset >= bytes.size()) {
        return Error<GC>::make_error_3675(activation);
    }
    
    const auto* raw_atf = &bytes[byte_array_offset];
    const size_t raw_atf_size = bytes.size() - byte_array_offset;

    auto atf_texture = ATFTexture::from_bytes(raw_atf, raw_atf_size)
        .expect("Failed to parse ATF texture");

    // Validate dimensions
    if (atf_texture.width != texture.handle()->width() ||
        atf_texture.height != texture.handle()->height()) {
        return Error<GC>::make_error_3679(activation);
    }

    // Validate cube map flag
    if (is_cube != atf_texture.cubemap) {
        return Error<GC>::make_error_3675(activation);
    }

    // Just use the first mip level for now. We ignore the builtin format - the JPEG-XR format
    // appears to override it
    std::vector<uint8_t> bitmap;

    const auto& face_mip_data = atf_texture.face_mip_data[0][0];
    
    if (auto* jpegxr_data = std::get_if<ATFTextureData::JpegXR>(&face_mip_data)) {
        // Standard JPEG-XR compressed texture
        auto [decoded_image, pixel_format] = jpegxr_to_tiff(
            atf_texture.width,
            atf_texture.height,
            *jpegxr_data
        );
        
        auto rgba_image = decoded_image.to_rgba8();
        bitmap = std::vector<uint8_t>(
            rgba_image.pixels().begin(),
            rgba_image.pixels().end()
        );
    }
    else if (auto* compressed_alpha = std::get_if<ATFTextureData::CompressedAlpha>(&face_mip_data)) {
        // CompressedAlpha format - DXT1/DXT5 with JPEG-XR encoded values
        // See https://github.com/adobe/dds2atf/issues/5
        // The ATF format uses a weird version of LZMA that doesn't store the uncompressed length.
        // Compute it ourselves, and insert it where it should be
        
        auto dxt1_alpha_compressed = compressed_alpha->dxt1_alpha_compressed;
        auto dxt5_rgb_compressed = compressed_alpha->dxt5_rgb_compressed;

        const uint64_t dxt1_uncompressed_length = std::numeric_limits<uint64_t>::max();
        const uint64_t dxt5_uncompressed_length = std::numeric_limits<uint64_t>::max();

        dxt1_alpha_compressed.insert(
            dxt1_alpha_compressed.begin() + 5,
            reinterpret_cast<const uint8_t*>(&dxt1_uncompressed_length),
            reinterpret_cast<const uint8_t*>(&dxt1_uncompressed_length) + sizeof(uint64_t)
        );
        dxt5_rgb_compressed.insert(
            dxt5_rgb_compressed.begin() + 5,
            reinterpret_cast<const uint8_t*>(&dxt5_uncompressed_length),
            reinterpret_cast<const uint8_t*>(&dxt5_uncompressed_length) + sizeof(uint64_t)
        );

        std::vector<uint8_t> dxt1_alpha;
        dxt1_alpha.reserve(dxt1_alpha_compressed.size());
        lzma_rs::lzma_decompress(&dxt1_alpha_compressed, &dxt1_alpha);

        std::vector<uint8_t> dxt5_rgb;
        dxt5_rgb.reserve(dxt5_rgb_compressed.size());
        lzma_rs::lzma_decompress(&dxt5_rgb_compressed, &dxt5_rgb);

        // 'COMPRESSED_ALPHA' images are encoded in a very strange way. The LZMA-compressed DXT1/DXT5
        // sections just hold the DXT lookup table block. The associated values for each block
        // are stored in JPEG-XR images, which are optionally compressed
        auto [jpegxr_alpha, pixel_format] = jpegxr_to_tiff(
            atf_texture.width / 4,
            atf_texture.height / 2,
            compressed_alpha->jpegxr_alpha
        );

        auto jpegxr_alpha_rgba = jpegxr_alpha.to_rgba8();
        if (pixel_format != jpegxr::PixelFormat::PixelFormat8bppGray) {
            return Error<GC>::from_string("Unexpected JPEG-XR alpha format");
        }

        auto jpegxr_bgr = jpegxr_to_raw_pixels(
            atf_texture.width / 4,
            atf_texture.height / 2,
            compressed_alpha->jpegxr_bgr
        );

        std::vector<uint8_t> reconstructed_dxt;

        if (dxt1_alpha.size() / 6 != dxt5_rgb.size() / 4) {
            return Error<GC>::from_string("Bad DXT data");
        }
        if (jpegxr_alpha_rgba.as_raw().size() / 8 != jpegxr_bgr.size() / 4) {
            return Error<GC>::from_string("Bad JPEG-XR data");
        }
        if (dxt1_alpha.size() / 6 != jpegxr_alpha_rgba.as_raw().size() / 8) {
            return Error<GC>::from_string("Dxt data doesn't match jpegxr data");
        }

        const size_t second_half = (atf_texture.width / 4) * (atf_texture.height / 4);

        // The two values for each pixels are split across the upper and lower halves of the image.
        // See https://github.com/adobe/dds2atf/blob/cbc479be2e77daa273306161af571f8255aec78d/pvr2atfcore.cpp#L884
        for (size_t i = 0; i < dxt1_alpha.size() / 6; ++i) {
            const auto* alpha_lookup_table = &dxt1_alpha[i * 6];
            const auto* jpegxr_alpha_first = &jpegxr_alpha_rgba.as_raw()[i * 4];
            const auto* jpegxr_alpha_second = &jpegxr_alpha_rgba.as_raw()[(second_half + (i * 4))];

            const auto* rgb_lookup_table = &dxt5_rgb[i * 4];
            const auto* jpegxr_bgr_first = &jpegxr_bgr[i * 2];
            // Each pixel is 2 bytes, so multiply second_half by 2 to get the correct
            // start of the second half of the image
            const auto* jpegxr_bgr_second = &jpegxr_bgr[(second_half * 2 + (i * 2))];

            // We decoded as grayscale, so just use the red values (all of the rgb values
            // should be the same)
            reconstructed_dxt.push_back(jpegxr_alpha_first[0]);
            reconstructed_dxt.push_back(jpegxr_alpha_second[0]);
            // Copy the corresponding lookup table
            reconstructed_dxt.insert(reconstructed_dxt.end(), alpha_lookup_table, alpha_lookup_table + 6);

            // Copy the two 16-bit values
            reconstructed_dxt.insert(reconstructed_dxt.end(), jpegxr_bgr_first, jpegxr_bgr_first + 2);
            reconstructed_dxt.insert(reconstructed_dxt.end(), jpegxr_bgr_second, jpegxr_bgr_second + 2);

            // Copy the lookup table
            reconstructed_dxt.insert(reconstructed_dxt.end(), rgb_lookup_table, rgb_lookup_table + 4);
        }

        bitmap = std::move(reconstructed_dxt);
    }
    else if (auto* compressed_raw_alpha = std::get_if<ATFTextureData::CompressedRawAlpha>(&face_mip_data)) {
        // DXT5 seems to be the most widely supported, so let's use that for now.
        // TODO - fallback to other formats if DXT5 data isn't present
        // (or if we're on Android/iOS).
        if (compressed_raw_alpha->dxt5.empty()) {
            // Stub method for unsupported format
            activation->avm2_stub_method(
                "flash.display3D.textures.Texture",
                "uploadCompressedTextureFromByteArray",
                "with empty DXT5 data in CompressedRawAlpha"
            );
        }
        bitmap = compressed_raw_alpha->dxt5;
    }
    else if (std::holds_alternative<ATFTextureData::Unknown>(face_mip_data)) {
        return Error<GC>::from_string(
            "Unsupported ATF format: " + std::to_string(static_cast<int>(atf_texture.format))
        );
    }

    // Upload the texture
    texture.context3d()->copy_pixels_to_texture(bitmap, texture.handle(), 0);

    return {};
}

} // namespace atf_jpegxr
} // namespace textures
} // namespace display3D
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY3D_TEXTURES_ATF_JPEGXR_H
