// C++ translation of desktop/src/gui/menu_bar.rs
//! Menu bar UI component

#ifndef RUFFLE_DESKTOP_GUI_MENU_BAR_H
#define RUFFLE_DESKTOP_GUI_MENU_BAR_H

#include <vector>
#include <string>
#include <optional>
#include <memory>

// Forward declarations
namespace ruffle {
namespace core {
    class Player;
    enum class StageScaleMode;
    enum class Letterbox;

    namespace focus_tracker {
        class DisplayObject;
    }
}

namespace render {
namespace quality {
    enum class StageQuality;
}
}

namespace desktop {

enum class OpenType {
    File,
    Directory
};

struct LaunchOptions;
class Dialogs;
struct DebugMessage;

namespace gui {

struct MenuBar {
    /// Cached recent files for display in menu
    std::optional<std::vector<Recent>> cached_recents;
    
    /// Currently opened content
    std::optional<std::pair<ContentDescriptor, LaunchOptions>> currently_opened;

    /// Consume keyboard shortcuts for menu actions
    ///
    /// \param egui_ctx The egui context
    /// \param dialogs Dialog manager
    /// \param player Optional player instance
    void consume_shortcuts(
        egui::Context* egui_ctx,
        Dialogs* dialogs,
        core::Player* player
    );

    /// Show the menu bar UI
    ///
    /// \param locale Current locale identifier
    /// \param egui_ctx The egui context
    /// \param dialogs Dialog manager
    /// \param player Optional player instance
    void show(
        const unic_langid::LanguageIdentifier* locale,
        egui::Context* egui_ctx,
        Dialogs* dialogs,
        core::Player* player
    );

private:
    /// File menu
    void file_menu(
        const unic_langid::LanguageIdentifier* locale,
        egui::Ui* ui,
        Dialogs* dialogs,
        bool player_exists
    );

    /// View menu
    void view_menu(
        const unic_langid::LanguageIdentifier* locale,
        egui::Ui* ui,
        core::Player* player
    );

    /// Controls menu
    void controls_menu(
        const unic_langid::LanguageIdentifier* locale,
        egui::Ui* ui,
        Dialogs* dialogs,
        core::Player* player
    );

    /// Browse and open file/directory
    void browse_and_open(OpenType open_type);

    /// Close current movie
    void close_movie(egui::Ui* ui);

    /// Reload current movie
    void reload_movie(egui::Ui* ui);

    /// Request application exit
    void request_exit();

    /// Launch website in browser
    void launch_website(egui::Ui* ui, const std::string& url);

    /// Export bundle
    void export_bundle(egui::Ui* ui);

    // Keyboard shortcuts
    static const egui::KeyboardShortcut SHORTCUT_FULLSCREEN;
    static const egui::KeyboardShortcut SHORTCUT_FULLSCREEN_WINDOWS;
    static const egui::KeyboardShortcut SHORTCUT_OPEN;
    static const egui::KeyboardShortcut SHORTCUT_OPEN_ADVANCED;
    static const egui::KeyboardShortcut SHORTCUT_PAUSE;
    static const egui::KeyboardShortcut SHORTCUT_STEP;
    static const egui::KeyboardShortcut SHORTCUT_QUIT;
};

} // namespace gui
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_GUI_MENU_BAR_H
