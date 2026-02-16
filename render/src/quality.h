// C++ translation of render/src/quality.rs
// The quality setting of the Stage

#ifndef RUFFLE_RENDER_QUALITY_H
#define RUFFLE_RENDER_QUALITY_H

#include <string>
#include <cstdint>

// Forward declarations
namespace ruffle {
namespace wstr {
    class WStr;
}
}

namespace ruffle {
namespace render {

/// The quality setting of the Stage
///
/// In the Flash Player, this setting affects anti-aliasing and bitmap smoothing
enum class StageQuality {
    /// No anti-aliasing, and bitmaps are never smoothed
    Low,

    /// 2x anti-aliasing
    Medium,

    /// 4x anti-aliasing (default)
    High,

    /// 4x anti-aliasing with high quality downsampling
    Best,

    /// 8x anti-aliasing
    High8x8,

    /// 8x anti-aliasing done in linear sRGB space
    High8x8Linear,

    /// 16x anti-aliasing
    High16x16,

    /// 16x anti-aliasing done in linear sRGB space
    High16x16Linear,
};

/// Default value is High
inline StageQuality get_default_stage_quality() {
    return StageQuality::High;
}

impl StageQuality {
    /// Returns the string representing the quality setting as returned by AVM1 _quality and
    /// AVM2 Stage.quality
    /// Flash Player always returns quality in uppercase
    const char* to_avm_str() const {
        switch (*this) {
            case StageQuality::Low: return "LOW";
            case StageQuality::Medium: return "MEDIUM";
            case StageQuality::High: return "HIGH";
            case StageQuality::Best: return "BEST";
            case StageQuality::High8x8:
            case StageQuality::High8x8Linear: return "8X8";
            case StageQuality::High16x16:
            case StageQuality::High16x16Linear: return "16X16";
        }
    }

    /// Returns the preferred anti-aliasing sample count for this quality
    uint32_t sample_count() const {
        switch (*this) {
            case StageQuality::Low: return 1;
            case StageQuality::Medium: return 2;
            case StageQuality::High: return 4;
            case StageQuality::Best: return 4;
            case StageQuality::High8x8:
            case StageQuality::High8x8Linear: return 8;
            case StageQuality::High16x16:
            case StageQuality::High16x16Linear: return 16;
        }
    }
};

/// String representation matching AS values
std::string to_string(StageQuality quality);

/// Parse from string - returns false on error
bool from_string(const std::string& s, StageQuality& out);

/// Parse from WStr - returns false on error
bool from_wstr(const ruffle::wstr::WStr& s, StageQuality& out);

} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_QUALITY_H
