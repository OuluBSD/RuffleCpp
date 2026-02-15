/*
 * C++ header for AVM string context functionality
 * This replaces the functionality of core/common/src/avm_string/context.rs
 */

#ifndef STRING_CONTEXT_H
#define STRING_CONTEXT_H

#include "avm_string.h"
#include "common_strings.h"
#include <memory>
#include <unordered_map>
#include <functional>

// Forward declarations
template<typename GCContext>
class AvmStringInterner;

template<typename GCContext>
class StringContext {
private:
    GCContext* gc_context;
    AvmStringInterner<GCContext>* interner;

public:
    // Constructor
    StringContext(GCContext& gc_ctx, AvmStringInterner<GCContext>& interner_ref)
        : gc_context(&gc_ctx), interner(&interner_ref) {}

    // Factory method to create from parts
    static StringContext from_parts(GCContext& gc_context, AvmStringInterner<GCContext>& interner);

    // Accessor for GC context
    GCContext& gc() const { return *gc_context; }

    // Accessor for common strings
    const CommonStrings<GCContext>& common() const { return interner->get_common(); }

    // Intern a wide string
    AvmAtom<GCContext> intern_wstr(const WString& str);

    // Intern a static wide string
    AvmAtom<GCContext> intern_static(const char16_t* str);

    // Intern an AvmString
    AvmAtom<GCContext> intern(const AvmString<GCContext>& str);

    // Get an interned string if it exists
    std::optional<AvmAtom<GCContext>> get_interned(const WString& str) const;

    // Get empty string
    AvmString<GCContext> empty() const;

    // Create a character string
    AvmString<GCContext> make_char(char16_t c) const;

    // Get ASCII character string (panics if not ASCII)
    AvmString<GCContext> ascii_char(uint8_t c) const;

    // Create substring
    AvmString<GCContext> substring(const AvmString<GCContext>& str, size_t start, size_t end) const;
};

// Trait-like interface for objects that have access to a string context
template<typename GCContext>
class HasStringContext {
public:
    virtual ~HasStringContext() = default;
    virtual const StringContext<GCContext>& strings_ref() const = 0;
};

// Implementation of the interface for StringContext itself
template<typename GCContext>
class StringContextHolder : public HasStringContext<GCContext> {
private:
    StringContext<GCContext> string_context;

public:
    explicit StringContextHolder(const StringContext<GCContext>& ctx) : string_context(ctx) {}

    const StringContext<GCContext>& strings_ref() const override {
        return string_context;
    }
};

// Implementation would go in a separate .cpp file
template<typename GCContext>
StringContext<GCContext> StringContext<GCContext>::from_parts(
    GCContext& gc_context, AvmStringInterner<GCContext>& interner) {
    return StringContext<GCContext>(gc_context, interner);
}

template<typename GCContext>
AvmAtom<GCContext> StringContext<GCContext>::intern_wstr(const WString& str) {
    return interner->intern(gc(), str);
}

template<typename GCContext>
AvmAtom<GCContext> StringContext<GCContext>::intern_static(const char16_t* str) {
    return interner->intern_static(gc(), str);
}

template<typename GCContext>
AvmAtom<GCContext> StringContext<GCContext>::intern(const AvmString<GCContext>& str) {
    // Check if already interned
    if (str.is_interned()) {
        return str.as_interned();
    } else {
        // Intern the string
        return interner->intern(gc(), str.as_wstr());
    }
}

template<typename GCContext>
std::optional<AvmAtom<GCContext>> StringContext<GCContext>::get_interned(const WString& str) const {
    return interner->get(gc(), str);
}

template<typename GCContext>
AvmString<GCContext> StringContext<GCContext>::empty() const {
    return static_cast<AvmString<GCContext>>(common().str_);
}

template<typename GCContext>
AvmString<GCContext> StringContext<GCContext>::make_char(char16_t c) const {
    if (c < ASCII_CHARS_LEN) {
        return static_cast<AvmString<GCContext>>(common().ascii_chars[c]);
    } else {
        return AvmString<GCContext>::new_string(gc(), WString(&c, 1));
    }
}

template<typename GCContext>
AvmString<GCContext> StringContext<GCContext>::ascii_char(uint8_t c) const {
    return static_cast<AvmString<GCContext>>(common().ascii_chars[c]);
}

template<typename GCContext>
AvmString<GCContext> StringContext<GCContext>::substring(
    const AvmString<GCContext>& str, size_t start, size_t end) const {
    return interner->substring(gc(), str, start, end);
}

#endif // STRING_CONTEXT_H