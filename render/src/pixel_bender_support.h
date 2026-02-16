// C++ translation of render/src/pixel_bender_support.rs
// Pixel Bender shader support types

#ifndef RUFFLE_RENDER_PIXEL_BENDER_SUPPORT_H
#define RUFFLE_RENDER_PIXEL_BENDER_SUPPORT_H

#include <vector>
#include <string>
#include <variant>
#include <cstdint>
#include <memory>

// Forward declarations
namespace ruffle {
namespace render {
    struct BitmapHandle;
    class RawTexture;
}
namespace pixel_bender {
    enum class PixelBenderType;
}
}

namespace ruffle {
namespace render {

/// Float pixel data storage
/// Stores pixel data as arrays of f32 values with 1-4 channels
enum class FloatPixelData {
    R,      // Single channel (red)
    Rg,     // Two channels (red, green)
    Rgb,    // Three channels (red, green, blue)
    Rgba    // Four channels (red, green, blue, alpha)
};

/// An image input for Pixel Bender shaders
///
/// This accepts both an owned BitmapHandle, and a borrowed texture
/// (used when applying a filter to a texture that we don't have
/// ownership of, and therefore cannot construct a BitmapHandle for).
class ImageInputTexture {
public:
    /// Bitmap handle variant - owns the bitmap
    struct BitmapVariant {
        BitmapHandle handle;
    };

    /// Texture reference variant - borrows a texture
    struct TextureRefVariant {
        const RawTexture* texture;
    };

    /// Float data variant - raw float pixel data
    struct FloatsVariant {
        uint32_t width;
        uint32_t height;
        FloatPixelData channel_type;
        std::vector<float> data;  // Packed channel data
    };

private:
    std::variant<BitmapVariant, TextureRefVariant, FloatsVariant> data_;

public:
    /// Construct from a BitmapHandle
    explicit ImageInputTexture(const BitmapHandle& bitmap);

    /// Construct from a texture reference
    explicit ImageInputTexture(const RawTexture* texture);

    /// Construct from float data
    ImageInputTexture(uint32_t width, uint32_t height, FloatPixelData channel_type, std::vector<float> data);

    /// Check if this is a bitmap variant
    bool is_bitmap() const;

    /// Check if this is a texture reference variant
    bool is_texture_ref() const;

    /// Check if this is a floats variant
    bool is_floats() const;

    /// Get the bitmap handle (if this is a bitmap variant)
    const BitmapHandle* get_bitmap() const;

    /// Get the texture reference (if this is a texture ref variant)
    const RawTexture* get_texture_ref() const;

    /// Get the float data info (if this is a floats variant)
    const FloatsVariant* get_floats() const;

    /// Equality comparison
    bool operator==(const ImageInputTexture& other) const;

    /// Inequality comparison
    bool operator!=(const ImageInputTexture& other) const {
        return !(*this == other);
    }
};

/// Pixel Bender shader argument
/// Represents either an image input or a value input to a Pixel Bender shader
class PixelBenderShaderArgument {
public:
    /// Image input argument
    struct ImageInput {
        uint8_t index;
        uint8_t channels;
        std::string name;
        std::optional<ImageInputTexture> texture;
    };

    /// Value input argument
    struct ValueInput {
        uint8_t index;
        pixel_bender::PixelBenderType value;
    };

private:
    std::variant<ImageInput, ValueInput> data_;

public:
    /// Create an image input argument
    static PixelBenderShaderArgument image_input(
        uint8_t index,
        uint8_t channels,
        const std::string& name,
        const std::optional<ImageInputTexture>& texture = std::nullopt
    );

    /// Create a value input argument
    static PixelBenderShaderArgument value_input(
        uint8_t index,
        pixel_bender::PixelBenderType value
    );

    /// Check if this is an image input
    bool is_image_input() const;

    /// Check if this is a value input
    bool is_value_input() const;

    /// Get the image input (if this is an image input variant)
    const ImageInput* get_image_input() const;

    /// Get the value input (if this is a value input variant)
    const ValueInput* get_value_input() const;
};

/// Get the number of channels for a FloatPixelData type
///
/// \param data The float pixel data type
/// \return The number of channels (1-4)
inline uint32_t get_channel_count(FloatPixelData data) {
    switch (data) {
        case FloatPixelData::R: return 1;
        case FloatPixelData::Rg: return 2;
        case FloatPixelData::Rgb: return 3;
        case FloatPixelData::Rgba: return 4;
    }
    return 0;  // Should never happen
}

} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_PIXEL_BENDER_SUPPORT_H
