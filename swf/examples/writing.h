// C++ translation of swf/examples/writing.rs
// Example: Creating a simple SWF file with a red background

#ifndef RUFFLE_SWF_EXAMPLES_WRITING_H
#define RUFFLE_SWF_EXAMPLES_WRITING_H

#include <cstdint>
#include <fstream>
#include <vector>

// Forward declarations for SWF types
namespace swf {

/// Compression method for SWF files
enum class Compression {
    None,
    Zlib,
    Lzma
};

/// SWF file header
struct Header {
    Compression compression;
    uint8_t version;
    Rectangle stage_size;
    Fixed8 frame_rate;
    uint16_t num_frames;
};

/// Rectangle defined in twips
struct Rectangle {
    int32_t x_min;
    int32_t x_max;
    int32_t y_min;
    int32_t y_max;
};

/// Fixed-point 8.8 number
struct Fixed8 {
    uint16_t value;
    
    static Fixed8 from_f32(float f) {
        Fixed8 result;
        result.value = static_cast<uint16_t>(f * 256.0f);
        return result;
    }
};

/// Twips unit (1/20 of a pixel)
struct Twips {
    int32_t value;
    
    static const Twips ZERO;
    
    static Twips from_pixels(float pixels) {
        Twips result;
        result.value = static_cast<int32_t>(pixels * 20.0f);
        return result;
    }
};

// Initialize ZERO
inline const Twips Twips::ZERO = Twips{0};

/// RGBA Color
struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

/// SWF tag types
enum class TagCode {
    SetBackgroundColor = 1,
    ShowFrame = 9,
    // ... other tags
};

/// Tag union
struct Tag {
    enum class Type {
        SetBackgroundColor,
        ShowFrame
    };
    
    Type type;
    Color color;  // Used for SetBackgroundColor
    
    static Tag set_background_color(Color c) {
        Tag tag;
        tag.type = Type::SetBackgroundColor;
        tag.color = c;
        return tag;
    }
    
    static Tag show_frame() {
        Tag tag;
        tag.type = Type::ShowFrame;
        return tag;
    }
};

/// Write a complete SWF file
///
/// \param header The SWF file header
/// \param tags Array of tags to write
/// \param num_tags Number of tags in the array
/// \param output_stream Output stream to write to
/// \return true on success, false on failure
bool write_swf(const Header& header, const Tag* tags, size_t num_tags, std::ostream& output_stream);

} // namespace swf

namespace swf {
namespace examples {

/// Example: Create a simple SWF file with a red background
///
/// This demonstrates the basic SWF file creation process:
/// 1. Create a header with compression, version, stage size, frame rate, and frame count
/// 2. Create tags (SetBackgroundColor, ShowFrame)
/// 3. Write the SWF file
///
/// Usage:
/// \code
/// swf::examples::create_simple_red_swf();
/// \endcode
void create_simple_red_swf();

} // namespace examples
} // namespace swf

#endif // RUFFLE_SWF_EXAMPLES_WRITING_H
