// C++ translation of render/src/bitmap.rs
// Bitmap handling for rendering

#ifndef RUFFLE_RENDER_BITMAP_H
#define RUFFLE_RENDER_BITMAP_H

#include <cstdint>
#include <memory>
#include <vector>
#include <array>
#include <functional>
#include <variant>

// Forward declarations
namespace swf {
    struct Rectangle;
    class Twips;
}

namespace ruffle {
namespace render {

// Forward declarations
class RenderBackend;
class Matrix;
class SyncHandle;

/// Bitmap handle - reference to a registered bitmap
struct BitmapHandle {
    std::shared_ptr<void> impl;

    bool operator==(const BitmapHandle& other) const {
        return impl == other.impl;
    }

    bool operator!=(const BitmapHandle& other) const {
        return impl != other.impl;
    }
};

/// Bitmap handle implementation trait
class BitmapHandleImpl {
public:
    virtual ~BitmapHandleImpl() = default;
};

/// Bitmap info returned from register_bitmap
struct BitmapInfo {
    BitmapHandle handle;
    uint32_t width;
    uint32_t height;
};

/// Bitmap size
struct BitmapSize {
    uint32_t width;
    uint32_t height;
};

/// Bitmap source interface - returns bitmaps by ID
class BitmapSource {
public:
    virtual ~BitmapSource() = default;

    virtual std::optional<BitmapSize> bitmap_size(uint16_t id) const = 0;
    virtual std::optional<BitmapHandle> bitmap_handle(
        uint16_t id,
        RenderBackend* renderer
    ) const = 0;
};

/// RGBA buffer read callback
using RgbaBufRead = std::function<void(const uint8_t*, uint32_t)>;

/// Sync handle for async operations
class SyncHandle {
public:
    virtual ~SyncHandle() = default;
};

/// Pixel snapping mode for bitmap rendering
enum class PixelSnapping {
    /// Always snap the bitmap to the pixel grid
    Always,

    /// Only snap if no rotation/skew and ~100% scale
    Auto,

    /// Never snap the bitmap to the pixel grid
    Never
};

/// Pixel format of bitmap data
enum class BitmapFormat {
    /// 24-bit RGB
    Rgb,

    /// 32-bit RGBA with premultiplied alpha
    Rgba,

    /// Planar YUV 420
    Yuv420p,

    /// Planar YUV 420 with alpha (RGB clamped after conversion)
    Yuva420p
};

/// Decoded bitmap data from SWF tag
class Bitmap {
public:
    Bitmap(uint32_t width, uint32_t height, BitmapFormat format, const std::vector<uint8_t>& data);
    Bitmap(uint32_t width, uint32_t height, BitmapFormat format, std::vector<uint8_t>&& data);

    /// Convert to RGB format
    Bitmap to_rgb() &&;

    /// Convert to RGBA format
    Bitmap to_rgba() &&;

    uint32_t width() const { return width_; }
    uint32_t height() const { return height_; }
    BitmapFormat format() const { return format_; }
    const uint8_t* data() const { return data_.data(); }
    const std::vector<uint8_t>& data_vec() const { return data_; }

    /// Get chroma width for YUV formats
    uint32_t chroma_width() const;

    /// Get chroma height for YUV formats
    uint32_t chroma_height() const;

    /// Iterate over colors as u32 (RGB or RGBA)
    class ColorIterator {
    public:
        ColorIterator(const Bitmap* bitmap, size_t index)
            : bitmap_(bitmap), index_(index) {}

        uint32_t operator*() const;
        ColorIterator& operator++() { index_++; return *this; }
        bool operator!=(const ColorIterator& other) const { return index_ != other.index_; }

    private:
        const Bitmap* bitmap_;
        size_t index_;
    };

    ColorIterator begin() const { return ColorIterator(this, 0); }
    ColorIterator end() const { return ColorIterator(this, data_.size() / bytes_per_pixel()); }

private:
    size_t bytes_per_pixel() const {
        return format_ == BitmapFormat::Rgb ? 3 : 4;
    }

    uint32_t width_;
    uint32_t height_;
    BitmapFormat format_;
    std::vector<uint8_t> data_;
};

/// Pixel region for bitmap operations
struct PixelRegion {
    uint32_t x_min;
    uint32_t y_min;
    uint32_t x_max;
    uint32_t y_max;

    /// Create region encompassing two Twips points
    static PixelRegion encompassing_twips(
        const std::pair<swf::Twips, swf::Twips>& a,
        const std::pair<swf::Twips, swf::Twips>& b
    );

    /// Create region from i32 coordinates
    static PixelRegion for_region_i32(int32_t x, int32_t y, int32_t width, int32_t height);

    /// Create region from u32 coordinates
    static PixelRegion for_region(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

    /// Create region encompassing two pixel points
    static PixelRegion encompassing_pixels_i32(
        const std::pair<int32_t, int32_t>& a,
        const std::pair<int32_t, int32_t>& b
    );

    /// Create region encompassing two pixel points (u32)
    static PixelRegion encompassing_pixels(
        const std::pair<uint32_t, uint32_t>& a,
        const std::pair<uint32_t, uint32_t>& b
    );

    /// Create region for whole size
    static PixelRegion for_whole_size(uint32_t width, uint32_t height);

    /// Create region for single pixel
    static PixelRegion for_pixel(uint32_t x, uint32_t y);

    /// Clamp region to bounds
    void clamp(uint32_t width, uint32_t height);

    /// Union with another region
    void union_with(const PixelRegion& other);

    /// Encompass a pixel
    void encompass(uint32_t x, uint32_t y);

    /// Check if intersects with another region
    bool intersects(const PixelRegion& other) const;

    uint32_t width() const { return x_max - x_min; }
    uint32_t height() const { return y_max - y_min; }

    /// Clamp with intersection for overlapping regions
    void clamp_with_intersection(
        const std::pair<int32_t, int32_t>& self_point,
        const std::pair<int32_t, int32_t>& other_point,
        const std::pair<int32_t, int32_t>& size,
        PixelRegion& other
    );

    bool operator==(const PixelRegion& other) const = default;
};

/// Convert SWF Rectangle to PixelRegion
PixelRegion pixel_region_from_rectangle(const swf::Rectangle<swf::Twips>& rect);

/// Apply pixel snapping to matrix
void apply_pixel_snapping(PixelSnapping mode, Matrix& matrix);

} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_BITMAP_H
