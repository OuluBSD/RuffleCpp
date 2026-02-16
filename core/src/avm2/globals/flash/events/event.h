// C++ translation of core/src/avm2/globals/flash/events/event.rs
// flash.events.Event builtin/prototype

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_EVENTS_EVENT_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_EVENTS_EVENT_H

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
namespace event {

// Re-export event_allocator from the object module
template<typename GC>
using event_allocator = ruffle::core::avm2::object::event_allocator<GC>;

/// Implements init
template<typename GC>
Value<GC> init(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements bubbles property's getter
template<typename GC>
Value<GC> get_bubbles(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements cancelable property's getter
template<typename GC>
Value<GC> get_cancelable(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements type property's getter
template<typename GC>
Value<GC> get_type(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements target property's getter
template<typename GC>
Value<GC> get_target(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements currentTarget property's getter
template<typename GC>
Value<GC> get_current_target(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements eventPhase property's getter
template<typename GC>
Value<GC> get_event_phase(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements isDefaultPrevented
template<typename GC>
Value<GC> is_default_prevented(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements preventDefault
template<typename GC>
Value<GC> prevent_default(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements stopPropagation
template<typename GC>
Value<GC> stop_propagation(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements stopImmediatePropagation
template<typename GC>
Value<GC> stop_immediate_propagation(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace event
} // namespace events
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_EVENTS_EVENT_H
