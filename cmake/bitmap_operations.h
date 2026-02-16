/*
 * C++ header for bitmap operations functionality
 * This replaces the functionality of core/src/bitmap/operations.rs
 */

#ifndef BITMAP_OPERATIONS_H
#define BITMAP_OPERATIONS_H

#include "bitmap_data.h"
#include "turbulence.h"
#include "color.h"
#include "rectangle.h"
#include "matrix.h"
#include "render_backend.h"
#include "transform.h"
#include <memory>
#include <vector>
#include <cstdint>

namespace ruffle {

// Forward declarations
class UpdateContext;
class RenderContext;
class MutationContext;
class RenderBackend;
class BitmapData;
class Color;
class ColorTransform;
class PixelRegion;

// Enum for channel options in bitmap operations
enum class ChannelOptions {
    RED = 1 << 0,
    GREEN = 1 << 1,
    BLUE = 1 << 2,
    ALPHA = 1 << 3
};

// Enum for threshold operations
enum class ThresholdOperation {
    EQUAL,
    NOT_EQUAL,
    LESS_THAN,
    LESS_EQUAL,
    GREATER_THAN,
    GREATER_EQUAL
};

// Helper class for bitmap operations
class BitmapOperations {
public:
    // Fill a rectangle with a color
    static void fill_rect(std::shared_ptr<MutationContext> mc,
                         std::shared_ptr<RenderBackend> renderer,
                         std::shared_ptr<BitmapData> target,
                         int32_t x, int32_t y, 
                         int32_t width, int32_t height,
                         uint32_t color) {
        // Create pixel region for the fill area
        auto rect = PixelRegion::for_region_i32(x, y, width, height);
        rect.clamp(target->width(), target->height());

        if (rect.width() == 0 || rect.height() == 0) {
            return;
        }

        bool is_full = (rect.width() == target->width() && rect.height() == target->height());

        // If we're filling the whole region, we can discard GPU data
        auto target_data = is_full ? 
            target->overwrite_cpu_pixels_from_gpu(mc) : 
            target->sync(renderer);

        auto write = target_data->borrow_mut(mc);
        auto rgba_color = Color::from_rgba(color).to_premultiplied_alpha(write->transparency());

        if (is_full) {
            write->fill(rgba_color);
        } else {
            for (int32_t y_pos = rect.y_min(); y_pos < rect.y_max(); ++y_pos) {
                write->set_pixel32_row_raw(rect.x_min(), rect.x_max(), y_pos, rgba_color);
            }
        }
        
        write->set_cpu_dirty(mc, rect);
    }

    // Set a single pixel with 32-bit color
    static void set_pixel32(std::shared_ptr<MutationContext> mc,
                           std::shared_ptr<RenderBackend> renderer,
                           std::shared_ptr<BitmapData> target,
                           uint32_t x, uint32_t y, uint32_t color) {
        if (x >= target->width() || y >= target->height()) {
            return;
        }
        
        auto synced_target = target->sync(renderer);
        auto write = synced_target->borrow_mut(mc);
        bool transparency = write->transparency();
        
        auto rgba_color = Color::from_rgba(color).to_premultiplied_alpha(transparency);
        write->set_pixel32_raw(x, y, rgba_color);
        write->set_cpu_dirty(mc, PixelRegion::for_pixel(x, y));
    }

    // Get a 32-bit pixel value
    static uint32_t get_pixel32(std::shared_ptr<BitmapData> target,
                               std::shared_ptr<RenderBackend> renderer,
                               uint32_t x, uint32_t y) {
        if (x >= target->width() || y >= target->height()) {
            return 0;
        }
        
        auto read_area = target->read_area(PixelRegion::for_pixel(x, y), renderer);
        auto pixel = read_area->get_pixel32_raw(x, y);
        
        if (read_area->transparency()) {
            return pixel.to_un_multiplied_alpha().to_rgba();
        } else {
            return pixel.to_rgba();
        }
    }

