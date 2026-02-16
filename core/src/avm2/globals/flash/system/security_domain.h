// C++ translation of core/src/avm2/globals/flash/system/security_domain.rs
// flash.system.SecurityDomain native methods

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_SYSTEM_SECURITY_DOMAIN_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_SYSTEM_SECURITY_DOMAIN_H

#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class ClassObject;
    template<typename T> class Error;
    template<typename T> class Object;
    template<typename T> class Value;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace system {
namespace security_domain {

/// Implements SecurityDomain allocator
/// Returns an error as SecurityDomain cannot be directly allocated
template<typename GC>
Error<GC> security_domain_allocator(
    ClassObject<GC> class_obj,
    Activation<GC>* activation
);

/// Implements SecurityDomain instantiation
/// Creates a new SecurityDomain object
template<typename GC>
Value<GC> instantiate_internal(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace security_domain
} // namespace system
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_SYSTEM_SECURITY_DOMAIN_H
