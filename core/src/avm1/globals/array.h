// C++ translation of core/src/avm1/globals/array.rs
//! Array class

#ifndef RUFFLE_CORE_AVM1_GLOBALS_ARRAY_H
#define RUFFLE_CORE_AVM1_GLOBALS_ARRAY_H

#include <cstdint>
#include <vector>
#include <algorithm>
#include <functional>
#include <compare>
#include <optional>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC> class AvmString;
    template<typename GC> class WStr;
    template<typename GC> class WString;
    template<typename GC> class Mutation;
    template<typename GC> class StringContext;

    namespace avm1 {
        template<typename GC> class Value;
        template<typename GC> class Object;
        template<typename GC> class Activation;
        template<typename GC> class Error;
        enum class Attribute : uint8_t;
        enum class NativeObject;

        namespace property_decl {
            template<typename GC> class DeclContext;
            template<typename GC> class StaticDeclarations;
            template<typename GC> class SystemClass;
        }
    }
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace array {

/// Options used by Array.sort and Array.sortOn.
class SortOptions {
private:
    int32_t bits_;

public:
    static constexpr int32_t CASE_INSENSITIVE     = 1 << 0;
    static constexpr int32_t DESCENDING           = 1 << 1;
    static constexpr int32_t UNIQUE_SORT          = 1 << 2;
    static constexpr int32_t RETURN_INDEXED_ARRAY = 1 << 3;
    static constexpr int32_t NUMERIC              = 1 << 4;

    SortOptions() : bits_(0) {}
    explicit SortOptions(int32_t bits) : bits_(bits) {}

    static SortOptions from_bits_truncate(int32_t bits) {
        return SortOptions(bits);
    }

    static SortOptions empty() {
        return SortOptions(0);
    }

    bool contains(SortOptions other) const {
        return (bits_ & other.bits_) != 0;
    }

    void remove(SortOptions other) {
        bits_ &= ~other.bits_;
    }

    int32_t bits() const { return bits_; }

    SortOptions operator|(SortOptions other) const {
        return SortOptions(bits_ | other.bits_);
    }

    SortOptions operator&(SortOptions other) const {
        return SortOptions(bits_ & other.bits_);
    }
};

/// Compare function used by Array.sort and Array.sortOn.
template<typename GC>
using CompareFn = std::function<std::compare_ordering(
    Activation<GC>*,
    const Value<GC>&,
    const Value<GC>&,
    SortOptions
)>;

/// Default ordering to return if comparison is invalid.
inline constexpr std::compare_ordering DEFAULT_ORDERING = std::compare_ordering::equivalent;

/// Intermediate builder for constructing ArrayObject,
/// used to work around borrow-checker issues.
template<typename GC>
class ArrayBuilder {
private:
    Mutation<GC>* mc_;
    AvmString<GC> length_prop_;
    AvmString<GC> proto_prop_;
    Object<GC> proto_;

public:
    static Object<GC> empty(Activation<GC>* activation);
    static ArrayBuilder<GC> new_(Activation<GC>* activation);
    static ArrayBuilder<GC> new_with_proto(StringContext<GC>* context, Object<GC> proto);

    ArrayBuilder(Mutation<GC>* mc, AvmString<GC> length_prop, AvmString<GC> proto_prop, Object<GC> proto)
        : mc_(mc), length_prop_(length_prop), proto_prop_(proto_prop), proto_(proto) {}

    void init_with(Object<GC> this_, const std::vector<Value<GC>>& elements);

    Object<GC> with(const std::vector<Value<GC>>& elements);
};

/// Implements Array constructor
template<typename GC>
Value<GC> constructor(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args);

/// Implements Array function
template<typename GC>
Value<GC> array_fn(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args);

/// Array.push - Add elements to the end of an array
template<typename GC>
Value<GC> push(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args);

/// Array.unshift - Add elements to the beginning of an array
template<typename GC>
Value<GC> unshift(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args);

/// Array.shift - Remove and return the first element
template<typename GC>
Value<GC> shift(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args);

/// Array.pop - Remove and return the last element
template<typename GC>
Value<GC> pop(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args);

/// Array.reverse - Reverse the array in place
template<typename GC>
Value<GC> reverse(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args);

/// Array.join - Join array elements into a string
template<typename GC>
Value<GC> join(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args);

/// Array.slice - Extract a section of an array
template<typename GC>
Value<GC> slice(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args);

/// Array.splice - Add/remove elements from an array
template<typename GC>
Value<GC> splice(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args);

/// Array.concat - Concatenate arrays
template<typename GC>
Value<GC> concat(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args);

/// Array.toString - Convert array to string
template<typename GC>
Value<GC> to_string(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args);

/// Array.sort - Sort the array
template<typename GC>
Value<GC> sort(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args);

/// Array.sortOn - Sort array by object field
template<typename GC>
Value<GC> sort_on(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args);

/// Create the Array class
template<typename GC>
property_decl::SystemClass<GC> create_class(
    property_decl::DeclContext<GC>* context,
    Object<GC> super_proto
);

// Helper functions

/// Handles an index parameter that may be positive (starting from beginning) or negative (starting from end).
/// The returned index will be positive and clamped from [0, length].
inline int32_t make_index_absolute(int32_t index, int32_t length) {
    if (index < 0) {
        return std::max(index + length, 0);
    } else {
        return std::min(index, length);
    }
}

/// Compare between two values, with specified sort options.
template<typename GC>
std::compare_ordering sort_compare(
    Activation<GC>* activation,
    const Value<GC>& a,
    const Value<GC>& b,
    SortOptions options
);

/// Create a compare function based on a user-provided custom AS function.
template<typename GC>
CompareFn<GC> sort_compare_custom(Object<GC> compare_fn);

/// Create a compare function based on field names and options.
template<typename GC>
CompareFn<GC> sort_on_compare(const std::vector<std::pair<AvmString<GC>, SortOptions>>& fields);

/// Common code for both Array.sort and Array.sortOn.
template<typename GC>
Value<GC> sort_internal(
    Activation<GC>* activation,
    Object<GC> this_,
    CompareFn<GC> compare_fn,
    SortOptions options,
    bool is_sort_on
);

/// Sort elements using the quicksort algorithm, mimicking Flash's behavior.
template<typename GC>
void qsort(
    Activation<GC>* activation,
    std::vector<std::pair<int32_t, Value<GC>>>& elements,
    const CompareFn<GC>& compare_fn,
    SortOptions options
);

// Inline implementations

template<typename GC>
Object<GC> ArrayBuilder<GC>::empty(Activation<GC>* activation) {
    return ArrayBuilder<GC>::new_(activation).with({});
}

template<typename GC>
ArrayBuilder<GC> ArrayBuilder<GC>::new_(Activation<GC>* activation) {
    return new_with_proto(&activation->context().strings, activation->prototypes().array);
}

template<typename GC>
ArrayBuilder<GC> ArrayBuilder<GC>::new_with_proto(StringContext<GC>* context, Object<GC> proto) {
    // In a real implementation, these would use interned strings
    // For now, we use placeholders
    return ArrayBuilder<GC>(
        context->gc(),
        AvmString<GC>(), // "length"
        AvmString<GC>(), // "__proto__"
        proto
    );
}

template<typename GC>
void ArrayBuilder<GC>::init_with(Object<GC> this_, const std::vector<Value<GC>>& elements) {
    int32_t length = 0;
    for (const auto& value : elements) {
        auto length_str = AvmString<GC>::new_utf8(mc_, std::to_string(length));
        this_->define_value(mc_, length_str, value, Attribute::empty());
        length++;
    }
    
    this_->define_value(
        mc_,
        length_prop_,
        Value<GC>::number(length),
        static_cast<Attribute>(Attribute::DONT_ENUM | Attribute::DONT_DELETE)
    );

    this_->set_native(mc_, NativeObject::Array);
}

template<typename GC>
Object<GC> ArrayBuilder<GC>::with(const std::vector<Value<GC>>& elements) {
    auto obj = Object<GC>::new_without_proto(mc_);
    obj.define_value(
        mc_,
        proto_prop_,
        Value<GC>::object(proto_),
        static_cast<Attribute>(Attribute::DONT_ENUM | Attribute::DONT_DELETE)
    );

    init_with(obj, elements);
    return obj;
}

template<typename GC>
Value<GC> constructor(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args) {
    auto builder = ArrayBuilder<GC>::new_(activation);
    
    if (args.size() == 1 && args[0].is_number()) {
        builder.init_with(this_, {});
        this_->set_length(activation, static_cast<int32_t>(args[0].as_number()));
    } else {
        builder.init_with(this_, args);
    }

    return Value<GC>::object(this_);
}

template<typename GC>
Value<GC> array_fn(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args) {
    if (args.size() == 1 && args[0].is_number()) {
        auto array = ArrayBuilder<GC>::empty(activation);
        array.set_length(activation, static_cast<int32_t>(args[0].as_number()));
        return Value<GC>::object(array);
    } else {
        return Value<GC>::object(ArrayBuilder<GC>::new_(activation).with(args));
    }
}

template<typename GC>
Value<GC> push(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args) {
    int32_t old_length = this_->length(activation);
    
    for (size_t i = 0; i < args.size(); i++) {
        this_->set_element(activation, old_length + static_cast<int32_t>(i), args[i]);
    }

    int32_t new_length = old_length + static_cast<int32_t>(args.size());
    this_->set_length(activation, new_length);
    
    return Value<GC>::number(new_length);
}

template<typename GC>
Value<GC> unshift(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args) {
    int32_t old_length = this_->length(activation);
    int32_t new_length = old_length + static_cast<int32_t>(args.size());
    
    for (int32_t i = old_length - 1; i >= 0; i--) {
        int32_t from = i;
        int32_t to = new_length - i - 1;
        if (this_->has_element(activation, from)) {
            auto element = this_->get_element(activation, from);
            this_->set_element(activation, to, element);
        } else {
            this_->delete_element(activation, to);
        }
    }

    for (size_t i = 0; i < args.size(); i++) {
        this_->set_element(activation, static_cast<int32_t>(i), args[i]);
    }

    if (this_->native() == NativeObject::Array) {
        this_->set_length(activation, new_length);
    }

    return Value<GC>::number(new_length);
}

template<typename GC>
Value<GC> shift(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args) {
    int32_t length = this_->length(activation);
    if (length == 0) {
        return Value<GC>::undefined();
    }

    auto first = this_->get_element(activation, 0);

    for (int32_t i = 1; i < length; i++) {
        if (this_->has_element(activation, i)) {
            auto element = this_->get_element(activation, i);
            this_->set_element(activation, i - 1, element);
        } else {
            this_->delete_element(activation, i - 1);
        }
    }

    this_->delete_element(activation, length - 1);

    if (this_->native() == NativeObject::Array) {
        this_->set_length(activation, length - 1);
    }

    return first;
}

template<typename GC>
Value<GC> pop(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args) {
    int32_t length = this_->length(activation);
    if (length == 0) {
        return Value<GC>::undefined();
    }

    auto last = this_->get_element(activation, length - 1);
    this_->delete_element(activation, length - 1);

    if (this_->native() == NativeObject::Array) {
        this_->set_length(activation, length - 1);
    }

    return last;
}

template<typename GC>
Value<GC> reverse(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args) {
    int32_t length = this_->length(activation);
    
    for (int32_t lower_index = 0; lower_index < length / 2; lower_index++) {
        bool has_lower = this_->has_element(activation, lower_index);
        auto lower_value = has_lower ? this_->get_element(activation, lower_index) : Value<GC>::undefined();

        int32_t upper_index = length - lower_index - 1;
        bool has_upper = this_->has_element(activation, upper_index);
        auto upper_value = has_upper ? this_->get_element(activation, upper_index) : Value<GC>::undefined();

        if (has_lower && has_upper) {
            this_->set_element(activation, lower_index, upper_value);
            this_->set_element(activation, upper_index, lower_value);
        } else if (has_lower && !has_upper) {
            this_->delete_element(activation, lower_index);
            this_->set_element(activation, upper_index, lower_value);
        } else if (!has_lower && has_upper) {
            this_->set_element(activation, lower_index, upper_value);
            this_->delete_element(activation, upper_index);
        } else {
            this_->delete_element(activation, lower_index);
            this_->delete_element(activation, upper_index);
        }
    }

    return Value<GC>::object(this_);
}

template<typename GC>
Value<GC> join(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args) {
    int32_t length = this_->length(activation);

    auto separator = args.empty() 
        ? AvmString<GC>::from_units(b",")
        : args[0].coerce_to_string(activation);

    if (length <= 0) {
        return Value<GC>::string(AvmString<GC>());
    }

    std::vector<AvmString<GC>> parts;
    for (int32_t i = 0; i < length; i++) {
        auto element = this_->get_element(activation, i);
        parts.push_back(element.coerce_to_string(activation));
    }

    auto joined = ruffle::core::string::join(parts, separator);
    return Value<GC>::string(AvmString<GC>::new_(activation->gc(), joined));
}

template<typename GC>
Value<GC> slice(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args) {
    int32_t length = this_->length(activation);

    int32_t start = make_index_absolute(args.empty() ? 0 : static_cast<int32_t>(args[0].as_number()), length);

    int32_t end = length;
    if (args.size() > 1) {
        end = make_index_absolute(static_cast<int32_t>(args[1].as_number()), length);
    }

    std::vector<Value<GC>> elements;
    for (int32_t i = start; i < end; i++) {
        elements.push_back(this_->get_element(activation, i));
    }

    return Value<GC>::object(ArrayBuilder<GC>::new_(activation).with(elements));
}

template<typename GC>
Value<GC> splice(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args) {
    if (args.empty()) {
        return Value<GC>::undefined();
    }

    int32_t start = static_cast<int32_t>(args[0].as_number());
    int32_t length = this_->length(activation);
    start = make_index_absolute(start, length);

    int32_t delete_count = length - start;
    if (args.size() > 1) {
        delete_count = std::min(static_cast<int32_t>(args[1].as_number()), length - start);
    }
    
    if (delete_count < 0) {
        return Value<GC>::undefined();
    }

    std::vector<Value<GC>> result_elements;
    for (int32_t i = 0; i < delete_count; i++) {
        result_elements.push_back(this_->get_element(activation, start + i));
    }

    auto result = ArrayBuilder<GC>::new_(activation).with(result_elements);

    auto items = args.size() > 2 ? std::vector<Value<GC>>(args.begin() + 2, args.end()) : std::vector<Value<GC>>();
    
    if (items.size() > static_cast<size_t>(delete_count)) {
        for (int32_t i = length - 1; i >= start + delete_count; i--) {
            if (this_->has_element(activation, i)) {
                auto element = this_->get_element(activation, i);
                this_->set_element(activation, i - delete_count + static_cast<int32_t>(items.size()), element);
            } else {
                this_->delete_element(activation, i - delete_count + static_cast<int32_t>(items.size()));
            }
        }
    } else {
        for (int32_t i = start + delete_count; i < length; i++) {
            if (this_->has_element(activation, i)) {
                auto element = this_->get_element(activation, i);
                this_->set_element(activation, i - delete_count + static_cast<int32_t>(items.size()), element);
            } else {
                this_->delete_element(activation, i - delete_count + static_cast<int32_t>(items.size()));
            }
        }
    }

    for (size_t i = 0; i < items.size(); i++) {
        this_->set_element(activation, start + static_cast<int32_t>(i), items[i]);
    }
    
    this_->set_length(activation, length - delete_count + static_cast<int32_t>(items.size()));

    return Value<GC>::object(result);
}

template<typename GC>
Value<GC> concat(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args) {
    std::vector<Value<GC>> elements;

    // Add this array's elements
    if (this_->native() == NativeObject::Array) {
        int32_t length = this_->length(activation);
        for (int32_t i = 0; i < length; i++) {
            elements.push_back(this_->get_element(activation, i));
        }
    } else {
        elements.push_back(Value<GC>::object(this_));
    }

    // Add arguments
    for (const auto& arg : args) {
        if (arg.is_object()) {
            auto obj = arg.as_object();
            if (obj.native() == NativeObject::Array) {
                int32_t length = obj.length(activation);
                for (int32_t i = 0; i < length; i++) {
                    elements.push_back(obj.get_element(activation, i));
                }
                continue;
            }
        }
        elements.push_back(arg);
    }

    return Value<GC>::object(ArrayBuilder<GC>::new_(activation).with(elements));
}

template<typename GC>
Value<GC> to_string(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args) {
    return join(activation, this_, {});
}

template<typename GC>
std::compare_ordering sort_compare(
    Activation<GC>* activation,
    const Value<GC>& a,
    const Value<GC>& b,
    SortOptions options
) {
    std::compare_ordering result;
    
    if (options.contains(SortOptions::NUMERIC) && a.is_number() && b.is_number()) {
        double a_num = a.as_number();
        double b_num = b.as_number();
        if (a_num < b_num) result = std::compare_ordering::less;
        else if (a_num > b_num) result = std::compare_ordering::greater;
        else result = std::compare_ordering::equivalent;
    } else {
        auto a_str = a.coerce_to_string(activation);
        auto b_str = b.coerce_to_string(activation);
        
        if (options.contains(SortOptions::CASE_INSENSITIVE)) {
            result = a_str.cmp_ignore_case(b_str);
        } else {
            result = a_str.cmp(b_str);
        }
    }

    if (options.contains(SortOptions::DESCENDING)) {
        return result == std::compare_ordering::less ? std::compare_ordering::greater :
               result == std::compare_ordering::greater ? std::compare_ordering::less :
               std::compare_ordering::equivalent;
    }
    
    return result;
}

template<typename GC>
Value<GC> sort(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args) {
    CompareFn<GC> compare_fn;
    SortOptions options = SortOptions::empty();

    if (!args.empty() && args[0].is_object()) {
        compare_fn = sort_compare_custom(args[0].as_object());
        if (args.size() > 1 && args[1].is_number()) {
            options = SortOptions::from_bits_truncate(static_cast<int32_t>(args[1].as_number()));
        }
    } else if (!args.empty() && args[0].is_number()) {
        compare_fn = [](Activation<GC>* a, const Value<GC>& b, const Value<GC>& c, SortOptions o) {
            return sort_compare(a, b, c, o);
        };
        options = SortOptions::from_bits_truncate(static_cast<int32_t>(args[0].as_number()));
    } else {
        compare_fn = [](Activation<GC>* a, const Value<GC>& b, const Value<GC>& c, SortOptions o) {
            return sort_compare(a, b, c, o);
        };
    }

    return sort_internal(activation, this_, compare_fn, options, false);
}

template<typename GC>
Value<GC> sort_on(Activation<GC>* activation, Object<GC> this_, const std::vector<Value<GC>>& args) {
    if (args.empty()) {
        return Value<GC>::undefined();
    }

    std::vector<std::pair<AvmString<GC>, SortOptions>> fields;

    if (args[0].is_object() && args[0].as_object().native() == NativeObject::Array) {
        auto field_names_array = args[0].as_object();
        int32_t length = field_names_array.length(activation);

        if (length <= 0) {
            return Value<GC>::object(this_);
        }

        for (int32_t i = 0; i < length; i++) {
            auto field_name = field_names_array.get_element(activation, i).coerce_to_string(activation);
            fields.emplace_back(field_name, SortOptions::empty());
        }

        if (args.size() > 1 && args[1].is_object() && args[1].as_object().native() == NativeObject::Array) {
            auto options_array = args[1].as_object();
            if (options_array.length(activation) == length) {
                for (int32_t i = 0; i < length; i++) {
                    fields[i].second = SortOptions::from_bits_truncate(
                        static_cast<int32_t>(options_array.get_element(activation, i).as_number())
                    );
                }
            }
        } else if (args.size() > 1) {
            auto options = SortOptions::from_bits_truncate(
                static_cast<int32_t>(args[1].as_number())
            );
            for (auto& field : fields) {
                field.second = options;
            }
        }
    } else {
        auto field_name = args[0].coerce_to_string(activation);
        SortOptions options = SortOptions::empty();
        if (args.size() > 1 && args[1].is_number()) {
            options = SortOptions::from_bits_truncate(static_cast<int32_t>(args[1].as_number()));
        }
        fields.emplace_back(field_name, options);
    }

    auto main_options = fields[0].second;
    auto compare_fn = sort_on_compare(fields);
    return sort_internal(activation, this_, compare_fn, main_options, true);
}

template<typename GC>
void qsort(
    Activation<GC>* activation,
    std::vector<std::pair<int32_t, Value<GC>>>& elements,
    const CompareFn<GC>& compare_fn,
    SortOptions options
) {
    if (elements.size() < 2) {
        return;
    }

    // Stack for storing inclusive subarray boundaries
    std::vector<std::pair<size_t, size_t>> stack;
    stack.emplace_back(0, elements.size() - 1);

    while (!stack.empty()) {
        auto [low, high] = stack.back();
        stack.pop_back();

        if (low >= high) {
            continue;
        }

        // Flash always chooses the leftmost element as the pivot
        auto pivot = elements[low].second;

        size_t left = low + 1;
        size_t right = high;

        while (true) {
            // Find an element greater than the pivot from the left
            while (left < right) {
                auto ordering = compare_fn(activation, pivot, elements[left].second, options);
                if (ordering != std::compare_ordering::less) {
                    break;
                }
                left++;
            }

            // Find an element lower than the pivot from the right
            while (right > low) {
                auto ordering = compare_fn(activation, pivot, elements[right].second, options);
                if (ordering != std::compare_ordering::greater) {
                    break;
                }
                right--;
            }

            // When left and right cross, then no element greater than
            // the pivot comes before an element lower than the pivot
            if (left >= right) {
                break;
            }

            // Otherwise, swap left and right, and keep going
            std::swap(elements[left], elements[right]);
        }

        // Move the pivot element to its position between the partitions
        std::swap(elements[low], elements[right]);

        // Push subarrays onto the stack for further sorting
        stack.emplace_back(right + 1, high);
        if (right > 0) {
            stack.emplace_back(low, right - 1);
        }
    }
}

} // namespace array
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_ARRAY_H
