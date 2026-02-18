// C++ translation of desktop/src/gui/dialogs/filesystem_access_dialog.rs
// Filesystem access dialog for desktop GUI

#ifndef RUFFLE_DESKTOP_GUI_DIALOGS_FILESYSTEM_ACCESS_DIALOG_H
#define RUFFLE_DESKTOP_GUI_DIALOGS_FILESYSTEM_ACCESS_DIALOG_H

#include <memory>
#include <vector>
#include <string>
#include <filesystem>
#include <optional>

// Forward declarations
namespace unic_langid {
    class LanguageIdentifier;
}

namespace egui {
    class Context;
    class Ui;
}

namespace tokio {
namespace sync {
    template<typename T>
    class oneshot_sender;
}
}

namespace ruffle {
namespace desktop {
namespace backends {
namespace navigator {
    class PathAllowList;
}
}
}
}

namespace ruffle {
namespace desktop {
namespace gui {
namespace dialogs {

/// Result of the filesystem access dialog
enum class FilesystemAccessDialogResult {
    Allow,
    Cancel
};

/// Configuration for the filesystem access dialog
class FilesystemAccessDialogConfiguration {
private:
    std::optional<std::shared_ptr<tokio::sync::oneshot_sender<FilesystemAccessDialogResult>>> notifier;
    ruffle::desktop::backends::navigator::PathAllowList allow_list;
    std::filesystem::path path;

public:
    FilesystemAccessDialogConfiguration(
        std::shared_ptr<tokio::sync::oneshot_sender<FilesystemAccessDialogResult>> notifier,
        const ruffle::desktop::backends::navigator::PathAllowList& allow_list,
        const std::filesystem::path& path
    );

    const std::filesystem::path& get_path() const { return path; }
    const ruffle::desktop::backends::navigator::PathAllowList& get_allow_list() const { return allow_list; }
    std::optional<std::shared_ptr<tokio::sync::oneshot_sender<FilesystemAccessDialogResult>>> take_notifier();
};

/// Filesystem access dialog for requesting file access permissions
class FilesystemAccessDialog {
private:
    FilesystemAccessDialogConfiguration config;
    bool allowed;
    bool remember_access;
    std::filesystem::path selected_path;
    std::vector<std::filesystem::path> selectable_paths;

    /// Send response to notifier
    void respond(FilesystemAccessDialogResult result);

    /// Get selectable paths for the dropdown menu
    static std::vector<std::filesystem::path> get_selectable_paths(
        const FilesystemAccessDialogConfiguration& config
    );

public:
    explicit FilesystemAccessDialog(FilesystemAccessDialogConfiguration config);

    /// Destructor - sends cancel if not already responded
    ~FilesystemAccessDialog();

    /// Show the dialog
    /// @return true if dialog should remain open
    bool show(
        const unic_langid::LanguageIdentifier* locale,
        egui::Context* egui_ctx
    );

    /// Render window contents
    /// @return true if dialog should close
    bool render_window_contents(
        const unic_langid::LanguageIdentifier* locale,
        egui::Ui* ui
    );

    /// Render the checkbox for remembering access
    void render_checkbox(
        const unic_langid::LanguageIdentifier* locale,
        egui::Ui* ui
    );
};

} // namespace dialogs
} // namespace gui
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_GUI_DIALOGS_FILESYSTEM_ACCESS_DIALOG_H
