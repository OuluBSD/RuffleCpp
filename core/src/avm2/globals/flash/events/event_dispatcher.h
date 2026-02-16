// C++ translation of core/src/avm2/globals/flash/events/event_dispatcher.rs
// flash.events.EventDispatcher builtin/prototype

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_EVENTS_EVENT_DISPATCHER_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_EVENTS_EVENT_DISPATCHER_H

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
namespace event_dispatcher {

/// Get an object's dispatch list, lazily initializing it if necessary.
template<typename GC>
Object<GC> dispatch_list(
    Activation<GC>* activation,
    Object<GC> this_obj
);

/// Implements EventDispatcher.addEventListener.
template<typename GC>
Value<GC> add_event_listener(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements EventDispatcher.removeEventListener.
template<typename GC>
Value<GC> remove_event_listener(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements EventDispatcher.hasEventListener.
template<typename GC>
Value<GC> has_event_listener(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements EventDispatcher.willTrigger.
template<typename GC>
Value<GC> will_trigger(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements EventDispatcher.dispatchEvent.
template<typename GC>
Value<GC> dispatch_event_internal(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace event_dispatcher
} // namespace events
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_EVENTS_EVENT_DISPATCHER_H