    // Set a single pixel with color object
    static void set_pixel(std::shared_ptr<MutationContext> mc,
                         std::shared_ptr<RenderBackend> renderer,
                         std::shared_ptr<BitmapData> target,
                         uint32_t x, uint32_t y, 
                         std::shared_ptr<Color> color) {
        if (x >= target->width() || y >= target->height()) {
            return;
        }
        
        auto synced_target = target->sync(renderer);
        auto write = synced_target->borrow_mut(mc);

        if (write->transparency()) {
            auto current_alpha = write->get_pixel32_raw(x, y).alpha();
            auto new_color = color->with_alpha(current_alpha).to_premultiplied_alpha(true);
            write->set_pixel32_raw(x, y, new_color);
        } else {
            auto new_color = color->with_alpha(0xFF);
            write->set_pixel32_raw(x, y, new_color);
        }
        
        write->set_cpu_dirty(mc, PixelRegion::for_pixel(x, y));
    }

    // Get a pixel value
    static uint32_t get_pixel(std::shared_ptr<BitmapData> target,
                             std::shared_ptr<RenderBackend> renderer,
                             uint32_t x, uint32_t y) {
        if (x >= target->width() || y >= target->height()) {
            return 0;
        }
        
        auto read_area = target->read_area(PixelRegion::for_pixel(x, y), renderer);
        auto pixel = read_area->get_pixel32_raw(x, y);
        return pixel.to_un_multiplied_alpha().with_alpha(0x0).to_rgba();
    }

    // Flood fill operation
    static bool flood_fill(std::shared_ptr<MutationContext> mc,
                          std::shared_ptr<RenderBackend> renderer,
                          std::shared_ptr<BitmapData> target,
                          uint32_t x, uint32_t y, uint32_t color) {
        if (x >= target->width() || y >= target->height()) {
            return false;
        }
        
        auto synced_target = target->sync(renderer);
        auto write = synced_target->borrow_mut(mc);
        auto expected_color = write->get_pixel32_raw(x, y);
        auto replace_color = Color::from_rgba(color).to_premultiplied_alpha(write->transparency());

        if (expected_color == replace_color) {
            // If we try to replace X with X, we'll infinite loop
            return false;
        }

        std::vector<std::pair<uint32_t, uint32_t>> pending = {{x, y}};
        auto dirty_region = PixelRegion::for_pixel(x, y);

        while (!pending.empty()) {
            auto [current_x, current_y] = pending.back();
            pending.pop_back();
            
            auto old_color = write->get_pixel32_raw(current_x, current_y);
            if (old_color == expected_color) {
                if (current_x > 0) {
                    pending.push_back({current_x - 1, current_y});
                }
                if (current_y > 0) {
                    pending.push_back({current_x, current_y - 1});
                }
                if (current_x < write->width() - 1) {
                    pending.push_back({current_x + 1, current_y});
                }
                if (current_y < write->height() - 1) {
                    pending.push_back({current_x, current_y + 1});
                }
                
                write->set_pixel32_raw(current_x, current_y, replace_color);
                dirty_region.encompass(current_x, current_y);
            }
        }
        
        write->set_cpu_dirty(mc, dirty_region);
        return true;
    }

