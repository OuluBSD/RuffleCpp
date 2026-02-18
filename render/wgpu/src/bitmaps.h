// C++ translation of render/wgpu/src/bitmaps.rs
// Bitmap samplers for WGPU rendering

#ifndef RUFFLE_RENDER_WGPU_BITMAPS_H
#define RUFFLE_RENDER_WGPU_BITMAPS_H

#include <unordered_map>
#include <string>
#include <cstdint>

// Forward declarations for wgpu types
namespace wgpu {
    class Device;
    class Sampler;
    enum class AddressMode;
    enum class FilterMode;
}

namespace ruffle {
namespace render {
namespace wgpu {

/// Sampler configuration for anisotropic filtering
struct WgpuSamplerConfig {
    uint8_t anisotropy_clamp;
    wgpu::AddressMode address_mode_u;
    wgpu::AddressMode address_mode_v;

    bool operator==(const WgpuSamplerConfig& other) const {
        return anisotropy_clamp == other.anisotropy_clamp &&
               address_mode_u == other.address_mode_u &&
               address_mode_v == other.address_mode_v;
    }
};

/// Hash function for WgpuSamplerConfig
struct WgpuSamplerConfigHash {
    std::size_t operator()(const WgpuSamplerConfig& config) const {
        size_t h1 = std::hash<uint8_t>{}(config.anisotropy_clamp);
        size_t h2 = std::hash<int>{}(static_cast<int>(config.address_mode_u));
        size_t h3 = std::hash<int>{}(static_cast<int>(config.address_mode_v));
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

/// Bitmap samplers for different filtering and addressing modes
class BitmapSamplers {
public:
    wgpu::Sampler repeat_linear;
    wgpu::Sampler repeat_nearest;
    wgpu::Sampler clamp_linear;
    wgpu::Sampler clamp_nearest;
    wgpu::Sampler clamp_u_repeat_v_linear;
    wgpu::Sampler clamp_u_repeat_v_nearest;
    wgpu::Sampler repeat_u_clamp_v_linear;
    wgpu::Sampler repeat_u_clamp_v_nearest;
    std::unordered_map<WgpuSamplerConfig, wgpu::Sampler, WgpuSamplerConfigHash> anisotropic;

    /// Create bitmap samplers for a device
    explicit BitmapSamplers(wgpu::Device* device);

    /// Get sampler based on repeating and smoothing settings
    const wgpu::Sampler* get_sampler(bool is_repeating, bool is_smoothed) const;

private:
    /// Create a sampler with specified parameters
    static wgpu::Sampler create_sampler(
        wgpu::Device* device,
        wgpu::AddressMode address_mode_u,
        wgpu::AddressMode address_mode_v,
        wgpu::FilterMode filter,
        const std::string& sampler_label
    );
};

} // namespace wgpu
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_WGPU_BITMAPS_H
