// C++ translation of core/src/avm2/globals/flash/display/stage.rs
// flash.display.Stage builtin/prototype

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_STAGE_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_STAGE_H

#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class AvmString;
    template<typename T> class VectorObject;
}
namespace display_object {
    template<typename T> class DisplayObject;
    template<typename T> class Stage;
    enum class StageAlign;
    enum class StageDisplayState;
    enum class StageScaleMode;
    enum class StageQuality;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace display {
namespace stage {

/// Implements flash.display.Stage.align getter
///
/// Returns the current stage alignment as a string.
/// Alignment flags: T (top), B (bottom), L (left), R (right)
///
/// @param activation The current activation context
/// @param this_value The Stage object
/// @param args Function arguments (unused)
/// @return Alignment string
template<typename GC>
Value<GC> get_align(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display.Stage.align setter
///
/// Sets the stage alignment from a string.
///
/// @param activation The current activation context
/// @param this_value The Stage object
/// @param args Function arguments (alignment string at index 0)
/// @return Value::Undefined
template<typename GC>
Value<GC> set_align(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display.Stage.browserZoomFactor getter
///
/// Returns the browser zoom factor / scale factor.
///
/// @param activation The current activation context
/// @param this_value The Stage object
/// @param args Function arguments (unused)
/// @return Scale factor as number
template<typename GC>
Value<GC> get_browser_zoom_factor(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display.Stage.color getter
///
/// Returns the stage background color as RGBA.
///
/// @param activation The current activation context
/// @param this_value The Stage object
/// @param args Function arguments (unused)
/// @return Color as 32-bit integer
template<typename GC>
Value<GC> get_color(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display.Stage.color setter
///
/// Sets the stage background color.
///
/// @param activation The current activation context
/// @param this_value The Stage object
/// @param args Function arguments (color at index 0)
/// @return Value::Undefined
template<typename GC>
Value<GC> set_color(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display.Stage.contentsScaleFactor getter
///
/// Returns the contents scale factor.
///
/// @param activation The current activation context
/// @param this_value The Stage object
/// @param args Function arguments (unused)
/// @return Scale factor as number
template<typename GC>
Value<GC> get_contents_scale_factor(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display.Stage.displayState getter
///
/// Returns the current display state as a string.
///
/// @param activation The current activation context
/// @param this_value The Stage object
/// @param args Function arguments (unused)
/// @return Display state string
template<typename GC>
Value<GC> get_display_state(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display.Stage.displayState setter
///
/// Sets the display state (e.g., FullScreen, Normal).
///
/// @param activation The current activation context
/// @param this_value The Stage object
/// @param args Function arguments (display state string at index 0)
/// @return Value::Undefined
/// @throws Error 2008 if invalid display state
template<typename GC>
Value<GC> set_display_state(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display.Stage.focus getter
///
/// Returns the currently focused interactive object.
///
/// @param activation The current activation context
/// @param this_value The Stage object
/// @param args Function arguments (unused)
/// @return Focused object or Value::Null
template<typename GC>
Value<GC> get_focus(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display.Stage.focus setter
///
/// Sets the focus to an interactive object.
///
/// @param activation The current activation context
/// @param this_value The Stage object
/// @param args Function arguments (object at index 0)
/// @return Value::Undefined
template<typename GC>
Value<GC> set_focus(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display.Stage.frameRate getter
///
/// Returns the current frame rate.
///
/// @param activation The current activation context
/// @param this_value The Stage object
/// @param args Function arguments (unused)
/// @return Frame rate as number
template<typename GC>
Value<GC> get_frame_rate(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display.Stage.frameRate setter
///
/// Sets the frame rate (clamped to 0.01-1000.0).
///
/// @param activation The current activation context
/// @param this_value The Stage object
/// @param args Function arguments (frame rate at index 0)
/// @return Value::Undefined
template<typename GC>
Value<GC> set_frame_rate(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display.Stage.showDefaultContextMenu getter
///
/// Returns whether the default context menu is shown.
///
/// @param activation The current activation context
/// @param this_value The Stage object
/// @param args Function arguments (unused)
/// @return Boolean indicating show menu setting
template<typename GC>
Value<GC> get_show_default_context_menu(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display.Stage.showDefaultContextMenu setter
///
/// Sets whether the default context menu is shown.
///
/// @param activation The current activation context
/// @param this_value The Stage object
/// @param args Function arguments (boolean at index 0)
/// @return Value::Undefined
template<typename GC>
Value<GC> set_show_default_context_menu(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display.Stage.scaleMode getter
///
/// Returns the current scale mode as a string.
///
/// @param activation The current activation context
/// @param this_value The Stage object
/// @param args Function arguments (unused)
/// @return Scale mode string
template<typename GC>
Value<GC> get_scale_mode(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display.Stage.scaleMode setter
///
/// Sets the scale mode.
///
/// @param activation The current activation context
/// @param this_value The Stage object
/// @param args Function arguments (scale mode string at index 0)
/// @return Value::Undefined
/// @throws Error 2008 if invalid scale mode
template<typename GC>
Value<GC> set_scale_mode(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display.Stage.stageFocusRect getter
///
/// Returns whether stage focus rectangle is shown.
///
/// @param activation The current activation context
/// @param this_value The Stage object
/// @param args Function arguments (unused)
/// @return Boolean indicating focus rect setting
template<typename GC>
Value<GC> get_stage_focus_rect(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display.Stage.stageFocusRect setter
///
/// Sets whether stage focus rectangle is shown.
///
/// @param activation The current activation context
/// @param this_value The Stage object
/// @param args Function arguments (boolean at index 0)
/// @return Value::Undefined
template<typename GC>
Value<GC> set_stage_focus_rect(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display.Stage.stageWidth getter
///
/// Returns the stage width in pixels.
///
/// @param activation The current activation context
/// @param this_value The Stage object
/// @param args Function arguments (unused)
/// @return Stage width as integer
template<typename GC>
Value<GC> get_stage_width(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display.Stage.stageWidth setter
///
/// Setter exists but does nothing.
///
/// @param activation The current activation context
/// @param this_value The Stage object
/// @param args Function arguments (unused)
/// @return Value::Undefined
template<typename GC>
Value<GC> set_stage_width(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display.Stage.stageHeight getter
///
/// Returns the stage height in pixels.
///
/// @param activation The current activation context
/// @param this_value The Stage object
/// @param args Function arguments (unused)
/// @return Stage height as integer
template<typename GC>
Value<GC> get_stage_height(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display.Stage.stageHeight setter
///
/// Setter exists but does nothing.
///
/// @param activation The current activation context
/// @param this_value The Stage object
/// @param args Function arguments (unused)
/// @return Value::Undefined
template<typename GC>
Value<GC> set_stage_height(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display.Stage.allowsFullScreen getter
///
/// Returns whether full screen is allowed.
///
/// @param activation The current activation context
/// @param this_value The Stage object
/// @param args Function arguments (unused)
/// @return Boolean (stubbed to true)
template<typename GC>
Value<GC> get_allows_full_screen(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display.Stage.allowsFullScreenInteractive getter
///
/// Returns whether full screen interactive is allowed.
///
/// @param activation The current activation context
/// @param this_value The Stage object
/// @param args Function arguments (unused)
/// @return Boolean (stubbed to false)
template<typename GC>
Value<GC> get_allows_full_screen_interactive(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display.Stage.quality getter
///
/// Returns the current quality setting as a string.
///
/// @param activation The current activation context
/// @param this_value The Stage object
/// @param args Function arguments (unused)
/// @return Quality string
template<typename GC>
Value<GC> get_quality(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display.Stage.quality setter
///
/// Sets the quality setting.
///
/// @param activation The current activation context
/// @param this_value The Stage object
/// @param args Function arguments (quality string at index 0)
/// @return Value::Undefined
template<typename GC>
Value<GC> set_quality(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display.Stage.stage3Ds getter
///
/// Returns a Vector of Stage3D objects.
///
/// @param activation The current activation context
/// @param this_value The Stage object
/// @param args Function arguments (unused)
/// @return Vector of Stage3D objects
template<typename GC>
Value<GC> get_stage3ds(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display.Stage.invalidate
///
/// Marks the stage as invalidated for redraw.
///
/// @param activation The current activation context
/// @param this_value The Stage object
/// @param args Function arguments (unused)
/// @return Value::Undefined
template<typename GC>
Value<GC> invalidate(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display.Stage.fullScreenHeight getter
///
/// Returns the full screen height (stubbed to 768).
///
/// @param activation The current activation context
/// @param this_value The Stage object
/// @param args Function arguments (unused)
/// @return Full screen height as integer
template<typename GC>
Value<GC> get_full_screen_height(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display.Stage.fullScreenWidth getter
///
/// Returns the full screen width (stubbed to 1024).
///
/// @param activation The current activation context
/// @param this_value The Stage object
/// @param args Function arguments (unused)
/// @return Full screen width as integer
template<typename GC>
Value<GC> get_full_screen_width(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display.Stage.tabChildren setter
///
/// Propagates tab children setting to the AVM2 root.
///
/// @param activation The current activation context
/// @param this_value The Stage object
/// @param args Function arguments (boolean at index 0)
/// @return Value::Undefined
template<typename GC>
Value<GC> set_tab_children(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace stage
} // namespace display
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_STAGE_H
