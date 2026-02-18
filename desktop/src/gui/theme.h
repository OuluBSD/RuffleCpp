// C++ translation of desktop/src/gui/theme.rs
// Theme controller for Ruffle desktop GUI

#ifndef RUFFLE_DESKTOP_GUI_THEME_H
#define RUFFLE_DESKTOP_GUI_THEME_H

#include <string>
#include <optional>
#include <memory>
#include <mutex>
#include <string_view>

// Forward declarations
namespace winit {
    namespace window {
        class Window;
        enum class Theme;
    }
}

namespace egui {
    class Context;
    enum class Theme;
}

namespace tokio {
    namespace sync {
        template<typename T>
        class Mutex;
        template<typename T>
        using MutexGuard = std::lock_guard<Mutex<T>>;
    }
}

namespace ruffle {
namespace desktop {
namespace preferences {
    class GlobalPreferences;
}
}
}

#ifdef TARGET_OS_LINUX
namespace ashpd {
    namespace desktop {
        namespace settings {
            enum class ColorScheme;
        }
    }
}

namespace ruffle {
namespace desktop {
namespace dbus {
    class FreedesktopSettings;
}
}
}
#endif

namespace ruffle {
namespace desktop {
namespace gui {

/// Theme preference enum
enum class ThemePreference {
    System,  ///< Use system theme
    Light,   ///< Force light theme
    Dark     ///< Force dark theme
};

/// Theme controller for managing window and egui themes
class ThemeControllerData {
private:
    std::weak_ptr<winit::window::Window> window_;
    egui::Context egui_ctx_;
    ThemePreference theme_preference_;

#ifdef TARGET_OS_LINUX
    std::optional<dbus::FreedesktopSettings> freedesktop_settings_;
#endif

public:
    ThemeControllerData(
        std::weak_ptr<winit::window::Window> window,
        egui::Context egui_ctx,
        ThemePreference theme_preference
#ifdef TARGET_OS_LINUX
        , std::optional<dbus::FreedesktopSettings> freedesktop_settings
#endif
    );

    std::weak_ptr<winit::window::Window> window() const { return window_; }
    const egui::Context& egui_ctx() const { return egui_ctx_; }
    egui::Context& egui_ctx() { return egui_ctx_; }
    ThemePreference theme_preference() const { return theme_preference_; }
    void set_theme_preference(ThemePreference pref) { theme_preference_ = pref; }

#ifdef TARGET_OS_LINUX
    const std::optional<dbus::FreedesktopSettings>& freedesktop_settings() const {
        return freedesktop_settings_;
    }
#endif
};

/// Theme controller for managing application theme
class ThemeController {
private:
    std::shared_ptr<tokio::sync::Mutex<ThemeControllerData>> data_;

    /// Get mutex guard for data access
    std::lock_guard<tokio::sync::Mutex<ThemeControllerData>> data();

    /// Set theme internally
    void set_theme_internal(
        std::lock_guard<tokio::sync::Mutex<ThemeControllerData>>& data,
        winit::window::Theme theme
    );

#ifdef TARGET_OS_LINUX
    /// Start D-Bus theme watcher on Linux
    void start_dbus_theme_watcher_linux();
#endif

    /// Start theme preference watcher
    void start_theme_preference_watcher(const preferences::GlobalPreferences& preferences);

public:
    /// Create a new theme controller
    ///
    /// @param window The window to control theme for
    /// @param preferences Global preferences for theme settings
    /// @param egui_ctx The egui context for theme application
    static std::shared_ptr<ThemeController> create(
        std::shared_ptr<winit::window::Window> window,
        const preferences::GlobalPreferences& preferences,
        egui::Context egui_ctx
    );

    /// Set theme based on system preference
    ///
    /// @param theme The theme to set
    void set_theme(winit::window::Theme theme);

    /// Get system theme
    ///
    /// @return The current system theme
    std::expected<winit::window::Theme, std::string> get_system_theme();
};

/// Convert color scheme to theme (Linux only)
#ifdef TARGET_OS_LINUX
winit::window::Theme scheme_to_theme(ashpd::desktop::settings::ColorScheme scheme);
#endif

} // namespace gui
} // namespace desktop
} // namespace ruffle

// ThemePreference implementation
namespace ruffle {
namespace desktop {
namespace gui {

inline std::optional<std::string_view> theme_preference_as_str(ThemePreference pref) {
    switch (pref) {
        case ThemePreference::System:
            return std::nullopt;
        case ThemePreference::Light:
            return "light";
        case ThemePreference::Dark:
            return "dark";
    }
}

/// Parse theme preference from string
///
/// @param s The string to parse
/// @return The parsed ThemePreference, or std::nullopt on error
inline std::optional<ThemePreference> theme_preference_from_str(std::string_view s) {
    if (s == "light") {
        return ThemePreference::Light;
    } else if (s == "dark") {
        return ThemePreference::Dark;
    }
    return std::nullopt;
}

} // namespace gui
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_GUI_THEME_H
