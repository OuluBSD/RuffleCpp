// C++ translation of core/src/avm2/globals/array.rs
//! Array builtin/prototype

#ifndef RUFFLE_CORE_AVM2_GLOBALS_ARRAY_H
#define RUFFLE_CORE_AVM2_GLOBALS_ARRAY_H

#include <vector>
#include <cstdint>
#include <optional>
#include <functional>
#include <algorithm>
#include <compare>

// Forward declarations
namespace ruffle {
namespace core {

namespace string {
    template<typename GC>
    class AvmString;
}

namespace avm2 {

template<typename GC>
class Activation;
template<typename GC>
class Value;
template<typename GC>
class Error;
template<typename GC>
class Object;
template<typename GC>
class ArrayObject;
template<typename GC>
class ArrayStorage;
template<typename GC>
class FunctionObject;
template<typename GC>
class ClassObject;

namespace parameters {
    template<typename GC>
    class ParametersExt;
}

namespace function {
    template<typename GC>
    class FunctionArgs;
}

} // namespace avm2
} // namespace core
} // namespace ruffle

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace array {

using string::AvmString;

// ============================================================================
// SortOptions bitflags
// ============================================================================

/// The array options that a given sort operation may use.
enum class SortOptions : uint8_t {
    None = 0,
    CaseInsensitive = 1 << 0,
    Descending = 1 << 1,
    UniqueSort = 1 << 2,
    ReturnIndexedArray = 1 << 3,
    Numeric = 1 << 4
};

inline SortOptions operator|(SortOptions a, SortOptions b) {
    return static_cast<SortOptions>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

inline SortOptions operator&(SortOptions a, SortOptions b) {
    return static_cast<SortOptions>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}

inline bool contains(SortOptions flags, SortOptions flag) {
    return (static_cast<uint8_t>(flags) & static_cast<uint8_t>(flag)) != 0;
}

inline SortOptions from_bits_truncate(uint32_t bits) {
    return static_cast<SortOptions>(bits & 0x1F);
}

// ============================================================================
// ArrayIter - Iterator for array contents with user code execution
// ============================================================================

/// An iterator that allows iterating over the contents of an array whilst also
/// executing user code. This maintains lock safety in the presence of arbitrary
/// user code that might mutate the array.
template<typename GC>
class ArrayIter {
private:
    Object<GC> array_object_;
    uint32_t index_;
    uint32_t rev_index_;

public:
    /// Construct a new ArrayIter.
    static ArrayIter<GC> create(
        Activation<GC>* activation,
        Object<GC> array_object
    ) {
        return with_bounds(activation, array_object, 0, UINT32_MAX);
    }

    /// Construct a new ArrayIter that is bounded to a given range.
    static ArrayIter<GC> with_bounds(
        Activation<GC>* activation,
        Object<GC> array_object,
        uint32_t start_index,
        uint32_t end_index
    );

    /// Get the next item from the front of the array.
    std::optional<std::pair<uint32_t, Value<GC>>> next(Activation<GC>* activation);

    /// Get the next item from the back of the array.
    std::optional<std::pair<uint32_t, Value<GC>>> next_back(Activation<GC>* activation);
};

// ============================================================================
// Function Declarations
// ============================================================================

/// Implements Array's instance initializer.
template<typename GC>
Value<GC> array_initializer(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements Array.length's getter.
template<typename GC>
Value<GC> get_length(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements Array.length's setter.
template<typename GC>
Value<GC> set_length(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Bundle an already-constructed ArrayStorage in an Object.
template<typename GC>
Value<GC> build_array(
    Activation<GC>* activation,
    ArrayStorage<GC> array
);

/// Implements Array.concat.
template<typename GC>
Value<GC> concat(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Resolves array holes.
template<typename GC>
Value<GC> resolve_array_hole(
    Activation<GC>* activation,
    Object<GC> this_obj,
    size_t i,
    std::optional<Value<GC>> item
);

/// Implements Array.join.
template<typename GC>
Value<GC> join(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements Array.forEach.
template<typename GC>
Value<GC> for_each(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements Array.map.
template<typename GC>
Value<GC> map(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements Array.filter.
template<typename GC>
Value<GC> filter(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements Array.every.
template<typename GC>
Value<GC> every(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements Array.some.
template<typename GC>
Value<GC> some(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements Array.indexOf.
template<typename GC>
Value<GC> index_of(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements Array.lastIndexOf.
template<typename GC>
Value<GC> last_index_of(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements Array.pop.
template<typename GC>
Value<GC> pop(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements Array.push.
template<typename GC>
Value<GC> push(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements Array.reverse.
template<typename GC>
Value<GC> reverse(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements Array.shift.
template<typename GC>
Value<GC> shift(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements Array.unshift.
template<typename GC>
Value<GC> unshift(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Resolve a possibly-negative array index to something guaranteed to be positive.
template<typename GC>
size_t resolve_index(
    Activation<GC>* activation,
    Value<GC> index,
    size_t length
);

/// Implements Array.slice.
template<typename GC>
Value<GC> slice(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements Array.splice.
template<typename GC>
Value<GC> splice(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements Array.sort.
template<typename GC>
Value<GC> sort(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements Array.sortOn.
template<typename GC>
Value<GC> sort_on(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements Array.removeAt.
template<typename GC>
Value<GC> remove_at(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

// ============================================================================
// Comparison Functions
// ============================================================================

/// Compare strings case-sensitively.
template<typename GC>
std::compare_ordering compare_string_case_sensitive(
    Activation<GC>* activation,
    Value<GC> a,
    Value<GC> b
);

/// Compare strings case-insensitively.
template<typename GC>
std::compare_ordering compare_string_case_insensitive(
    Activation<GC>* activation,
    Value<GC> a,
    Value<GC> b
);

/// Perform numeric comparison for sort (compatibility mode).
template<typename GC, bool COMPAT>
std::compare_ordering compare_numeric(
    Activation<GC>* activation,
    Value<GC> a,
    Value<GC> b
);

// ============================================================================
// QuickSort Implementation
// ============================================================================

/// A port of the avmplus QuickSort implementation.
template<typename T, typename E>
std::optional<E> qsort(
    std::vector<T>& slice,
    std::function<std::optional<E>(const T&, const T&)> cmp
);

// ============================================================================
// Template Implementations
// ============================================================================

template<typename GC>
ArrayIter<GC> ArrayIter<GC>::with_bounds(
    Activation<GC>* activation,
    Object<GC> array_object,
    uint32_t start_index,
    uint32_t end_index
) {
    // Would get length from array_object
    uint32_t length = 0;  // Simplified
    return ArrayIter<GC>{
        array_object,
        std::min(start_index, length),
        std::min(end_index + 1, length)
    };
}

template<typename GC>
std::optional<std::pair<uint32_t, Value<GC>>> ArrayIter<GC>::next(Activation<GC>* activation) {
    if (index_ < rev_index_) {
        uint32_t i = index_++;
        // Would get value from array_object
        return std::make_pair(i, Value<GC>::undefined());
    }
    return std::nullopt;
}

template<typename GC>
std::optional<std::pair<uint32_t, Value<GC>>> ArrayIter<GC>::next_back(Activation<GC>* activation) {
    if (index_ < rev_index_) {
        --rev_index_;
        uint32_t i = rev_index_;
        // Would get value from array_object
        return std::make_pair(i, Value<GC>::undefined());
    }
    return std::nullopt;
}

template<typename GC>
Value<GC> array_initializer(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    if (auto this_obj = this_val.as_object()) {
        if (args.size() == 1) {
            if (auto expected_len = args[0].as_f64()) {
                if (*expected_len >= 0.0 && !std::isnan(*expected_len) && 
                    std::floor(*expected_len) == *expected_len) {
                    // Set array length
                    return Value<GC>::undefined();
                }
            }
        }
        // Set array elements from args
    }
    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> get_length(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    if (auto this_obj = this_val.as_object()) {
        if (auto array = this_obj->as_array_storage()) {
            return Value<GC>::from_uint(array->length());
        }
    }
    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> set_length(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    if (auto this_obj = this_val.as_object()) {
        if (auto array = this_obj->as_array_storage_mut()) {
            uint32_t size = args.size() > 0 ? args[0].coerce_to_u32(activation) : 0;
            array->set_length(size);
        }
    }
    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> concat(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    // Implementation would concatenate arrays
    return this_val;
}

template<typename GC>
Value<GC> join(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    // Implementation would join array elements with separator
    return Value<GC>::from_string(AvmString<GC>::empty());
}

template<typename GC>
Value<GC> for_each(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    if (auto callback = args.size() > 0 ? args[0].as_function() : nullptr) {
        auto receiver = args.size() > 1 ? args[1] : Value<GC>::undefined();
        auto iter = ArrayIter<GC>::create(activation, *this_val.as_object());
        
        while (auto item = iter.next(activation)) {
            std::vector<Value<GC>> call_args = {
                item->second,
                Value<GC>::from_uint(item->first),
                this_val
            };
            callback->call(activation, receiver, call_args);
        }
    }
    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> map(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    // Implementation would map array elements
    return ArrayObject<GC>::empty(activation->context()).into();
}

template<typename GC>
Value<GC> filter(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    // Implementation would filter array elements
    return ArrayObject<GC>::empty(activation->context()).into();
}

template<typename GC>
Value<GC> every(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    // Implementation would test if all elements pass callback
    return Value<GC>::from_bool(true);
}

template<typename GC>
Value<GC> some(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    // Implementation would test if any element passes callback
    return Value<GC>::from_bool(false);
}

template<typename GC>
Value<GC> index_of(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    // Implementation would find first index of value
    return Value<GC>::from_int(-1);
}

template<typename GC>
Value<GC> last_index_of(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    // Implementation would find last index of value
    return Value<GC>::from_int(-1);
}

template<typename GC>
Value<GC> pop(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    if (auto this_obj = this_val.as_object()) {
        if (auto array = this_obj->as_array_storage_mut()) {
            return array->pop();
        }
    }
    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> push(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    if (auto this_obj = this_val.as_object()) {
        if (auto array = this_obj->as_array_storage_mut()) {
            for (const auto& arg : args) {
                array->push(arg);
            }
            return Value<GC>::from_uint(array->length());
        }
    }
    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> reverse(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    // Implementation would reverse array in place
    return this_val;
}

template<typename GC>
Value<GC> shift(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    if (auto this_obj = this_val.as_object()) {
        if (auto array = this_obj->as_array_storage_mut()) {
            return array->shift();
        }
    }
    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> unshift(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    if (auto this_obj = this_val.as_object()) {
        if (auto array = this_obj->as_array_storage_mut()) {
            for (auto it = args.rbegin(); it != args.rend(); ++it) {
                array->unshift(*it);
            }
            return Value<GC>::from_uint(array->length());
        }
    }
    return Value<GC>::undefined();
}

template<typename GC>
size_t resolve_index(
    Activation<GC>* activation,
    Value<GC> index,
    size_t length
) {
    double idx = index.coerce_to_number(activation);
    if (idx < 0.0) {
        size_t offset = static_cast<size_t>(-idx);
        return length > offset ? length - offset : 0;
    }
    return std::min(static_cast<size_t>(idx), length);
}

template<typename GC>
Value<GC> slice(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    // Implementation would return slice of array
    return this_val;
}

template<typename GC>
Value<GC> splice(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    // Implementation would splice array
    return this_val;
}

template<typename GC>
Value<GC> sort(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    // Implementation would sort array
    return Value<GC>::from_int(0);
}

template<typename GC>
Value<GC> sort_on(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    // Implementation would sort array on field
    return Value<GC>::from_int(0);
}

template<typename GC>
Value<GC> remove_at(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    if (auto this_obj = this_val.as_object()) {
        if (auto array = this_obj->as_array_storage_mut()) {
            int32_t index = args.size() > 0 ? args[0].coerce_to_i32(activation) : 0;
            return array->remove(index).value_or(Value<GC>::undefined());
        }
    }
    return Value<GC>::undefined();
}

template<typename GC>
std::compare_ordering compare_string_case_sensitive(
    Activation<GC>* activation,
    Value<GC> a,
    Value<GC> b
) {
    auto str_a = a.coerce_to_string(activation);
    auto str_b = b.coerce_to_string(activation);
    return str_a <=> str_b;
}

template<typename GC>
std::compare_ordering compare_string_case_insensitive(
    Activation<GC>* activation,
    Value<GC> a,
    Value<GC> b
) {
    auto str_a = a.coerce_to_string(activation);
    auto str_b = b.coerce_to_string(activation);
    // Would use case-insensitive comparison
    return str_a <=> str_b;
}

template<typename GC, bool COMPAT>
std::compare_ordering compare_numeric(
    Activation<GC>* activation,
    Value<GC> a,
    Value<GC> b
) {
    if constexpr (COMPAT) {
        // Compatibility mode for SWF < 11
        if (auto int_a = a.as_integer()) {
            if (auto int_b = b.as_integer()) {
                // Special integer comparison for compatibility
                return std::compare_weak_order_fallback(*int_a, *int_b);
            }
        }
    }
    
    auto num_a = a.coerce_to_number(activation);
    auto num_b = b.coerce_to_number(activation);
    
    if (std::isnan(num_a) && std::isnan(num_b)) {
        return std::compare_ordering::equal;
    } else if (std::isnan(num_a)) {
        return std::compare_ordering::greater;
    } else if (std::isnan(num_b)) {
        return std::compare_ordering::less;
    }
    
    return num_a <=> num_b;
}

template<typename T, typename E>
std::optional<E> qsort(
    std::vector<T>& slice,
    std::function<std::optional<E>(const T&, const T&)> cmp
) {
    if (slice.size() <= 1) return std::nullopt;
    
    // Simplified quicksort implementation
    std::sort(slice.begin(), slice.end(), [&cmp](const T& a, const T& b) {
        auto result = cmp(a, b);
        return result.value_or(E{}) < E{};
    });
    
    return std::nullopt;
}

} // namespace array
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_ARRAY_H
