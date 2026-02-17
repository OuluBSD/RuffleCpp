// C++ translation of core/common/src/avm_string/context.rs
// String context for managing AvmStrings

#ifndef RUFFLE_CORE_COMMON_AVM_STRING_CONTEXT_H
#define RUFFLE_CORE_COMMON_AVM_STRING_CONTEXT_H

#include <cstdint>
#include <memory>
#include <optional>
#include <string_view>

// Forward declarations
namespace ruffle {
namespace common {
namespace wstr {
    class WStr;
    class WString;
}
}
}

namespace ruffle {
namespace core {
namespace avm_string {

// Forward declarations
template<typename GC>
class AvmString;

template<typename GC>
class AvmStringInterner;

template<typename GC>
class AvmAtom;

class CommonStrings;

template<typename GC>
class AvmStringRepr;

/// Range type for substring operations
struct Range {
    size_t start;
    size_t end;
};

/// Context for managing AvmStrings: allocating them, interning them, etc...
template<typename GC>
class StringContext {
private:
    // The mutation context to allocate and mutate GC pointers
    GC* gc_context;

    // The global string interner
    AvmStringInterner<GC>* interner;

public:
    /// Create StringContext from parts
    /// @param gc_context The mutation context
    /// @param interner The string interner
    StringContext(GC* gc_context, AvmStringInterner<GC>* interner)
        : gc_context(gc_context), interner(interner) {}

    /// Get the GC mutation context
    GC* gc() const {
        return gc_context;
    }

    /// Get common strings
    const CommonStrings* common() const {
        return &interner->common;
    }

    /// Intern a WStr into an atom
    /// @param s The string to intern (as WStr or WString)
    /// @return The interned atom
    template<typename S>
    AvmAtom<GC> intern_wstr(const S& s);

    /// Intern a static WStr
    /// @param s The static string to intern
    /// @return The interned atom
    AvmAtom<GC> intern_static(const ruffle::common::wstr::WStr* s);

    /// Intern an AvmString
    /// @param s The string to intern
    /// @return The interned atom
    AvmAtom<GC> intern(AvmString<GC> s);

    /// Get an interned string if it exists
    /// @param s The string to look up
    /// @return The interned atom if found
    std::optional<AvmAtom<GC>> get_interned(const ruffle::common::wstr::WStr* s);

    /// Get the empty string
    /// @return The empty AvmString
    AvmString<GC> empty() const;

    /// Create a single character string
    /// @param c The character code (UTF-16 unit)
    /// @return The character as AvmString
    AvmString<GC> make_char(uint16_t c) const;

    /// Create an ASCII character string (panics if not ASCII)
    /// @param c The ASCII character
    /// @return The character as AvmString
    AvmString<GC> ascii_char(uint8_t c) const;

    /// Create a substring
    /// @param s The source string
    /// @param range The range to extract
    /// @return The substring
    AvmString<GC> substring(AvmString<GC> s, Range range) const;
};

/// Trait for types that have a string context
template<typename GC>
class HasStringContext {
public:
    virtual const StringContext<GC>* strings_ref() const = 0;
};

/// StringContext implements HasStringContext for itself
template<typename GC>
class StringContextHasStringContext : public HasStringContext<GC> {
private:
    const StringContext<GC>* context;

public:
    StringContextHasStringContext(const StringContext<GC>* context) : context(context) {}

    const StringContext<GC>* strings_ref() const override {
        return context;
    }
};

} // namespace avm_string
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_COMMON_AVM_STRING_CONTEXT_H
