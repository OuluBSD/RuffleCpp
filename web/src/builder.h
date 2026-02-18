// C++ translation of web/src/builder.rs
// Web platform Ruffle instance builder

#ifndef RUFFLE_WEB_BUILDER_H
#define RUFFLE_WEB_BUILDER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <optional>
#include <chrono>
#include <functional>

// Forward declarations
namespace ruffle {
namespace core {
    namespace backend {
        namespace audio {
            class AudioBackend;
        }
        namespace storage {
            class StorageBackend;
        }
        namespace ui {
            struct FontDefinition;
        }
    }
    namespace config {
        enum class Letterbox;
        enum class NetworkingAccessMode;
    }
    namespace events {
        enum class GamepadButton;
        enum class KeyCode;
    }
    namespace font {
        enum class DefaultFont;
        struct FontFileData;
    }
    namespace compatibility_rules {
        class CompatibilityRules;
    }
    class Color;
    class Player;
    enum class PlayerRuntime;
    enum class StageAlign;
    enum class StageScaleMode;
    namespace swf {
        // SWF types
    }
}
namespace render {
    namespace backend {
        class RenderBackend;
    }
    namespace quality {
        enum class StageQuality;
    }
}
namespace video {
namespace external {
namespace backend {
    class ExternalVideoBackend;
}
}
}
namespace web {
    class JavascriptInterface;
    namespace navigator {
        enum class OpenUrlMode;
        class WebNavigatorBackend;
    }
    enum class ScrollingBehavior;
    enum class DeviceFontRenderer;
    struct SocketProxy;
}
}

// For tracing
namespace tracing {
    enum class Level;
}

namespace ruffle {
namespace web {

/// Builder for creating Ruffle instances in the web platform
class RuffleInstanceBuilder {
private:
    bool allow_script_access_;
    std::optional<ruffle::core::Color> background_color_;
    ruffle::core::config::Letterbox letterbox_;
    bool upgrade_to_https_;
    ruffle::core::compatibility_rules::CompatibilityRules compatibility_rules_;
    std::optional<std::string> base_url_;
    bool show_menu_;
    bool allow_fullscreen_;
    ruffle::core::StageAlign stage_align_;
    bool force_align_;
    ruffle::render::quality::StageQuality quality_;
    ruffle::core::StageScaleMode scale_;
    bool force_scale_;
    std::optional<double> frame_rate_;
    std::optional<std::string> wmode_;
    tracing::Level log_level_;
    std::chrono::duration<double> max_execution_duration_;
    std::optional<uint8_t> player_version_;
    std::optional<std::string> preferred_renderer_;
    navigator::OpenUrlMode open_url_mode_;
    ruffle::core::config::NetworkingAccessMode allow_networking_;
    std::vector<SocketProxy> socket_proxy_;
    std::vector<std::string> credential_allow_list_;
    ruffle::core::PlayerRuntime player_runtime_;
    float volume_;
    std::unordered_map<ruffle::core::font::DefaultFont, std::vector<std::string>> default_fonts_;
    std::vector<std::pair<std::string, std::vector<uint8_t>>> custom_fonts_;
    std::unordered_map<ruffle::core::events::GamepadButton, ruffle::core::events::KeyCode> gamepad_button_mapping_;
    std::vector<std::pair<std::string, std::string>> url_rewrite_rules_;
    ScrollingBehavior scrolling_behavior_;
    DeviceFontRenderer device_font_renderer_;

public:
    RuffleInstanceBuilder();

