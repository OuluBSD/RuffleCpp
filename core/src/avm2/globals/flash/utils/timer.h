// C++ translation of core/src/avm2/globals/flash/utils/timer.rs
// flash.utils.Timer native methods

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_UTILS_TIMER_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_UTILS_TIMER_H

#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class TObject;
    enum class TimerCallback;
}
namespace timer {
    template<typename T> class Timers;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace slots {
    // Forward declare slot constants from flash_utils_timer
    namespace flash_utils_timer {
        extern const int _TIMER_ID;
        extern const int _DELAY;
        extern const int _ON_UPDATE_CLOSURE;
    }
}
}
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace utils {
namespace timer {

/// Implements flash.utils.Timer.stop
/// Stops the timer and removes it from the timer queue
template<typename GC>
Value<GC> stop(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.utils.Timer.start
/// Starts the timer or restarts it if already running
/// Note: Does not check if currentCount >= repeatCount
template<typename GC>
Value<GC> start(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.utils.Timer.updateDelay
/// Updates the delay of a running timer
template<typename GC>
Value<GC> update_delay(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace timer
} // namespace utils
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_UTILS_TIMER_H
