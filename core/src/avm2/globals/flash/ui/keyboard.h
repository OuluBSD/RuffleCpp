// C++ translation of core/src/avm2/globals/flash/ui/keyboard.rs
// flash.ui.Keyboard builtin/prototype

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_UI_KEYBOARD_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_UI_KEYBOARD_H

#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class AvmString;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace ui {
namespace keyboard {

/// Implements flash.ui.Keyboard.capsLock getter
/// Returns whether Caps Lock is enabled
template<typename GC>
Value<GC> get_caps_lock(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.ui.Keyboard.hasVirtualKeyboard getter
/// Returns whether a virtual keyboard is available
template<typename GC>
Value<GC> get_has_virtual_keyboard(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.ui.Keyboard.numLock getter
/// Returns whether Num Lock is enabled
template<typename GC>
Value<GC> get_num_lock(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.ui.Keyboard.physicalKeyboardType getter
/// Returns the type of physical keyboard
template<typename GC>
Value<GC> get_physical_keyboard_type(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.ui.Keyboard.isAccessible
/// Returns whether the keyboard is accessible
template<typename GC>
Value<GC> is_accessible(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace keyboard
} // namespace ui
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_UI_KEYBOARD_H