    // Getters
    bool allow_script_access() const { return allow_script_access_; }
    const std::optional<ruffle::core::Color>& background_color() const { return background_color_; }
    ruffle::core::config::Letterbox letterbox() const { return letterbox_; }
    bool upgrade_to_https() const { return upgrade_to_https_; }
    const ruffle::core::compatibility_rules::CompatibilityRules& compatibility_rules() const { return compatibility_rules_; }
    const std::optional<std::string>& base_url() const { return base_url_; }
    bool show_menu() const { return show_menu_; }
    bool allow_fullscreen() const { return allow_fullscreen_; }
    ruffle::core::StageAlign stage_align() const { return stage_align_; }
    bool force_align() const { return force_align_; }
    ruffle::render::quality::StageQuality quality() const { return quality_; }
    ruffle::core::StageScaleMode scale() const { return scale_; }
    bool force_scale() const { return force_scale_; }
    const std::optional<double>& frame_rate() const { return frame_rate_; }
    const std::optional<std::string>& wmode() const { return wmode_; }
    tracing::Level log_level() const { return log_level_; }
    std::chrono::duration<double> max_execution_duration() const { return max_execution_duration_; }
    const std::optional<uint8_t>& player_version() const { return player_version_; }
    const std::optional<std::string>& preferred_renderer() const { return preferred_renderer_; }
    navigator::OpenUrlMode open_url_mode() const { return open_url_mode_; }
    ruffle::core::config::NetworkingAccessMode allow_networking() const { return allow_networking_; }
    const std::vector<SocketProxy>& socket_proxy() const { return socket_proxy_; }
    const std::vector<std::string>& credential_allow_list() const { return credential_allow_list_; }
    ruffle::core::PlayerRuntime player_runtime() const { return player_runtime_; }
    float volume() const { return volume_; }
    const std::unordered_map<ruffle::core::font::DefaultFont, std::vector<std::string>>& default_fonts() const { return default_fonts_; }
    const std::vector<std::pair<std::string, std::vector<uint8_t>>>& custom_fonts() const { return custom_fonts_; }
    const std::unordered_map<ruffle::core::events::GamepadButton, ruffle::core::events::KeyCode>& gamepad_button_mapping() const { return gamepad_button_mapping_; }
    const std::vector<std::pair<std::string, std::string>>& url_rewrite_rules() const { return url_rewrite_rules_; }
    ScrollingBehavior scrolling_behavior() const { return scrolling_behavior_; }
    DeviceFontRenderer device_font_renderer() const { return device_font_renderer_; }

    // Setters
    void set_allow_script_access(bool value) { allow_script_access_ = value; }
    void set_background_color(std::optional<uint32_t> value);
    void set_upgrade_to_https(bool value) { upgrade_to_https_ = value; }
    void set_compatibility_rules(bool value);
    void set_letterbox(const std::string& value);
    void set_base_url(std::optional<std::string> value) { base_url_ = value; }
    void set_show_menu(bool value) { show_menu_ = value; }
    void set_allow_fullscreen(bool value) { allow_fullscreen_ = value; }
    void set_stage_align(const std::string& value);
    void set_force_align(bool value) { force_align_ = value; }
    void set_quality(const std::string& value);
    void set_scale(const std::string& value);
    void set_force_scale(bool value) { force_scale_ = value; }
    void set_frame_rate(std::optional<double> value) { frame_rate_ = value; }
    void set_wmode(std::optional<std::string> value) { wmode_ = value; }
    void set_log_level(const std::string& value);
    void set_max_execution_duration(double value);
    void set_player_version(std::optional<uint8_t> value) { player_version_ = value; }
    void set_preferred_renderer(std::optional<std::string> value) { preferred_renderer_ = value; }
    void set_open_url_mode(const std::string& value);
    void set_allow_networking(const std::string& value);
    void add_socket_proxy(const std::string& host, uint16_t port, const std::string& proxy_url);
    void set_credential_allow_list(const std::vector<std::string>& value) { credential_allow_list_ = value; }
    void set_player_runtime(const std::string& value);
    void set_volume(float value) { volume_ = value; }
    void add_font(const std::string& font_name, const std::vector<uint8_t>& data);
    void set_default_font(const std::string& default_name, const std::vector<std::string>& fonts);
    void add_gamepad_button_mapping(const std::string& button, uint32_t keycode);
    void add_url_rewrite_rule(const std::string& regexp, const std::string& replacement);
    void set_scrolling_behavior(const std::string& scrolling_behavior);
    void set_device_font_renderer(const std::string& device_font_renderer);

    // Build methods
    void setup_fonts(ruffle::core::Player* player);
    std::shared_ptr<tracing::Subscriber> create_log_subscriber() const;
    std::pair<std::unique_ptr<ruffle::render::backend::RenderBackend>, void*> create_renderer() const;

private:
    static void register_ttf_face_by_name(
        const std::string& url,
        const std::vector<uint8_t>& bytes,
        void* face,  // ttf_parser::Face
        uint32_t index,
        ruffle::core::Player* player
    );
};

} // namespace web
} // namespace ruffle

#endif // RUFFLE_WEB_BUILDER_H
