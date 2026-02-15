/*
 * C++ header for bitmap functionality
 * This replaces the functionality of core/src/bitmap.rs
 */

#ifndef BITMAP_H
#define BITMAP_H

#include <cstdint>
#include <memory>

namespace ruffle {

// Forward declarations for bitmap modules
class BitmapData;
class BitmapOperations;
class Turbulence;

// Determine if a particular bitmap data size is valid.
//
// This enforces limits on BitmapData as specified in the Flash documentation.
// Specifically, from <https://help.adobe.com/en_US/FlashPlatform/reference/actionscript/3/flash/display/BitmapData.html>:
//
// "In AIR 1.5 and Flash Player 10, the maximum size for a BitmapData object
// is 8,191 pixels in width or height, and the total number of pixels cannot
// exceed 16,777,215 pixels. (So, if a BitmapData object is 8,191 pixels wide,
// it can only be 2,048 pixels high.) In Flash Player 9 and earlier and
// AIR 1.1 and earlier, the limitation is 2,880 pixels in height and 2,880 in
// width. Starting with AIR 3 and Flash player 11, the size limits for a
// BitmapData object have been removed. The maximum size of a bitmap is now
// dependent on the operating system."
//
// In addition, we found the following undocumented limits:
//
//  - Width and height of 0 are invalid in all versions
//  - Widths and heights exceeding 0x666666 are invalid in all versions
//  - Pixel counts (of any width/height) exceeding 0x20000000 pixels
//
// All of these are currently enforced.
inline bool is_size_valid(uint8_t swf_version, uint32_t width, uint32_t height) {
    // From Flash documentation:
    //
    // In addition, width and height of 0 are invalid in all versions.
    if (width == 0 || height == 0) {
        return false;
    }
    
    if (swf_version <= 9) {
        if (width > 2880 || height > 2880) {
            return false;
        }
    } else if (swf_version <= 12) {
        if (width >= 0x2000 || height >= 0x2000 || width * height >= 0x1000000) {
            return false;
        }
    } else {
        // These limits are undocumented, but seem to be reliable.
        // TODO: Do they vary across different machines?
        if (width > 0x6666666 || height > 0x6666666 || 
            static_cast<uint64_t>(width) * static_cast<uint64_t>(height) >= 0x20000000) {
            return false;
        }
    }
    
    return true;
}

// Main bitmap namespace
namespace bitmap {

// Import the modules
using BitmapData = ruffle::BitmapData;
using Operations = ruffle::BitmapOperations;
using Turbulence = ruffle::Turbulence;

// Helper class for bitmap operations
class BitmapUtils {
public:
    // Validate bitmap dimensions for a given SWF version
    static bool validate_size(uint8_t swf_version, uint32_t width, uint32_t height) {
        return is_size_valid(swf_version, width, height);
    }
    
    // Calculate the total number of pixels
    static uint64_t pixel_count(uint32_t width, uint32_t height) {
        return static_cast<uint64_t>(width) * static_cast<uint64_t>(height);
    }
    
    // Check if dimensions are within safe limits (not considering SWF version)
    static bool is_safe_size(uint32_t width, uint32_t height) {
        // Reasonable safety limits to prevent excessive memory allocation
        const uint32_t MAX_DIMENSION = 16384; // 16K x 16K should be plenty
        const uint64_t MAX_PIXELS = 268435456ULL; // 256M pixels max
        
        if (width > MAX_DIMENSION || height > MAX_DIMENSION) {
            return false;
        }
        
        return pixel_count(width, height) <= MAX_PIXELS;
    }
};

} // namespace bitmap

} // namespace ruffle

#endif // BITMAP_H