// C++ translation of core/src/avm2/globals/flash/text/engine/text_line.rs
// flash.text.engine.TextLine implementation

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_TEXT_ENGINE_TEXT_LINE_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_TEXT_ENGINE_TEXT_LINE_H

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
namespace text {
namespace engine {
namespace text_line {

/// Implements flash.text.engine.TextLine.textWidth getter
/// Returns the measured text width in pixels
template<typename GC>
Value<GC> get_text_width(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.text.engine.TextLine.textHeight getter
/// Returns the measured text height in pixels
template<typename GC>
Value<GC> get_text_height(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace text_line
} // namespace engine
} // namespace text
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_TEXT_ENGINE_TEXT_LINE_H
