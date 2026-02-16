// C++ translation of core/src/avm2/globals/flash/events/timer_event.rs
// flash.events.TimerEvent

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_EVENTS_TIMER_EVENT_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_EVENTS_TIMER_EVENT_H

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
namespace timer_event {

/// Implements updateAfterEvent.
template<typename GC>
Value<GC> update_after_event(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace timer_event
} // namespace events
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_EVENTS_TIMER_EVENT_H
