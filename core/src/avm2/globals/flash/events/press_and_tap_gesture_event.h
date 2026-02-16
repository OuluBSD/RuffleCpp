// C++ translation of core/src/avm2/globals/flash/events/press_and_tap_gesture_event.rs
// flash.events.PressAndTapGestureEvent

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_EVENTS_PRESS_AND_TAP_GESTURE_EVENT_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_EVENTS_PRESS_AND_TAP_GESTURE_EVENT_H

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
namespace events {
namespace press_and_tap_gesture_event {

/// Implements tapStageX's getter.
template<typename GC>
Value<GC> get_tap_stage_x(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements tapStageY's getter.
template<typename GC>
Value<GC> get_tap_stage_y(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace press_and_tap_gesture_event
} // namespace events
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_EVENTS_PRESS_AND_TAP_GESTURE_EVENT_H
