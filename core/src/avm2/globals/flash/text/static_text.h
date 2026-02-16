// C++ translation of core/src/avm2/globals/flash/text/static_text.rs
// StaticText.text property getter

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_TEXT_STATIC_TEXT_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_TEXT_STATIC_TEXT_H

#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Error;
    template<typename T> class Value;
}
namespace string {
    template<typename T> class AvmString;
}
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace text {
namespace static_text {

/// Implements StaticText.text property getter
/// 
/// Returns the text content of a StaticText display object.
/// Returns Null if the object is not a valid text object or has no text.
/// Returns Undefined if the object is not a StaticText.
template<typename GC>
Value<GC> get_text(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace static_text
} // namespace text
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_TEXT_STATIC_TEXT_H
