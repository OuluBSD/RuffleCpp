// C++ translation of desktop/src/gui/dialogs/preferences_dialog.rs
// Preferences dialog for desktop application

#ifndef RUFFLE_DESKTOP_GUI_DIALOGS_PREFERENCES_DIALOG_H
#define RUFFLE_DESKTOP_GUI_DIALOGS_PREFERENCES_DIALOG_H

#include <cstdint>
#include <string>
#include <vector>
#include <optional>
#include <memory>

// Forward declarations
namespace ruffle {
namespace desktop {
namespace cli {
    enum class GameModePreference;
    enum class OpenUrlMode;
}
namespace gui {
    enum class ThemePreference;
    std::vector<std::string> available_languages();
    std::string optional_text(const std::string& locale, const std::string& key);
    std::string text(const std::string& locale, const std::string& key);
}
namespace log {
    enum class FilenamePattern;
}
namespace preferences {
    class GlobalPreferences;
    namespace storage {
        enum class StorageBackend;
    }
}
}
}

// Egui types (immediate mode GUI)
namespace egui {
    class Context;
    class Ui;
    struct Align2;
    struct Vec2;
}

// WGPU types
namespace wgpu {
    enum class Backends;
}

namespace ruffle {
namespace render_wgpu {
namespace backend {
    void* create_wgpu_instance();
}
namespace clap {
    enum class GraphicsBackend;
    enum class PowerPreference;
}
}
}

// CPAL types (audio)
namespace cpal {
namespace traits {
    class DeviceTrait;
    class HostTrait;
}
}

// Language identifier
namespace unic_langid {
    struct LanguageIdentifier;
}

namespace ruffle {
namespace desktop {
namespace gui {
namespace dialogs {

/// Preferences dialog
/// Handles display and editing of global preferences
class PreferencesDialog {
private:
    wgpu::Backends available_backends;
    std::shared_ptr<preferences::GlobalPreferences> preferences;

    // Graphics settings
    ruffle::render_wgpu::clap::GraphicsBackend graphics_backend;
    bool graphics_backend_readonly;
    bool graphics_backend_changed;

    ruffle::render_wgpu::clap::PowerPreference power_preference;
    bool power_preference_readonly;
    bool power_preference_changed;

    // Game mode setting (Linux only)
    cli::GameModePreference gamemode_preference;
    bool gamemode_preference_readonly;
    bool gamemode_preference_changed;

    // Language setting
    unic_langid::LanguageIdentifier language;
    bool language_changed;

    // Audio output device
    std::optional<std::string> output_device;
    std::vector<std::string> available_output_devices;
    bool output_device_changed;

    // Video codec settings
    bool enable_openh264;
    bool enable_openh264_changed;
    bool openh264_license_visible;

    // Recent files limit
    size_t recent_limit;
    bool recent_limit_changed;

    // Log filename pattern
    log::FilenamePattern log_filename_pattern;
    bool log_filename_pattern_changed;

    // Storage backend
    preferences::storage::StorageBackend storage_backend;
    bool storage_backend_readonly;
    bool storage_backend_changed;

    // Theme preference
    ThemePreference theme_preference;
    bool theme_preference_changed;

    // Open URL mode
    cli::OpenUrlMode open_url_mode;
    bool open_url_mode_readonly;
    bool open_url_mode_changed;

    // IME (Input Method Editor) setting
    std::optional<bool> ime_enabled;
    bool ime_enabled_changed;

public:
    /// Create a new preferences dialog
    /// 
    /// @param preferences Global preferences to edit
    explicit PreferencesDialog(std::shared_ptr<preferences::GlobalPreferences> preferences);

    /// Show the preferences dialog
    /// 
    /// @param locale The locale for translations
    /// @param egui_ctx The egui context
    /// @return true if dialog should remain open, false if closed
    bool show(const unic_langid::LanguageIdentifier& locale, egui::Context* egui_ctx);

