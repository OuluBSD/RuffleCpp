// C++ translation of wstr/src/ops.rs
// Operations on WStr (iteration, comparison, formatting, etc.)

#ifndef RUFFLE_WSTR_OPS_H
#define RUFFLE_WSTR_OPS_H

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <functional>
#include <optional>
#include <compare>

// Forward declarations
namespace ruffle {
namespace wstr {
    class WStr;
    class WString;
    template<typename T, typename U>
    struct Units;
}
}

namespace ruffle {
namespace wstr {
namespace ops {

/// Iterator over WStr code units
class Iter {
    Units<std::vector<std::uint8_t>::const_iterator,
          std::vector<std::uint16_t>::const_iterator> inner_;

public:
    explicit Iter(const Units<std::vector<std::uint8_t>::const_iterator,
                              std::vector<std::uint16_t>::const_iterator>& inner)
        : inner_(inner) {}

    using value_type = std::uint16_t;

    std::optional<std::uint16_t> next();
    std::optional<std::uint16_t> next_back();
};

/// Iterator over WStr characters (UTF-16 decoded)
class Chars {
    Iter iter_;
    // UTF-16 decoding state would be tracked here

public:
    explicit Chars(const Iter& iter) : iter_(iter) {}

    std::optional<char32_t> next();
};

/// Iterator over WStr characters with indices
class CharIndices {
    Chars chars_;
    std::size_t start_;

public:
    explicit CharIndices(const Chars& chars) : chars_(chars), start_(0) {}

    struct Item {
        std::size_t index;
        std::optional<char32_t> character;
    };

    std::optional<Item> next();
};

/// Create an iterator over a WStr
Iter str_iter(const WStr* s);

/// Create a char indices iterator
CharIndices str_char_indices(const WStr* s);

/// Format a WStr to a string
std::string str_fmt(const WStr* s);

/// Debug format a WStr (with quotes and escaping)
std::string str_debug_fmt(const WStr* s);

/// Compare two WStr for equality
bool str_eq(const WStr* left, const WStr* right);

/// Compare two WStr for equality (case-insensitive)
bool str_eq_ignore_case(const WStr* left, const WStr* right);

/// Compare two WStr lexicographically
std::strong_ordering str_cmp(const WStr* left, const WStr* right);

/// Compare two WStr lexicographically (case-insensitive)
std::strong_ordering str_cmp_ignore_case(const WStr* left, const WStr* right);

/// Hash a WStr
std::size_t str_hash(const WStr* s);

/// Get the offset of one WStr within another
std::optional<std::size_t> str_offset_in(const WStr* s, const WStr* other);

/// Convert WStr to ASCII lowercase
WString str_to_ascii_lowercase(const WStr* s);

/// Convert WStr to ASCII lowercase in-place
void str_make_ascii_lowercase(WStr* s);

/// Convert WStr to ASCII uppercase
WString str_to_ascii_uppercase(const WStr* s);

/// Convert WStr to ASCII uppercase in-place
void str_make_ascii_uppercase(WStr* s);

/// Check if WStr contains only LATIN1 characters
bool str_is_latin1(const WStr* s);

/// Join multiple WStr with a separator
WString str_join(const std::vector<const WStr*>& elems, const WStr* sep);

/// Repeat a WStr n times
WString str_repeat(const WStr* s, std::size_t count);

/// Replace all occurrences of a pattern with a replacement
WString str_replace(const WStr* haystack, const WStr* pattern, const WStr* with);

/// Find the first occurrence of a pattern
std::optional<std::size_t> str_find(const WStr* haystack, const WStr* pattern);

/// Find the last occurrence of a pattern
std::optional<std::size_t> str_rfind(const WStr* haystack, const WStr* pattern);

/// Split a WStr by a pattern
class Split {
    const WStr* string_;
    std::size_t prev_end_;
    const WStr* pattern_;
    std::size_t pattern_pos_;

public:
    Split(const WStr* string, const WStr* pattern)
        : string_(string), prev_end_(0), pattern_(pattern), pattern_pos_(0) {}

    std::optional<const WStr*> next();
};

Split str_split(const WStr* string, const WStr* pattern);

/// Split a WStr once by a pattern
std::optional<std::pair<const WStr*, const WStr*>> str_split_once(
    const WStr* string, const WStr* pattern);

/// Split a WStr once from the right by a pattern
std::optional<std::pair<const WStr*, const WStr*>> str_rsplit_once(
    const WStr* string, const WStr* pattern);

/// Check if WStr starts with a pattern
bool starts_with(const WStr* string, const WStr* pattern);

/// Check if WStr ends with a pattern
bool ends_with(const WStr* string, const WStr* pattern);

/// Strip prefix matching a pattern
std::optional<const WStr*> strip_prefix(const WStr* string, const WStr* pattern);

/// Strip suffix matching a pattern
std::optional<const WStr*> strip_suffix(const WStr* string, const WStr* pattern);

/// Trim matches from both ends
const WStr* str_trim_matches(const WStr* string, const WStr* pattern);

/// Trim matches from the start
const WStr* str_trim_start_matches(const WStr* string, const WStr* pattern);

/// Trim matches from the end
const WStr* str_trim_end_matches(const WStr* string, const WStr* pattern);

} // namespace ops
} // namespace wstr
} // namespace ruffle

#endif // RUFFLE_WSTR_OPS_H
