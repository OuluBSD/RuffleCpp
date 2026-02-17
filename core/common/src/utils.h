// C++ translation of core/common/src/utils.rs
// Common utilities

#ifndef RUFFLE_CORE_COMMON_UTILS_H
#define RUFFLE_CORE_COMMON_UTILS_H

#include <cstdint>
#include <type_traits>
#include <memory>

namespace ruffle {
namespace common {
namespace utils {

/// A trait indicating that `Self` has `Inner` as an initial prefix.
///
/// A field prefix is the first field in a struct that has the same address as the struct
/// in the memory. If a struct has a prefix field, we can reinterpret the struct pointer
/// as a pointer to the field.
///
/// This trait provides various methods to cast `Self` references to `Inner`
/// references, which can be used e.g. to implement OOP-style class hierarchies.
///
/// In C++, this is implemented using static_assert for compile-time checks
/// and reinterpret_cast for the actual casting.
///
/// # Safety
/// - `Self` must have a field of type `Inner` at the start of its layout;
/// - `Self` must not impose additional safety invariants on the `Inner` prefix;
/// - The methods of this trait should not be overriden;
/// - Any layout constraints that can't be checked by the type-system should
///   be checked by assertions in the ASSERT_PREFIX_FIELD constant.
template<typename Self, typename Inner>
struct HasPrefixField {
    /// This constant should *always* be evaluated before relying on the safety
    /// guarantees of this trait.
    static constexpr void ASSERT_PREFIX_FIELD() {
        // Casting between types requires matching alignment.
        static_assert(alignof(Self) == alignof(Inner), 
            "Prefix field must have same alignment as containing type");
        static_assert(sizeof(Self) >= sizeof(Inner),
            "Prefix field must not be larger than containing type");
        static_assert(offsetof(Self, prefix_field) == 0,
            "Prefix field must be at offset 0");
    }

    /// Casts a shared_ptr object to its prefix field.
    template<typename T>
    static std::shared_ptr<Inner> as_prefix(std::shared_ptr<Self> ptr) {
        ASSERT_PREFIX_FIELD();
        // SAFETY: The above asserts guarantee that the layouts are compatible.
        return std::reinterpret_pointer_cast<Inner>(ptr);
    }

    /// Casts a raw pointer to its prefix field.
    static Inner* as_prefix_raw(Self* ptr) {
        ASSERT_PREFIX_FIELD();
        return reinterpret_cast<Inner*>(ptr);
    }

    /// Casts a const raw pointer to its prefix field.
    static const Inner* as_prefix_raw(const Self* ptr) {
        ASSERT_PREFIX_FIELD();
        return reinterpret_cast<const Inner*>(ptr);
    }
};

/// A `u8` which is always zero. Useful to artificially introduce niches into a struct.
enum class ZeroU8 : uint8_t {
    Zero = 0,
};

inline constexpr ZeroU8 ZeroU8Default = ZeroU8::Zero;

} // namespace utils
} // namespace common
} // namespace ruffle

#endif // RUFFLE_CORE_COMMON_UTILS_H
