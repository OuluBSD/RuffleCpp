/*
 * C++ header for bitmap data functionality
 * This replaces the functionality of core/src/bitmap/bitmap_data.rs
 */

#ifndef BITMAP_DATA_H
#define BITMAP_DATA_H

#include "bitmap.h"
#include "avm1.h"
#include "avm2.h"
#include "context.h"
#include "color.h"
#include "rectangle.h"
#include "point.h"
#include "matrix.h"
#include <memory>
#include <vector>
#include <cstdint>

namespace ruffle {

// Forward declarations
class UpdateContext;
class RenderContext;
class Avm2Activation;

// Enum for bitmap data copying options
enum class BitmapDataCopyOption {
    COPY,
    CLONE,
    RESIZE
};

// BitmapData class for handling bitmap image data
class BitmapData {
private:
    uint32_t width_;
    uint32_t height_;
    bool transparent_;
    uint32_t background_color_;
    std::vector<uint32_t> pixels_;  // ARGB format
    std::vector<std::weak_ptr<DisplayObject>> display_objects_;  // Objects using this bitmap data

public:
    BitmapData(uint32_t width, uint32_t height, bool transparent = true, uint32_t background_color = 0x00000000)
        : width_(width), height_(height), transparent_(transparent), 
          background_color_(background_color) {
        pixels_.resize(width * height, background_color);
    }

    // Static factory method
    static std::shared_ptr<BitmapData> create(uint32_t width, uint32_t height, 
                                           bool transparent = true, 
                                           uint32_t background_color = 0x00000000) {
        return std::make_shared<BitmapData>(width, height, transparent, background_color);
    }

    // Create with specific pixel data
    static std::shared_ptr<BitmapData> create_with_pixels(uint32_t width, uint32_t height,
                                                        bool transparent,
                                                        const std::vector<uint32_t>& pixels) {
        auto bitmap_data = std::make_shared<BitmapData>(width, height, transparent, 0x00000000);
        bitmap_data->pixels_ = pixels;
        return bitmap_data;
    }

    // Getters
    uint32_t width() const { return width_; }
    uint32_t height() const { return height_; }
    bool is_transparent() const { return transparent_; }
    uint32_t background_color() const { return background_color_; }
    const std::vector<uint32_t>& pixels() const { return pixels_; }
    std::vector<uint32_t>& pixels() { return pixels_; }

    // Get a pixel at specific coordinates
    uint32_t get_pixel(int32_t x, int32_t y) const {
        if (x >= 0 && x < static_cast<int32_t>(width_) && 
            y >= 0 && y < static_cast<int32_t>(height_)) {
            size_t index = y * width_ + x;
            return pixels_[index];
        }
        return 0; // Return transparent/black for out of bounds
    }

    // Set a pixel at specific coordinates
    void set_pixel(int32_t x, int32_t y, uint32_t color) {
        if (x >= 0 && x < static_cast<int32_t>(width_) && 
            y >= 0 && y < static_cast<int32_t>(height_)) {
            size_t index = y * width_ + x;
            pixels_[index] = color;
        }
    }

    // Fill a rectangle with a color
    void fill_rect(const Rectangle<int32_t>& rect, uint32_t color) {
        int32_t x_min = std::max(0, rect.x_min);
        int32_t y_min = std::max(0, rect.y_min);
        int32_t x_max = std::min(static_cast<int32_t>(width_), rect.x_max);
        int32_t y_max = std::min(static_cast<int32_t>(height_), rect.y_max);

        for (int32_t y = y_min; y < y_max; ++y) {
            for (int32_t x = x_min; x < x_max; ++x) {
                set_pixel(x, y, color);
            }
        }
    }

