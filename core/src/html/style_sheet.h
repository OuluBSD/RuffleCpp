// C++ translation of core/src/html/style_sheet.rs
// CSS style sheet parsing for HTML text fields

#ifndef RUFFLE_CORE_HTML_STYLE_SHEET_H
#define RUFFLE_CORE_HTML_STYLE_SHEET_H

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <optional>
#include <cstdint>
#include <string_view>

// Forward declarations
namespace ruffle {
namespace wstr {
    class WString;
    class WStr;
}
namespace core {
namespace html {
    class TextFormat;
}
}
}

namespace ruffle {
namespace core {
namespace html {

/// CSS style sheet for HTML text fields
template<typename GC>
class StyleSheet {
public:
    StyleSheet() = default;
    explicit StyleSheet(std::shared_ptr<StyleSheetData> data) : data_(std::move(data)) {}

    /// Create a new style sheet
    static StyleSheet create();

    /// Get a style by selector
    std::optional<TextFormat> get_style(const wstr::WStr& selector) const;

    /// Set a style for a selector
    void set_style(wstr::WString selector, const TextFormat& format);

    /// Remove a style by selector
    void remove_style(const wstr::WStr& selector);

    /// Clear all styles
    void clear();

    /// Get all selectors
    std::vector<wstr::WString> selectors() const;

private:
    std::shared_ptr<StyleSheetData> data_;
};

/// Style sheet data
class StyleSheetData {
public:
    std::unordered_map<wstr::WString, TextFormat> styles;
};

/// CSS properties map
using CssProperties = std::unordered_map<const wstr::WStr*, const wstr::WStr*>;

/// CSS parsing errors
enum class CssError {
    /// Invalid selector syntax: Name cannot contain a space
    SpaceInSelectorName,

    /// Invalid selector syntax: Expected a property block to start
    ExpectedPropertiesBlock,

    /// Invalid property syntax: Name cannot contain a space
    SpaceInPropertyName,

    /// Invalid property syntax: No value specified
    PropertyValueMissing
};

/// CSS stream parser
class CssStream {
public:
    explicit CssStream(const wstr::WStr& input)
        : input_(&input), pos_(0) {}

    /// Parse CSS into a map of selectors and properties
    std::unordered_map<const wstr::WStr*, CssProperties> parse();

    /// Parse a list of selectors (ends at '{')
    std::vector<const wstr::WStr*> parse_selectors();

    /// Parse properties from inside a block (ends at '}')
    CssProperties parse_properties();

    /// Get current position (for testing)
    size_t pos() const { return pos_; }

private:
    const wstr::WStr* input_;
    size_t pos_;

    /// Skip whitespace and comments
    bool skip_whitespace_and_comments();

    /// Skip a comment (/* ... */)
    bool skip_comment();

    /// Peek at the next character
    std::optional<uint16_t> peek() const;

    /// Peek at the next two characters
    std::optional<std::pair<uint16_t, uint16_t>> peek2() const;

    /// Consume a character if it matches any in the expected list
    std::optional<uint16_t> consume_any(const uint16_t* expected, size_t count);

    /// Consume characters until any in the expected list is found
    const wstr::WStr* consume_until_any(const uint16_t* expected, size_t count);
};

/// Transform CSS property names from dashes to camelCase
std::string transform_dashes_to_camel_case(const wstr::WStr& input);

/// Parse a font list (comma-separated font names)
wstr::WString parse_font_list(const wstr::WStr& input);

// Character constants for CSS parsing
namespace css_chars {
    inline constexpr uint16_t ASTERISK = '*';
    inline constexpr uint16_t OPEN_BLOCK = '{';
    inline constexpr uint16_t CLOSE_BLOCK = '}';
    inline constexpr uint16_t COMMA = ',';
    inline constexpr uint16_t COLON = ':';
    inline constexpr uint16_t SEMI_COLON = ';';
    inline constexpr uint16_t SLASH = '/';
    inline constexpr uint16_t SPACE = ' ';
    inline constexpr uint16_t NEWLINE = '\n';
    inline constexpr uint16_t RETURN = '\r';
    inline constexpr uint16_t TAB = '\t';

    inline constexpr uint16_t ANY_VALID_WHITESPACE[] = {SPACE, NEWLINE, RETURN, TAB};
}

} // namespace html
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_HTML_STYLE_SHEET_H
