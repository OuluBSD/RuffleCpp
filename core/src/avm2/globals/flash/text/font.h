// C++ translation of core/src/avm2/globals/flash/text/font.rs
// flash.text.Font implementation

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_TEXT_FONT_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_TEXT_FONT_H

#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class Object;
    template<typename T> class AvmString;
    namespace parameters {
        class ParametersExt;
    }
}
namespace font {
    template<typename T> class Font;
    enum class FontType;
}
namespace character {
    enum class Character;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace text {
namespace font {

/// Implements flash.text.Font.fontName getter
/// Returns the name of the font
/// @param activation The current AVM2 activation
/// @param this_value The Font object
template<typename GC>
Value<GC> get_font_name(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.text.Font.fontStyle getter
/// Returns the style of the font (regular, italic, bold, boldItalic)
/// @param activation The current AVM2 activation
/// @param this_value The Font object
template<typename GC>
Value<GC> get_font_style(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.text.Font.fontType getter
/// Returns the type of the font (embedded, embeddedCFF, device)
/// @param activation The current AVM2 activation
/// @param this_value The Font object
template<typename GC>
Value<GC> get_font_type(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.text.Font.hasGlyphs
/// Checks if the font has glyphs for the specified string
/// @param activation The current AVM2 activation
/// @param this_value The Font object
/// @param args Method arguments: string to check
template<typename GC>
Value<GC> has_glyphs(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.text.Font.enumerateFonts
/// Returns an array of all available fonts
/// @param activation The current AVM2 activation
/// @param args Method arguments: enumerateDeviceFonts (bool)
template<typename GC>
Value<GC> enumerate_fonts(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.text.Font.registerFont
/// Registers a font class for global use
/// @param activation The current AVM2 activation
/// @param args Method arguments: font (Class object)
template<typename GC>
Value<GC> register_font(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace font
} // namespace text
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_TEXT_FONT_H
