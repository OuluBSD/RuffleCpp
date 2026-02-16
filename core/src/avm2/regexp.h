// C++ translation of core/src/avm2/regexp.rs
//! RegExp Structure

#ifndef RUFFLE_CORE_AVM2_REGEXP_H
#define RUFFLE_CORE_AVM2_REGEXP_H

#include <cstdint>
#include <optional>
#include <string>
#include <vector>
#include <memory>

// Forward declarations
namespace ruffle {
namespace core {
    class UpdateContext;
    class AvmString;

    namespace avm2 {
        template<typename T> class Activation;
        template<typename T> class Value;
        template<typename T> class Error;
        template<typename T> class FunctionObject;
        template<typename T> class ArrayObject;
        template<typename T> class RegExpObject;

        namespace function {
            template<typename GC>
            class FunctionArgs;
        }
    }
}
}

// Placeholder for regress regex library
namespace regress {
    struct Match {
        struct Range {
            size_t start;
            size_t end;
        };
        Range range;
        std::vector<std::optional<Range>> captures;

        size_t start() const { return range.start; }
        size_t end() const { return range.end; }
    };

    struct Flags {
        bool icase;
        bool multiline;
        bool dot_all;
        bool extended;
        bool no_opt;
        bool unicode;
        bool unicode_sets;
    };

    class Regex {
    public:
        static std::optional<Regex> with_flags(const std::string& pattern, Flags flags);
        std::vector<Match> find_from(const std::string& text, size_t start) const;
    };
}

namespace ruffle {
namespace core {
namespace avm2 {

/// RegExp flags bitfield
enum class RegExpFlags : uint8_t {
    None       = 0,
    Global     = 1 << 0,
    IgnoreCase = 1 << 1,
    Multiline  = 1 << 2,
    DotAll     = 1 << 3,
    Extended   = 1 << 4,
};

/// Combine RegExpFlags
inline RegExpFlags operator|(RegExpFlags a, RegExpFlags b) {
    return static_cast<RegExpFlags>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

/// Check if RegExpFlags contains a flag
inline bool contains(RegExpFlags flags, RegExpFlags flag) {
    return (static_cast<uint8_t>(flags) & static_cast<uint8_t>(flag)) != 0;
}

/// Cached text for regex operations
/// Stores both the original text and its UTF-8 conversion
template<typename GC>
class CachedText {
public:
    explicit CachedText(AvmString<GC> text);

    /// Get the UTF-8 representation of the text
    const std::string& utf8() const;

    /// Map UTF-16 index to UTF-8 index
    std::optional<size_t> utf8_index(size_t utf16_index);

    /// Map UTF-8 index to UTF-16 index
    std::optional<size_t> utf16_index(size_t utf8_index);

private:
    void reset();
    std::optional<void> advance();

    AvmString<GC> text_;
    std::optional<std::string> utf8_;
    size_t utf8_prefix_len_;
    size_t cur_utf8_index_;
    size_t cur_utf16_index_;
};

/// Regular expression representation
template<typename GC>
class RegExp {
public:
    /// Create a new RegExp with the given source
    explicit RegExp(AvmString<GC> source);

    /// Get the source pattern string
    AvmString<GC> source() const { return source_; }

    /// Set the source pattern string
    void set_source(AvmString<GC> source);

    /// Get the regex flags
    RegExpFlags flags() const { return flags_; }

    /// Set the regex flags
    void set_flags(RegExpFlags flags);

    /// Get the last index (for global matching)
    size_t last_index() const { return last_index_; }

    /// Set the last index
    void set_last_index(size_t index) { last_index_ = index; }

    /// Test if the regex matches the text
    /// Updates last_index for global matches
    bool test(AvmString<GC> text);

    /// Replace matches using a function
    ///
    /// \param regexp The RegExpObject containing this regex
    /// \param activation The current activation
    /// \param text The text to search in
    /// \param f The replacement function
    /// \return The resulting string after replacements
    static AvmString<GC> replace_fn(
        RegExpObject<GC> regexp,
        Activation<GC>* activation,
        AvmString<GC> text,
        FunctionObject<GC> f
    );

    /// Replace matches using a replacement string
    /// Handles $ sequences like $&, $`, $', $n
    static AvmString<GC> replace_string(
        RegExpObject<GC> regexp,
        Activation<GC>* activation,
        AvmString<GC> text,
        AvmString<GC> replacement
    );

    /// Split text by regex matches
    ArrayObject<GC> split(
        UpdateContext* context,
        AvmString<GC> text,
        size_t limit
    );

    /// Execute the regex and return the match
    /// Updates last_index for global matches
    std::optional<regress::Match> exec(AvmString<GC> text);

    /// Find a match starting at the given UTF-16 index
    std::optional<regress::Match> find_utf16_match(AvmString<GC> text, size_t start);

private:
    /// Find a match in UTF-8 text and apply a transformation function
    template<typename F, typename T>
    std::optional<T> find_utf8_match_at(AvmString<GC> text, size_t start, F&& f);

    /// Helper for replace operations
    template<typename F>
    static AvmString<GC> replace_with_fn(
        RegExpObject<GC> regexp,
        Activation<GC>* activation,
        AvmString<GC> text,
        F&& f
    );

    /// Evaluate replacement string with $ sequences
    static std::string effective_replacement(
        AvmString<GC> replacement,
        AvmString<GC> text,
        const regress::Match& match
    );

    AvmString<GC> source_;
    RegExpFlags flags_;
    size_t last_index_;
    std::optional<regress::Regex> cached_regex_;
    std::optional<CachedText<GC>> cached_text_;
};

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_REGEXP_H