    // Noise operation
    static void noise(std::shared_ptr<MutationContext> mc,
                     std::shared_ptr<BitmapData> target,
                     int32_t seed, uint8_t low, uint8_t high,
                     ChannelOptions channel_options, bool gray_scale) {
        auto [synced_target, _] = target->overwrite_cpu_pixels_from_gpu(mc);
        auto write = synced_target->borrow_mut(mc);
        bool transparency = write->transparency();

        uint32_t true_seed = seed <= 0 ? static_cast<uint32_t>(-seed + 1) : static_cast<uint32_t>(seed);
        auto rng = LehmerRng::with_seed(true_seed);

        for (uint32_t y = 0; y < write->height(); ++y) {
            for (uint32_t x = 0; x < write->width(); ++x) {
                Color pixel_color;
                
                if (gray_scale) {
                    uint8_t gray = rng.random_range(low, high);
                    uint8_t alpha = (transparency && (static_cast<int>(channel_options) & static_cast<int>(ChannelOptions::ALPHA))) ?
                                   rng.random_range(low, high) : 255;
                    pixel_color = Color::rgb(gray, gray, gray, alpha);
                } else {
                    uint8_t r = (static_cast<int>(channel_options) & static_cast<int>(ChannelOptions::RED)) ?
                               rng.random_range(low, high) : 0;
                    uint8_t g = (static_cast<int>(channel_options) & static_cast<int>(ChannelOptions::GREEN)) ?
                               rng.random_range(low, high) : 0;
                    uint8_t b = (static_cast<int>(channel_options) & static_cast<int>(ChannelOptions::BLUE)) ?
                               rng.random_range(low, high) : 0;
                    uint8_t a = (transparency && (static_cast<int>(channel_options) & static_cast<int>(ChannelOptions::ALPHA))) ?
                               rng.random_range(low, high) : 255;
                    pixel_color = Color::rgb(r, g, b, a);
                }

                write->set_pixel32_raw(x, y, pixel_color.to_premultiplied_alpha(transparency));
            }
        }
        
        auto region = PixelRegion::for_whole_size(write->width(), write->height());
        write->set_cpu_dirty(mc, region);
    }

    // Perlin noise operation
    static void perlin_noise(std::shared_ptr<MutationContext> mc,
                            std::shared_ptr<BitmapData> target,
                            std::pair<double, double> base_freq,
                            size_t num_octaves,
                            int64_t random_seed,
                            bool stitch,
                            bool fractal_noise,
                            ChannelOptions channel_options,
                            bool grayscale,
                            const std::vector<std::pair<double, double>>& offsets) {
        auto [synced_target, _] = target->overwrite_cpu_pixels_from_gpu(mc);
        auto write = synced_target->borrow_mut(mc);

        auto turbulence = Turbulence::from_seed(random_seed);

        auto adjusted_freq = std::make_pair(
            (base_freq.first == 0.0) ? 0.0 : 1.0 / base_freq.first,
            (base_freq.second == 0.0) ? 0.0 : 1.0 / base_freq.second
        );

        for (uint32_t y = 0; y < write->height(); ++y) {
            for (uint32_t x = 0; x < write->width(); ++x) {
                double px = static_cast<double>(x);
                double py = static_cast<double>(y);

                std::array<double, 4> noise = {0.0, 0.0, 0.0, 0.0};

                if (grayscale) {
                    noise[0] = turbulence.turbulence(
                        0, {px, py}, adjusted_freq, num_octaves, fractal_noise, 
                        stitch, {0.0, 0.0}, {static_cast<double>(write->width()), 
                        static_cast<double>(write->height())}, offsets);

                    noise[1] = noise[0];
                    noise[2] = noise[0];

                    noise[3] = (static_cast<int>(channel_options) & static_cast<int>(ChannelOptions::ALPHA)) ?
                        turbulence.turbulence(
                            1, {px, py}, adjusted_freq, num_octaves, fractal_noise,
                            stitch, {0.0, 0.0}, {static_cast<double>(write->width()),
                            static_cast<double>(write->height())}, offsets) : 1.0;
                } else {
                    // Non-grayscale mode
                    int channel = 0;

                    for (int c = 0; c < 4; ++c) {
                        noise[c] = (c == 3) ? 1.0 : -1.0;

                        ChannelOptions option = static_cast<ChannelOptions>(1 << c);
                        if (static_cast<int>(channel_options) & static_cast<int>(option)) {
                            noise[c] = turbulence.turbulence(
                                channel, {px, py}, adjusted_freq, num_octaves, fractal_noise,
                                stitch, {0.0, 0.0}, {static_cast<double>(write->width()),
                                static_cast<double>(write->height())}, offsets);
                            channel++;
                        }
                    }
                }

                std::array<uint8_t, 4> color = {0, 0, 0, 0};
                for (int chan = 0; chan < 4; ++chan) {
                    // Converting the -1..1 or 0..1 floats to u8
                    if (fractal_noise) {
                        color[chan] = static_cast<uint8_t>(
                            ((noise[chan] * 255.0 + 255.0) + 0.5) / 2.0
                        );
                    } else {
                        color[chan] = static_cast<uint8_t>((noise[chan] * 255.0) + 0.5);
                    }
                }

                if (!write->transparency()) {
                    color[3] = 255;
                }

                write->set_pixel32_raw(x, y, Color::rgba(color[0], color[1], color[2], color[3]));
            }
        }
        
        auto region = PixelRegion::for_whole_size(write->width(), write->height());
        write->set_cpu_dirty(mc, region);
    }

