// C++ translation of core/src/avm2/globals/flash/display/interactive_object.rs
// flash.display.InteractiveObject builtin/prototype

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_INTERACTIVE_OBJECT_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_INTERACTIVE_OBJECT_H

#include <vector>
#include <optional>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class TObject;
    namespace object {
        template<typename T> class ContextMenuObject;
    }
}
namespace display_object {
    template<typename T> class TInteractiveObject;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace display {
namespace interactive_object {

/// Implements InteractiveObject.mouseEnabled getter
/// Returns whether the interactive object can receive mouse events
template<typename GC>
Value<GC> get_mouse_enabled(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements InteractiveObject.mouseEnabled setter
/// Sets whether the interactive object can receive mouse events
template<typename GC>
Value<GC> set_mouse_enabled(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements InteractiveObject.doubleClickEnabled getter
/// Returns whether double-click events are enabled
template<typename GC>
Value<GC> get_double_click_enabled(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements InteractiveObject.doubleClickEnabled setter
/// Sets whether double-click events are enabled
template<typename GC>
Value<GC> set_double_click_enabled(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements InteractiveObject.contextMenu getter
/// Returns the context menu for this interactive object
template<typename GC>
Value<GC> get_context_menu(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements InteractiveObject.contextMenu setter
/// Sets the context menu for this interactive object
template<typename GC>
Value<GC> set_context_menu(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements InteractiveObject.tabEnabled getter
/// Returns whether this object can receive focus via tab navigation
template<typename GC>
Value<GC> get_tab_enabled(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements InteractiveObject.tabEnabled setter
/// Sets whether this object can receive focus via tab navigation
template<typename GC>
Value<GC> set_tab_enabled(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements InteractiveObject.tabIndex getter
/// Returns the tab index of this object
template<typename GC>
Value<GC> get_tab_index(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements InteractiveObject.tabIndex setter
/// Sets the tab index of this object
/// Note: -1 is allowed and means tabIndex is unset
/// Values less than -1 will throw Error #2027
template<typename GC>
Value<GC> set_tab_index(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements InteractiveObject.focusRect getter
/// Returns the focus rectangle setting
/// Returns null if not set, true/false otherwise
template<typename GC>
Value<GC> get_focus_rect(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements InteractiveObject.focusRect setter
/// Sets the focus rectangle setting
/// true = show focus rect, false = hide, null = use default
template<typename GC>
Value<GC> set_focus_rect(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace interactive_object
} // namespace display
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_INTERACTIVE_OBJECT_H