    // Copy pixels from another bitmap data
    void copy_pixels(std::shared_ptr<BitmapData> source_bitmap,
                    const Rectangle<int32_t>& source_rect,
                    const Point<int32_t>& dest_point,
                    std::shared_ptr<BitmapData> alpha_bitmap = nullptr,
                    const Point<int32_t>& alpha_point = Point<int32_t>(0, 0),
                    uint32_t merge_alpha = false) {
        
        int32_t src_x = std::max(0, source_rect.x_min);
        int32_t src_y = std::max(0, source_rect.y_min);
        int32_t src_x_max = std::min(static_cast<int32_t>(source_bitmap->width()), source_rect.x_max);
        int32_t src_y_max = std::min(static_cast<int32_t>(source_bitmap->height()), source_rect.y_max);

        int32_t dst_x_offset = dest_point.x - source_rect.x_min;
        int32_t dst_y_offset = dest_point.y - source_rect.y_min;

        for (int32_t src_y_pos = src_y; src_y_pos < src_y_max; ++src_y_pos) {
            for (int32_t src_x_pos = src_x; src_x_pos < src_x_max; ++src_x_pos) {
                int32_t dst_x_pos = src_x_pos + dst_x_offset;
                int32_t dst_y_pos = src_y_pos + dst_y_offset;

                if (dst_x_pos >= 0 && dst_x_pos < static_cast<int32_t>(width_) &&
                    dst_y_pos >= 0 && dst_y_pos < static_cast<int32_t>(height_)) {
                    
                    uint32_t color = source_bitmap->get_pixel(src_x_pos, src_y_pos);
                    
                    // Apply alpha blending if alpha bitmap is provided
                    if (alpha_bitmap) {
                        int32_t alpha_x = src_x_pos + (alpha_point.x - source_rect.x_min);
                        int32_t alpha_y = src_y_pos + (alpha_point.y - source_rect.y_min);
                        
                        if (alpha_x >= 0 && alpha_x < static_cast<int32_t>(alpha_bitmap->width()) &&
                            alpha_y >= 0 && alpha_y < static_cast<int32_t>(alpha_bitmap->height())) {
                            
                            uint32_t alpha_color = alpha_bitmap->get_pixel(alpha_x, alpha_y);
                            uint8_t alpha = (alpha_color >> 24) & 0xFF;
                            
                            if (merge_alpha) {
                                uint8_t existing_alpha = (color >> 24) & 0xFF;
                                uint8_t final_alpha = (existing_alpha * alpha) / 255;
                                color = (color & 0x00FFFFFF) | (final_alpha << 24);
                            } else {
                                color = (color & 0x00FFFFFF) | (alpha << 24);
                            }
                        }
                    }
                    
                    set_pixel(dst_x_pos, dst_y_pos, color);
                }
            }
        }
    }

    // Clone this bitmap data
    std::shared_ptr<BitmapData> clone() const {
        auto new_bitmap = std::make_shared<BitmapData>(width_, height_, transparent_, background_color_);
        new_bitmap->pixels_ = pixels_;
        return new_bitmap;
    }

    // Resize the bitmap data
    void resize(uint32_t new_width, uint32_t new_height) {
        if (new_width == width_ && new_height == height_) {
            return; // No change needed
        }

        std::vector<uint32_t> new_pixels(new_width * new_height, background_color_);

        // Copy existing pixels to the new array
        uint32_t copy_width = std::min(width_, new_width);
        uint32_t copy_height = std::min(height_, new_height);

        for (uint32_t y = 0; y < copy_height; ++y) {
            for (uint32_t x = 0; x < copy_width; ++x) {
                size_t old_index = y * width_ + x;
                size_t new_index = y * new_width + x;
                new_pixels[new_index] = pixels_[old_index];
            }
        }

        width_ = new_width;
        height_ = new_height;
        pixels_ = std::move(new_pixels);
    }

    // Add a display object that uses this bitmap data
    void add_display_object(std::weak_ptr<DisplayObject> display_object) {
        display_objects_.push_back(display_object);
    }

    // Remove a display object from the list
    void remove_display_object(std::weak_ptr<DisplayObject> display_object) {
        auto it = std::find_if(display_objects_.begin(), display_objects_.end(),
            [&display_object](const std::weak_ptr<DisplayObject>& obj) {
                return obj.lock() == display_object.lock();
            });
        if (it != display_objects_.end()) {
            display_objects_.erase(it);
        }
    }

    // Invalidate all display objects that use this bitmap data
    void invalidate_all_display_objects() {
        for (auto& weak_obj : display_objects_) {
            if (auto obj = weak_obj.lock()) {
                obj->invalidate_cached_bitmap();
            }
        }
    }

    // Get the bounds of this bitmap data
    Rectangle<int32_t> get_rect() const {
        return Rectangle<int32_t>(0, 0, width_, height_);
    }

    // Check if coordinates are within bounds
    bool in_bounds(int32_t x, int32_t y) const {
        return x >= 0 && x < static_cast<int32_t>(width_) &&
               y >= 0 && y < static_cast<int32_t>(height_);
    }

    // Clamp coordinates to valid range
    Point<int32_t> clamp_coordinates(int32_t x, int32_t y) const {
        x = std::max(0, std::min(x, static_cast<int32_t>(width_) - 1));
        y = std::max(0, std::min(y, static_cast<int32_t>(height_) - 1));
        return Point<int32_t>(x, y);
    }

    // Render this bitmap data to a context
    void render(std::shared_ptr<RenderContext> context, 
               const Matrix& transform, 
               bool smoothing = false) const {
        // In a real implementation, this would render the bitmap to the context
        // using the provided transform and smoothing settings
        // For now, we'll just note that this would happen
    }

    // Post instantiation setup for AVM2
    void post_instantiation_avm2(std::shared_ptr<Avm2Activation> activation) {
        // In a real implementation, this would set up the AVM2 object
        // and perform any necessary initialization
    }
};

} // namespace ruffle

#endif // BITMAP_DATA_H