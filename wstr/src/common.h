// C++ translation of wstr/src/common.rs
// Wide string (WStr) representation and operations

#ifndef RUFFLE_WSTR_COMMON_H
#define RUFFLE_WSTR_COMMON_H

// Standard library
#include <vector>
#include <string>
#include <cstdint>
#include <cstddef>
#include <optional>
#include <variant>
#include <memory>
#include <functional>
#include <compare>
#include <hash>
#include <format>
#include <iterator>

namespace ruffle {
namespace wstr {

/// Maximum string length, equals to 2^31-1
inline constexpr size_t WSTR_MAX_LEN = 0x7FFFFFFF;

/// A raw string buffer containing u8 or u16 code units.
template<typename T, typename U>
struct Units {
    /// Variant holding either bytes or wide units
    std::variant<T, U> data;
    
    /// Check if this is a wide string
    bool is_wide() const {
        return std::holds_alternative<U>(data);
    }
};

/// Type aliases for common Units types
using UnitsRef = Units<const uint8_t*, const uint16_t*>;
using UnitsMutRef = Units<uint8_t*, uint16_t*>;
using UnitsVec = Units<std::vector<uint8_t>, std::vector<uint16_t>>;

/// Forward declarations
class WString;
template<typename T> class Iter;
template<typename T> class Chars;
template<typename T> class CharIndices;

/// A UCS2 string slice, analogous to std::string_view
class WStr {
public:
    /// The maximum string length
    static constexpr size_t MAX_LEN = WSTR_MAX_LEN;

    /// Create a WStr from a buffer containing 1 or 2-bytes code units
    static const WStr* from_units(UnitsRef units);
    
    /// Create a mutable WStr from a mutable buffer
    static WStr* from_units_mut(UnitsMutRef units);
    
    /// Create an empty string
    static const WStr* empty();
    
    /// Create an empty mutable string
    static WStr* empty_mut();

    /// Get the underlying buffer
    UnitsRef units() const;
    
    /// Get mutable access to the underlying buffer
    UnitsMutRef units_mut();

    /// Returns true if this is a wide string
    bool is_wide() const;
    
    /// Returns the number of code units
    size_t len() const;
    
    /// Returns true if the string contains no code units
    bool is_empty() const;

    /// Returns the i-th code unit; panics if out of range
    uint16_t at(size_t i) const;
    
    /// Returns the i-th code unit, or nullopt if out of range
    std::optional<uint16_t> get(size_t i) const;
    
    /// Returns the i-th code unit without bounds checking
    /// @warning Unsafe - caller must ensure i < len()
    uint16_t get_unchecked(size_t i) const;

    /// Returns a subslice, or nullopt if indices are out of range
    template<typename Range>
    std::optional<const WStr*> slice(Range range) const;
    
    /// Returns a mutable subslice, or nullopt if indices are out of range
    template<typename Range>
    std::optional<WStr*> slice_mut(Range range);
    
    /// Returns a subslice without bounds checking
    /// @warning Unsafe - caller must ensure valid range
    template<typename Range>
    const WStr* slice_unchecked(Range range) const;
    
    /// Returns a mutable subslice without bounds checking
    /// @warning Unsafe - caller must ensure valid range
    template<typename Range>
    WStr* slice_unchecked_mut(Range range);

    /// Iterate over code units
    Iter<const WStr*> iter() const;
    
    /// Iterate over Unicode characters
    Chars<const WStr*> chars() const;
    
    /// Iterate over Unicode characters with indices
    CharIndices<const WStr*> char_indices() const;

    /// Returns the offset of this string in another, if it's a substring
    std::optional<size_t> offset_in(const WStr* other) const;

    /// Compare two strings for equality, ignoring case (Flash Player style)
    bool eq_ignore_case(const WStr* other) const;
    
    /// Compare two strings with specified case sensitivity
    template<typename T, typename U>
    bool eq_with_case(Units<T, U> other, bool case_sensitive) const;

    /// Compare two strings, ignoring case
    std::strong_ordering cmp_ignore_case(const WStr* other) const;

