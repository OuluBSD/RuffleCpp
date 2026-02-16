// C++ translation of desktop/src/gui/dialogs/select_path_dialog.rs
// Dialog for selecting a file path from a directory

#ifndef RUFFLE_DESKTOP_GUI_DIALOGS_SELECT_PATH_DIALOG_H
#define RUFFLE_DESKTOP_GUI_DIALOGS_SELECT_PATH_DIALOG_H

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <filesystem>

// Forward declarations
namespace unic_langid {
    class LanguageIdentifier;
}

// wxWidgets forward declarations
namespace wx {
    class Window;
    class Dialog;
    class ListBox;
    class CheckBox;
    class Button;
    class BoxSizer;
    class StaticText;
}

// Forward declarations for async operations
template<typename T>
class Future;

namespace tokio {
namespace sync {
namespace oneshot {
    template<typename T>
    class Sender;
}
}
}

namespace ruffle {
namespace desktop {
namespace gui {

/// Localizable text - text that can be translated based on locale
class LocalizableText {
public:
    /// Get the localized string for the given locale
    std::string localize(const unic_langid::LanguageIdentifier& locale) const;
};

} // namespace gui
} // namespace desktop
} // namespace ruffle

namespace ruffle {
namespace desktop {
namespace gui {
namespace dialogs {

/// Result of the select path dialog
enum class SelectPathDialogResult {
    PathSelected,
    Canceled,
};

/// Configuration for the select path dialog
class SelectPathDialogConfiguration {
private:
    std::optional<tokio::sync::oneshot::Sender<SelectPathDialogResult>> notifier_;
    std::filesystem::path directory_;
    LocalizableText title_;
    std::optional<LocalizableText> message_;
    LocalizableText label_;
    std::optional<const char*> extension_;

public:
    /// Create a new select path dialog configuration
    SelectPathDialogConfiguration(
        tokio::sync::oneshot::Sender<SelectPathDialogResult> notifier,
        std::filesystem::path directory,
        LocalizableText title,
        std::optional<LocalizableText> message,
        LocalizableText label,
        std::optional<const char*> extension
    )
        : notifier_(std::move(notifier))
        , directory_(std::move(directory))
        , title_(std::move(title))
        , message_(std::move(message))
        , label_(std::move(label))
        , extension_(std::move(extension)) {}

    // Getters
    const std::optional<tokio::sync::oneshot::Sender<SelectPathDialogResult>>& notifier() const { return notifier_; }
    const std::filesystem::path& directory() const { return directory_; }
    const LocalizableText& title() const { return title_; }
    const std::optional<LocalizableText>& message() const { return message_; }
    const LocalizableText& label() const { return label_; }
    const std::optional<const char*>& extension() const { return extension_; }

    // Setters for mutable access
    std::optional<tokio::sync::oneshot::Sender<SelectPathDialogResult>>& notifier() { return notifier_; }
};

/// Dialog for selecting a file path from a directory
/// Shows a list of files and allows the user to select one
class SelectPathDialog {
private:
    SelectPathDialogConfiguration config_;
    std::vector<std::pair<std::filesystem::path, std::string>> files_;
    std::optional<std::filesystem::path> selected_file_;
    bool show_all_files_;

    /// Send the result to the notifier
    void respond(SelectPathDialogResult result);

    /// Compare paths for display ordering
    /// Files are returned first, directories later
    static int compare_paths(const std::filesystem::path& a, const std::filesystem::path& b);

public:
    /// Create a new select path dialog
    /// @param config The dialog configuration
    explicit SelectPathDialog(SelectPathDialogConfiguration config);

    /// Destructor - sends Canceled result if not already responded
    ~SelectPathDialog();

    /// Show the dialog
    /// @param locale The language identifier for localization
    /// @param parent The parent window for the dialog
    /// @return true if dialog should remain open, false if closed
    bool show(const unic_langid::LanguageIdentifier& locale, wx::Window* parent);

    /// Render the window contents
    /// @param locale The language identifier for localization
    /// @param sizer The sizer to add controls to
    /// @return true if dialog should close
    bool render_window_contents(const unic_langid::LanguageIdentifier& locale, wx::BoxSizer* sizer);

    // Getters
    const std::optional<std::filesystem::path>& selected_file() const { return selected_file_; }
    bool is_showing_all_files() const { return show_all_files_; }

    // Setters
    void set_selected_file(std::optional<std::filesystem::path> path) { selected_file_ = std::move(path); }
    void set_show_all_files(bool show) { show_all_files_ = show; }
};

} // namespace dialogs
} // namespace gui
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_GUI_DIALOGS_SELECT_PATH_DIALOG_H
