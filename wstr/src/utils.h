// C++ translation of wstr/src/utils.rs
// Utilities for operating on strings in SWF files

#ifndef RUFFLE_WSTR_UTILS_H
#define RUFFLE_WSTR_UTILS_H

#include <cstdint>
#include <cstddef>
#include <vector>
#include <string>
#include <string_view>
#include <algorithm>
#include <iterator>

// Forward declaration for WStr
namespace ruffle {
namespace wstr {
    class WStr;
    enum class Units;
}
}

// Include tables for case conversion
#include "tables.h"

namespace ruffle {
namespace wstr {
namespace utils {

/// Check if there's a surrogate pair at the given position.
///
/// @param us The UTF-16 code unit array
/// @param pos The position to check
/// @return true if a surrogate pair starts at pos-2, false otherwise
inline bool is_surrogate_pair_at(const uint16_t* us, size_t len, size_t pos) {
    if (pos < 2 || pos > len) {
        return false;
    }
    uint16_t high = us[pos - 2];
    uint16_t low = us[pos - 1];
    bool has_high = (high >= 0xD800 && high <= 0xDBFF);
    bool has_low = (low >= 0xDC00 && low <= 0xDFFF);
    return has_high && has_low;
}

/// Gets the position of the previous UTF-16 character.
///
/// `pos` must already lie on a character boundary.
///
/// @param slice The WStr slice
/// @param pos The current position
/// @return The position of the previous character
inline size_t prev_char_boundary(const WStr& slice, size_t pos) {
    if (pos <= 1) {
        return 0;
    }

    // LATIN-1 strings only contain 1-byte characters
    if (slice.units() == Units::Bytes) {
        return pos - 1;
    }

    // Wide strings may have surrogate pairs
    if (slice.units() == Units::Wide) {
        const uint16_t* us = slice.wide_units();
        if (is_surrogate_pair_at(us, slice.len(), pos)) {
            return pos - 2;
        }
    }

    return pos - 1;
}

/// Gets the position of the next UTF-16 character.
///
/// `pos` must already lie on a character boundary.
///
/// @param slice The WStr slice
/// @param pos The current position
/// @return The position of the next character
inline size_t next_char_boundary(const WStr& slice, size_t pos) {
    if (pos >= slice.len()) {
        return slice.len();
    }

    // LATIN-1 strings only contain 1-byte characters
    if (slice.units() == Units::Bytes) {
        return pos + 1;
    }

    // Wide strings may have surrogate pairs
    if (slice.units() == Units::Wide) {
        const uint16_t* us = slice.wide_units();
        if (is_surrogate_pair_at(us, slice.len(), pos + 2)) {
            return pos + 2;
        }
    }

    return pos + 1;
}

/// Returns true if the given UTF-16 code unit is whitespace according to Flash Player.
///
/// @param c The UTF-16 code unit
/// @return true if whitespace, false otherwise
inline bool swf_is_whitespace(uint16_t c) {
    if (c > 0xFF) {
        return false;
    }
    uint8_t b = static_cast<uint8_t>(c);
    return (b == ' ' || b == '\t' || b == '\n' || b == '\r');
}

/// Returns true if the given UTF-16 code unit is a newline according to Flash Player.
///
/// @param c The UTF-16 code unit
/// @return true if newline, false otherwise
inline bool swf_is_newline(uint16_t c) {
    if (c > 0xFF) {
        return false;
    }
    uint8_t b = static_cast<uint8_t>(c);
    return (b == '\n' || b == '\r');
}

/// Finds the longest prefix of the slice that is entirely ASCII,
/// and returns it as a UTF-8 string, together with the remaining tail.
///
/// @param slice The byte slice
/// @return Pair of (ASCII prefix as string, remaining tail)
inline std::pair<std::string_view, std::span<const uint8_t>> split_ascii_prefix_bytes(
    const uint8_t* slice,
    size_t len
) {
    size_t first_non_ascii = len;
    for (size_t i = 0; i < len; i++) {
        if (slice[i] >= 0x80) {
            first_non_ascii = i;
            break;
        }
    }

    std::string_view head(reinterpret_cast<const char*>(slice), first_non_ascii);
    std::span<const uint8_t> tail(slice + first_non_ascii, len - first_non_ascii);
    return {head, tail};
}

/// Finds the longest prefix of the string that is entirely ASCII,
/// and returns it as a byte slice, together with the remaining tail.
///
/// @param str The UTF-8 string
/// @return Pair of (ASCII prefix as bytes, remaining tail)
inline std::pair<std::span<const uint8_t>, std::string_view> split_ascii_prefix(
    std::string_view str
) {
    auto [head, tail] = split_ascii_prefix_bytes(
        reinterpret_cast<const uint8_t*>(str.data()),
        str.size()
    );
    return {
        std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(head.data()), head.size()),
        tail
    };
}

/// Maps a UTF-16 code unit into a char.
///
/// Surrogate characters are replaced with the Unicode replacement character.
///
/// @param c The UTF-16 code unit
/// @return The corresponding char
inline char32_t utf16_code_unit_to_char(uint16_t c) {
    if (c >= 0xD800 && c <= 0xDFFF) {
        return U'\xFFFD';  // Replacement character
    }
    return static_cast<char32_t>(c);
}

/// Maps a UCS-2 code unit to its lowercase variant according to Flash Player.
///
/// Note that this mapping is different from standard Unicode lowercase.
///
/// @param c The UCS-2 code unit
/// @return The lowercase variant
inline uint16_t swf_to_lowercase(uint16_t c) {
    if (c < 0x80) {
        return static_cast<uint16_t>(static_cast<uint8_t>(c) | 0x20);
    }

    // Binary search in LOWERCASE_TABLE
    auto it = std::lower_bound(
        std::begin(LOWERCASE_TABLE),
        std::end(LOWERCASE_TABLE),
        c,
        [](const auto& entry, uint16_t value) {
            return entry.first < value;
        }
    );

    if (it != std::end(LOWERCASE_TABLE) && it->first == c) {
        return it->second;
    }

    return c;
}

/// Maps a UCS-2 code unit to its uppercase variant according to Flash Player.
///
/// Note that this mapping is different from standard Unicode uppercase.
///
/// @param c The UCS-2 code unit
/// @return The uppercase variant
inline uint16_t swf_to_uppercase(uint16_t c) {
    if (c < 0x80) {
        return static_cast<uint16_t>(static_cast<uint8_t>(c) & ~0x20);
    }

    // Binary search in UPPERCASE_TABLE
    auto it = std::lower_bound(
        std::begin(UPPERCASE_TABLE),
        std::end(UPPERCASE_TABLE),
        c,
        [](const auto& entry, uint16_t value) {
            return entry.first < value;
        }
    );

    if (it != std::end(UPPERCASE_TABLE) && it->first == c) {
        return it->second;
    }

    return c;
}

/// Check if the given UTF-16 code unit is an ASCII hex digit.
///
/// @param c The UTF-16 code unit
/// @return true if ASCII hex digit, false otherwise
inline bool swf_is_ascii_hexdigit(uint16_t c) {
    if (c > 0x7F) {
        return false;
    }
    uint8_t b = static_cast<uint8_t>(c);
    return ((b >= '0' && b <= '9') ||
            (b >= 'a' && b <= 'f') ||
            (b >= 'A' && b <= 'F'));
}

/// Check if the character is CJK-like.
///
/// @param c The character
/// @return true if CJK-like, false otherwise
inline bool swf_is_cjk_like(char32_t c) {
    // Not guaranteed to be equivalent to FP's set
    return c >= U'\u2300';
}

/// Check if the character is an opening bracket/parenthesis.
///
/// @param c The character
/// @return true if opening bracket, false otherwise
inline bool swf_is_opening(char32_t c) {
    // Not guaranteed to be equivalent to FP's set
    switch (c) {
        case U'(': case U'[': case U'{':
        case U'\uFF08': case U'\uFF3B': case U'\uFF5B':  // （ ［ ｛
        case U'\u3008': case U'\u300A': case U'\u300C':  // 〈 《 「
        case U'\u2045': case U'\u300E':                  // ⁅ 『
        case U'\u3010': case U'\u3016': case U'\u301A':  // 【 〖 〚
        case U'\uFF08':                                  // ﴾
        case U'\uFE64':                                  // ﹙
        case U'\u201D':                                  // 〝
        case U'\uFE3B': case U'\uFE41': case U'\uFE43':  // ﹻ ﹁ ﹃
        case U'\uFE35':                                  // ﹵
            return true;
        default:
            return false;
    }
}

/// Check if the character is a closing bracket/parenthesis.
///
/// @param c The character
/// @return true if closing bracket, false otherwise
inline bool swf_is_closing(char32_t c) {
    // Not guaranteed to be equivalent to FP's set
    switch (c) {
        case U')': case U']': case U'}':
        case U'\uFF09': case U'\uFF3D': case U'\uFF5D':  // ） ］ ｝
        case U'\u3009': case U'\u300B':                  // 〉 》
        case U'?': case U'!': case U';': case U':':      // ? ! ; :
        case U',': case U'.': case U'\u300D':            // , . 」
        case U'\u2046': case U'\u300F':                  // ⁆ 』
        case U'\u3011': case U'\u3017': case U'\u3019':  // 】 〕 〙
        case U'\uFE5E':                                  // ﹞
        case U'\uFF09':                                  // ﴿
        case U'\uFE5A':                                  // ﹚
        case U'\uFF1F': case U'\uFF01': case U'\uFF1B':  // ？ ！ ；
        case U'\uFF1A': case U'\uFF0C': case U'\uFF0E':  // ： ， ．
        case U'\u3001': case U'\u3002':                  // 、 。
        case U'\u301C':                                  // 〜
        case U'\uFE3C': case U'\uFE3E': case U'\uFE42':  // ﹼ ﹾ ﹂
        case U'\uFE44': case U'\uFE36':                  // ﹄ ﹶ
        case U'\u301F':                                  // 〟
            return true;
        default:
            return false;
    }
}

/// Iterator for decoding AVM UTF-8.
///
/// This is similar to std::u32string_view::iterator, except it uses Flash's
/// non-standard UTF-8 decoding rules. Invalid UTF-8 sequences are interpreted
/// as LATIN-1 characters, and surrogate ranges are not validated.
class DecodeAvmUtf8 {
private:
    const uint8_t* src_;
    size_t len_;
    size_t index_;

public:
    /// Create a new decoder.
    /// @param src The source byte array
    /// @param len The length of the source
    DecodeAvmUtf8(const uint8_t* src, size_t len)
        : src_(src), len_(len), index_(0) {}

