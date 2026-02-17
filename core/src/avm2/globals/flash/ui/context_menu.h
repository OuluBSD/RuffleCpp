// C++ translation of core/src/avm2/globals/flash/ui/context_menu.rs
// flash.ui.ContextMenu utilities

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_UI_CONTEXT_MENU_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_UI_CONTEXT_MENU_H

#include <vector>
#include <optional>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Object;
}
namespace display_object {
    template<typename T> class DisplayObject;
}
}
}

namespace ruffle {
namespace core {
namespace context_menu {
    template<typename T> class ContextMenuState;
    template<typename T> class ContextMenuItem;
    template<typename T> class ContextMenuCallback;
    struct BuiltInItemFlags;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace slots {
    // Forward declare slot constants
    namespace flash_display_native_menu_item {
        extern const int _ENABLED;
    }
    namespace flash_ui_context_menu {
        extern const int _BUILT_IN_ITEMS;
        extern const int _CUSTOM_ITEMS;
    }
    namespace flash_ui_context_menu_built_in_items {
        extern const int _ZOOM;
        extern const int _QUALITY;
        extern const int _PLAY;
        extern const int _LOOP;
        extern const int _REWIND;
        extern const int _FORWARD_AND_BACK;
        extern const int _PRINT;
    }
    namespace flash_ui_context_menu_item {
        extern const int _CAPTION;
        extern const int _VISIBLE;
        extern const int _SEPARATOR_BEFORE;
    }
}
}
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace ui {
namespace context_menu {

/// Creates a ContextMenuState from an AVM2 ContextMenu object
///
/// Processes built-in items flags and custom items from the AVM2 object
/// to build the native context menu state.
///
/// @param menu The AVM2 ContextMenu object (may be null)
/// @param object The display object the menu is associated with (may be null)
/// @param activation The current activation context
/// @return ContextMenuState populated with menu items
template<typename GC>
ContextMenuState<GC> make_context_menu_state(
    std::optional<Object<GC>> menu,
    std::optional<typename ruffle::core::display_object::DisplayObject<GC>> object,
    Activation<GC>* activation
);

} // namespace context_menu
} // namespace ui
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_UI_CONTEXT_MENU_H
