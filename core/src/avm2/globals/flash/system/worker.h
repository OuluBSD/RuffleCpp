// C++ translation of core/src/avm2/globals/flash/system/worker.rs
// flash.system.Worker native methods

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_SYSTEM_WORKER_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_SYSTEM_WORKER_H

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
namespace system {
namespace worker {

/// Implements Worker.createMessageChannel
template<typename GC>
Value<GC> create_message_channel(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Worker.instantiateInternal
template<typename GC>
Value<GC> instantiate_internal(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace worker
} // namespace system
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_SYSTEM_WORKER_H
