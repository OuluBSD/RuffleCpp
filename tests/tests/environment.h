// C++ translation of tests/tests/environment.rs
//! Test environment implementation

#ifndef RUFFLE_TESTS_TESTS_ENVIRONMENT_H
#define RUFFLE_TESTS_TESTS_ENVIRONMENT_H

#include <memory>
#include <string>
#include <optional>
#include <any>
#include <mutex>
#include <atomic>

// Forward declarations
namespace ruffle {
namespace tests {
namespace framework {
    namespace options {
        struct RenderOptions;
    }
    namespace environment {
        class Environment;
        class RenderInterface;
        class RenderBackend;
    }
}
namespace render {
namespace wgpu {
namespace backend {
    class WgpuRenderBackend;
    class Descriptors;
}
namespace descriptors {
    struct Descriptors;
}
namespace target {
    class TextureTarget;
}
}
}
namespace image {
    class RgbaImage;
}
}
}

namespace ruffle {
namespace tests {
namespace tests {

/// Native test environment implementation
class NativeEnvironment {
public:
    NativeEnvironment() = default;

#ifdef RUFFLE_IMGTESTS
    /// Check if rendering is supported with the given requirements
    ///
    /// @param requirements The render options to check
    /// @return true if rendering is supported
    bool is_render_supported(const framework::options::RenderOptions& requirements) const {
        return renderer::is_supported(requirements);
    }

    /// Create a renderer pair (interface + backend)
    ///
    /// @param width The width of the render target
    /// @param height The height of the render target
    /// @return Optional pair of RenderInterface and RenderBackend, or nullopt if not supported
    std::optional<std::pair<
        std::unique_ptr<framework::environment::RenderInterface>,
        std::unique_ptr<framework::environment::RenderBackend>
    >> create_renderer(uint32_t width, uint32_t height) const {
        return renderer::NativeRenderInterface::create_pair(width, height);
    }
#endif
};

#ifdef RUFFLE_IMGTESTS
/// Renderer module for native environment
namespace renderer {

/// Native render interface implementation
class NativeRenderInterface : public framework::environment::RenderInterface {
public:
    NativeRenderInterface() = default;

    /// Create a pair of RenderInterface and RenderBackend
    ///
    /// @param width The width of the render target
    /// @param height The height of the render target
    /// @return Optional pair, or nullopt if WGPU is not available
    static std::optional<std::pair<
        std::unique_ptr<framework::environment::RenderInterface>,
        std::unique_ptr<framework::environment::RenderBackend>
    >> create_pair(uint32_t width, uint32_t height) {
        auto descriptors = get_descriptors();
        if (!descriptors) {
            return std::nullopt;
        }

        // Create texture target
        // Note: This is a simplified version - real implementation would use actual WGPU types
        auto target = std::make_unique<void>(); // Placeholder for TextureTarget

        auto interface = std::make_unique<NativeRenderInterface>();
        // auto backend = std::make_unique<WgpuRenderBackend<TextureTarget>>(descriptors, target);

        return std::make_pair(std::move(interface), std::unique_ptr<framework::environment::RenderBackend>());
    }

    /// Get the name of this render interface
    std::string name() const override {
        auto descriptors = get_descriptors();
        if (descriptors) {
            // Would get adapter info from descriptors
            return std::string(get_os_name()) + "-WGPU";
        }
        return std::string(get_os_name());
    }

    /// Capture the current frame
    ///
    /// @param backend The render backend to capture from
    /// @return The captured RGBA image
    image::RgbaImage capture(framework::environment::RenderBackend* backend) override {
        // Downcast to WgpuRenderBackend and capture frame
        // auto* renderer = dynamic_cast<WgpuRenderBackend<TextureTarget>*>(backend);
        // return renderer->capture_frame();
        return image::RgbaImage(); // Stub
    }
};

/// Check if the renderer is supported with the given requirements
///
/// @param requirements The render options to check
/// @return true if supported
bool is_supported(const framework::options::RenderOptions& requirements);

/// Get or create WGPU descriptors (singleton pattern)
///
/// Descriptors are expensive to create, so we cache them across tests.
/// This is important for `cargo test` which runs all tests in the same process.
/// For `cargo nextest run` which creates a different process per test,
/// the cost is incurred per test but only if the test actually uses WGPU.
///
/// @return Shared pointer to descriptors, or nullptr if WGPU is not available
std::shared_ptr<render::wgpu::descriptors::Descriptors> get_descriptors();

/// Create WGPU device (instance, adapter, device, queue)
///
/// @return Tuple of (instance, adapter, device, queue) or nullopt on failure
std::optional<std::tuple<
    void*, // wgpu::Instance (placeholder)
    void*, // wgpu::Adapter (placeholder)
    void*, // wgpu::Device (placeholder)
    void*  // wgpu::Queue (placeholder)
>> create_wgpu_device();

/// Build WGPU descriptors from device
///
/// @return Shared pointer to descriptors, or nullptr on failure
std::shared_ptr<render::wgpu::descriptors::Descriptors> build_wgpu_descriptors();

/// Get the OS name
inline const char* get_os_name() {
#if defined(_WIN32)
    return "Windows";
#elif defined(__APPLE__)
    return "macOS";
#elif defined(__linux__)
    return "Linux";
#else
    return "Unknown";
#endif
}

} // namespace renderer
#endif // RUFFLE_IMGTESTS

} // namespace tests
} // namespace tests
} // namespace ruffle

#endif // RUFFLE_TESTS_TESTS_ENVIRONMENT_H
