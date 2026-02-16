// C++ translation of core/src/avm2/globals/flash/system/worker_domain.rs
// flash.system.WorkerDomain native methods

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_SYSTEM_WORKER_DOMAIN_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_SYSTEM_WORKER_DOMAIN_H

#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Error;
    template<typename T> class Value;
    template<typename T> class WorkerDomainObject;
    template<typename T> class WorkerObject;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace system {
namespace worker_domain {

/// Implements WorkerDomain.createWorker
/// Creates a new worker from the given SWF
template<typename GC>
Value<GC> create_worker(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements WorkerDomain instantiation internal method
/// Creates a new WorkerDomain object
template<typename GC>
Value<GC> instantiate_internal(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace worker_domain
} // namespace system
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_SYSTEM_WORKER_DOMAIN_H
