// C++ translation of desktop/src/gui/dialogs/volume_controls.rs
// Volume controls for Ruffle GUI

#ifndef RUFFLE_DESKTOP_GUI_DIALOGS_VOLUME_CONTROLS_H
#define RUFFLE_DESKTOP_GUI_DIALOGS_VOLUME_CONTROLS_H

#include <string>

// Forward declarations
namespace unic_langid {
    class LanguageIdentifier;
}

namespace egui {
    class Context;
}

namespace ruffle {
namespace core {
    class Player;
}
namespace desktop {
namespace preferences {
    class GlobalPreferences;
}
}
}

namespace ruffle {
namespace desktop {
namespace gui {
namespace dialogs {

/// The volume controls of the Ruffle GUI.
class VolumeControls {
public:
    VolumeControls() = default;

    /// Create new volume controls from preferences.
    ///
    /// @param preferences The global preferences to initialize from
    explicit VolumeControls(const GlobalPreferences* preferences);

    /// Show the volume controls window.
    ///
    /// @param locale The current locale for localization
    /// @param egui_ctx The egui context for rendering
    /// @param player Optional player to update volume on
    /// @param preferences The global preferences for persistence
    /// @return true if the window should remain open, false otherwise
    bool show(
        const unic_langid::LanguageIdentifier& locale,
        egui::Context* egui_ctx,
        ruffle::core::Player* player,
        const GlobalPreferences* preferences
    );

    /// Returns the volume between 0 and 1 (calculated from the
    /// checkbox and the slider).
    ///
    /// @return Volume value between 0.0 and 1.0
    float get_volume() const;

private:
    bool is_muted_ = false;
    float volume_ = 100.0f;
};

} // namespace dialogs
} // namespace gui
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_GUI_DIALOGS_VOLUME_CONTROLS_H
