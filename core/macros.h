// C++ translation of core/macros/src/lib.rs
// Proc macros for Ruffle - C++ equivalents
//
// Note: Rust proc macros don't have direct C++ equivalents.
// This header provides C++ alternatives for the macros defined in the Rust proc macro crate.

#ifndef RUFFLE_CORE_MACROS_H
#define RUFFLE_CORE_MACROS_H

#include <cstddef>
#include <type_traits>
#include <string>

// Forward declarations
namespace ruffle {
namespace common {
namespace utils {
    template<typename T>
    struct HasPrefixField;
}
}

namespace core {
    template<typename GC>
    class AvmString;
    template<typename GC>
    class AvmAtom;
    template<typename GC>
    class Activation;
    class StringContext;

    namespace avm2 {
        template<typename GC>
        class Activation;
    }
}
}
}

namespace ruffle {
namespace macros {

// ============================================================================
// enum_trait_object equivalent
// ============================================================================
// In Rust, this is a proc macro that generates enum-based trait dispatch.
// In C++, we use std::variant with visitor pattern or direct inheritance.
//
// Usage in Rust:
//   #[enum_trait_object(pub enum MyTraitEnum { Object(Object) })]
//   trait MyTrait {}
//
// C++ equivalent uses std::variant:
//   template<typename GC>
//   using MyTraitEnum = std::variant<Object<GC>>;

// ============================================================================
// HasPrefixField derive macro equivalent
// ============================================================================
// In Rust: #[derive(HasPrefixField)]
// This verifies that a repr(C) struct has a specific field at offset 0.
//
// C++ equivalent using static_assert and offsetof:
#define HAS_PREFIX_FIELD(StructType, FieldType, FieldName) \
    static_assert(offsetof(StructType, FieldName) == 0, \
                  #StructType " must have " #FieldName " at offset 0")

// Helper trait for HasPrefixField
namespace impl_ {
    template<typename T, typename Field>
    struct has_prefix_field_impl {
        static constexpr bool value = offsetof(T, FieldName) == 0;
    };
}

// ============================================================================
// atom! macro equivalent
// ============================================================================
// In Rust: atom!("string") -> AvmAtom
// Expands to: activation.context.strings.common().str_string
//
// C++ equivalent - uses string interning
//
// Note: C++ doesn't have proc macros, so we provide inline functions
// that achieve similar functionality.

/// Get an interned AvmAtom for a string literal.
///
/// In Rust, this is: atom!("description")
/// Which expands to: activation.context.strings.common().str_description
///
/// In C++, we provide a template function that uses the string context.
template<typename GC>
ruffle::core::AvmAtom<GC> atom_literal(
    ruffle::core::Activation<GC>* activation,
    const char* str
) {
    // This would use the string interner to get the atom
    // For now, this is a placeholder
    // In actual implementation, this would look up the string in the interner
    return ruffle::core::AvmAtom<GC>::undefined();
}

/// Get an interned AvmString for a string literal.
///
/// In Rust, this is: istr!("description")
/// Which expands to: AvmString::from(activation.context.strings.common().str_description)
///
/// In C++, we provide a template function.
template<typename GC>
ruffle::core::AvmString<GC> istr_literal(
    ruffle::core::Activation<GC>* activation,
    const char* str
) {
    // This would use the string interner to get the string
    // For now, this is a placeholder
    return ruffle::core::AvmString<GC>::empty(activation->context().strings);
}

/// Get an interned AvmAtom for a string literal with explicit context.
///
/// In Rust: atom!(context, "description")
/// Expands to: HasStringContext::strings_ref(context).common().str_description
template<typename GC, typename ContextType>
ruffle::core::AvmAtom<GC> atom_with_context(
    ContextType* context,
    const char* str
) {
    // Placeholder implementation
    return ruffle::core::AvmAtom<GC>::undefined();
}

/// Get an interned AvmString for a string literal with explicit context.
template<typename GC, typename ContextType>
ruffle::core::AvmString<GC> istr_with_context(
    ContextType* context,
    const char* str
) {
    // Placeholder implementation
    return ruffle::core::AvmString<GC>::empty(context->strings);
}

// ============================================================================
// Convenience macros for common strings
// ============================================================================
// These provide quick access to commonly used interned strings.

#define ATOM_empty (activation.context.strings.common().empty)
#define ATOM___proto__ (activation.context.strings.common().str___proto__)
#define ATOM___constructor__ (activation.context.strings.common().str___constructor__)
#define ATOM_length (activation.context.strings.common().str_length)
#define ATOM_toString (activation.context.strings.common().str_toString)
#define ATOM_valueOf (activation.context.strings.common().str_valueOf)

// ============================================================================
// ASCII character shortcuts
// ============================================================================
// In Rust: atom!("A") expands to activation.context.strings.common().ascii_chars[65]

template<typename GC>
inline ruffle::core::AvmAtom<GC> atom_ascii(
    ruffle::core::Activation<GC>* activation,
    char c
) {
    return activation->context().strings.common().ascii_chars[static_cast<unsigned char>(c)];
}

template<typename GC>
inline ruffle::core::AvmString<GC> istr_ascii(
    ruffle::core::Activation<GC>* activation,
    char c
) {
    return ruffle::core::AvmString<GC>::from(atom_ascii(activation, c));
}

} // namespace macros
} // namespace ruffle

// Convenience macros using the macros namespace
#define atom(str) ruffle::macros::atom_literal(activation, str)
#define istr(str) ruffle::macros::istr_literal(activation, str)

#endif // RUFFLE_CORE_MACROS_H