    // Copy channel operation
    static void copy_channel(std::shared_ptr<MutationContext> mc,
                            std::shared_ptr<RenderBackend> renderer,
                            std::shared_ptr<BitmapData> target,
                            std::pair<int32_t, int32_t> dest_point,
                            std::pair<int32_t, int32_t, int32_t, int32_t> src_rect,
                            std::shared_ptr<BitmapData> source_bitmap,
                            int32_t source_channel,
                            int32_t dest_channel) {
        auto [min_x, min_y] = dest_point;
        auto [src_min_x, src_min_y, src_width, src_height] = src_rect;

        auto channel_shift = get_channel_shift(source_channel);
        bool transparency = target->transparency();

        auto source_region = PixelRegion::for_whole_size(source_bitmap->width(), source_bitmap->height());
        auto dest_region = PixelRegion::for_whole_size(target->width(), target->height());
        dest_region.clamp_with_intersection(
            {min_x, min_y},
            {src_min_x, src_min_y},
            {src_width, src_height},
            source_region
        );

        if (dest_region.width() == 0 || dest_region.height() == 0) {
            return;
        }

        std::shared_ptr<BitmapData> source = nullptr;
        if (!target->ptr_eq(source_bitmap)) {
            source = source_bitmap->read_area(source_region, renderer);
        }

        auto synced_target = target->sync(renderer);
        auto write = synced_target->borrow_mut(mc);

        for (uint32_t y = 0; y < dest_region.height() && y < source_region.height(); ++y) {
            for (uint32_t x = 0; x < dest_region.width() && x < source_region.width(); ++x) {
                uint32_t dst_x = dest_region.x_min() + x;
                uint32_t dst_y = dest_region.y_min() + y;
                uint32_t src_x = source_region.x_min() + x;
                uint32_t src_y = source_region.y_min() + y;

                uint32_t original_color = static_cast<uint32_t>(
                    write->get_pixel32_raw(dst_x, dst_y).to_un_multiplied_alpha()
                );

                uint32_t source_color = 0;
                if (source) {
                    source_color = static_cast<uint32_t>(
                        source->get_pixel32_raw(src_x, src_y).to_un_multiplied_alpha()
                    );
                } else {
                    source_color = static_cast<uint32_t>(
                        write->get_pixel32_raw(src_x, src_y).to_un_multiplied_alpha()
                    );
                }

                uint8_t source_part = 0;
                if (channel_shift.has_value()) {
                    source_part = (source_color >> channel_shift.value()) & 0xFF;
                }

                uint32_t result_color = apply_dest_channel(original_color, source_part, dest_channel);

                write->set_pixel32_raw(
                    dst_x,
                    dst_y,
                    Color::from_rgba(result_color).to_premultiplied_alpha(transparency)
                );
            }
        }

        write->set_cpu_dirty(mc, dest_region);
    }

