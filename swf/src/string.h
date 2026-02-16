// C++ translation of swf/src/string.rs
// SWF string type

#ifndef RUFFLE_SWF_STRING_H
#define RUFFLE_SWF_STRING_H

#include <string>
#include <string_view>
#include <cstdint>
#include <optional>
#include <vector>

namespace ruffle {
namespace swf {

/// Encoding constants for SWF strings
struct SwfEncoding {
    static constexpr const char* UTF_8 = "UTF-8";
    static constexpr const char* WINDOWS_1252 = "WINDOWS-1252";
    static constexpr const char* SHIFT_JIS = "SHIFT_JIS";
};

/// A bstr-like string type analogous to `std::string_view` that's returned by SWF parsing functions:
///
/// * The encoding depends on the SWF version (UTF-8 for SWF6 and higher).
///   Use `encoding_for_version` to get the proper encoding.
/// * Invalid data for any particular encoding is allowed;
///   any conversions to std::string will be lossy for invalid data.
///
/// To convert this to a standard Rust string, use `to_str_lossy`.
class SwfStr {
private:
    std::string_view string;

public:
    /// Creates a new `SwfStr` from a byte slice.
    /// The data is not required to be valid for the given encoding.
    static SwfStr from_bytes(std::string_view bytes) {
        return SwfStr(bytes);
    }

    /// Creates a `SwfStr` from a byte slice by reading until a NULL byte (`0`) is encountered.
    /// Returns `std::nullopt` if no NULL byte was found.
    static std::optional<SwfStr> from_bytes_null_terminated(std::string_view bytes) {
        auto pos = bytes.find('\0');
        if (pos == std::string_view::npos) {
            return std::nullopt;
        }
        return SwfStr(bytes.substr(0, pos));
    }

    /// Creates a new UTF-8 `SwfStr` from a std::string.
    static SwfStr from_utf8_str(const std::string& str) {
        return SwfStr(str);
    }

    /// Creates a new UTF-8 `SwfStr` from a std::string_view.
    static SwfStr from_utf8_str_view(std::string_view str) {
        return SwfStr(str);
    }

    /// Creates a new UTF-8 `SwfStr` from a C-string by reading until NULL.
    static std::optional<SwfStr> from_utf8_str_null_terminated(std::string_view str) {
        return from_bytes_null_terminated(str);
    }

    /// Returns the suggested string encoding for the given SWF version.
    ///
    /// For SWF version 6 and higher, this is always UTF-8.
    /// For SWF version 5 and lower, this is locale-dependent,
    /// and we default to WINDOWS-1252.
    static const char* encoding_for_version(uint8_t swf_version) {
        if (swf_version >= 6) {
            return SwfEncoding::UTF_8;
        } else {
            return SwfEncoding::WINDOWS_1252;
        }
    }

    /// Returns the byte slice of this string.
    constexpr std::string_view as_bytes() const {
        return string;
    }

    /// Returns `true` if the string has a length of zero, and `false` otherwise.
    constexpr bool is_empty() const {
        return string.empty();
    }

    /// Returns the length of the string in bytes.
    constexpr size_t len() const {
        return string.size();
    }

    /// Decodes the string into a Rust UTF-8 `std::string`.
    ///
    /// The UTF-8 replacement character will be used for any invalid data.
    /// 
    /// Note: This is a simplified version. A full implementation would need
    /// proper encoding conversion using a library like iconv or encoding_rs.
    std::string to_str_lossy(const char* encoding) const {
        // For UTF-8, we can just return the bytes as-is (with potential lossiness)
        // For other encodings, a proper conversion would be needed
        if (encoding == SwfEncoding::UTF_8 || encoding == std::string("UTF-8")) {
            return std::string(string);
        }
        // For other encodings, we'd need a proper conversion library
        // This is a placeholder that just returns the raw bytes
        return std::string(string);
    }

    /// Decodes the string into a Rust UTF-8 `std::string`.
    std::string to_string_lossy(const char* encoding) const {
        return to_str_lossy(encoding);
    }

    /// Default constructor - empty string
    constexpr SwfStr() : string("") {}

private:
    explicit constexpr SwfStr(std::string_view s) : string(s) {}
};

// Default for reference to SwfStr
inline SwfStr swf_str_default() {
    return SwfStr::from_bytes("");
}

} // namespace swf
} // namespace ruffle

#endif // RUFFLE_SWF_STRING_H
