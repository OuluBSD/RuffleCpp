// C++ translation of frontend-utils/src/player_options/write.rs
//! Player options writer for TOML serialization

#ifndef RUFFLE_FRONTEND_UTILS_PLAYER_OPTIONS_WRITE_H
#define RUFFLE_FRONTEND_UTILS_PLAYER_OPTIONS_WRITE_H

#include <vector>
#include <string>
#include <optional>
#include <chrono>
#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
    enum class StageAlign;
    enum class StageScaleMode;
    enum class LoadBehavior;
    enum class PlayerRuntime;

    namespace config {
        enum class Letterbox;
    }
}

namespace render {
namespace quality {
    enum class StageQuality;
}
}

namespace frontend_utils {

struct PlayerOptions;

namespace parse {
    template<typename T> class DocumentHolder;
}

/// Writer for PlayerOptions to TOML format
class PlayerOptionsWriter {
public:
    /// Create a new writer
    ///
    /// \param preferences Document holder for player options
    explicit PlayerOptionsWriter(parse::DocumentHolder<PlayerOptions>* preferences);

    /// Remove all parameters
    void remove_parameters();

    /// Add a parameter
    ///
    /// \param key Parameter key
    /// \param string_value Parameter value
    void add_parameter(const std::string& key, const std::string& string_value);

    /// Set max execution duration
    ///
    /// \param max_execution_duration Duration or nullopt to remove
    void set_max_execution_duration(
        std::optional<std::chrono::duration<double>> max_execution_duration
    );

    /// Set base URL
    ///
    /// \param base Base URL or nullopt to remove
    void set_base(std::optional<url::Url> base);

    /// Set stage quality
    ///
    /// \param quality Quality or nullopt to remove
    void set_quality(std::optional<render::quality::StageQuality> quality);

    /// Set stage align
    ///
    /// \param align Align or nullopt to remove
    void set_align(std::optional<core::StageAlign> align);

    /// Set force align flag
    ///
    /// \param force_align Force align or nullopt to remove
    void set_force_align(std::optional<bool> force_align);

    /// Set scale mode
    ///
    /// \param scale Scale mode or nullopt to remove
    void set_scale(std::optional<core::StageScaleMode> scale);

    /// Set force scale flag
    ///
    /// \param force_scale Force scale or nullopt to remove
    void set_force_scale(std::optional<bool> force_scale);

    /// Set upgrade to HTTPS flag
    ///
    /// \param upgrade_to_https Upgrade flag or nullopt to remove
    void set_upgrade_to_https(std::optional<bool> upgrade_to_https);

    /// Set load behavior
    ///
    /// \param load_behavior Load behavior or nullopt to remove
    void set_load_behavior(std::optional<core::LoadBehavior> load_behavior);

    /// Set letterbox mode
    ///
    /// \param letterbox Letterbox mode or nullopt to remove
    void set_letterbox(std::optional<core::config::Letterbox> letterbox);

    /// Set spoof URL
    ///
    /// \param spoof_url Spoof URL or nullopt to remove
    void set_spoof_url(std::optional<url::Url> spoof_url);

    /// Set player version
    ///
    /// \param player_version Player version or nullopt to remove
    void set_player_version(std::optional<uint8_t> player_version);

    /// Set player runtime
    ///
    /// \param player_runtime Player runtime or nullopt to remove
    void set_player_runtime(std::optional<core::PlayerRuntime> player_runtime);

    /// Set frame rate
    ///
    /// \param frame_rate Frame rate or nullopt to remove
    void set_frame_rate(std::optional<double> frame_rate);

    /// Set dummy external interface flag
    ///
    /// \param dummy_external_interface Flag or nullopt to remove
    void set_dummy_external_interface(std::optional<bool> dummy_external_interface);

private:
    parse::DocumentHolder<PlayerOptions>* preferences_;
};

/// Write player options to document
///
/// \param writer The writer to use
/// \param options The options to write
void write_player_options(
    PlayerOptionsWriter* writer,
    const PlayerOptions* options
);

} // namespace frontend_utils
} // namespace ruffle

#endif // RUFFLE_FRONTEND_UTILS_PLAYER_OPTIONS_WRITE_H
