// C++ translation of wstr/src/buf.rs
// WString - owned, extensible UCS2 string

#ifndef RUFFLE_WSTR_BUF_H
#define RUFFLE_WSTR_BUF_H

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <variant>

// Forward declarations
namespace ruffle {
namespace wstr {
    class WStr;
    struct WStrMetadata;
    template<typename T, typename U>
    struct Units;
}
}

namespace ruffle {
namespace wstr {

/// Maximum length for WStr (must match Rust constant)
inline constexpr std::size_t WSTR_MAX_LEN = 0x7FFFFFFF;

/// An owned, extensible UCS2 string, analogous to std::string
class WString {
    void* data_;
    WStrMetadata meta_;
    std::uint32_t capacity_;

    static_assert(sizeof(WString) == 16, "WString should be 16 bytes on 64-bit");

public:
    /// Creates a new empty WString
    WString();

    /// Creates a new empty WString with the given capacity
    WString(std::size_t capacity, bool wide);

    /// Creates a WString from an owned buffer
    static WString from_buf(Units<std::vector<std::uint8_t>, std::vector<std::uint16_t>> buf);

    /// Creates a WString from an existing WStr
    static WString from_wstr(const WStr* s);

    /// Creates a WString from a UTF-8 string
    static WString from_utf8(const std::string& s);

    /// Creates a WString from a UTF-8 string, reusing allocation if possible
    static WString from_utf8_owned(std::string s);

    /// Creates a WString from UTF-8 bytes
    static WString from_utf8_bytes(const std::vector<std::uint8_t>& b);

    /// Creates a WString from a single UCS2 code unit
    static WString from_unit(std::uint16_t c);

    /// Creates a WString from a single Unicode character
    static WString from_char(char32_t c);

    /// Converts this WString into a WStr pointer
    [[nodiscard]] const WStr* as_wstr() const;

    /// Converts this WString into a mutable WStr pointer
    [[nodiscard]] WStr* as_wstr_mut();

    /// Converts this WString into its internal buffer
    [[nodiscard]] Units<std::vector<std::uint8_t>, std::vector<std::uint16_t>> into_buf();

    /// Decomposes the WString into raw components
    struct RawParts {
        void* ptr;
        WStrMetadata meta;
        std::uint32_t capacity;
    };
    [[nodiscard]] RawParts into_raw_parts();

    /// Rebuilds a WString from raw components
    static WString from_raw_parts(void* ptr, WStrMetadata meta, std::uint32_t capacity);

    /// Truncates this WString, removing all contents
    void clear();

    /// Appends a UTF-16 code unit
    void push(std::uint16_t ch);

    /// Appends a LATIN1 code unit
    void push_byte(std::uint8_t ch);

    /// Appends a Unicode character
    void push_char(char32_t ch);

    /// Appends a UTF-8 string
    void push_utf8(const std::string& s);

    /// Appends UTF-8 bytes
    void push_utf8_bytes(const std::vector<std::uint8_t>& utf8);

    /// Appends another WStr
    void push_str(const WStr* s);

    /// Get the length
    [[nodiscard]] std::size_t len() const;

    /// Check if empty
    [[nodiscard]] bool is_empty() const;

    /// Get capacity
    [[nodiscard]] std::size_t capacity() const;

    /// Reserve capacity
    void reserve(std::size_t additional);

    /// Shrink to fit
    void shrink_to_fit();

    /// Check if wide
    [[nodiscard]] bool is_wide() const;

    /// Get units
    [[nodiscard]] Units<const std::vector<std::uint8_t>*, const std::vector<std::uint16_t>*> units() const;

    /// Get mutable units
    [[nodiscard]] Units<std::vector<std::uint8_t>*, std::vector<std::uint16_t>*> units_mut();

    // Copy constructor
    WString(const WString& other);

    // Copy assignment
    WString& operator=(const WString& other);

    // Move constructor
    WString(WString&& other) noexcept;

    // Move assignment
    WString& operator=(WString&& other) noexcept;

    // Destructor
    ~WString();

    // Default constructor
    WString() = default;
};

// Default implementation
inline WString::WString() : data_(nullptr), capacity_(0) {}

// Copy constructor
inline WString::WString(const WString& other) {
    // Clone from other
    *this = other;
}

// Move constructor
inline WString::WString(WString&& other) noexcept
    : data_(other.data_), meta_(other.meta_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.capacity_ = 0;
}

// Destructor
inline WString::~WString() {
    // Free resources if needed
}

/// Join elements with a separator
WString str_join(const std::vector<const WStr*>& elems, const WStr* sep);

/// Repeat a string n times
WString str_repeat(const WStr* s, std::size_t count);

} // namespace wstr
} // namespace ruffle

#endif // RUFFLE_WSTR_BUF_H
