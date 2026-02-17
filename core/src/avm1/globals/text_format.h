// C++ translation of core/src/avm1/globals/text_format.rs
// TextFormat impl for AVM1

#ifndef RUFFLE_CORE_AVM1_GLOBALS_TEXT_FORMAT_H
#define RUFFLE_CORE_AVM1_GLOBALS_TEXT_FORMAT_H

#include <cstdint>
#include <vector>
#include <optional>
#include <memory>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Error;
    template<typename T> class NativeObject;
    template<typename T> class Object;
    template<typename T> class Value;
    struct DeclContext;
    struct StaticDeclarations;
    template<typename T> class SystemClass;
    template<typename T> class ArrayBuilder;
}
namespace html {
    template<typename T> class TextFormat;
    enum class TextDisplay;
}
namespace display_object {
    template<typename T> class EditText;
    template<typename T> class TDisplayObject;
    enum class AutoSizeMode;
}
namespace gc_arena {
    template<typename T> class Gc;
}
namespace swf {
    struct Color;
    enum class TextAlign;
}
namespace string {
    template<typename T> class AvmString;
    template<typename T> class WStr;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace text_format {

/// Property declarations for TextFormat prototype
extern const StaticDeclarations PROTO_DECLS;

/// Create the TextFormat class
///
/// @param context The declaration context for creating the class
/// @param super_proto The prototype object for the superclass
/// @return A SystemClass containing the created TextFormat class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// TextFormat constructor
///
/// Creates a new TextFormat object with optional initial properties.
///
/// @param activation The current activation context
/// @param this_obj The object being constructed
/// @param args Constructor arguments (font, size, color, bold, italic, underline, url, target, align, leftMargin, rightMargin, indent, leading)
/// @return The constructed TextFormat object
template<typename GC>
Value<GC> constructor(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Get the font property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object
/// @return The font name or null
template<typename GC>
Value<GC> font(
    Activation<GC>* activation,
    const html::TextFormat<GC>* text_format
);

/// Set the font property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object to modify
/// @param value The new font value
template<typename GC>
void set_font(
    Activation<GC>* activation,
    html::TextFormat<GC>* text_format,
    const Value<GC>* value
);

/// Get the size property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object
/// @return The font size or null
template<typename GC>
Value<GC> size(
    Activation<GC>* activation,
    const html::TextFormat<GC>* text_format
);

/// Set the size property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object to modify
/// @param value The new size value
template<typename GC>
void set_size(
    Activation<GC>* activation,
    html::TextFormat<GC>* text_format,
    const Value<GC>* value
);

/// Get the color property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object
/// @return The color value (RGBA) or null
template<typename GC>
Value<GC> color(
    Activation<GC>* activation,
    const html::TextFormat<GC>* text_format
);

/// Set the color property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object to modify
/// @param value The new color value
template<typename GC>
void set_color(
    Activation<GC>* activation,
    html::TextFormat<GC>* text_format,
    const Value<GC>* value
);

/// Get the url property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object
/// @return The URL string or null
template<typename GC>
Value<GC> url(
    Activation<GC>* activation,
    const html::TextFormat<GC>* text_format
);

/// Set the url property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object to modify
/// @param value The new URL value
template<typename GC>
void set_url(
    Activation<GC>* activation,
    html::TextFormat<GC>* text_format,
    const Value<GC>* value
);

/// Get the target property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object
/// @return The target string or null
template<typename GC>
Value<GC> target(
    Activation<GC>* activation,
    const html::TextFormat<GC>* text_format
);

/// Set the target property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object to modify
/// @param value The new target value
template<typename GC>
void set_target(
    Activation<GC>* activation,
    html::TextFormat<GC>* text_format,
    const Value<GC>* value
);

/// Get the bold property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object
/// @return The bold flag or null
template<typename GC>
Value<GC> bold(
    Activation<GC>* activation,
    const html::TextFormat<GC>* text_format
);

/// Set the bold property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object to modify
/// @param value The new bold value
template<typename GC>
void set_bold(
    Activation<GC>* activation,
    html::TextFormat<GC>* text_format,
    const Value<GC>* value
);

/// Get the italic property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object
/// @return The italic flag or null
template<typename GC>
Value<GC> italic(
    Activation<GC>* activation,
    const html::TextFormat<GC>* text_format
);

/// Set the italic property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object to modify
/// @param value The new italic value
template<typename GC>
void set_italic(
    Activation<GC>* activation,
    html::TextFormat<GC>* text_format,
    const Value<GC>* value
);

/// Get the underline property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object
/// @return The underline flag or null
template<typename GC>
Value<GC> underline(
    Activation<GC>* activation,
    const html::TextFormat<GC>* text_format
);

/// Set the underline property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object to modify
/// @param value The new underline value
template<typename GC>
void set_underline(
    Activation<GC>* activation,
    html::TextFormat<GC>* text_format,
    const Value<GC>* value
);

/// Get the align property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object
/// @return The align string ("left", "center", "right", "justify") or null
template<typename GC>
Value<GC> align(
    Activation<GC>* activation,
    const html::TextFormat<GC>* text_format
);

/// Set the align property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object to modify
/// @param value The new align value
template<typename GC>
void set_align(
    Activation<GC>* activation,
    html::TextFormat<GC>* text_format,
    const Value<GC>* value
);

/// Get the leftMargin property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object
/// @return The left margin value or null
template<typename GC>
Value<GC> left_margin(
    Activation<GC>* activation,
    const html::TextFormat<GC>* text_format
);

/// Set the leftMargin property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object to modify
/// @param value The new left margin value
template<typename GC>
void set_left_margin(
    Activation<GC>* activation,
    html::TextFormat<GC>* text_format,
    const Value<GC>* value
);

/// Get the rightMargin property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object
/// @return The right margin value or null
template<typename GC>
Value<GC> right_margin(
    Activation<GC>* activation,
    const html::TextFormat<GC>* text_format
);

/// Set the rightMargin property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object to modify
/// @param value The new right margin value
template<typename GC>
void set_right_margin(
    Activation<GC>* activation,
    html::TextFormat<GC>* text_format,
    const Value<GC>* value
);

/// Get the indent property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object
/// @return The indent value or null
template<typename GC>
Value<GC> indent(
    Activation<GC>* activation,
    const html::TextFormat<GC>* text_format
);

/// Set the indent property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object to modify
/// @param value The new indent value
template<typename GC>
void set_indent(
    Activation<GC>* activation,
    html::TextFormat<GC>* text_format,
    const Value<GC>* value
);

/// Get the leading property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object
/// @return The leading value or null
template<typename GC>
Value<GC> leading(
    Activation<GC>* activation,
    const html::TextFormat<GC>* text_format
);

/// Set the leading property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object to modify
/// @param value The new leading value
template<typename GC>
void set_leading(
    Activation<GC>* activation,
    html::TextFormat<GC>* text_format,
    const Value<GC>* value
);

/// Get the blockIndent property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object
/// @return The block indent value or null
template<typename GC>
Value<GC> block_indent(
    Activation<GC>* activation,
    const html::TextFormat<GC>* text_format
);

/// Set the blockIndent property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object to modify
/// @param value The new block indent value
template<typename GC>
void set_block_indent(
    Activation<GC>* activation,
    html::TextFormat<GC>* text_format,
    const Value<GC>* value
);

/// Get the tabStops property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object
/// @return Array of tab stop values or null
template<typename GC>
Value<GC> tab_stops(
    Activation<GC>* activation,
    const html::TextFormat<GC>* text_format
);

/// Set the tabStops property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object to modify
/// @param value The new tab stops array
template<typename GC>
void set_tab_stops(
    Activation<GC>* activation,
    html::TextFormat<GC>* text_format,
    const Value<GC>* value
);

/// Get the bullet property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object
/// @return The bullet flag or null
template<typename GC>
Value<GC> bullet(
    Activation<GC>* activation,
    const html::TextFormat<GC>* text_format
);

/// Set the bullet property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object to modify
/// @param value The new bullet value
template<typename GC>
void set_bullet(
    Activation<GC>* activation,
    html::TextFormat<GC>* text_format,
    const Value<GC>* value
);

/// Get the display property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object
/// @return The display string ("block", "inline", "none") or null
template<typename GC>
Value<GC> display(
    Activation<GC>* activation,
    const html::TextFormat<GC>* text_format
);

/// Set the display property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object to modify
/// @param value The new display value
template<typename GC>
void set_display(
    Activation<GC>* activation,
    html::TextFormat<GC>* text_format,
    const Value<GC>* value
);

/// Get the kerning property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object
/// @return The kerning flag or null
template<typename GC>
Value<GC> kerning(
    Activation<GC>* activation,
    const html::TextFormat<GC>* text_format
);

/// Set the kerning property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object to modify
/// @param value The new kerning value
template<typename GC>
void set_kerning(
    Activation<GC>* activation,
    html::TextFormat<GC>* text_format,
    const Value<GC>* value
);

/// Get the letterSpacing property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object
/// @return The letter spacing value or null
template<typename GC>
Value<GC> letter_spacing(
    Activation<GC>* activation,
    const html::TextFormat<GC>* text_format
);

/// Set the letterSpacing property
///
/// @param activation The current activation context
/// @param text_format The TextFormat object to modify
/// @param value The new letter spacing value
template<typename GC>
void set_letter_spacing(
    Activation<GC>* activation,
    html::TextFormat<GC>* text_format,
    const Value<GC>* value
);

/// Implements TextFormat.getTextExtent method
///
/// Measures the dimensions of text with this format.
///
/// @param activation The current activation context
/// @param text_format The TextFormat object
/// @param args Arguments (text, width)
/// @return Object with metrics (ascent, descent, width, height, textFieldHeight, textFieldWidth)
template<typename GC>
Value<GC> get_text_extent(
    Activation<GC>* activation,
    const html::TextFormat<GC>* text_format,
    const Value<GC>* args,
    size_t arg_count
);

} // namespace text_format
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_TEXT_FORMAT_H
