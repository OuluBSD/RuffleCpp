// C++ translation of desktop/src/gui/dialogs/open_url_dialog.rs
// Dialog for confirming URL opening

#ifndef RUFFLE_DESKTOP_GUI_DIALOGS_OPEN_URL_DIALOG_H
#define RUFFLE_DESKTOP_GUI_DIALOGS_OPEN_URL_DIALOG_H

#include <string>

// Forward declarations
namespace unic_langid {
    class LanguageIdentifier;
}

// egui forward declarations
namespace egui {
    class Context;
    class Ui;
}

// url forward declarations
namespace url {
    class Url;
}

namespace ruffle {
namespace desktop {
namespace gui {
namespace dialogs {

/// Dialog for confirming URL opening
/// Shows the URL and asks the user if they want to open it
class OpenUrlDialog {
private:
    url::Url url_;

public:
    /// Create a new open URL dialog
    /// @param url The URL to open
    explicit OpenUrlDialog(url::Url url)
        : url_(std::move(url)) {}

    /// Show the dialog
    /// @param locale The language identifier for localization
    /// @param egui_ctx The egui context
    /// @return true if dialog should remain open, false if closed
    bool show(const unic_langid::LanguageIdentifier& locale, egui::Context* egui_ctx);

    /// Render the window contents
    /// @param locale The language identifier for localization
    /// @param ui The UI to render to
    /// @return true if dialog should close
    bool render_window_contents(const unic_langid::LanguageIdentifier& locale, egui::Ui* ui);

    /// Get the URL
    const url::Url& url() const { return url_; }
};

} // namespace dialogs
} // namespace gui
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_GUI_DIALOGS_OPEN_URL_DIALOG_H
