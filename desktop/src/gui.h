// C++ translation of desktop/src/gui.rs
// Main GUI controller for desktop Ruffle

#ifndef RUFFLE_DESKTOP_GUI_H
#define RUFFLE_DESKTOP_GUI_H

#include <memory>
#include <optional>
#include <vector>
#include <weak_ptr>
#include <filesystem>

// Forward declarations
namespace winit {
namespace event_loop {
    template<typename T>
    class EventLoopProxy;
}
namespace window {
    class Window;
}
}

namespace egui {
    class Context;
}

namespace rfd {
    class AsyncFileDialog;
}

namespace unic_langid {
    class LanguageIdentifier;
}

namespace ruffle {
namespace core {
    class Player;
    enum class PlayerEvent;
    struct ContextMenuItem;
    namespace debug_ui {
        struct Message;
    }
}
namespace frontend_utils {
namespace content {
    class ContentDescriptor;
}
}
}

namespace ruffle {
namespace desktop {

// Forward declarations from other modules
class LaunchOptions;
namespace preferences {
    class GlobalPreferences;
}
namespace custom_event {
    class RuffleEvent;
}
namespace gui {

// Forward declarations from GUI submodules
namespace context_menu {
    class ContextMenu;
}
namespace dialogs {
    class Dialogs;
    class DialogDescriptor;
}
namespace menu_bar {
    class MenuBar;
}

// Re-exports from locale module
class LocalizableText;
LocalizableText text(const unic_langid::LanguageIdentifier& locale, const std::string& key);
std::optional<LocalizableText> optional_text(const unic_langid::LanguageIdentifier& locale, const std::string& key);
std::string text_with_args(const unic_langid::LanguageIdentifier& locale, const std::string& key, const std::vector<std::string>& args);
std::vector<unic_langid::LanguageIdentifier> available_languages();

// Re-export from theme module
enum class ThemePreference {
    Light,
    Dark,
    System,
};

/// Size of the top menu bar in pixels.
/// This is the offset at which the movie will be shown,
/// and added to the window size if trying to match a movie.
constexpr uint32_t MENU_HEIGHT = 24;

/// Movie view - displays the Flash movie content
class MovieView {
    // Implementation in movie.h
};

/// File picker - abstract file selection interface
class FilePicker {
    // Implementation in picker.h
};

/// Main controller for the Ruffle GUI
/// Manages the menu bar, dialogs, context menus, and player interaction
class RuffleGui {
private:
    winit::event_loop::EventLoopProxy<custom_event::RuffleEvent>* event_loop_;
    std::optional<context_menu::ContextMenu> context_menu_;
    dialogs::Dialogs dialogs_;
    menu_bar::MenuBar menu_bar_;

    bool was_suspended_before_debug_;
    preferences::GlobalPreferences preferences_;

public:
    /// Create a new RuffleGui instance
    /// @param window Weak reference to the window
    /// @param event_loop Event loop proxy for sending events
    /// @param default_content Optional default content to load
    /// @param default_launch_options Default launch options
    /// @param preferences Global preferences
    static std::unique_ptr<RuffleGui> create(
        std::weak_ptr<winit::window::Window> window,
        winit::event_loop::EventLoopProxy<custom_event::RuffleEvent>* event_loop,
        std::optional<frontend_utils::content::ContentDescriptor> default_content,
        LaunchOptions default_launch_options,
        preferences::GlobalPreferences preferences
    );

    /// Update and render the GUI
    /// @param egui_ctx The egui context
    /// @param show_menu Whether to show the menu bar
    /// @param player Optional player instance
    /// @param menu_height_offset Offset for menu height
    void update(
        egui::Context* egui_ctx,
        bool show_menu,
        Player* player,
        double menu_height_offset
    );

    /// Show the context menu
    /// @param menu List of context menu items
    /// @param close_event Event to send when menu closes
    void show_context_menu(
        std::vector<ruffle::core::ContextMenuItem> menu,
        ruffle::core::PlayerEvent close_event
    );

    /// Close the context menu
    /// @param player The player instance
    void close_context_menu(Player* player);

    /// Check if context menu is visible
    bool is_context_menu_visible() const;

    /// Notify that the player has been destroyed
    void on_player_destroyed();

    /// Notify that a new player was created
    /// @param options Launch options
    /// @param content_descriptor Content descriptor
    /// @param player Player instance
    void on_player_created(
        LaunchOptions options,
        frontend_utils::content::ContentDescriptor content_descriptor,
        Player* player
    );

    // Getters
    dialogs::Dialogs& dialogs() { return dialogs_; }
    const dialogs::Dialogs& dialogs() const { return dialogs_; }
    menu_bar::MenuBar& menu_bar() { return menu_bar_; }
    const menu_bar::MenuBar& menu_bar() const { return menu_bar_; }
    const preferences::GlobalPreferences& preferences() const { return preferences_; }
};

} // namespace gui
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_GUI_H
