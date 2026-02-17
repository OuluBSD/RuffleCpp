// C++ translation of tests/framework/src/environment.rs
// Test environment interface

#ifndef RUFFLE_TESTS_FRAMEWORK_ENVIRONMENT_H
#define RUFFLE_TESTS_FRAMEWORK_ENVIRONMENT_H

#include <string>
#include <memory>
#include <optional>

// Forward declarations
namespace image {
    class RgbaImage;
}

namespace ruffle {
namespace render {
namespace backend {
    class RenderBackend;
}
}
}

namespace ruffle {
namespace tests {
namespace framework {

struct RenderOptions;

/// Environment trait for test execution
/// Checks if this environment supports rendering the given test.
/// 
/// This isn't a guarantee that it _will_ construct a renderer,
/// but rather a check that it theoretically _can_.
/// 
/// This should be a cheap test to filter out test viability early,
/// without creating any expensive rendering overhead.
class Environment {
public:
    virtual ~Environment() = default;

    /// Check if rendering is supported for the given requirements
    /// @param requirements The render options to check
    /// @return true if rendering is supported, false otherwise
    virtual bool is_render_supported(const RenderOptions& requirements) const {
        return false;
    }

    /// Creates a render backend for a new test run.
    /// 
    /// This method must return both a RenderBackend and RenderInterface as a pair.
    /// 
    /// All relevant methods in the RenderInterface will receive the same RenderBackend
    /// that was provided here with that interface.
    /// 
    /// If std::nullopt is returned, a test will be performed without any renderer.
    /// 
    /// If is_render_supported() returned false, this won't be attempted.
    /// 
    /// @param width Viewport width
    /// @param height Viewport height
    /// @return Optional pair of RenderInterface and RenderBackend
    virtual std::optional<std::pair<std::unique_ptr<RenderInterface>, 
                                     std::unique_ptr<render::backend::RenderBackend>>>
    create_renderer(uint32_t width, uint32_t height) {
        return std::nullopt;
    }
};

/// Render interface for capturing test output
class RenderInterface {
public:
    virtual ~RenderInterface() = default;

    /// Gets the name of this environment, for use in test reporting.
    /// 
    /// This name may be used in file paths, so it should contain appropriate characters for such.
    /// @return The environment name
    virtual std::string name() const = 0;

    /// Capture the stage rendered out by the given render backend.
    /// 
    /// The provided backend is guaranteed to be the same one paired with this interface.
    /// 
    /// @param renderer The render backend to capture from
    /// @return The captured RGBA image
    virtual image::RgbaImage capture(render::backend::RenderBackend* renderer) = 0;
};

} // namespace framework
} // namespace tests
} // namespace ruffle

#endif // RUFFLE_TESTS_FRAMEWORK_ENVIRONMENT_H
