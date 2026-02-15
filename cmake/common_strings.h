/*
 * C++ header for common AVM strings
 * This replaces the functionality of core/common/src/avm_string/common.rs
 */

#ifndef COMMON_STRINGS_H
#define COMMON_STRINGS_H

#include "avm_string.h"
#include <array>

// Forward declaration
template<typename GCContext>
class AvmAtom;

// Define the length of ASCII characters
constexpr size_t ASCII_CHARS_LEN = 0x80;

// Static array of ASCII characters
extern const std::array<uint8_t, ASCII_CHARS_LEN> ASCII_CHARS;

// Macro to define common strings similar to the Rust version
#define DEFINE_COMMON_STRINGS(ascii_field, ...) \
    template<typename GCContext> \
    class CommonStrings { \
    public: \
        std::array<AvmAtom<GCContext>, ASCII_CHARS_LEN> ascii_field; \
        __VA_ARGS__ \
        \
        static CommonStrings<GCContext> create(GCContext& gc_context) { \
            CommonStrings<GCContext> result; \
            /* Initialize ASCII characters */ \
            for (size_t i = 0; i < ASCII_CHARS_LEN; ++i) { \
                std::vector<uint8_t> char_bytes = {ASCII_CHARS[i]}; \
                result.ascii_field[i] = AvmAtom<GCContext>::intern_static(gc_context, char_bytes); \
            } \
            /* Initialize other common strings */ \
            initialize_common_strings(result, gc_context); \
            return result; \
        } \
    \
    private: \
        static void initialize_common_strings(CommonStrings<GCContext>& cs, GCContext& gc_context); \
    };