    // Color transform operation
    static void color_transform(std::shared_ptr<MutationContext> mc,
                               std::shared_ptr<RenderBackend> renderer,
                               std::shared_ptr<BitmapData> target,
                               uint32_t x_min, uint32_t y_min,
                               uint32_t x_max, uint32_t y_max,
                               const ColorTransform& color_transform) {
        // Flash bug: applying a color transform with only an alpha multiplier > 1 has no effect.
        if (color_transform.r_multiply == Fixed8::ONE &&
            color_transform.g_multiply == Fixed8::ONE &&
            color_transform.b_multiply == Fixed8::ONE &&
            color_transform.a_multiply >= Fixed8::ONE &&
            color_transform.r_add == 0 &&
            color_transform.g_add == 0 &&
            color_transform.b_add == 0 &&
            color_transform.a_add == 0) {
            return;
        }

        // Clamp coordinates to target size
        x_min = std::min(x_min, target->width());
        y_min = std::min(y_min, target->height());
        x_max = std::min(x_max, target->width());
        y_max = std::min(y_max, target->height());

        if (x_max == 0 || y_max == 0 || x_min >= x_max || y_min >= y_max) {
            return;
        }

        auto synced_target = target->sync(renderer);
        auto write = synced_target->borrow_mut(mc);
        bool transparency = write->transparency();

        for (uint32_t y = y_min; y < y_max; ++y) {
            for (uint32_t x = x_min; x < x_max; ++x) {
                auto color = write->get_pixel32_raw(x, y).to_un_multiplied_alpha();
                auto transformed_color = color_transform * Color::from(color);
                write->set_pixel32_raw(
                    x, y,
                    Color::from(transformed_color).to_premultiplied_alpha(transparency)
                );
            }
        }
        
        auto region = PixelRegion::encompassing_pixels({x_min, y_min}, {x_max - 1, y_max - 1});
        write->set_cpu_dirty(mc, region);
    }

    // Threshold operation
    static uint32_t threshold(std::shared_ptr<MutationContext> mc,
                             std::shared_ptr<RenderBackend> renderer,
                             std::shared_ptr<BitmapData> target,
                             std::shared_ptr<BitmapData> source_bitmap,
                             std::pair<int32_t, int32_t, int32_t, int32_t> src_rect,
                             std::pair<int32_t, int32_t> dest_point,
                             ThresholdOperation operation,
                             uint32_t threshold_val,
                             uint32_t color,
                             uint32_t mask,
                             bool copy_source) {
        uint32_t masked_threshold = threshold_val & mask;

        auto [src_min_x, src_min_y, src_width, src_height] = src_rect;
        auto [dest_min_x, dest_min_y] = dest_point;

        uint32_t modified_count = 0;
        std::optional<PixelRegion> dirty_area;

        auto source_region = PixelRegion::for_whole_size(source_bitmap->width(), source_bitmap->height());
        auto dest_region = PixelRegion::for_whole_size(target->width(), target->height());
        dest_region.clamp_with_intersection(
            {dest_min_x, dest_min_y},
            {src_min_x, src_min_y},
            {src_width, src_height},
            source_region
        );

        if (dest_region.width() == 0 || dest_region.height() == 0) {
            return 0;
        }

        std::shared_ptr<BitmapData> source = nullptr;
        if (!source_bitmap->ptr_eq(target)) {
            source = source_bitmap->read_area(source_region, renderer);
        }

        auto synced_target = target->sync(renderer);
        auto write = synced_target->borrow_mut(mc);

        for (uint32_t y = 0; y < dest_region.height(); ++y) {
            for (uint32_t x = 0; x < dest_region.width(); ++x) {
                uint32_t dest_x = dest_region.x_min() + x;
                uint32_t dest_y = dest_region.y_min() + y;
                uint32_t src_x = source_region.x_min() + x;
                uint32_t src_y = source_region.y_min() + y;

                auto source_color = source ? 
                    source->get_pixel32_raw(src_x, src_y) :
                    write->get_pixel32_raw(src_x, src_y);

                uint32_t source_color_val = static_cast<uint32_t>(source_color);
                if (operation_matches(operation, source_color_val & mask, masked_threshold)) {
                    modified_count++;
                    write->set_pixel32_raw(
                        dest_x, dest_y,
                        Color::from_rgba(color).to_premultiplied_alpha(true)  // Always premultiply for threshold
                    );
                } else if (copy_source) {
                    write->set_pixel32_raw(dest_x, dest_y, source_color);
                }

                if (!dirty_area.has_value()) {
                    dirty_area = PixelRegion::for_pixel(dest_x, dest_y);
                } else {
                    dirty_area->encompass(dest_x, dest_y);
                }
            }
        }

        if (dirty_area.has_value()) {
            write->set_cpu_dirty(mc, dirty_area.value());
        }

        return modified_count;
    }

