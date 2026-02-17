// C++ translation of core/src/avm2/globals/flash/text/engine/text_block.rs
// flash.text.engine TextBlock functionality

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_TEXT_ENGINE_TEXT_BLOCK_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_TEXT_ENGINE_TEXT_BLOCK_H

#include <cstdint>
#include <vector>
#include <optional>
#include <memory>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename GC> class Activation;
    template<typename GC> class Value;
    template<typename GC> class Error;
    template<typename GC> class Object;
}
namespace display_object {
    template<typename GC> class EditText;
}
namespace html {
    struct TextFormat;
}
namespace string {
    class WStr;
}
}
namespace swf {
    struct Color;
}
}

// Slot indices for flash.text.engine.TextBlock
namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace slots {
namespace flash_text_engine_text_block {
    inline constexpr int _CONTENT = 0;
    inline constexpr int _TEXT_LINE_CREATION_RESULT = 1;
    inline constexpr int _FIRST_LINE = 2;
}
}
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
namespace engine {
namespace text_block {

/// Implements flash.text.engine.TextBlock::createTextLine
///
/// Creates a text line from the content of this text block.
/// 
/// \param activation The current AVM2 activation
/// \param this_value The TextBlock instance (this)
/// \param args Function arguments:
///   - [0] previousTextLine: The previous text line (optional)
///   - [1] width: The width of the text line
/// \return A new TextLine instance, or null if no content
template<typename GC>
Value<GC> create_text_line(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Apply text format to an EditText display object
///
/// \param activation The current AVM2 activation
/// \param display_object The EditText to apply formatting to
/// \param text The text content
/// \param element_format The ElementFormat object (optional)
/// \return Result of the operation
template<typename GC>
std::optional<Error<GC>> apply_format(
    Activation<GC>* activation,
    display_object::EditText<GC> display_object,
    const string::WStr& text,
    std::optional<Object<GC>> element_format
);

} // namespace text_block
} // namespace engine
} // namespace text
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_TEXT_ENGINE_TEXT_BLOCK_H
