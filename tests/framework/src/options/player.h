// C++ translation of tests/framework/src/options/player.rs
// Player options for test framework

#ifndef RUFFLE_TESTS_FRAMEWORK_OPTIONS_PLAYER_H
#define RUFFLE_TESTS_FRAMEWORK_OPTIONS_PLAYER_H

#include <optional>
#include <string>
#include <chrono>
#include <memory>

// Forward declarations
namespace ruffle {
namespace core {
    class PlayerBuilder;
    enum class PlayerRuntime;
    enum class PlayerMode;
    namespace tag_utils {
        class SwfMovie;
    }
}
namespace render {
namespace backend {
    class RenderBackend;
    struct ViewportDimensions;
}
}
}

namespace ruffle {
namespace tests {
namespace framework {

// Forward declarations for test environment
class Environment;
class RenderInterface;

/// Render options for tests
struct RenderOptions {
    // Quality setting would be defined here
    // This is a placeholder for the actual render options
    int quality() const { return 3; } // Default high quality
};

/// Player options for test framework
///
/// This class configures how the Ruffle player is set up for testing.
/// It allows configuring execution duration, renderer, audio, video,
/// runtime, version, and other player settings.
class PlayerOptions {
private:
    std::optional<std::chrono::milliseconds> max_execution_duration_;
    std::optional<ruffle::core::render::backend::ViewportDimensions> viewport_dimensions_;
    std::optional<RenderOptions> with_renderer_;
    bool with_audio_ = false;
    bool with_video_ = false;
    ruffle::core::PlayerRuntime runtime_;
    std::optional<uint8_t> version_;
    std::optional<ruffle::core::PlayerMode> mode_;
    bool with_default_font_ = false;

public:
    PlayerOptions() = default;

    /// Set the maximum execution duration
    ///
    /// \param duration The maximum duration for player execution
    void set_max_execution_duration(std::chrono::milliseconds duration) {
        max_execution_duration_ = duration;
    }

    /// Set the viewport dimensions
    ///
    /// \param dimensions The viewport dimensions
    void set_viewport_dimensions(const ruffle::core::render::backend::ViewportDimensions& dimensions) {
        viewport_dimensions_ = dimensions;
    }

    /// Set the render options
    ///
    /// \param options The render options
    void set_render_options(const RenderOptions& options) {
        with_renderer_ = options;
    }

    /// Enable audio
    ///
    /// \param enabled Whether to enable audio
    void set_with_audio(bool enabled) {
        with_audio_ = enabled;
    }

    /// Enable video
    ///
    /// \param enabled Whether to enable video
    void set_with_video(bool enabled) {
        with_video_ = enabled;
    }

    /// Set the player runtime
    ///
    /// \param runtime The player runtime to use
    void set_runtime(ruffle::core::PlayerRuntime runtime) {
        runtime_ = runtime;
    }

    /// Set the player version
    ///
    /// \param version The Flash player version to emulate
    void set_version(uint8_t version) {
        version_ = version;
    }

    /// Set the player mode
    ///
    /// \param mode The player mode (e.g., Debug, Release)
    void set_mode(ruffle::core::PlayerMode mode) {
        mode_ = mode;
    }

    /// Set whether to use the default font
    ///
    /// \param enabled Whether to use the default font
    void set_with_default_font(bool enabled) {
        with_default_font_ = enabled;
    }

    /// Setup the player builder with these options
    ///
    /// \param player_builder The player builder to configure
    /// \return The configured player builder
    std::optional<ruffle::core::PlayerBuilder> setup(ruffle::core::PlayerBuilder player_builder) const;

    /// Check if these options can run in the given environment
    ///
    /// \param check_renderer Whether to check renderer support
    /// \param environment The test environment
    /// \return true if the options can run
    bool can_run(bool check_renderer, const Environment& environment) const;

    /// Get the viewport dimensions for a movie
    ///
    /// \param movie The SWF movie
    /// \return The viewport dimensions
    ruffle::core::render::backend::ViewportDimensions viewport_dimensions(
        const ruffle::core::tag_utils::SwfMovie& movie) const;

    /// Create a renderer for these options
    ///
    /// \param environment The test environment
    /// \param dimensions The viewport dimensions
    /// \return Optional pair of render interface and backend
    std::optional<std::pair<std::unique_ptr<RenderInterface>, 
                            std::unique_ptr<ruffle::core::render::backend::RenderBackend>>>
    create_renderer(const Environment& environment,
                    const ruffle::core::render::backend::ViewportDimensions& dimensions) const;
};

} // namespace framework
} // namespace tests
} // namespace ruffle

#endif // RUFFLE_TESTS_FRAMEWORK_OPTIONS_PLAYER_H
