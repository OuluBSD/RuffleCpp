/*
 * C++ header for common utilities functionality
 * This replaces the functionality of core/common/src/utils.rs
 */

#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include <type_traits>
#include <cassert>
#include <memory>

// Forward declaration for GC-related functionality
template<typename T>
class GcPtr;

// A trait indicating that `Self` has `Inner` as an initial prefix.
//
// A field prefix is the first field in a struct that has the same address as the struct
// in the memory. If a struct has a prefix field, we can reinterpret the struct pointer
// as a pointer to the field.
//
// Implementing this trait provides various methods to cast `Self` references to `Inner`
// references, which can be used e.g. to implement OOP-style class hierarchies.
template<typename Inner>
class HasPrefixField {
public:
    // Static assertion to verify that the prefix field is at the start of the layout
    template<typename Self>
    static constexpr void assert_prefix_field() {
        static_assert(std::is_standard_layout_v<Self>, "Self must be standard layout");
        static_assert(std::is_standard_layout_v<Inner>, "Inner must be standard layout");
        static_assert(offsetof(Self, prefix_field) == 0, "Prefix field must be at offset 0");
        static_assert(std::is_same_v<std::decay_t<decltype(std::declval<Self>().prefix_field)>, Inner>, 
                     "Prefix field must be of type Inner");
    }

    // Casts a GC'd object to its prefix field.
    template<typename Self>
    static GcPtr<Inner> as_prefix_gc(GcPtr<Self> gc) {
        // Static assertion to ensure layout compatibility
        assert_prefix_field<Self>();
        
        // Verify alignment compatibility
        static_assert(alignof(Self) == alignof(Inner), 
                     "Self and Inner must have the same alignment");
        
        // Perform the cast - in a real implementation this would be more complex
        // For now, we'll just static_cast the underlying pointer
        return GcPtr<Inner>(static_cast<Inner*>(gc.get()));
    }
};

// A smart pointer simulating the GC behavior
template<typename T>
class GcPtr {
private:
    std::shared_ptr<T> ptr_;

public:
    explicit GcPtr(T* ptr) : ptr_(ptr) {}
    explicit GcPtr(std::shared_ptr<T> ptr) : ptr_(std::move(ptr)) {}
    
    T* get() const { return ptr_.get(); }
    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_.get(); }
    
    template<typename U>
    static GcPtr<U> cast(GcPtr<T> original) {
        return GcPtr<U>(std::static_pointer_cast<U>(original.ptr_));
    }
};

// A `uint8_t` which is always zero. Useful to artificially introduce niches into a struct.
enum class ZeroU8 : uint8_t {
    ZERO = 0
};

// Default constructor for ZeroU8
inline constexpr ZeroU8 make_zero_u8() {
    return ZeroU8::ZERO;
}

// Utility function to check if a ZeroU8 is indeed zero
inline constexpr bool is_zero(ZeroU8 z) {
    return static_cast<uint8_t>(z) == 0;
}

// Macro to help implement HasPrefixField (similar to the Rust derive macro)
#define HAS_PREFIX_FIELD(ClassName, FieldType) \
    template<> \
    class HasPrefixField<FieldType> { \
    public: \
        template<typename Self> \
        static constexpr void assert_prefix_field() { \
            static_assert(std::is_standard_layout_v<Self>, "Self must be standard layout"); \
            static_assert(std::is_standard_layout_v<FieldType>, "FieldType must be standard layout"); \
            static_assert(offsetof(Self, prefix_field) == 0, "Prefix field must be at offset 0"); \
            static_assert(std::is_same_v<std::decay_t<decltype(std::declval<Self>().prefix_field)>, FieldType>, \
                         "Prefix field must be of type FieldType"); \
        } \
        \
        template<typename Self> \
        static GcPtr<FieldType> as_prefix_gc(GcPtr<Self> gc) { \
            assert_prefix_field<Self>(); \
            static_assert(alignof(Self) == alignof(FieldType), \
                         "Self and FieldType must have the same alignment"); \
            return GcPtr<FieldType>(static_cast<FieldType*>(gc.get())); \
        } \
    }

#endif // COMMON_UTILS_H