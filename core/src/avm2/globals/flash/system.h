// C++ translation of core/src/avm2/globals/flash/system.rs
// flash.system namespace

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_SYSTEM_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_SYSTEM_H

#include <string>

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

// flash.system submodules
namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace system {

// Submodules
namespace application_domain {
    // ApplicationDomain functionality defined in application_domain.h
}

namespace capabilities {
    // Capabilities functionality defined in capabilities.h
}

namespace security {
    // Security functionality defined in security.h
}

namespace security_domain {
    // SecurityDomain functionality defined in security_domain.h
}

namespace system {
    // System functionality defined in system.h
}

namespace worker {
    // Worker functionality defined in worker.h
}

namespace worker_domain {
    // WorkerDomain functionality defined in worker_domain.h
}

/// Implements flash.system.fscommand method
template<typename GC>
Value<GC> fscommand(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace system
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_SYSTEM_H
