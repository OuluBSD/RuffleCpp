// C++ translation of desktop/src/gui/dialogs/message_dialog.rs
// Message dialog for displaying simple messages to the user

#ifndef RUFFLE_DESKTOP_GUI_DIALOGS_MESSAGE_DIALOG_H
#define RUFFLE_DESKTOP_GUI_DIALOGS_MESSAGE_DIALOG_H

#include <string>

// Forward declarations
namespace unic_langid {
    class LanguageIdentifier;
}

// wxWidgets forward declarations
namespace wx {
    class Window;
    class Dialog;
    class StaticText;
    class Button;
    class BoxSizer;
}

namespace ruffle {
namespace desktop {
namespace gui {

/// Localizable text - text that can be translated based on locale
class LocalizableText {
public:
    /// Get the localized string for the given locale
    std::string localize(const unic_langid::LanguageIdentifier& locale) const;

    // Helper function to get localized text (matches Rust text() function)
    static std::string text(const unic_langid::LanguageIdentifier& locale, const std::string& key);
};

} // namespace gui
} // namespace desktop
} // namespace ruffle

namespace ruffle {
namespace desktop {
namespace gui {
namespace dialogs {

/// Configuration for a message dialog
class MessageDialogConfiguration {
private:
    LocalizableText title_;
    LocalizableText body_;

public:
    /// Create a new message dialog configuration
    /// @param title The title of the dialog
    /// @param body The body text of the dialog
    MessageDialogConfiguration(LocalizableText title, LocalizableText body)
        : title_(std::move(title)), body_(std::move(body)) {}

    /// Get the title
    const LocalizableText& title() const { return title_; }

    /// Get the body text
    const LocalizableText& body() const { return body_; }
};

/// Message dialog for displaying simple messages with an OK button
class MessageDialog {
private:
    MessageDialogConfiguration config_;

public:
    /// Create a new message dialog
    /// @param config The dialog configuration
    explicit MessageDialog(MessageDialogConfiguration config)
        : config_(std::move(config)) {}

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
};

} // namespace dialogs
} // namespace gui
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_GUI_DIALOGS_MESSAGE_DIALOG_H
