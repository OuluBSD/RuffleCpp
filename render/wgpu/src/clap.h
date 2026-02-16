// C++ translation of render/wgpu/src/clap.rs
// Graphics backend and power preference enums for clap CLI

#ifndef RUFFLE_RENDER_WGPU_CLAP_H
#define RUFFLE_RENDER_WGPU_CLAP_H

#include <cstdint>
#include <string>
#include <string_view>
#include <optional>

namespace ruffle {
namespace render {
namespace wgpu {

/// Graphics backend selection for wgpu
enum class GraphicsBackend : uint8_t {
    Default,
    Vulkan,
    Metal,
    Dx12,
    Gl,
};

/// Helper functions for GraphicsBackend
namespace graphics_backend {
    /// Get the backend as a string
    [[nodiscard]] inline const char* as_str(GraphicsBackend backend) {
        switch (backend) {
            case GraphicsBackend::Default:
                return "default";
            case GraphicsBackend::Vulkan:
                return "vulkan";
            case GraphicsBackend::Metal:
                return "metal";
            case GraphicsBackend::Dx12:
                return "dx12";
            case GraphicsBackend::Gl:
                return "gl";
            default:
                return "unknown";
        }
    }

    /// Parse a GraphicsBackend from a string
    [[nodiscard]] inline std::optional<GraphicsBackend> from_str(std::string_view s) {
        if (s == "default") return GraphicsBackend::Default;
        if (s == "vulkan") return GraphicsBackend::Vulkan;
        if (s == "metal") return GraphicsBackend::Metal;
        if (s == "dx12") return GraphicsBackend::Dx12;
        if (s == "gl") return GraphicsBackend::Gl;
        return std::nullopt;
    }
}

/// Power preference selection for wgpu
enum class PowerPreference : uint8_t {
    Low,
    High,
};

/// Helper functions for PowerPreference
namespace power_preference {
    /// Get the preference as a string
    [[nodiscard]] inline const char* as_str(PowerPreference preference) {
        switch (preference) {
            case PowerPreference::High:
                return "high";
            case PowerPreference::Low:
                return "low";
            default:
                return "unknown";
        }
    }

    /// Parse a PowerPreference from a string
    [[nodiscard]] inline std::optional<PowerPreference> from_str(std::string_view s) {
        if (s == "high") return PowerPreference::High;
        if (s == "low") return PowerPreference::Low;
        return std::nullopt;
    }
}

} // namespace wgpu
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_WGPU_CLAP_H
