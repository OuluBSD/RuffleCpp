// C++ translation of core/src/avm1/globals/stage.rs
// Stage object

#ifndef RUFFLE_CORE_AVM1_GLOBALS_STAGE_H
#define RUFFLE_CORE_AVM1_GLOBALS_STAGE_H

#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
namespace string {
    template<typename T> class AvmString;
    template<typename T> class WStr;
    template<typename T> class WString;
}
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Error;
    template<typename T> class Object;
    template<typename T> class Value;
    struct DeclContext;
    struct StaticDeclarations;
    namespace globals {
        namespace as_broadcaster {
            template<typename T> struct BroadcasterFunctions;
        }
    }
}
namespace display_object {
    enum class StageAlign;
    enum class StageDisplayState;
    enum class StageScaleMode;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace stage {

/// Property declarations for Stage object
extern const StaticDeclarations OBJECT_DECLS;

/// Create the Stage object
///
/// \param context The declaration context for creating the object
/// \param broadcaster_functions The broadcaster functions for AsBroadcaster integration
/// \param array_proto The array prototype for the _listeners array
/// \return The created Stage object
template<typename GC>
Object<GC>* create(
    DeclContext<GC>* context,
    globals::as_broadcaster::BroadcasterFunctions<GC> broadcaster_functions,
    Object<GC>* array_proto
);

/// Implements Stage.align getter
///
/// Gets the current stage alignment as a string.
/// Returns a combination of 'L', 'T', 'R', 'B' characters.
///
/// \param activation The current activation context
/// \param this_obj The Stage object (unused)
/// \param args Arguments (unused)
/// \return The alignment string
template<typename GC>
Value<GC> align(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args
);

/// Implements Stage.align setter
///
/// Sets the stage alignment from a string.
///
/// \param activation The current activation context
/// \param this_obj The Stage object (unused)
/// \param args Arguments (alignment string)
/// \return Value::Undefined
template<typename GC>
Value<GC> set_align(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args
);

/// Implements Stage.height getter
///
/// Gets the current stage height.
///
/// \param activation The current activation context
/// \param this_obj The Stage object (unused)
/// \param args Arguments (unused)
/// \return The stage height
template<typename GC>
Value<GC> height(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args
);

/// Implements Stage.scaleMode getter
///
/// Gets the current stage scale mode.
///
/// \param activation The current activation context
/// \param this_obj The Stage object (unused)
/// \param args Arguments (unused)
/// \return The scale mode string
template<typename GC>
Value<GC> scale_mode(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args
);

/// Implements Stage.scaleMode setter
///
/// Sets the stage scale mode from a string.
///
/// \param activation The current activation context
/// \param this_obj The Stage object (unused)
/// \param args Arguments (scale mode string)
/// \return Value::Undefined
template<typename GC>
Value<GC> set_scale_mode(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args
);

/// Implements Stage.displayState getter
///
/// Gets the current display state ("fullScreen" or "normal").
///
/// \param activation The current activation context
/// \param this_obj The Stage object (unused)
/// \param args Arguments (unused)
/// \return The display state string
template<typename GC>
Value<GC> display_state(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args
);

/// Implements Stage.displayState setter
///
/// Sets the display state from a string.
///
/// \param activation The current activation context
/// \param this_obj The Stage object (unused)
/// \param args Arguments (display state string)
/// \return Value::Undefined
template<typename GC>
Value<GC> set_display_state(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args
);

/// Implements Stage.showMenu getter
///
/// Gets whether the context menu is shown.
///
/// \param activation The current activation context
/// \param this_obj The Stage object (unused)
/// \param args Arguments (unused)
/// \return true if menu is shown, false otherwise
template<typename GC>
Value<GC> show_menu(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args
);

/// Implements Stage.showMenu setter
///
/// Sets whether the context menu is shown.
///
/// \param activation The current activation context
/// \param this_obj The Stage object (unused)
/// \param args Arguments (show menu boolean)
/// \return Value::Undefined
template<typename GC>
Value<GC> set_show_menu(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args
);

/// Implements Stage.width getter
///
/// Gets the current stage width.
///
/// \param activation The current activation context
/// \param this_obj The Stage object (unused)
/// \param args Arguments (unused)
/// \return The stage width
template<typename GC>
Value<GC> width(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args
);

} // namespace stage
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_STAGE_H
