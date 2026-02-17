// C++ translation of desktop/src/preferences/write.rs
// Preferences writer for desktop application

#ifndef RUFFLE_DESKTOP_PREFERENCES_WRITE_H
#define RUFFLE_DESKTOP_PREFERENCES_WRITE_H

#include <string>
#include <optional>
#include <functional>
#include <memory>

// Forward declarations
namespace ruffle {
namespace render {
namespace wgpu {
namespace clap {
    enum class GraphicsBackend;
    enum class PowerPreference;
}
}
}
}

namespace unic_langid {
    struct LanguageIdentifier {
        std::string to_string() const { return str; }
        std::string str;
    };
}

namespace ruffle {
namespace desktop {
namespace cli {
    enum class GameModePreference;
    enum class OpenUrlMode;
}
namespace gui {
    enum class ThemePreference;
}
namespace log {
    enum class FilenamePattern {
        SingleFile,
        WithTimestamp
    };
}
namespace preferences {
    namespace storage {
        enum class StorageBackend {
            Disk,
            Memory
        };
    }
    struct SavedGlobalPreferences;
    struct GlobalPreferencesWatchers;
}
}
}

namespace ruffle {
namespace frontend {
namespace utils {
namespace parse {
    template<typename T>
    class DocumentHolder;
}
}
}
}

namespace ruffle {
namespace desktop {
namespace preferences {

/// Preferences writer for modifying and persisting global preferences
class PreferencesWriter {
private:
    ruffle::frontend::utils::parse::DocumentHolder<SavedGlobalPreferences>* preferences;
    const GlobalPreferencesWatchers* watchers;

public:
    /// Create a new preferences writer
    /// @param preferences The document holder containing preferences
    explicit PreferencesWriter(ruffle::frontend::utils::parse::DocumentHolder<SavedGlobalPreferences>* prefs)
        : preferences(prefs), watchers(nullptr) {}

    /// Set the watchers for change notifications
    void set_watchers(const GlobalPreferencesWatchers* watchers) {
        this->watchers = watchers;
    }

    /// Set the graphics backend
    /// @param backend The graphics backend to use
    void set_graphics_backend(ruffle::render::wgpu::clap::GraphicsBackend backend);

    /// Set the graphics power preference
    /// @param preference The power preference (low, high, etc.)
    void set_graphics_power_preference(ruffle::render::wgpu::clap::PowerPreference preference);

    /// Set the UI language
    /// @param language The language identifier
    void set_language(unic_langid::LanguageIdentifier language);

    /// Set the audio output device
    /// @param name The device name, or std::nullopt to use default
    void set_output_device(std::optional<std::string> name);

    /// Set the mute state
    /// @param mute Whether to mute audio
    void set_mute(bool mute);

    /// Set the volume level
    /// @param volume Volume level (0.0 to 1.0)
    void set_volume(float volume);

    /// Set whether OpenH264 decoder is enabled
    /// @param enable Whether to enable OpenH264
    void set_enable_openh264(bool enable);

    /// Set the log filename pattern
    /// @param pattern The filename pattern to use
    void set_log_filename_pattern(log::FilenamePattern pattern);

    /// Set the storage backend
    /// @param backend The storage backend to use
    void set_storage_backend(storage::StorageBackend backend);

    /// Set the recent files limit
    /// @param limit Maximum number of recent files to remember
    void set_recent_limit(size_t limit);

    /// Set the theme preference
    /// @param theme_preference The theme preference (light, dark, system)
    void set_theme_preference(gui::ThemePreference theme_preference);

    /// Set the game mode preference
    /// @param gamemode_preference The game mode preference
    void set_gamemode_preference(cli::GameModePreference gamemode_preference);

    /// Set the open URL mode
    /// @param open_url_mode How to handle URL opening (allow, deny, confirm)
    void set_open_url_mode(cli::OpenUrlMode open_url_mode);

    /// Set whether IME is enabled
    /// @param ime_enabled Whether IME is enabled, or std::nullopt to remove setting
    void set_ime_enabled(std::optional<bool> ime_enabled);

private:
    /// Helper to edit preferences and TOML document together
    void edit(std::function<void(SavedGlobalPreferences&, std::string&)> fun);
};

} // namespace preferences
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_PREFERENCES_WRITE_H