    /// Check if there are more characters to decode.
    /// @return true if more characters available
    bool has_next() const {
        return index_ < len_;
    }

    /// Decode the next character.
    ///
    /// @return The decoded character as u32, or 0 if no more characters
    uint32_t next() {
        if (index_ >= len_) {
            return 0;
        }

        uint8_t first = src_[index_];
        index_++;

        uint8_t ones = 0;
        uint8_t temp = first;
        while ((temp & 0x80) != 0) {
            ones++;
            temp <<= 1;
        }

        if (ones <= 1) {
            return static_cast<uint32_t>(first);
        }

        uint8_t mb_count = std::min(ones - 1, static_cast<uint8_t>(3));
        uint8_t bm = 0xFF >> ones;
        uint32_t ch = bm & first;

        if (index_ + mb_count <= len_) {
            bool valid = true;
            for (uint8_t i = 0; i < mb_count; i++) {
                uint8_t b = src_[index_ + i];
                // Continuation bytes should start with a single leading 1
                if ((b & 0xC0) != 0x80) {
                    valid = false;
                    break;
                }
                ch <<= 6;
                ch |= (b & 0x3F);
            }

            if (valid && ch >= 0x80) {
                index_ += mb_count;
                return ch;
            }
        }

        return static_cast<uint32_t>(first);
    }
};

/// Encode a raw character point into UTF-16.
///
/// Unlike standard UTF-16 encoding, this does not require that the character
/// point is valid. Invalid code points are still encoded.
///
/// @param ch The character to encode
/// @param dst The destination vector
inline void encode_raw_utf16(uint32_t ch, std::vector<uint16_t>& dst) {
    if (ch < 0x10000) {
        dst.push_back(static_cast<uint16_t>(ch));
        return;
    }

    ch -= 0x10000;
    uint16_t w1 = 0xD800;
    uint16_t w2 = 0xDC00;
    w1 |= static_cast<uint16_t>(ch >> 10);
    w2 |= static_cast<uint16_t>(ch & 0x3FF);
    dst.push_back(w1);
    dst.push_back(w2);
}

} // namespace utils
} // namespace wstr
} // namespace ruffle

#endif // RUFFLE_WSTR_UTILS_H
