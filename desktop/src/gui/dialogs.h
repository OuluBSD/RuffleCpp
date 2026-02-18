// C++ translation of desktop/src/gui/dialogs.rs
// Dialog management for desktop GUI

#ifndef RUFFLE_DESKTOP_GUI_DIALOGS_H
#define RUFFLE_DESKTOP_GUI_DIALOGS_H

#include <memory>
#include <vector>
#include <deque>
#include <optional>
#include <filesystem>

// Forward declarations
namespace url {
    class Url;
}

namespace unic_langid {
    class LanguageIdentifier;
}

namespace egui {
    class Context;
}

namespace winit {
namespace event_loop {
    template<typename T>
    class EventLoopProxy;
}
namespace window {
    class Window;
}
}

namespace ruffle {
namespace core {
    class Player;
}
namespace frontend {
namespace utils {
namespace content {
    class ContentDescriptor;
}
}
}
}

namespace ruffle {
namespace desktop {

// Custom event
namespace custom_event {
    enum class RuffleEvent;
}

// Player
namespace player {
    struct LaunchOptions;
}

// Preferences
namespace preferences {
    class GlobalPreferences;
}

// GUI components
namespace gui {
    class FilePicker;

    namespace dialogs {
        // Dialog classes
        class PreferencesDialog;
        class BookmarksDialog;
        class BookmarkAddDialog;
        class OpenUrlDialog;
        class MessageDialog;
        class ExportBundleDialog;
        class SelectPathDialog;
        class NetworkAccessDialog;
        class FilesystemAccessDialog;
        class OpenDialog;
        class VolumeControls;

        // Dialog configurations
        class MessageDialogConfiguration;
        class NetworkAccessDialogConfiguration;
        class FilesystemAccessDialogConfiguration;
        class ExportBundleDialogConfiguration;
        class SelectPathDialogConfiguration;
    }
}
}
}

namespace ruffle {
namespace desktop {
namespace gui {

/// Dialog descriptor for opening different dialog types
enum class DialogDescriptor {
    OpenUrl,
    ShowMessage,
    NetworkAccess,
    FilesystemAccess,
    ExportBundle,
    SelectPath
};

/// Dialog manager for desktop GUI
class Dialogs {
private:
    std::shared_ptr<winit::event_loop::EventLoopProxy<ruffle::desktop::custom_event::RuffleEvent>> event_loop_;
    FilePicker picker_;
    ruffle::desktop::preferences::GlobalPreferences preferences_;

    // Dialog instances
    std::optional<std::unique_ptr<dialogs::PreferencesDialog>> preferences_dialog_;
    std::optional<std::unique_ptr<dialogs::BookmarksDialog>> bookmarks_dialog_;
    std::optional<std::unique_ptr<dialogs::BookmarkAddDialog>> bookmark_add_dialog_;
    std::optional<std::unique_ptr<dialogs::OpenUrlDialog>> open_url_dialog_;
    std::optional<std::unique_ptr<dialogs::MessageDialog>> message_dialog_;
    std::optional<std::unique_ptr<dialogs::ExportBundleDialog>> export_bundle_dialog_;
    std::optional<std::unique_ptr<dialogs::SelectPathDialog>> pick_path_dialog_;

    // Queued dialogs
    std::deque<dialogs::NetworkAccessDialog> network_access_dialog_queue_;
    std::optional<dialogs::FilesystemAccessDialog> filesystem_access_dialog_;
    std::deque<dialogs::FilesystemAccessDialogConfiguration> filesystem_access_dialog_queue_;

    // Open dialog
    dialogs::OpenDialog open_dialog_;
    bool is_open_dialog_visible_;

    // Volume controls
    dialogs::VolumeControls volume_controls_;
    bool is_volume_visible_;

    // About screen
    bool is_about_visible_;

    // Show methods for individual dialogs
    void show_open_dialog(const unic_langid::LanguageIdentifier* locale, egui::Context* egui_ctx);
    void show_preferences_dialog(const unic_langid::LanguageIdentifier* locale, egui::Context* egui_ctx);
    void show_bookmarks_dialog(const unic_langid::LanguageIdentifier* locale, egui::Context* egui_ctx);
    void show_bookmark_add_dialog(const unic_langid::LanguageIdentifier* locale, egui::Context* egui_ctx);
    void show_volume_controls(
        const unic_langid::LanguageIdentifier* locale,
        egui::Context* egui_ctx,
        ruffle::core::Player* player
    );
    void show_about_dialog(const unic_langid::LanguageIdentifier* locale, egui::Context* egui_ctx);
    void show_open_url_dialog(const unic_langid::LanguageIdentifier* locale, egui::Context* egui_ctx);
    void show_message_dialog(const unic_langid::LanguageIdentifier* locale, egui::Context* egui_ctx);
    void show_network_access_dialog(const unic_langid::LanguageIdentifier* locale, egui::Context* egui_ctx);
    void show_filesystem_access_dialog(const unic_langid::LanguageIdentifier* locale, egui::Context* egui_ctx);
    void show_export_bundle_dialog(const unic_langid::LanguageIdentifier* locale, egui::Context* egui_ctx);
    void show_pick_path_dialog(const unic_langid::LanguageIdentifier* locale, egui::Context* egui_ctx);

public:
    Dialogs(
        const ruffle::desktop::preferences::GlobalPreferences& preferences,
        const ruffle::desktop::player::LaunchOptions& player_options,
        const std::optional<ruffle::frontend::utils::content::ContentDescriptor>& default_content,
        std::weak_ptr<winit::window::Window> window,
        std::shared_ptr<winit::event_loop::EventLoopProxy<ruffle::desktop::custom_event::RuffleEvent>> event_loop
    );

    /// Get file picker
    FilePicker file_picker() const;

    /// Close all dialogs with notifiers
    void close_dialogs_with_notifiers();

    /// Recreate open dialog
    void recreate_open_dialog(
        const ruffle::desktop::player::LaunchOptions& opt,
        const std::optional<ruffle::frontend::utils::content::ContentDescriptor>& content_descriptor,
        std::shared_ptr<winit::event_loop::EventLoopProxy<ruffle::desktop::custom_event::RuffleEvent>> event_loop
    );

    /// Open file advanced dialog
    void open_file_advanced();

    /// Open preferences dialog
    void open_preferences();

    /// Open bookmarks dialog
    void open_bookmarks();

    /// Open add bookmark dialog
    void open_add_bookmark(const std::optional<ruffle::frontend::utils::content::ContentDescriptor>& content_descriptor);

    /// Open volume controls
    void open_volume_controls();

    /// Open about screen
    void open_about_screen();

    /// Get saved content descriptor
    std::optional<ruffle::frontend::utils::content::ContentDescriptor> saved_content_descriptor() const;

    /// Get saved launch options
    const ruffle::desktop::player::LaunchOptions& saved_launch_options() const;

    /// Open a dialog based on descriptor
    void open_dialog(const DialogDescriptor& event);

    /// Show all dialogs
    void show(
        const unic_langid::LanguageIdentifier* locale,
        egui::Context* egui_ctx,
        ruffle::core::Player* player
    );
};

} // namespace gui
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_GUI_DIALOGS_H
