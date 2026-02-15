/*
 * C++ header for macro utilities functionality
 * This replaces the functionality of core/macros/src/lib.rs
 */

#ifndef MACRO_UTILS_H
#define MACRO_UTILS_H

#include <type_traits>
#include <utility>

// Forward declarations for types that might be used with these macros
namespace ruffle {
    namespace common {
        namespace utils {
            template<typename T>
            class HasPrefixField {};
        }
    }
    
    template<typename T>
    class AvmAtom;
    
    template<typename T>
    class AvmString;
}

// Define an enum whose variants each implement a trait.
// This macro creates an enum that can be used as a dynamic dispatch alternative
// to trait objects, which is useful for GC'd types.
#define ENUM_TRAIT_OBJECT(EnumName, TraitName, ...) \
    enum class EnumName ## Tag { \
        __VA_ARGS__ \
    }; \
    \
    class EnumName { \
    private: \
        EnumName ## Tag tag_; \
        union { \
            __VA_ARGS__ \
        } data_; \
    public: \
        template<typename T> \
        static EnumName make(T&& value) { \
            EnumName obj; \
            obj.tag_ = /* appropriate tag based on T */; \
            new(&obj.data_) decltype(/* appropriate field */)(std::forward<T>(value)); \
            return obj; \
        } \
        \
        ~EnumName() { \
            /* appropriate cleanup based on tag_ */ \
        } \
        \
        /* Implement methods that delegate to underlying type */ \
        /* This would need to be customized based on the specific trait */ \
    };

// Macro to implement the HasPrefixField trait for C++ classes
// This is used to indicate that a class has another type as its initial prefix field
#define HAS_PREFIX_FIELD_DERIVE(ClassName, PrefixType) \
    template<> \
    class ruffle::common::utils::HasPrefixField<ClassName> { \
    public: \
        static_assert(std::is_standard_layout_v<ClassName>, "ClassName must be standard layout"); \
        static_assert(std::is_standard_layout_v<PrefixType>, "PrefixType must be standard layout"); \
        static_assert(offsetof(ClassName, prefix_field) == 0, "Prefix field must be at offset 0"); \
        \
        template<typename T> \
        static T* as_prefix(T* obj) { \
            static_assert(std::is_same_v<T, ClassName>); \
            return static_cast<PrefixType*>(obj); \
        } \
        \
        template<typename T> \
        static const T* as_prefix(const T* obj) { \
            static_assert(std::is_same_v<T, ClassName>); \
            return static_cast<const PrefixType*>(obj); \
        } \
    };

// Macro to get an interned AvmAtom from a string literal
// This assumes there's a context available to retrieve the interned string
#define ATOM(str_literal) \
    ([]() -> ruffle::AvmAtom<void*> { \
        /* In a real implementation, this would retrieve the interned string from context */ \
        static_assert(sizeof(str_literal) > 0, "String literal must not be empty"); \
        return ruffle::AvmAtom<void*>(nullptr); /* Placeholder */ \
    }())

// Macro similar to ATOM but returns an AvmString instead of an AvmAtom
#define ISTR(str_literal) \
    ([]() -> ruffle::AvmString<void*> { \
        /* In a real implementation, this would retrieve the interned string from context */ \
        static_assert(sizeof(str_literal) > 0, "String literal must not be empty"); \
        return ruffle::AvmString<void*>(nullptr); /* Placeholder */ \
    }())

// Helper macro to implement From conversion for enum variants
#define IMPLEMENT_FROM_CONVERSION(EnumName, VariantType, VariantTag) \
    template<> \
    inline EnumName from_variant(VariantType&& value) { \
        EnumName result; \
        result.tag_ = VariantTag; \
        new(&result.data_.VariantType##_) VariantType(std::forward<VariantType>(value)); \
        return result; \
    }

// More complex macro to generate the full enum_trait_object pattern
// This would need to be used with template metaprogramming to generate the right code
#define GENERATE_ENUM_TRAIT_IMPL(EnumName, TraitName, ...) \
    template<typename T> \
    struct EnumName ## _Traits { \
        /* Define trait methods here based on the specific trait */ \
    }; \
    \
    /* Generate implementations for each variant */ \
    /* This would be expanded based on the variadic arguments */ \
    \
    template<typename VariantType> \
    struct EnumName ## _Converter { \
        static EnumName convert(VariantType&& v) { \
            return EnumName::make<VariantType>(std::forward<VariantType>(v)); \
        } \
    };

// Example usage of the macros (commented out as examples):
/*
ENUM_TRAIT_OBJECT(MyTraitEnum, MyTrait,
    Object1(ObjectType1),
    Object2(ObjectType2)
)

HAS_PREFIX_FIELD_DERIVE(MyClass, BaseClass)
*/

#endif // MACRO_UTILS_H