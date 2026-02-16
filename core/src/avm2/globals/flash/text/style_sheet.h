// C++ translation of core/src/avm2/globals/flash/text/style_sheet.rs
// flash.text.StyleSheet methods

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_TEXT_STYLE_SHEET_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_TEXT_STYLE_SHEET_H

#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class ScriptObject;
}
namespace string {
    template<typename T> class AvmString;
}
namespace html {
    class CssStream;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace text {
namespace style_sheet {

/// Re-export the style_sheet_allocator from object module
// Note: This would be implemented in the object module
// using ruffle::core::avm2::object::style_sheet_allocator;

/// Implements StyleSheet.parseCSS
///
/// Parses a CSS string and returns an object with selectors as properties
/// and style objects as values.
template<typename GC>
Value<GC> inner_parse_css(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements StyleSheet.parseColor
///
/// Parses a color string and returns a numeric color value.
/// Supports hex colors with # prefix.
template<typename GC>
Value<GC> inner_parse_color(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements StyleSheet.parseFontFamily
///
/// Parses a font family string and returns a processed font list.
template<typename GC>
Value<GC> inner_parse_font_family(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements StyleSheet.clear
///
/// Clears all styles from the style sheet.
template<typename GC>
Value<GC> clear_internal(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements StyleSheet.setStyle
///
/// Sets or removes a style for the given selector.
template<typename GC>
Value<GC> set_style_internal(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace style_sheet
} // namespace text
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_TEXT_STYLE_SHEET_H