    // Scroll operation
    static void scroll(std::shared_ptr<MutationContext> mc,
                      std::shared_ptr<RenderBackend> renderer,
                      std::shared_ptr<BitmapData> target,
                      int32_t x, int32_t y) {
        int32_t width = static_cast<int32_t>(target->width());
        int32_t height = static_cast<int32_t>(target->height());

        if ((x == 0 && y == 0) || std::abs(x) >= width || std::abs(y) >= height) {
            return; // no-op
        }

        // Since this is an "in-place copy," we have to iterate from bottom to top
        // when scrolling downwards - so if y is positive
        bool reverse_y = y > 0;
        // and if only scrolling horizontally, we have to iterate from right to left
        // when scrolling right - so if x is positive
        bool reverse_x = y == 0 && x > 0;

        // iteration ranges to use as source for the copy, from is inclusive, to is exclusive
        int32_t y_from = reverse_y ? height - y - 1 : -y;
        int32_t y_to = reverse_y ? -1 : height;
        int32_t dy = reverse_y ? -1 : 1;

        int32_t x_from = reverse_x ? width - x - 1 : std::max(-x, 0);
        int32_t x_to = reverse_x ? -1 : std::min(width, width - x);
        int32_t dx = reverse_x ? -1 : 1;

        auto synced_target = target->sync(renderer);
        auto write = synced_target->borrow_mut(mc);

        for (int32_t src_y = y_from; src_y != y_to; src_y += dy) {
            for (int32_t src_x = x_from; src_x != x_to; src_x += dx) {
                if (src_x >= 0 && src_x < width && src_y >= 0 && src_y < height) {
                    auto color = write->get_pixel32_raw(static_cast<uint32_t>(src_x), 
                                                      static_cast<uint32_t>(src_y));
                    uint32_t dest_x = static_cast<uint32_t>(src_x + x);
                    uint32_t dest_y = static_cast<uint32_t>(src_y + y);
                    if (dest_x < target->width() && dest_y < target->height()) {
                        write->set_pixel32_raw(dest_x, dest_y, color);
                    }
                }
            }
        }

        auto region = PixelRegion::for_whole_size(write->width(), write->height());
        write->set_cpu_dirty(mc, region);
    }

    // Palette map operation
    static void palette_map(std::shared_ptr<MutationContext> mc,
                           std::shared_ptr<RenderBackend> renderer,
                           std::shared_ptr<BitmapData> target,
                           std::shared_ptr<BitmapData> source_bitmap,
                           std::pair<int32_t, int32_t, int32_t, int32_t> src_rect,
                           std::pair<int32_t, int32_t> dest_point,
                           std::array<std::array<uint32_t, 256>, 4> channel_arrays) {
        auto [src_min_x, src_min_y, src_width, src_height] = src_rect;
        auto [dest_min_x, dest_min_y] = dest_point;

        auto source_region = PixelRegion::for_whole_size(source_bitmap->width(), source_bitmap->height());
        auto dest_region = PixelRegion::for_whole_size(target->width(), target->height());
        dest_region.clamp_with_intersection(
            {dest_min_x, dest_min_y},
            {src_min_x, src_min_y},
            {src_width, src_height},
            source_region
        );

        if (dest_region.width() == 0 || dest_region.height() == 0) {
            return;
        }

        std::shared_ptr<BitmapData> source = nullptr;
        if (!source_bitmap->ptr_eq(target)) {
            source = source_bitmap->read_area(source_region, renderer);
        }

        auto synced_target = target->sync(renderer);
        auto write = synced_target->borrow_mut(mc);

        for (uint32_t y = 0; y < dest_region.height(); ++y) {
            for (uint32_t x = 0; x < dest_region.width(); ++x) {
                uint32_t dest_x = dest_region.x_min() + x;
                uint32_t dest_y = dest_region.y_min() + y;
                uint32_t src_x = source_region.x_min() + x;
                uint32_t src_y = source_region.y_min() + y;

                auto source_color = source ? 
                    source->get_pixel32_raw(src_x, src_y).to_un_multiplied_alpha() :
                    write->get_pixel32_raw(src_x, src_y).to_un_multiplied_alpha();

                uint8_t r = channel_arrays[0][source_color.red()];
                uint8_t g = channel_arrays[1][source_color.green()];
                uint8_t b = channel_arrays[2][source_color.blue()];
                uint8_t a = channel_arrays[3][source_color.alpha()];

                uint32_t sum = r + g + b + a;
                auto mix_color = Color::from_rgba(sum).to_premultiplied_alpha(true);

                write->set_pixel32_raw(dest_x, dest_y, mix_color);
            }
        }

        write->set_cpu_dirty(mc, dest_region);
    }

