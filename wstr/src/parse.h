// C++ translation of wstr/src/parse.rs
// Parsing utilities for WStr

#ifndef RUFFLE_WSTR_PARSE_H
#define RUFFLE_WSTR_PARSE_H

#include "common.h"
#include <cstdint>
#include <optional>
#include <string>
#include <fmt/format.h>

namespace ruffle {
namespace wstr {

/// Error returned by integer parsing functions
struct ParseNumError {
    // Empty struct - error type only
};

/// Analog of core::str::FromStr, but for Ruffle's WStr
template<typename T>
concept FromWStr = requires(const WStr* s) {
    { T::from_wstr(s) } -> std::same_as<std::optional<T>>;
};

/// Trait for integer types that can be parsed from a WStr
template<typename T>
concept Integer = FromWStr<T> && requires(const WStr* s, uint32_t radix) {
    { T::from_wstr_radix(s, radix) } -> std::same_as<std::optional<T>>;
};

namespace detail {

/// Parse special float values (NaN, Infinity)
inline std::optional<double> parse_special_floats(const WStr* s) {
    auto nan = WStr::from_units(reinterpret_cast<const uint8_t*>("NaN"), 3);
    auto inf = WStr::from_units(reinterpret_cast<const uint8_t*>("inf"), 3);
    
    if (s->len() == 3) {
        if (s->equals(nan)) return std::numeric_limits<double>::quiet_NaN();
        if (s->equals(inf)) return std::numeric_limits<double>::infinity();
        return std::nullopt;
    }
    
    if (s->len() == 4) {
        auto slice = s->slice(1, 4);
        bool is_nan = slice->equals(nan);
        bool is_inf = slice->equals(inf);
        
        if (!is_nan && !is_inf) return std::nullopt;
        
        auto first = s->at(0);
        if (first == static_cast<uint16_t>('+')) {
            return is_nan ? std::numeric_limits<double>::quiet_NaN() 
                          : std::numeric_limits<double>::infinity();
        }
        if (first == static_cast<uint16_t>('-')) {
            return is_nan ? std::numeric_limits<double>::quiet_NaN() 
                          : -std::numeric_limits<double>::infinity();
        }
        return std::nullopt;
    }
    
    return std::nullopt;
}

/// Integer parsing helper
template<typename T>
struct IntParse {
    static constexpr bool SIGNED = std::is_signed_v<T>;
    
    static T from_digit(uint32_t n) {
        return static_cast<T>(n);
    }
    
    static std::optional<T> checked_add(T self, uint32_t n) {
        T result = self + static_cast<T>(n);
        if constexpr (SIGNED) {
            if ((n > 0 && result < self) || (n < 0 && result > self)) return std::nullopt;
        } else {
            if (result < self) return std::nullopt;
        }
        return result;
    }
    
    static std::optional<T> checked_sub(T self, uint32_t n) {
        T result = self - static_cast<T>(n);
        if constexpr (SIGNED) {
            if ((n > 0 && result > self) || (n < 0 && result < self)) return std::nullopt;
        } else {
            if (result > self) return std::nullopt;
        }
        return result;
    }
    
    static std::optional<T> checked_mul(T self, uint32_t n) {
        if (self == 0 || n == 0) return T(0);
        T result = self * static_cast<T>(n);
        if (result / static_cast<T>(n) != self) return std::nullopt;
        return result;
    }
};

/// Parse integer from WStr with given radix
template<typename T>
inline std::optional<T> from_wstr_radix(const WStr* s, uint32_t radix) {
    if (radix < 2 || radix > 36) {
        // In debug mode, panic; in release, return nullopt
        return std::nullopt;
    }
    
    if (s->len() == 0) return std::nullopt;
    
    bool is_neg = false;
    size_t start = 0;
    
    auto first = s->at(0);
    if (first == static_cast<uint16_t>('-')) {
        is_neg = true;
        start = 1;
    } else if (first == static_cast<uint16_t>('+')) {
        start = 1;
    }
    
    // Empty after sign
    if (start >= s->len()) return std::nullopt;
    
    // Unsigned types can't parse negative numbers
    if (is_neg && !IntParse<T>::SIGNED) return std::nullopt;
    
    T num = IntParse<T>::from_digit(0);
    
    for (size_t i = start; i < s->len(); i++) {
        auto c = s->at(i);
        if (c > 0xFF) return std::nullopt; // Non-byte character
        
        uint32_t digit;
        char byte = static_cast<char>(c);
        if (byte >= '0' && byte <= '9') {
            digit = byte - '0';
        } else if (byte >= 'a' && byte <= 'z') {
            digit = byte - 'a' + 10;
        } else if (byte >= 'A' && byte <= 'Z') {
            digit = byte - 'A' + 10;
        } else {
            return std::nullopt;
        }
        
        if (digit >= radix) return std::nullopt;
        
        auto mult = IntParse<T>::checked_mul(num, radix);
        if (!mult) return std::nullopt;
        
        if (is_neg) {
            num = IntParse<T>::checked_sub(*mult, digit).value_or(T(0));
        } else {
            num = IntParse<T>::checked_add(*mult, digit).value_or(T(0));
        }
    }
    
    return num;
}

} // namespace detail

/// Parse f64 from WStr
inline std::optional<double> parse_f64(const WStr* s) {
    if (auto f = detail::parse_special_floats(s)) {
        return f;
    }
    
    // Early-reject strings with non-float chars
    bool is_valid = true;
    for (size_t i = 0; i < s->len(); i++) {
        auto c = s->at(i);
        if (c > 0xFF) {
            is_valid = false;
            break;
        }
        char byte = static_cast<char>(c);
        if (!((byte >= '0' && byte <= '9') || 
              byte == '.' || byte == '+' || byte == '-' || 
              byte == 'e' || byte == 'E')) {
            is_valid = false;
            break;
        }
    }
    
    if (is_valid) {
        // Convert to UTF-8 string and parse
        std::string utf8 = s->to_utf8_lossy();
        try {
            size_t pos;
            double result = std::stod(utf8, &pos);
            if (pos == utf8.size()) {
                return result;
            }
        } catch (...) {
            // Parse failed
        }
    }
    
    return std::nullopt;
}

/// Parse integer types from WStr
template<typename T>
inline std::optional<T> parse_integer(const WStr* s) {
    return detail::from_wstr_radix<T>(s, 10);
}

/// Parse integer types from WStr with radix
template<typename T>
inline std::optional<T> parse_integer_radix(const WStr* s, uint32_t radix) {
    return detail::from_wstr_radix<T>(s, radix);
}

} // namespace wstr
} // namespace ruffle

#endif // RUFFLE_WSTR_PARSE_H
