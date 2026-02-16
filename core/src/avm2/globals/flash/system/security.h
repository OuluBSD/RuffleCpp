// C++ translation of core/src/avm2/globals/flash/system/security.rs
//! flash.system.Security native methods

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_SYSTEM_SECURITY_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_SYSTEM_SECURITY_H

#include <vector>
#include <string>
#include <optional>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename T> class AvmString;
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
namespace security {

/// Implements flash.system.Security.pageDomain getter
///
/// Returns the domain of the page containing the SWF, or null if unavailable.
///
/// \param activation The current activation context
/// \param this_value The this value (unused)
/// \param args Arguments (unused)
/// \return The page domain as a string, or null
template<typename GC>
Value<GC> get_page_domain(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.system.Security.sandboxType getter
///
/// Returns the sandbox type of the current movie.
///
/// \param activation The current activation context
/// \param this_value The this value (unused)
/// \param args Arguments (unused)
/// \return The sandbox type as a string
template<typename GC>
Value<GC> get_sandbox_type(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.system.Security.allowDomain method
///
/// Stub method - not yet implemented.
///
/// \param activation The current activation context
/// \param this_value The this value (unused)
/// \param args Arguments (unused)
/// \return Undefined
template<typename GC>
Value<GC> allow_domain(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.system.Security.allowInsecureDomain method
///
/// Stub method - not yet implemented.
///
/// \param activation The current activation context
/// \param this_value The this value (unused)
/// \param args Arguments (unused)
/// \return Undefined
template<typename GC>
Value<GC> allow_insecure_domain(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.system.Security.loadPolicyFile method
///
/// Stub method - not yet implemented.
///
/// \param activation The current activation context
/// \param this_value The this value (unused)
/// \param args Arguments (unused)
/// \return Undefined
template<typename GC>
Value<GC> load_policy_file(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.system.Security.showSettings method
///
/// Stub method - not yet implemented.
///
/// \param activation The current activation context
/// \param this_value The this value (unused)
/// \param args Arguments (unused)
/// \return Undefined
template<typename GC>
Value<GC> show_settings(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace security
} // namespace system
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_SYSTEM_SECURITY_H