    // Compare two bitmap data objects
    static std::optional<std::shared_ptr<BitmapData>> compare(
        std::shared_ptr<MutationContext> mc,
        std::shared_ptr<RenderBackend> renderer,
        std::shared_ptr<BitmapData> left,
        std::shared_ptr<BitmapData> right) {
        
        // This function expects that the two bitmaps have the same dimensions
        if (left->width() != right->width() || left->height() != right->height()) {
            // In a real implementation, this might return a special value
            return std::nullopt;
        }

        auto left_data = left->sync(renderer)->borrow();
        auto right_data = right->sync(renderer)->borrow();

        bool different = false;
        std::vector<uint32_t> pixels;
        
        auto left_pixels = left_data->pixels();
        auto right_pixels = right_data->pixels();
        
        for (size_t i = 0; i < left_pixels.size(); ++i) {
            if (left_pixels[i] != right_pixels[i]) {
                different = true;
                // Calculate difference (simplified)
                pixels.push_back(left_pixels[i] ^ right_pixels[i]); // XOR for difference
            } else {
                pixels.push_back(0); // Same pixels result in 0
            }
        }

        if (!different) {
            return std::nullopt; // Bitmaps are equivalent
        }

        // Create a difference bitmap
        auto diff_bitmap = std::make_shared<BitmapData>(left->width(), left->height(), 
                                                       left->transparency(), 0x00000000);
        diff_bitmap->set_pixels(pixels);
        return diff_bitmap;
    }

private:
    // Helper function to get channel shift value
    static std::optional<uint32_t> get_channel_shift(int32_t channel) {
        switch (channel) {
            case 1: return 16;  // Red
            case 2: return 8;   // Green
            case 4: return 0;   // Blue
            case 8: return 24;  // Alpha
            default: return std::nullopt;
        }
    }

    // Helper function to apply destination channel
    static uint32_t apply_dest_channel(uint32_t original_color, uint8_t source_part, int32_t dest_channel) {
        switch (dest_channel) {
            case 1:  // Red
                return (original_color & 0xFF00FFFF) | (static_cast<uint32_t>(source_part) << 16);
            case 2:  // Green
                return (original_color & 0xFFFF00FF) | (static_cast<uint32_t>(source_part) << 8);
            case 4:  // Blue
                return (original_color & 0xFFFFFF00) | static_cast<uint32_t>(source_part);
            case 8:  // Alpha
                return (original_color & 0x00FFFFFF) | (static_cast<uint32_t>(source_part) << 24);
            default:
                return original_color;
        }
    }

    // Helper function to check if operation matches
    static bool operation_matches(ThresholdOperation op, uint32_t value, uint32_t threshold) {
        switch (op) {
            case ThresholdOperation::EQUAL: return value == threshold;
            case ThresholdOperation::NOT_EQUAL: return value != threshold;
            case ThresholdOperation::LESS_THAN: return value < threshold;
            case ThresholdOperation::LESS_EQUAL: return value <= threshold;
            case ThresholdOperation::GREATER_THAN: return value > threshold;
            case ThresholdOperation::GREATER_EQUAL: return value >= threshold;
            default: return false;
        }
    }
};

} // namespace ruffle

#endif // BITMAP_OPERATIONS_H