// C++ translation of desktop/src/gui/dialogs/about_dialog.rs
//! About Ruffle dialog

#ifndef RUFFLE_DESKTOP_GUI_DIALOGS_ABOUT_DIALOG_H
#define RUFFLE_DESKTOP_GUI_DIALOGS_ABOUT_DIALOG_H

#include <string>
#include <string_view>

// Forward declarations
namespace unic_langid {
    class LanguageIdentifier;
}

// wxWidgets forward declarations
namespace wx {
    class Window;
    class WindowDisabler;
}

namespace ruffle {
namespace desktop {
namespace gui {
namespace dialogs {
namespace about_dialog {

/// Constant for unknown vergen output
constexpr std::string_view VERGEN_UNKNOWN = "VERGEN_IDEMPOTENT_OUTPUT";

/// Renders the About Ruffle dialog
/// @param locale The language identifier for localization
/// @param parent The parent window for the dialog
/// @return true if dialog should remain open, false if closed
bool show_about_dialog(
    const unic_langid::LanguageIdentifier& locale,
    wx::Window* parent);

} // namespace about_dialog
} // namespace dialogs
} // namespace gui
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_GUI_DIALOGS_ABOUT_DIALOG_H
