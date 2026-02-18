// C++ translation of render/src/utils.rs
// Utility functions for render module

#ifndef RUFFLE_RENDER_UTILS_H
#define RUFFLE_RENDER_UTILS_H

#include <cstdint>
#include <cstddef>
#include <vector>
#include <string>
#include <string_view>
#include <expected>
#include <memory>

// Forward declarations
namespace ruffle {
namespace render {
namespace bitmap {
    class Bitmap;
    enum class BitmapFormat;
}
}
}

namespace swf {
    struct Color;
}

namespace ruffle {
namespace render {
namespace utils {

/// The format of image data in a DefineBitsJpeg2/3 tag.
/// Generally this will be JPEG, but according to SWF19, these tags can also contain PNG and GIF data.
enum class JpegTagFormat {
    Jpeg,
    Png,
    Gif,
    Unknown
};

/// Determines the format of the image data in `data` from a DefineBitsJPEG2/3 tag.
///
/// @param data The image data to analyze
/// @return The detected JPEG tag format
JpegTagFormat determine_jpeg_tag_format(const std::vector<uint8_t>& data);

/// Decodes bitmap data from a DefineBitsJPEG2/3 tag.
/// The data is returned with pre-multiplied alpha.
///
/// @param data The JPEG data to decode
/// @param alpha_data Optional alpha channel data
/// @return The decoded bitmap, or an error
std::expected<bitmap::Bitmap, std::string> decode_define_bits_jpeg(
    const std::vector<uint8_t>& data,
    const std::vector<uint8_t>* alpha_data
);

/// Decodes dimensions from DefineBitsJPEG data.
///
/// @param data The JPEG data to analyze
/// @return The width and height, or an error
std::expected<std::pair<uint32_t, uint32_t>, std::string> decode_define_bits_jpeg_dimensions(
    const std::vector<uint8_t>& data
);

/// Glues the JPEG encoding tables from a JPEGTables SWF tag to the JPEG data
/// in a DefineBits tag, producing complete JPEG data suitable for a decoder.
///
/// @param jpeg_data The JPEG data from DefineBits tag
/// @param jpeg_tables Optional JPEG tables from JPEGTables tag
/// @return Complete JPEG data (may be a view or owned vector)
std::vector<uint8_t> glue_tables_to_jpeg(
    const std::vector<uint8_t>& jpeg_data,
    const std::vector<uint8_t>* jpeg_tables
);

/// Removes potential invalid JPEG data from SWF DefineBitsJPEG tags.
/// These bytes need to be removed for the JPEG to decode properly.
///
/// @param data The JPEG data to clean
/// @return Cleaned JPEG data
std::vector<uint8_t> remove_invalid_jpeg_data(const std::vector<uint8_t>& data);

/// Decodes the bitmap data in DefineBitsLossless tag into RGBA.
/// DefineBitsLossless is Zlib encoded pixel data (similar to PNG), possibly
/// palletized.
///
/// @param swf_tag The DefineBitsLossless tag data
/// @return The decoded bitmap, or an error
std::expected<bitmap::Bitmap, std::string> decode_define_bits_lossless(
    const swf::DefineBitsLossless& swf_tag
);

/// Converts standard RGBA to premultiplied alpha.
///
/// @param rgba The RGBA data to convert (modified in place)
void premultiply_alpha_rgba(std::vector<uint8_t>& rgba);

/// Converts premultiplied RGBA to unmultiplied RGBA.
///
/// @param rgba The premultiplied RGBA data to convert (modified in place)
void unmultiply_alpha_rgba(std::vector<uint8_t>& rgba);

/// Decodes zlib-compressed data.
///
/// @param data The zlib-compressed data
/// @return The decompressed data, or an error
std::expected<std::vector<uint8_t>, std::string> decompress_zlib(const std::vector<uint8_t>& data);

} // namespace utils
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_UTILS_H
