// C++ translation of core/src/avm2/globals/flash/ui/mouse.rs
// flash.ui.Mouse builtin

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_UI_MOUSE_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_UI_MOUSE_H

#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace ui {
namespace mouse {

/// Implements flash.ui.Mouse.hide
/// Hides the mouse cursor
template<typename GC>
Value<GC> hide(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.ui.Mouse.show
/// Shows the mouse cursor
template<typename GC>
Value<GC> show(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace mouse
} // namespace ui
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_UI_MOUSE_H
