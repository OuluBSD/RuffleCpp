// C++ translation of desktop/src/preferences.rs
// Application preferences management

#ifndef RUFFLE_DESKTOP_PREFERENCES_H
#define RUFFLE_DESKTOP_PREFERENCES_H

// Forward declarations
namespace ruffle {
namespace desktop {
namespace cli {
    struct Opt;
    enum class GameModePreference;
    enum class OpenUrlMode;
}
namespace gui {
    enum class ThemePreference;
}
namespace log {
    struct FilenamePattern;
}
namespace preferences {
namespace read {
    struct ReadResult;
    ReadResult read_preferences(const std::string& contents);
}
namespace write {
    class PreferencesWriter;
}
}
}
}

// External dependencies
namespace ruffle {
namespace frontend_utils {
namespace bookmarks {
    struct Bookmarks;
    class BookmarksWriter;
    Bookmarks read_bookmarks(const std::string& contents);
}
namespace recents {
    struct Recents;
    class RecentsWriter;
    Recents read_recents(const std::string& contents);
}
}
namespace render_wgpu {
namespace clap {
    enum class GraphicsBackend;
    enum class PowerPreference;
}
}
namespace core {
namespace backend {
namespace ui {
    extern const LanguageIdentifier US_ENGLISH;
}
}
}
}

// Standard library
#include <string>
#include <memory>
#include <mutex>
#include <optional>
#include <functional>

// External crates
#include <anyhow/error.h>
#include <sys_locale/locale.h>
#include <tokio/sync/broadcast.h>
#include <unic_langid/language_identifier.h>

namespace ruffle {
namespace desktop {

/// Preferences storage backend options
namespace preferences {
namespace storage {

enum class StorageBackend {
    // Storage backend options would be defined here
};

} // namespace storage
} // namespace preferences

/// Log-related preferences
struct LogPreferences {
    log::FilenamePattern filename_pattern;
    
    LogPreferences() = default;
};

/// Storage-related preferences
struct StoragePreferences {
    preferences::storage::StorageBackend backend;
    
    StoragePreferences() = default;
};

/// The global preferences that relate to the application itself.
///
/// This structure is safe to clone, internally it holds a shared_ptr to any mutable properties.
///
/// The general priority order for preferences should look as follows, where top is "highest priority":
/// - User-selected movie-specific setting (if applicable, such as through Open Advanced)
/// - Movie-specific settings (if applicable and we implement this, stored on disk)
/// - CLI (if applicable)
/// - Persisted preferences (if applicable, saved to toml)
/// - Ruffle defaults
class GlobalPreferences {
public:
    /// As the CLI holds properties ranging from initial movie settings (ie url),
    /// to application itself (ie render backend),
    /// this field is available for checking where needed.
    // TODO: This should really not be public and we should split up CLI somehow,
    // or make it all getters in here?
    cli::Opt cli;

    /// Load preferences from disk
    /// @param cli Command-line options
    /// @return Result containing GlobalPreferences or error
    static Result<GlobalPreferences, anyhow::Error> load(cli::Opt cli);

    /// Get graphics backends preference
    render_wgpu::clap::GraphicsBackend graphics_backends() const;

    /// Get graphics power preference
    render_wgpu::clap::PowerPreference graphics_power_preference() const;

    /// Get gamemode preference
    cli::GameModePreference gamemode_preference() const;

    /// Get language preference
    unic_langid::LanguageIdentifier language() const;

    /// Get output device name
    std::optional<std::string> output_device_name() const;

    /// Get mute setting
    bool mute() const;

    /// Get preferred volume
    float preferred_volume() const;

    /// Check if OpenH264 is enabled
    bool openh264_enabled() const;

    /// Get log filename pattern
    log::FilenamePattern log_filename_pattern() const;

    /// Access bookmarks with a callback
    void bookmarks(std::function<void(const frontend_utils::bookmarks::Bookmarks&)> fun);

    /// Check if there are bookmarks
    bool have_bookmarks() const;

    /// Get storage backend
    preferences::storage::StorageBackend storage_backend() const;

    /// Get recent items limit
    size_t recent_limit() const;

    /// Get theme preference
    gui::ThemePreference theme_preference() const;

    /// Get theme preference watcher
    tokio::sync::broadcast::Receiver<gui::ThemePreference> theme_preference_watcher() const;

    /// Get open URL mode
    cli::OpenUrlMode open_url_mode() const;

    /// Get IME enabled setting
    std::optional<bool> ime_enabled() const;

    /// Access recents with a callback
    template<typename R>
    R recents(std::function<R(const frontend_utils::recents::Recents&)> fun);

    /// Write preferences with a callback
    Result<void, anyhow::Error> write_preferences(
        std::function<void(preferences::write::PreferencesWriter&)> fun);

    /// Write bookmarks with a callback
    Result<void, anyhow::Error> write_bookmarks(
        std::function<void(frontend_utils::bookmarks::BookmarksWriter&)> fun);

    /// Write recents with a callback
    Result<void, anyhow::Error> write_recents(
        std::function<void(frontend_utils::recents::RecentsWriter&)> fun);

private:
    // Private implementation details would go here
};

/// Saved global preferences structure
struct SavedGlobalPreferences {
    render_wgpu::clap::GraphicsBackend graphics_backend;
    render_wgpu::clap::PowerPreference graphics_power_preference;
    cli::GameModePreference gamemode_preference;
    unic_langid::LanguageIdentifier language;
    std::optional<std::string> output_device;
    bool mute;
    float volume;
    bool enable_openh264;
    size_t recent_limit;
    LogPreferences log;
    StoragePreferences storage;
    gui::ThemePreference theme_preference;
    cli::OpenUrlMode open_url_mode;
    std::optional<bool> ime_enabled;

    SavedGlobalPreferences() = default;
    
    /// Create with default values
    static SavedGlobalPreferences default_();
};

/// Global preferences watchers for reactive updates
class GlobalPreferencesWatchers {
public:
    GlobalPreferencesWatchers() = default;
    
    static GlobalPreferencesWatchers default_();

private:
    std::shared_ptr<tokio::sync::broadcast::Sender<gui::ThemePreference>> theme_preference_watcher;
};

} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_PREFERENCES_H