// Define common string fields
#define COMMON_STRING_FIELDS \
    AvmAtom<GCContext> str_; \
    AvmAtom<GCContext> str___constructor__; \
    AvmAtom<GCContext> str___proto__; \
    AvmAtom<GCContext> str___resolve__; \
    AvmAtom<GCContext> str__bytesLoaded; \
    AvmAtom<GCContext> str__bytesTotal; \
    AvmAtom<GCContext> str__css; \
    AvmAtom<GCContext> str__listeners; \
    AvmAtom<GCContext> str__styles; \
    AvmAtom<GCContext> str_aa; \
    AvmAtom<GCContext> str_ab; \
    AvmAtom<GCContext> str_access; \
    AvmAtom<GCContext> str_accessors; \
    AvmAtom<GCContext> str_addListener; \
    AvmAtom<GCContext> str_advanced; \
    AvmAtom<GCContext> str_album; \
    AvmAtom<GCContext> str_alphaMultiplier; \
    AvmAtom<GCContext> str_alphaOffset; \
    AvmAtom<GCContext> str_always; \
    AvmAtom<GCContext> str_arguments; \
    AvmAtom<GCContext> str_artist; \
    AvmAtom<GCContext> str_ascent; \
    AvmAtom<GCContext> str_asyncError; \
    AvmAtom<GCContext> str_auto; \
    AvmAtom<GCContext> str_ba; \
    AvmAtom<GCContext> str_baseline; \
    AvmAtom<GCContext> str_baselineConstrained; \
    AvmAtom<GCContext> str_baselineExtended; \
    AvmAtom<GCContext> str_bases; \
    AvmAtom<GCContext> str_bb; \
    AvmAtom<GCContext> str_bigEndian; \
    AvmAtom<GCContext> str_block; \
    AvmAtom<GCContext> str_blueMultiplier; \
    AvmAtom<GCContext> str_blueOffset; \
    AvmAtom<GCContext> str_bold; \
    AvmAtom<GCContext> str_boldItalic; \
    AvmAtom<GCContext> str_boolean; \
    AvmAtom<GCContext> str_Boolean; \
    AvmAtom<GCContext> str_broadcastMessage; \
    AvmAtom<GCContext> str_builtInItems; \
    AvmAtom<GCContext> str_bytesLoaded; \
    AvmAtom<GCContext> str_bytesTotal; \
    AvmAtom<GCContext> str_callee; \
    AvmAtom<GCContext> str_caller; \
    AvmAtom<GCContext> str_caption; \
    AvmAtom<GCContext> str_center; \
    AvmAtom<GCContext> str_clamp; \
    AvmAtom<GCContext> str_click; \
    AvmAtom<GCContext> str_code; \
    AvmAtom<GCContext> str_color; \
    AvmAtom<GCContext> str_comment; \
    AvmAtom<GCContext> str_complete; \
    AvmAtom<GCContext> str_constructor; \
    AvmAtom<GCContext> str_customItems; \
    AvmAtom<GCContext> str_data; \
    AvmAtom<GCContext> str_declaredBy; \
    AvmAtom<GCContext> str_decode; \
    AvmAtom<GCContext> str_descent; \
    AvmAtom<GCContext> str_description; \
    AvmAtom<GCContext> str_device; \
    AvmAtom<GCContext> str_doubleClick; \
    AvmAtom<GCContext> str_duration; \
    AvmAtom<GCContext> str_dynamic; \
    AvmAtom<GCContext> str_embedded; \
    AvmAtom<GCContext> str_embeddedCFF; \
    AvmAtom<GCContext> str_enabled; \
    AvmAtom<GCContext> str_error; \
    AvmAtom<GCContext> str_extension; \
    AvmAtom<GCContext> str_false; \
    AvmAtom<GCContext> str_flushed; \
    AvmAtom<GCContext> str_focusEnabled; \
    AvmAtom<GCContext> str_fontStyle; \
    AvmAtom<GCContext> str_fontWeight; \
    AvmAtom<GCContext> str_forward_back; \
    AvmAtom<GCContext> str_full; \
    AvmAtom<GCContext> str_fullScreen; \
    AvmAtom<GCContext> str_function; \
    AvmAtom<GCContext> str_ga; \
    AvmAtom<GCContext> str_gb; \
    AvmAtom<GCContext> str_genre; \
    AvmAtom<GCContext> str_global; \
    AvmAtom<GCContext> str_greenMultiplier; \
    AvmAtom<GCContext> str_greenOffset; \
    AvmAtom<GCContext> str_height; \
    AvmAtom<GCContext> str_httpStatus; \
    AvmAtom<GCContext> str_id3; \
    AvmAtom<GCContext> str_ignore; \
    AvmAtom<GCContext> str_ignoreWhite; \
    AvmAtom<GCContext> str_index; \
    AvmAtom<GCContext> str_Infinity; \
    AvmAtom<GCContext> str_inline; \
    AvmAtom<GCContext> str_inner; \
    AvmAtom<GCContext> str_input; \
    AvmAtom<GCContext> str_interfaces; \
    AvmAtom<GCContext> str_ioError; \
    AvmAtom<GCContext> str_isDynamic; \
    AvmAtom<GCContext> str_isFinal; \
    AvmAtom<GCContext> str_isStatic; \
    AvmAtom<GCContext> str_italic; \
    AvmAtom<GCContext> str_justify; \
    AvmAtom<GCContext> str_key; \
    AvmAtom<GCContext> str_Key; \
    AvmAtom<GCContext> str_keyDown; \
    AvmAtom<GCContext> str_keyUp; \
    AvmAtom<GCContext> str_left; \
    AvmAtom<GCContext> str_length; \
    AvmAtom<GCContext> str_level; \
    AvmAtom<GCContext> str_littleEndian; \
    AvmAtom<GCContext> str_ll; \
    AvmAtom<GCContext> str_loaded; \
    AvmAtom<GCContext> str_localhost; \
    AvmAtom<GCContext> str_localName; \
    AvmAtom<GCContext> str_loop; \
    AvmAtom<GCContext> str_lr; \
    AvmAtom<GCContext> str_macType; \
    AvmAtom<GCContext> str_matrixType; \
    AvmAtom<GCContext> str_menu; \
    AvmAtom<GCContext> str_menuItemSelect; \
    AvmAtom<GCContext> str_menuSelect; \
    AvmAtom<GCContext> str_message; \
    AvmAtom<GCContext> str_metadata; \
    AvmAtom<GCContext> str_methods; \
    AvmAtom<GCContext> str_middleClick; \
    AvmAtom<GCContext> str_middleMouseDown; \
    AvmAtom<GCContext> str_middleMouseUp; \
    AvmAtom<GCContext> str_Mouse; \
    AvmAtom<GCContext> str_mouseDown; \
    AvmAtom<GCContext> str_mouseMove; \
    AvmAtom<GCContext> str_mouseOut; \
    AvmAtom<GCContext> str_mouseOver; \
    AvmAtom<GCContext> str_mouseUp; \
    AvmAtom<GCContext> str_mouseWheel; \
    AvmAtom<GCContext> str_movieclip; \
    AvmAtom<GCContext> str_name; \
    AvmAtom<GCContext> str_NaN; \
    AvmAtom<GCContext> str_netStatus; \
    AvmAtom<GCContext> str_never; \
    AvmAtom<GCContext> str_none; \
    AvmAtom<GCContext> str_normal; \
    AvmAtom<GCContext> str_null; \
    AvmAtom<GCContext> str_number; \
    AvmAtom<GCContext> str_Number; \
    AvmAtom<GCContext> str_object; \
    AvmAtom<GCContext> str_onCancel; \
    AvmAtom<GCContext> str_onChanged; \
    AvmAtom<GCContext> str_onClose; \
    AvmAtom<GCContext> str_onComplete; \
    AvmAtom<GCContext> str_onConnect; \
    AvmAtom<GCContext> str_onData; \
    AvmAtom<GCContext> str_onDragOut; \
    AvmAtom<GCContext> str_onDragOver; \
    AvmAtom<GCContext> str_onEnterFrame; \
    AvmAtom<GCContext> str_onFullScreen; \
    AvmAtom<GCContext> str_onHTTPError; \
    AvmAtom<GCContext> str_onHTTPStatus; \
    AvmAtom<GCContext> str_onID3; \
    AvmAtom<GCContext> str_onIOError; \
    AvmAtom<GCContext> str_onKeyDown; \
    AvmAtom<GCContext> str_onKeyUp; \
    AvmAtom<GCContext> str_onLoad; \
    AvmAtom<GCContext> str_onLoadComplete; \
    AvmAtom<GCContext> str_onLoadError; \
    AvmAtom<GCContext> str_onLoadInit; \
    AvmAtom<GCContext> str_onLoadProgress; \
    AvmAtom<GCContext> str_onLoadStart; \
    AvmAtom<GCContext> str_onMouseDown; \
    AvmAtom<GCContext> str_onMouseMove; \
    AvmAtom<GCContext> str_onMouseUp; \
    AvmAtom<GCContext> str_onMouseWheel; \
    AvmAtom<GCContext> str_onOpen; \
    AvmAtom<GCContext> str_onPress; \
    AvmAtom<GCContext> str_onProgress; \
    AvmAtom<GCContext> str_onRelease; \
    AvmAtom<GCContext> str_onReleaseOutside; \
    AvmAtom<GCContext> str_onResize; \
    AvmAtom<GCContext> str_onResult; \
    AvmAtom<GCContext> str_onRollOut; \
    AvmAtom<GCContext> str_onRollOver; \
    AvmAtom<GCContext> str_onScroller; \
    AvmAtom<GCContext> str_onSelect; \
    AvmAtom<GCContext> str_onSetFocus; \
    AvmAtom<GCContext> str_onStatus; \
    AvmAtom<GCContext> str_onUnload; \
    AvmAtom<GCContext> str_onXML; \
    AvmAtom<GCContext> str_optional; \
    AvmAtom<GCContext> str_outer; \
    AvmAtom<GCContext> str_parameters; \
    AvmAtom<GCContext> str_parse; \
    AvmAtom<GCContext> str_parseXML; \
    AvmAtom<GCContext> str_pixel; \
    AvmAtom<GCContext> str_play; \
    AvmAtom<GCContext> str_position; \
    AvmAtom<GCContext> str_prefix; \
    AvmAtom<GCContext> str_print; \
    AvmAtom<GCContext> str_prototype; \
    AvmAtom<GCContext> str_push; \
    AvmAtom<GCContext> str_quality; \
    AvmAtom<GCContext> str_ra; \
    AvmAtom<GCContext> str_rb; \
    AvmAtom<GCContext> str_readonly; \
    AvmAtom<GCContext> str_readwrite; \
    AvmAtom<GCContext> str_redMultiplier; \
    AvmAtom<GCContext> str_redOffset; \
    AvmAtom<GCContext> str_regular; \
    AvmAtom<GCContext> str_releaseOutside; \
    AvmAtom<GCContext> str_removeListener; \
    AvmAtom<GCContext> str_returnType; \
    AvmAtom<GCContext> str_rewind; \
    AvmAtom<GCContext> str_right; \
    AvmAtom<GCContext> str_rightClick; \
    AvmAtom<GCContext> str_rightMouseDown; \
    AvmAtom<GCContext> str_rightMouseUp; \
    AvmAtom<GCContext> str_rl; \
    AvmAtom<GCContext> str_rollOut; \
    AvmAtom<GCContext> str_rollOver; \
    AvmAtom<GCContext> str_rr; \
    AvmAtom<GCContext> str_save; \
    AvmAtom<GCContext> str_Selection; \
    AvmAtom<GCContext> str_separatorBefore; \
    AvmAtom<GCContext> str_songname; \
    AvmAtom<GCContext> str_splice; \
    AvmAtom<GCContext> str_Stage; \
    AvmAtom<GCContext> str_standard; \
    AvmAtom<GCContext> str_standardConstrained; \
    AvmAtom<GCContext> str_standardExtended; \
    AvmAtom<GCContext> str_status; \
    AvmAtom<GCContext> str_string; \
    AvmAtom<GCContext> str_String; \
    AvmAtom<GCContext> str_subpixel; \
    AvmAtom<GCContext> str_subtract; \
    AvmAtom<GCContext> str_success; \
    AvmAtom<GCContext> str_super; \
    AvmAtom<GCContext> str_tabChildren; \
    AvmAtom<GCContext> str_tabEnabled; \
    AvmAtom<GCContext> str_target; \
    AvmAtom<GCContext> str_textFieldHeight; \
    AvmAtom<GCContext> str_textFieldWidth; \
    AvmAtom<GCContext> str_TextSnapshot; \
    AvmAtom<GCContext> str_toJSON; \
    AvmAtom<GCContext> str_toString; \
    AvmAtom<GCContext> str_toXMLString; \
    AvmAtom<GCContext> str_track; \
    AvmAtom<GCContext> str_traits; \
    AvmAtom<GCContext> str_transform; \
    AvmAtom<GCContext> str_true; \
    AvmAtom<GCContext> str_tx; \
    AvmAtom<GCContext> str_ty; \
    AvmAtom<GCContext> str_type; \
    AvmAtom<GCContext> str_undefined; \
    AvmAtom<GCContext> str_uri; \
    AvmAtom<GCContext> str_useHandCursor; \
    AvmAtom<GCContext> str_value; \
    AvmAtom<GCContext> str_valueOf; \
    AvmAtom<GCContext> str_variables; \
    AvmAtom<GCContext> str_visible; \
    AvmAtom<GCContext> str_void; \
    AvmAtom<GCContext> str_width; \
    AvmAtom<GCContext> str_wrap; \
    AvmAtom<GCContext> str_writeonly; \
    AvmAtom<GCContext> str_xMax; \
    AvmAtom<GCContext> str_xMin; \
    AvmAtom<GCContext> str_xml; \
    AvmAtom<GCContext> str_year; \
    AvmAtom<GCContext> str_yMax; \
    AvmAtom<GCContext> str_yMin; \
    AvmAtom<GCContext> str_zoom;

// Define the CommonStrings class using the macro
DEFINE_COMMON_STRINGS(ascii_chars, COMMON_STRING_FIELDS)

// Initialize the ASCII character array
constexpr std::array<uint8_t, ASCII_CHARS_LEN> init_ascii_chars() {
    std::array<uint8_t, ASCII_CHARS_LEN> chars{};
    for (size_t i = 0; i < ASCII_CHARS_LEN; ++i) {
        chars[i] = static_cast<uint8_t>(i);
    }
    return chars;
}

const std::array<uint8_t, ASCII_CHARS_LEN> ASCII_CHARS = init_ascii_chars();

#endif // COMMON_STRINGS_H