    /// Parse the string into another type
    template<typename T>
    Result<T, typename T::Err> parse() const;

    /// Returns true if the string contains only LATIN1 characters
    bool is_latin1() const;

    /// Convert to UTF-8 String (unpaired surrogates replaced with replacement char)
    std::string to_utf8_lossy() const;

    /// Return a new string with ASCII characters mapped to lowercase
    WString to_ascii_lowercase() const;
    
    /// Convert ASCII characters to lowercase in-place
    void make_ascii_lowercase();
    
    /// Return a new string with ASCII characters mapped to uppercase
    WString to_ascii_uppercase() const;
    
    /// Convert ASCII characters to uppercase in-place
    void make_ascii_uppercase();

    /// Replace all occurrences of pattern with replacement
    template<typename Pattern>
    WString replace(Pattern pattern, const WStr* with) const;
    
    /// Find first occurrence of pattern
    template<typename Pattern>
    std::optional<size_t> find(Pattern pattern) const;
    
    /// Find last occurrence of pattern
    template<typename Pattern>
    std::optional<size_t> rfind(Pattern pattern) const;
    
    /// Check if string contains pattern
    template<typename Pattern>
    bool contains(Pattern pattern) const;

    /// Split string by separator
    template<typename Pattern>
    Split<const WStr*, Pattern> split(Pattern separator) const;
    
    /// Split string at index
    std::pair<const WStr*, const WStr*> split_at(size_t index) const;
    
    /// Split string once at first occurrence of pattern
    template<typename Pattern>
    std::optional<std::pair<const WStr*, const WStr*>> split_once(Pattern pattern) const;
    
    /// Split string once at last occurrence of pattern
    template<typename Pattern>
    std::optional<std::pair<const WStr*, const WStr*>> rsplit_once(Pattern pattern) const;

    /// Trim characters matching pattern from both ends
    template<typename Pattern>
    const WStr* trim_matches(Pattern pattern) const;
    
    /// Trim characters matching pattern from start
    template<typename Pattern>
    const WStr* trim_start_matches(Pattern pattern) const;
    
    /// Trim characters matching pattern from end
    template<typename Pattern>
    const WStr* trim_end_matches(Pattern pattern) const;
    
    /// Trim whitespace (Flash definition) from both ends
    const WStr* trim() const;
    
    /// Trim whitespace from start
    const WStr* trim_start() const;
    
    /// Trim whitespace from end
    const WStr* trim_end() const;

    /// Check if string starts with pattern
    template<typename Pattern>
    bool starts_with(Pattern pattern) const;
    
    /// Check if string ends with pattern
    template<typename Pattern>
    bool ends_with(Pattern pattern) const;
    
    /// Remove prefix if it matches pattern
    template<typename Pattern>
    std::optional<const WStr*> strip_prefix(Pattern pattern) const;
    
    /// Remove suffix if it matches pattern
    template<typename Pattern>
    std::optional<const WStr*> strip_suffix(Pattern pattern) const;

    /// Repeat the string count times
    WString repeat(size_t count) const;

    // Comparison operators
    bool operator==(const WStr& other) const;
    auto operator<=>(const WStr& other) const;

    // Hash support
    size_t hash() const;

    // String conversion
    std::string to_string() const;
};

/// Default for const WStr pointer
inline const WStr* default_wstr() {
    return WStr::empty();
}

/// Default for mutable WStr pointer
inline WStr* default_wstr_mut() {
    return WStr::empty_mut();
}

/// Trait for types that can be parsed from WStr
template<typename T>
concept FromWStr = requires(const WStr* s) {
    { T::from_wstr(s) } -> std::same_as<Result<T, typename T::Err>>;
};

/// Pattern trait for string searching
template<typename T>
concept Pattern = requires(T t, const WStr* haystack) {
    { t.into_searcher(haystack) };
};

// Index operators
inline const WStr* operator[](const WStr* str, size_t index);
inline WStr* operator[](WStr* str, size_t index);

} // namespace wstr
} // namespace ruffle

#endif // RUFFLE_WSTR_COMMON_H