    /// Check if restart is required for changes to take effect
    bool restart_required() const;

    /// Save the changed preferences
    void save();

private:
    /// Find available graphics backends
    static wgpu::Backends find_available_graphics_backends();

    /// Show graphics preferences
    void show_graphics_preferences(
        const unic_langid::LanguageIdentifier& locale,
        const std::string& locked_text,
        egui::Ui* ui
    );

    /// Show language preferences
    void show_language_preferences(
        const unic_langid::LanguageIdentifier& locale,
        egui::Ui* ui
    );

    /// Show theme preferences
    void show_theme_preferences(
        const unic_langid::LanguageIdentifier& locale,
        egui::Ui* ui
    );

    /// Show gamemode preferences (Linux only)
    void show_gamemode_preferences(
        const unic_langid::LanguageIdentifier& locale,
        const std::string& locked_text,
        egui::Ui* ui
    );

    /// Show open URL mode preferences
    void show_open_url_mode_preferences(
        const unic_langid::LanguageIdentifier& locale,
        const std::string& locked_text,
        egui::Ui* ui
    );

    /// Show IME preferences
    void show_ime_preferences(
        const unic_langid::LanguageIdentifier& locale,
        egui::Ui* ui
    );

    /// Show audio preferences
    void show_audio_preferences(
        const unic_langid::LanguageIdentifier& locale,
        egui::Ui* ui
    );

    /// Show video preferences
    void show_video_preferences(
        egui::Context* egui_ctx,
        const unic_langid::LanguageIdentifier& locale,
        egui::Ui* ui
    );

    /// Show log preferences
    void show_log_preferences(
        const unic_langid::LanguageIdentifier& locale,
        egui::Ui* ui
    );

    /// Show storage preferences
    void show_storage_preferences(
        const unic_langid::LanguageIdentifier& locale,
        const std::string& locked_text,
        egui::Ui* ui
    );

    /// Show miscellaneous preferences
    void show_misc_preferences(
        const unic_langid::LanguageIdentifier& locale,
        egui::Ui* ui
    );
};

// Helper functions for displaying preference names

/// Get graphics backend name for display
std::string graphics_backend_name(
    const unic_langid::LanguageIdentifier& locale,
    ruffle::render_wgpu::clap::GraphicsBackend backend
);

/// Get graphics power preference name for display
std::string graphics_power_name(
    const unic_langid::LanguageIdentifier& locale,
    ruffle::render_wgpu::clap::PowerPreference power_preference
);

/// Get language name for display
std::string language_name(const unic_langid::LanguageIdentifier& language);

/// Get theme preference name for display
std::string theme_preference_name(
    const unic_langid::LanguageIdentifier& locale,
    ThemePreference theme_preference
);

/// Get gamemode preference name for display
std::string gamemode_preference_name(
    const unic_langid::LanguageIdentifier& locale,
    cli::GameModePreference gamemode_preference
);

/// Get gamemode preference tooltip
std::optional<std::string> gamemode_preference_tooltip(
    const unic_langid::LanguageIdentifier& locale,
    cli::GameModePreference gamemode_preference
);

/// Get open URL mode preference name for display
std::string open_url_mode_preference_name(
    const unic_langid::LanguageIdentifier& locale,
    cli::OpenUrlMode open_url_mode
);

/// Get IME enabled name for display
std::string ime_enabled_name(
    const unic_langid::LanguageIdentifier& locale,
    std::optional<bool> ime_enabled
);

/// Get storage backend name for display
std::string storage_backend_name(
    const unic_langid::LanguageIdentifier& locale,
    preferences::storage::StorageBackend storage_backend
);

/// Get filename pattern name for display
std::string filename_pattern_name(
    const unic_langid::LanguageIdentifier& locale,
    log::FilenamePattern pattern
);

} // namespace dialogs
} // namespace gui
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_GUI_DIALOGS_PREFERENCES_DIALOG_H
