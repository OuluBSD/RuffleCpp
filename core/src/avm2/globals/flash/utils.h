// C++ translation of core/src/avm2/globals/flash/utils.rs
// flash.utils namespace

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_UTILS_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_UTILS_H

#include <cstdint>
#include <string>
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

// flash.utils submodules
namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace utils {

// ByteArray module
namespace byte_array {
    // ByteArray functionality defined in byte_array.h
}

// Dictionary module
namespace dictionary {
    // Dictionary functionality defined in dictionary.h
}

// Proxy module
namespace proxy {
    // Proxy functionality defined in proxy.h
}

// Timer module
namespace timer {
    // Timer functionality defined in timer.h
}

/// Implements flash.utils.getTimer
template<typename GC>
Value<GC> get_timer(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.utils.setInterval
template<typename GC>
Value<GC> set_interval(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.utils.clearInterval
template<typename GC>
Value<GC> clear_interval(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.utils.setTimeout
template<typename GC>
Value<GC> set_timeout(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.utils.clearTimeout
template<typename GC>
Value<GC> clear_timeout(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.utils.escapeMultiByte
template<typename GC>
Value<GC> escape_multi_byte(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.utils.unescapeMultiByte
template<typename GC>
Value<GC> unescape_multi_byte(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.utils.getQualifiedClassName
template<typename GC>
Value<GC> get_qualified_class_name(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.utils.getQualifiedSuperclassName
template<typename GC>
Value<GC> get_qualified_superclass_name(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.utils.getDefinitionByName
template<typename GC>
Value<GC> get_definition_by_name(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace utils
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_UTILS_H
