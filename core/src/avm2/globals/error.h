// C++ translation of core/src/avm2/globals/error.rs
// AVM2 Error builtin/prototype implementation

#ifndef RUFFLE_CORE_AVM2_GLOBALS_ERROR_H
#define RUFFLE_CORE_AVM2_GLOBALS_ERROR_H

#include <vector>
#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class AvmString;
    template<typename T> class ErrorObject;
    enum class PlayerMode;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {

/// Implements Error.getErrorMessage
/// Returns a formatted error message string like "Error #<id>"
template<typename GC>
Value<GC> get_error_message(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Error.prototype.getStackTrace
/// Returns the stack trace as a string if enabled, otherwise null
///
/// Stack trace behavior depends on Flash Player version and SWF version:
/// - For Flash Player 11.5+ (player_version >= 18) and SWF >= 18: always enabled
/// - For Flash Player 11.4 and earlier, or SWF < 18: enabled only in Debug mode
template<typename GC>
Value<GC> get_stack_trace(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_ERROR_H
