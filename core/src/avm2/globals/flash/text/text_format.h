// C++ translation of core/src/avm2/globals/flash/text/text_format.rs
// TextFormat property getters and setters for AVM2

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_TEXT_TEXT_FORMAT_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_TEXT_TEXT_FORMAT_H

#include <vector>
#include <optional>
#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC>
    class UpdateContext;
    template<typename GC>
    class AvmString;

    namespace avm2 {
        template<typename GC>
        class Activation;
        template<typename GC>
        class Value;
        template<typename GC>
        class Error;
        template<typename GC>
        class Object;
        template<typename GC>
        class ArrayObject;
        template<typename GC>
        class ArrayStorage;

        namespace parameters {
            // Parameters extension traits
        }
    }

    namespace html {
        enum class TextDisplay;
    }

    namespace ecma_conversions {
        // ECMA conversion utilities
        template<typename T>
        T round_to_even(double value);
    }
}
}

namespace swf {
    enum class TextAlign;
    struct Color;
}

namespace ruffle {
namespace core {

using avm2::Activation;
using avm2::Value;
using avm2::Error;
using avm2::Object;
using avm2::ArrayObject;
using avm2::ArrayStorage;
using html::TextDisplay;
using swf::TextAlign;
using swf::Color;

namespace avm2 {
namespace globals {
namespace flash {
namespace text {
namespace text_format {

/// TextFormat allocator re-export
/// This would be defined in textformat_allocator.h
template<typename GC>
void text_format_allocator();

// ============================================================================
// Align property
// ============================================================================

/// Get the align property of a TextFormat object.
template<typename GC>
Value<GC> get_align(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Set the align property of a TextFormat object.
template<typename GC>
Value<GC> set_align(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

// ============================================================================
// BlockIndent property
// ============================================================================

/// Get the blockIndent property of a TextFormat object.
template<typename GC>
Value<GC> get_block_indent(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Set the blockIndent property of a TextFormat object.
template<typename GC>
Value<GC> set_block_indent(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

// ============================================================================
// Bold property
// ============================================================================

/// Get the bold property of a TextFormat object.
template<typename GC>
Value<GC> get_bold(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Set the bold property of a TextFormat object.
template<typename GC>
Value<GC> set_bold(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

// ============================================================================
// Bullet property
// ============================================================================

/// Get the bullet property of a TextFormat object.
template<typename GC>
Value<GC> get_bullet(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Set the bullet property of a TextFormat object.
template<typename GC>
Value<GC> set_bullet(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

// ============================================================================
// Color property
// ============================================================================

/// Get the color property of a TextFormat object.
template<typename GC>
Value<GC> get_color(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Set the color property of a TextFormat object.
template<typename GC>
Value<GC> set_color(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

// ============================================================================
// Display property
// ============================================================================

/// Get the display property of a TextFormat object.
template<typename GC>
Value<GC> get_display(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Set the display property of a TextFormat object.
template<typename GC>
Value<GC> set_display(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

// ============================================================================
// Font property
// ============================================================================

/// Get the font property of a TextFormat object.
template<typename GC>
Value<GC> get_font(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Set the font property of a TextFormat object.
template<typename GC>
Value<GC> set_font(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

// ============================================================================
// Indent property
// ============================================================================

/// Get the indent property of a TextFormat object.
template<typename GC>
Value<GC> get_indent(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Set the indent property of a TextFormat object.
template<typename GC>
Value<GC> set_indent(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

// ============================================================================
// Italic property
// ============================================================================

/// Get the italic property of a TextFormat object.
template<typename GC>
Value<GC> get_italic(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Set the italic property of a TextFormat object.
template<typename GC>
Value<GC> set_italic(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

// ============================================================================
// Kerning property
// ============================================================================

/// Get the kerning property of a TextFormat object.
template<typename GC>
Value<GC> get_kerning(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Set the kerning property of a TextFormat object.
template<typename GC>
Value<GC> set_kerning(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

// ============================================================================
// Leading property
// ============================================================================

/// Get the leading property of a TextFormat object.
template<typename GC>
Value<GC> get_leading(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Set the leading property of a TextFormat object.
template<typename GC>
Value<GC> set_leading(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

// ============================================================================
// LeftMargin property
// ============================================================================

/// Get the leftMargin property of a TextFormat object.
template<typename GC>
Value<GC> get_left_margin(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Set the leftMargin property of a TextFormat object.
template<typename GC>
Value<GC> set_left_margin(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

// ============================================================================
// LetterSpacing property
// ============================================================================

/// Get the letterSpacing property of a TextFormat object.
template<typename GC>
Value<GC> get_letter_spacing(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Set the letterSpacing property of a TextFormat object.
template<typename GC>
Value<GC> set_letter_spacing(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

// ============================================================================
// RightMargin property
// ============================================================================

/// Get the rightMargin property of a TextFormat object.
template<typename GC>
Value<GC> get_right_margin(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Set the rightMargin property of a TextFormat object.
template<typename GC>
Value<GC> set_right_margin(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

// ============================================================================
// Size property
// ============================================================================

/// Get the size property of a TextFormat object.
template<typename GC>
Value<GC> get_size(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Set the size property of a TextFormat object.
template<typename GC>
Value<GC> set_size(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

// ============================================================================
// TabStops property
// ============================================================================

/// Get the tabStops property of a TextFormat object.
template<typename GC>
Value<GC> get_tab_stops(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Set the tabStops property of a TextFormat object.
template<typename GC>
Value<GC> set_tab_stops(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

// ============================================================================
// Target property
// ============================================================================

/// Get the target property of a TextFormat object.
template<typename GC>
Value<GC> get_target(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Set the target property of a TextFormat object.
template<typename GC>
Value<GC> set_target(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

// ============================================================================
// Underline property
// ============================================================================

/// Get the underline property of a TextFormat object.
template<typename GC>
Value<GC> get_underline(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Set the underline property of a TextFormat object.
template<typename GC>
Value<GC> set_underline(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

// ============================================================================
// URL property
// ============================================================================

/// Get the url property of a TextFormat object.
template<typename GC>
Value<GC> get_url(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Set the url property of a TextFormat object.
template<typename GC>
Value<GC> set_url(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace text_format
} // namespace text
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_TEXT_TEXT_FORMAT_H
