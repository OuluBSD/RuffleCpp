// C++ translation of core/src/avm2/globals/flash/system/system.rs
// flash.system.System native methods

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_SYSTEM_SYSTEM_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_SYSTEM_SYSTEM_H

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
namespace system {

/// Implements flash.system.System.setClipboard method
/// Sets clipboard content with the given text
/// Note: Plugin restrictions may apply (not enforced in desktop builds)
template<typename GC>
Value<GC> set_clipboard(
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

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_SYSTEM_SYSTEM_H
