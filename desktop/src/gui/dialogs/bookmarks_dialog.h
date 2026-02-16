// C++ translation of desktop/src/gui/dialogs/bookmarks_dialog.rs
// Dialogs for managing bookmarks

#ifndef RUFFLE_DESKTOP_GUI_DIALOGS_BOOKMARKS_DIALOG_H
#define RUFFLE_DESKTOP_GUI_DIALOGS_BOOKMARKS_DIALOG_H

#include <string>
#include <vector>
#include <optional>
#include <memory>

// Forward declarations
namespace unic_langid {
    class LanguageIdentifier;
}

// egui forward declarations
namespace egui {
    class Context;
    class Ui;
    class EventLoopProxy;
}

// ruffle forward declarations
namespace ruffle {
namespace frontend_utils {
namespace bookmarks {
    class Bookmark;
}
namespace content {
    class ContentDescriptor;
}
}
namespace desktop {
namespace gui {
    class FilePicker;
    class LocalizableText;
    namespace widgets {
        class PathOrUrlField;
    }
}
class GlobalPreferences;
namespace player {
    class LaunchOptions;
}
namespace custom_event {
    class RuffleEvent;
}
}
}

namespace ruffle {
namespace desktop {
namespace gui {
namespace dialogs {

/// Dialog for adding a new bookmark
class BookmarkAddDialog {
private:
    ruffle::desktop::GlobalPreferences preferences_;
    std::string name_;
    widgets::PathOrUrlField url_;

public:
    /// Create a new bookmark add dialog
    /// @param preferences The global preferences
    /// @param content_descriptor Optional content descriptor to pre-fill the dialog
    /// @param picker The file picker for selecting files
    BookmarkAddDialog(
        ruffle::desktop::GlobalPreferences preferences,
        std::optional<ruffle::frontend_utils::content::ContentDescriptor> content_descriptor,
        FilePicker picker
    );

    /// Check if the dialog is valid (has name and URL)
    bool is_valid() const;

    /// Show the dialog
    /// @param locale The language identifier for localization
    /// @param egui_ctx The egui context
    /// @return true if dialog should remain open, false if closed
    bool show(const unic_langid::LanguageIdentifier& locale, egui::Context* egui_ctx);

    // Getters
    const std::string& name() const { return name_; }
    const widgets::PathOrUrlField& url() const { return url_; }

    // Setters
    void set_name(const std::string& name) { name_ = name; }
};

/// Selected bookmark data for editing
struct SelectedBookmark {
    size_t index;
    std::string name;
    widgets::PathOrUrlField url;
};

/// Dialog for managing bookmarks
/// Shows a list of bookmarks and allows adding, removing, and editing them
class BookmarksDialog {
private:
    winit::event_loop::EventLoopProxy<ruffle::desktop::custom_event::RuffleEvent> event_loop_;
    FilePicker picker_;
    ruffle::desktop::GlobalPreferences preferences_;
    std::optional<SelectedBookmark> selected_bookmark_;

    /// Show the bookmark table
    /// @param locale The language identifier for localization
    /// @param ui The UI to render to
    /// @return true if dialog should close
    bool show_bookmark_table(const unic_langid::LanguageIdentifier& locale, egui::Ui* ui);

    /// Show the bookmark panel for editing selected bookmark
    /// @param locale The language identifier for localization
    /// @param ui The UI to render to
    void show_bookmark_panel(const unic_langid::LanguageIdentifier& locale, egui::Ui* ui);

public:
    /// Create a new bookmarks dialog
    /// @param preferences The global preferences
    /// @param picker The file picker for selecting files
    /// @param event_loop The event loop proxy for sending events
    BookmarksDialog(
        ruffle::desktop::GlobalPreferences preferences,
        FilePicker picker,
        winit::event_loop::EventLoopProxy<ruffle::desktop::custom_event::RuffleEvent> event_loop
    )
        : event_loop_(std::move(event_loop))
        , picker_(std::move(picker))
        , preferences_(std::move(preferences))
        , selected_bookmark_(std::nullopt) {}

    /// Show the dialog
    /// @param locale The language identifier for localization
    /// @param egui_ctx The egui context
    /// @return true if dialog should remain open, false if closed
    bool show(const unic_langid::LanguageIdentifier& locale, egui::Context* egui_ctx);

    // Getters
    const std::optional<SelectedBookmark>& selected_bookmark() const { return selected_bookmark_; }

    // Setters
    void set_selected_bookmark(std::optional<SelectedBookmark> bookmark) { selected_bookmark_ = std::move(bookmark); }
};

} // namespace dialogs
} // namespace gui
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_GUI_DIALOGS_BOOKMARKS_DIALOG_H
