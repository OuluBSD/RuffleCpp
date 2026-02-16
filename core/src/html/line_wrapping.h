// C++ translation of core/src/html/line_wrapping.rs
//! HTML line wrapping behavior
//!
//! FP line wrapping behavior summary.
//! NOTE: this is derived from experiment and is not guaranteed to fully match FP.
//!
//! SWF >= 8:
//! - In ASCII, only allowed wrapping points are after '-' and after (runs of) spaces.
//! - Given "aaa    bbb", the splitting (observed by getLineLength()) works as if
//!   the text was split into "aaa    " and "bbb",
//!   but for the purposes of width measurement, they behave like "aaa" and "    bbb".
//!   (this behaves as-if spaces were "collapsed")
//! - Break is allowed on any side of CJK characters
//! - BUT if CJK has a matching opening/closing character on its side, don't break.
//!   the opening/closing character doesn't need to be CJK, can be just '('.
//! - Fallback when even 1st slice doesn't fit: break on last fitting char.
//! - Final fallback:
//!   if nothing fits, fit one character per line (it'll get cut off visually).
//!
//! SWF <= 7 differences:
//! - Breaks are allowed on both sides of '-'.
//! - Any space is a valid break point.
//! - Spaces are not collapsed; only the final space of the slice (if present)
//!   is dropped for measurements.
//!   (this means we can never split "a " apart, since its space never counts)
//! - Final fallback:
//!   if resulting length has <=1 char, yield entire line without wrapping.

#ifndef RUFFLE_CORE_HTML_LINE_WRAPPING_H
#define RUFFLE_CORE_HTML_LINE_WRAPPING_H

#include <cstdint>
#include <vector>
#include <optional>

// Forward declarations
namespace ruffle {
namespace core {
namespace font {
    template<typename T> class FontLike;
    template<typename T> class EvalParameters;
}
namespace string {
    template<typename T> class WStr;
}
namespace render {
    struct Twips;
}
}
}

namespace ruffle {
namespace core {
namespace html {
namespace line_wrapping {

/// Given a line of text, find the first breakpoint within the text.
/// This assumes the input doesn't contain any mandatory breaks (newlines).
///
/// The given `offset` determines the start of the initial line, while the
/// `width` indicates how long the line is supposed to be. Be careful to
/// note that it is possible for this function to return `0`; that
/// indicates that the string itself cannot fit on the line and should
/// break onto the next one.
/// If is_start_of_line, the function is guaranteed to not return 0.
///
/// This function yields `None` if the line is not broken.
///
/// @param font The font to use for measuring text
/// @param text The text to wrap
/// @param params Evaluation parameters (height, letter spacing, kerning)
/// @param width The total width of the line
/// @param offset The start offset of the line
/// @param is_start_of_line Whether this is the start of a line
/// @param swf_version The SWF version (affects wrapping behavior)
/// @return The byte offset of the break point, or nullopt if no break is needed
template<typename GC>
std::optional<size_t> wrap_line(
    const font::FontLike<GC>* font,
    const string::WStr<GC>& text,
    const font::EvalParameters<GC>& params,
    render::Twips width,
    render::Twips offset,
    bool is_start_of_line,
    uint8_t swf_version
);

/// Find the text indices delimiting (ending) non-breakable spans of text.
///
/// @param text The text to find breaks in
/// @param swf8 Whether SWF version is >= 8 (affects break behavior)
/// @return Vector of text indices where breaks are allowed
std::vector<size_t> find_allowed_breaks(const string::WStr<GC>& text, bool swf8);

// Helper functions for CJK character detection
// These would be implemented in the string module

/// Check if a character is CJK-like
bool is_cjk_like(char32_t ch);

/// Check if a character is an opening character
bool is_opening(char32_t ch);

/// Check if a character is a closing character
bool is_closing(char32_t ch);

} // namespace line_wrapping
} // namespace html
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_HTML_LINE_WRAPPING_H
