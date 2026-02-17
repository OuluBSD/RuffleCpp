// C++ translation of desktop/src/gui/context_menu.rs
// Context menu for Ruffle GUI

#ifndef RUFFLE_DESKTOP_GUI_CONTEXT_MENU_H
#define RUFFLE_DESKTOP_GUI_CONTEXT_MENU_H

#include <vector>
#include <optional>

// Forward declarations
namespace unic_langid {
    class LanguageIdentifier;
}

namespace egui {
    class Context;
    struct Pos2;
}

namespace winit {
namespace event_loop {
    template<typename T>
    class EventLoopProxy;
}
}

namespace ruffle {
namespace core {
    struct ContextMenuItem;
    enum class PlayerEvent;
}
namespace desktop {
namespace custom_event {
    enum class RuffleEvent;
}
}
}

namespace ruffle {
namespace desktop {
namespace gui {

/// Context menu displayed on right-click.
class ContextMenu {
public:
    ContextMenu() = default;

    /// Create a new context menu.
    ///
    /// @param items The menu items to display
    /// @param close_event The event to send when the menu closes
    ContextMenu(std::vector<ruffle::core::ContextMenuItem> items, ruffle::core::PlayerEvent close_event);

    /// Get the close event for this menu.
    ///
    /// @return The player event to send on close
    ruffle::core::PlayerEvent close_event() const { return close_event_; }

    /// Show the context menu.
    ///
    /// @param locale The current locale for localization
    /// @param egui_ctx The egui context for rendering
    /// @param event_loop The event loop proxy for sending events
    /// @param fullscreen Whether the player is in fullscreen mode
    /// @return true if the menu should remain open, false otherwise
    bool show(
        const unic_langid::LanguageIdentifier& locale,
        egui::Context* egui_ctx,
        winit::event_loop::EventLoopProxy<custom_event::RuffleEvent>* event_loop,
        bool fullscreen
    );

private:
    std::vector<ruffle::core::ContextMenuItem> items_;
    std::optional<egui::Pos2> position_;
    ruffle::core::PlayerEvent close_event_;
};

} // namespace gui
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_GUI_CONTEXT_MENU_H
