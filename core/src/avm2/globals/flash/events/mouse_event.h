// C++ translation of core/src/avm2/globals/flash/events/mouse_event.rs
// flash.events.MouseEvent

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_EVENTS_MOUSE_EVENT_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_EVENTS_MOUSE_EVENT_H

#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Object;
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
namespace mouse_event {

/// Implements stageX's getter.
template<typename GC>
Value<GC> get_stage_x(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements stageY's getter.
template<typename GC>
Value<GC> get_stage_y(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements updateAfterEvent.
template<typename GC>
Value<GC> update_after_event(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Helper function to convert local X to stage X.
template<typename GC>
Value<GC> local_to_stage_x(
    Activation<GC>* activation,
    Object<GC> this_obj,
    uint32_t slot_x,
    uint32_t slot_y
);

/// Helper function to convert local Y to stage Y.
template<typename GC>
Value<GC> local_to_stage_y(
    Activation<GC>* activation,
    Object<GC> this_obj,
    uint32_t slot_x,
    uint32_t slot_y
);

} // namespace mouse_event
} // namespace events
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_EVENTS_MOUSE_EVENT_H
