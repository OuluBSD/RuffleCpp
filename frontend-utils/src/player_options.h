// C++ translation of frontend-utils/src/player_options.rs
// Player options configuration

#ifndef RUFFLE_FRONTEND_UTILS_PLAYER_OPTIONS_H
#define RUFFLE_FRONTEND_UTILS_PLAYER_OPTIONS_H

#include <vector>
#include <string>
#include <optional>
#include <chrono>

// Forward declarations
namespace url {
    class Url;
}

namespace ruffle {
namespace core {
namespace config {
    enum class Letterbox;
}
    enum class LoadBehavior;
    enum class PlayerRuntime;
    enum class StageAlign;
    enum class StageScaleMode;
}
namespace render {
namespace quality {
    enum class StageQuality;
}
}
}

namespace ruffle {
namespace frontend {
namespace utils {
namespace player_options {

using Letterbox = core::config::Letterbox;
using LoadBehavior = core::LoadBehavior;
using PlayerRuntime = core::PlayerRuntime;
using StageAlign = core::StageAlign;
using StageScaleMode = core::StageScaleMode;
using StageQuality = render::quality::StageQuality;
using Url = url::Url;
using Duration = std::chrono::duration<double>;

/// Player options structure
/// Contains all configurable options for the Ruffle player
struct PlayerOptions {
    /// SWF parameters (name-value pairs)
    std::vector<std::pair<std::string, std::string>> parameters;

    /// Maximum execution duration (optional timeout)
    std::optional<Duration> max_execution_duration;

    /// Base URL for relative URLs (optional)
    std::optional<Url> base;

    /// Stage quality setting (optional)
    std::optional<StageQuality> quality;

    /// Stage alignment (optional)
    std::optional<StageAlign> align;

    /// Force alignment override (optional)
    std::optional<bool> force_align;

    /// Stage scale mode (optional)
    std::optional<StageScaleMode> scale;

    /// Force scale override (optional)
    std::optional<bool> force_scale;

    /// Upgrade to HTTPS (optional)
    std::optional<bool> upgrade_to_https;

    /// Load behavior setting (optional)
    std::optional<LoadBehavior> load_behavior;

    /// Letterbox mode (optional)
    std::optional<Letterbox> letterbox;

    /// Spoof URL (optional)
    std::optional<Url> spoof_url;

    /// Referer URL (optional)
    std::optional<Url> referer;

    /// Cookie string (optional)
    std::optional<std::string> cookie;

    /// Player version (optional)
    std::optional<uint8_t> player_version;

    /// Player runtime (optional)
    std::optional<PlayerRuntime> player_runtime;

    /// Frame rate (optional)
    std::optional<double> frame_rate;

    /// Use dummy external interface (optional)
    std::optional<bool> dummy_external_interface;

    /// Default constructor
    PlayerOptions() = default;

    /// Merge with another PlayerOptions, giving priority to 'other'
    /// @param other The other options to merge with
    /// @return Merged options
    PlayerOptions merge(const PlayerOptions& other) const {
        PlayerOptions result;
        
        // Parameters: other's parameters first, then self's
        result.parameters = other.parameters;
        result.parameters.insert(result.parameters.end(), 
                                  parameters.begin(), parameters.end());
        
        // Optional fields: self takes precedence over other
        result.max_execution_duration = max_execution_duration.value_or(other.max_execution_duration);
        result.base = base.value_or(other.base);
        result.quality = quality.value_or(other.quality);
        result.align = align.value_or(other.align);
        result.force_align = force_align.value_or(other.force_align);
        result.scale = scale.value_or(other.scale);
        result.force_scale = force_scale.value_or(other.force_scale);
        result.upgrade_to_https = upgrade_to_https.value_or(other.upgrade_to_https);
        result.load_behavior = load_behavior.value_or(other.load_behavior);
        result.letterbox = letterbox.value_or(other.letterbox);
        result.spoof_url = spoof_url.value_or(other.spoof_url);
        result.referer = referer.value_or(other.referer);
        result.cookie = cookie.value_or(other.cookie);
        result.player_version = player_version.value_or(other.player_version);
        result.player_runtime = player_runtime.value_or(other.player_runtime);
        result.frame_rate = frame_rate.value_or(other.frame_rate);
        result.dummy_external_interface = dummy_external_interface.value_or(other.dummy_external_interface);
        
        return result;
    }
};

} // namespace player_options
} // namespace utils
} // namespace frontend
} // namespace ruffle

#endif // RUFFLE_FRONTEND_UTILS_PLAYER_OPTIONS_H
