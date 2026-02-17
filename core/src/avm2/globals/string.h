// C++ translation of core/src/avm2/globals/string.rs
//! `String` builtin/prototype

#ifndef RUFFLE_CORE_AVM2_GLOBALS_STRING_H
#define RUFFLE_CORE_AVM2_GLOBALS_STRING_H

#include <vector>
#include <optional>
#include <string>
#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
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
    class AvmString;
    template<typename GC>
    class RegExp;

    namespace globals {
        template<typename GC>
        class RegExpObject;
    }
}

namespace string {
    class WString;
}

namespace gc_arena {
    class Mutation;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace string {

using gc_arena::Mutation;

/// String constructor
template<typename GC>
Value<GC> string_constructor(
    Activation<GC>* activation,
    const std::vector<Value<GC>>& args
);

/// Call handler for String (when called without new)
template<typename GC>
Value<GC> call_handler(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `length` property's getter
template<typename GC>
Value<GC> get_length(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `String.charAt`
template<typename GC>
Value<GC> char_at(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `String.charCodeAt`
template<typename GC>
Value<GC> char_code_at(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `String.concat`
template<typename GC>
Value<GC> concat(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `String.fromCharCode`
template<typename GC>
Value<GC> from_char_code(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `String.indexOf`
template<typename GC>
Value<GC> index_of(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `String.lastIndexOf`
template<typename GC>
Value<GC> last_index_of(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `String.localeCompare`
template<typename GC>
Value<GC> locale_compare(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `String.match`
template<typename GC>
Value<GC> match_internal(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `String.replace`
template<typename GC>
Value<GC> replace(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `String.search`
template<typename GC>
Value<GC> search(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `String.slice`
template<typename GC>
Value<GC> slice(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `String.split`
template<typename GC>
Value<GC> split(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `String.substr`
template<typename GC>
Value<GC> substr(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `String.substring`
template<typename GC>
Value<GC> substring(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `String.toLowerCase`
template<typename GC>
Value<GC> to_lower_case(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

/// Implements `String.toUpperCase`
template<typename GC>
Value<GC> to_upper_case(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
);

// ============================================================================
// Helper Functions
// ============================================================================

/// Normalizes an index parameter used in `String` functions such as `substring`.
/// The returned index will be within the range of `[0, len]`.
inline size_t string_index(double i, size_t len) {
    if (i == std::numeric_limits<double>::infinity()) {
        return len;
    } else if (i < 0.0) {
        return 0;
    } else {
        return std::min(static_cast<size_t>(i), len);
    }
}

/// Normalizes a wrapping index parameter used in `String` functions such as `slice`.
/// Values less than or equal to -1.0 will count backwards from `len`.
/// The returned index will be within the range of `[0, len]`.
inline size_t string_wrapping_index(double i, size_t len) {
    if (i <= -1.0) {
        if (std::isinf(i)) {
            return 0;
        }
        auto offset = static_cast<isize>(i);
        return len - std::min(len, static_cast<size_t>(-offset));
    } else {
        if (std::isinf(i)) {
            return len;
        }
        return std::min(static_cast<size_t>(i), len);
    }
}

// ============================================================================
// Template Implementations
// ============================================================================

template<typename GC>
Value<GC> string_constructor(
    Activation<GC>* activation,
    const std::vector<Value<GC>>& args
) {
    auto string_value = args.size() > 0
        ? args[0].coerce_to_string(activation)
        : AvmString<GC>::empty();

    return Value<GC>::from_string(string_value);
}

template<typename GC>
Value<GC> call_handler(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    if (args.size() > 0) {
        return Value<GC>::from_string(args[0].coerce_to_string(activation));
    }
    return Value<GC>::from_string(AvmString<GC>::empty());
}

template<typename GC>
Value<GC> get_length(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    if (auto s = this_val.as_string()) {
        return Value<GC>::from_int(static_cast<int32_t>(s->len()));
    }
    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> char_at(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    if (auto s = this_val.as_string()) {
        // This function takes Number, so we use get_f64 to handle overflow
        double n = args.size() > 0 ? args[0].as_f64() : 0.0;

        if (n < 0.0) {
            return Value<GC>::from_string(AvmString<GC>::empty());
        }

        size_t index = std::isnan(n) ? 0 : static_cast<size_t>(n);
        auto ret = s->get(index)
            ? AvmString<GC>::make_char(activation->strings(), s->get(index).value())
            : AvmString<GC>::empty();
        return Value<GC>::from_string(ret);
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> char_code_at(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    if (auto s = this_val.as_string()) {
        // This function takes Number, so we use get_f64 to handle overflow
        double n = args.size() > 0 ? args[0].as_f64() : 0.0;

        if (n < 0.0) {
            return Value<GC>::from_number(std::numeric_limits<double>::quiet_NaN());
        }

        size_t index = std::isnan(n) ? 0 : static_cast<size_t>(n);
        double ret = s->get(index)
            ? static_cast<double>(s->get(index).value())
            : std::numeric_limits<double>::quiet_NaN();
        return Value<GC>::from_number(ret);
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> concat(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto ret = this_val.coerce_to_string(activation);

    for (const auto& arg : args) {
        auto s = arg.coerce_to_string(activation);
        ret = AvmString<GC>::concat(activation->gc(), ret, s);
    }

    return Value<GC>::from_string(ret);
}

template<typename GC>
Value<GC> from_char_code(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    string::WString out;
    out.reserve(args.size());

    for (const auto& arg : args) {
        auto i = static_cast<uint16_t>(arg.coerce_to_u32(activation));
        out.push_back(i);
    }

    return Value<GC>::from_string(AvmString<GC>::new_string(activation->gc(), out));
}

template<typename GC>
Value<GC> index_of(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_str = this_val.coerce_to_string(activation);
    auto pattern = args.size() > 0 ? args[0].coerce_to_string(activation) : AvmString<GC>::empty();
    int32_t start_index = std::max(args.size() > 1 ? args[1].as_i32() : 0, 0);

    auto slice = this_str.slice(start_index);
    if (slice.has_value()) {
        if (auto pos = slice->find(pattern)) {
            return Value<GC>::from_int(static_cast<int32_t>(*pos + start_index));
        }
    }

    return Value<GC>::from_int(-1);
}

template<typename GC>
Value<GC> last_index_of(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_str = this_val.coerce_to_string(activation);
    auto pattern = args.size() > 0 ? args[0].coerce_to_string(activation) : AvmString<GC>::empty();

    double start_index_f64 = args.size() > 1 ? args[1].as_f64() : 0.0;
    size_t start_index;

    if (std::isnan(start_index_f64)) {
        start_index = this_str.len();
    } else if (std::isinf(start_index_f64) && start_index_f64 > 0) {
        start_index = this_str.len();
    } else if (std::isinf(start_index_f64) && start_index_f64 < 0) {
        return Value<GC>::from_int(-1);
    } else {
        int32_t idx = static_cast<int32_t>(start_index_f64);
        if (idx < 0) {
            return Value<GC>::from_int(-1);
        }
        start_index = static_cast<size_t>(idx) + pattern.len();
    }

    auto slice = this_str.slice(0, start_index);
    if (slice.has_value()) {
        if (auto pos = slice->rfind(pattern)) {
            return Value<GC>::from_int(static_cast<int32_t>(*pos));
        }
    }

    return Value<GC>::from_int(-1);
}

template<typename GC>
Value<GC> locale_compare(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_str = this_val.coerce_to_string(activation);
    auto other_val = args.size() > 0 ? args[0] : Value<GC>::undefined();

    // FP version < 11 behavior
    if (activation->caller_movie_or_root()->version() < 11) {
        if (other_val.is_null() || other_val.is_undefined()) {
            if (this_str.is_empty()) {
                return Value<GC>::from_int(1);
            } else {
                return Value<GC>::from_int(0);
            }
        }
    }

    auto other = other_val.coerce_to_string(activation);

    for (auto [tc, oc] : this_str.zip(other)) {
        auto res = static_cast<int32_t>(tc) - static_cast<int32_t>(oc);
        if (res != 0) {
            return Value<GC>::from_int(res);
        }
    }

    if (this_str.len() < other.len()) {
        return Value<GC>::from_int(-1);
    }
    if (this_str.len() > other.len()) {
        return Value<GC>::from_int(1);
    }

    return Value<GC>::from_int(0);
}

template<typename GC>
Value<GC> match_internal(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_str = this_val.coerce_to_string(activation);
    auto pattern = args.size() > 0 ? args[0] : Value<GC>::undefined();

    auto regexp_class = activation->avm2().classes().regexp;
    auto pattern_obj = pattern.is_of_type(regexp_class.inner_class_definition())
        ? pattern
        : regexp_class.construct(activation, {Value<GC>::from_string(pattern.coerce_to_string(activation))});

    auto pattern_object = pattern_obj.as_object().value();
    if (auto regexp = pattern_object->as_regexp_mut(activation->gc())) {
        if (regexp->flags().test(RegExpFlags::GLOBAL)) {
            auto storage = ArrayObject<GC>::new_array(activation->context, 0);
            size_t last = 0;
            auto old_last_index = regexp->last_index();
            regexp->set_last_index(0);

            while (auto result = regexp->exec(this_str)) {
                if (regexp->last_index() == last) {
                    break;
                }
                storage->push(activation->strings()->substring(this_str, result.range()));
                last = regexp->last_index();
            }

            regexp->set_last_index(0);
            if (old_last_index == regexp->last_index()) {
                regexp->set_last_index(1);
            }

            return Value<GC>::from_object(storage);
        } else {
            auto old = regexp->last_index();
            regexp->set_last_index(0);

            if (auto result = regexp->exec(this_str)) {
                auto storage = ArrayObject<GC>::new_array(activation->context, 0);
                for (const auto& group : result.groups()) {
                    if (group.has_value()) {
                        storage->push(activation->strings()->substring(this_str, *group));
                    } else {
                        storage->push(Value<GC>::undefined());
                    }
                }

                regexp->set_last_index(old);

                storage->set_dynamic_property("index", Value<GC>::from_int(static_cast<int32_t>(result.start())), activation->gc());
                storage->set_dynamic_property("input", Value<GC>::from_string(this_str), activation->gc());

                return Value<GC>::from_object(storage);
            } else {
                regexp->set_last_index(old);
                return Value<GC>::null();
            }
        }
    }

    return Value<GC>::null();
}

template<typename GC>
Value<GC> replace(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_str = this_val.coerce_to_string(activation);
    auto pattern = args.size() > 0 ? args[0] : Value<GC>::undefined();
    auto replacement = args.size() > 1 ? args[1] : Value<GC>::undefined();

    if (auto regexp_obj = pattern.as_object()) {
        if (auto regexp = regexp_obj->as_regexp_object()) {
            if (auto f = replacement.as_object()->as_function_object()) {
                // TODO: Implement RegExp::replace_fn
                return Value<GC>::from_string(this_str);
            } else {
                auto replacement_str = replacement.coerce_to_string(activation);
                // TODO: Implement RegExp::replace_string
                return Value<GC>::from_string(this_str);
            }
        }
    }

    // String pattern
    auto pattern_str = pattern.coerce_to_string(activation);
    if (auto pos = this_str.find(pattern_str)) {
        string::WString ret;
        ret.append(this_str.slice(0, *pos).value_or(this_str));

        if (auto f = replacement.as_object()->as_function_object()) {
            std::vector<Value<GC>> func_args = {
                Value<GC>::from_string(pattern_str),
                Value<GC>::from_int(static_cast<int32_t>(*pos)),
                Value<GC>::from_string(this_str)
            };
            auto v = f->call(activation, Value<GC>::null(), func_args);
            ret.append(v.coerce_to_string(activation).as_wstr());
        } else {
            auto replacement_str = replacement.coerce_to_string(activation);
            ret.append(replacement_str.as_wstr());
        }

        ret.append(this_str.slice(*pos + pattern_str.len()).value_or(string::WString()));
        return Value<GC>::from_string(AvmString<GC>::new_string(activation->gc(), ret));
    }

    return Value<GC>::from_string(this_str);
}

template<typename GC>
Value<GC> search(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_str = this_val.coerce_to_string(activation);
    auto pattern = args.size() > 0 ? args[0] : Value<GC>::undefined();

    auto regexp_class = activation->avm2().classes().regexp;
    auto pattern_obj = pattern.is_of_type(regexp_class.inner_class_definition())
        ? pattern
        : regexp_class.construct(activation, {Value<GC>::from_string(pattern.coerce_to_string(activation))});

    auto pattern_object = pattern_obj.as_object().value();
    if (auto regexp = pattern_object->as_regexp_mut(activation->gc())) {
        auto old = regexp->last_index();
        regexp->set_last_index(0);

        if (auto result = regexp->exec(this_str)) {
            auto found_index = static_cast<int32_t>(result.groups().flatten().next().value_or(0).start);
            regexp->set_last_index(old);
            return Value<GC>::from_int(found_index);
        } else {
            regexp->set_last_index(old);
            return Value<GC>::from_int(-1);
        }
    }

    return Value<GC>::undefined();
}

template<typename GC>
Value<GC> slice(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_str = this_val.coerce_to_string(activation);

    double start_index_f64 = args.size() > 0 ? args[0].as_f64() : 0.0;
    auto start_index = string_wrapping_index(start_index_f64, this_str.len());

    double end_index_f64 = args.size() > 1 ? args[1].as_f64() : 0.0;
    auto end_index = string_wrapping_index(end_index_f64, this_str.len());

    if (start_index < end_index) {
        return Value<GC>::from_string(activation->strings()->substring(this_str, start_index, end_index));
    }

    return Value<GC>::from_string(AvmString<GC>::empty());
}

template<typename GC>
Value<GC> split(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_str = this_val.coerce_to_string(activation);
    auto delimiter = args.size() > 0 ? args[0] : Value<GC>::undefined();

    size_t limit = args.size() > 1
        ? static_cast<size_t>(args[1].coerce_to_u32(activation))
        : std::numeric_limits<size_t>::max();

    if (limit == 0) {
        return Value<GC>::from_object(ArrayObject<GC>::empty(activation->context));
    }

    // RegExp delimiter
    if (auto delimiter_obj = delimiter.as_object()) {
        if (auto regexp = delimiter_obj->as_regexp_mut(activation->gc())) {
            // TODO: Implement regexp.split
            return Value<GC>::from_object(ArrayObject<GC>::empty(activation->context));
        }
    }

    auto delimiter_str = delimiter.coerce_to_string(activation);

    std::vector<Value<GC>> storage;
    if (delimiter_str.is_empty()) {
        if (this_str.is_empty()) {
            storage.push_back(activation->strings()->empty());
        } else {
            for (size_t i = 0; i < std::min(limit, this_str.len()); ++i) {
                storage.push_back(activation->strings()->make_char(this_str[i]));
            }
        }
    } else {
        // TODO: Implement string split with limit
        auto parts = this_str.split(delimiter_str);
        for (size_t i = 0; i < std::min(limit, parts.size()); ++i) {
            storage.push_back(AvmString<GC>::new_string(activation->gc(), parts[i]));
        }
    }

    return Value<GC>::from_object(ArrayObject<GC>::from_array(activation->context, storage));
}

template<typename GC>
Value<GC> substr(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_str = this_val.coerce_to_string(activation);

    double start_index_f64 = args.size() > 0 ? args[0].as_f64() : 0.0;
    auto start_index = string_wrapping_index(start_index_f64, this_str.len());

    double len_f64 = args.size() > 1 ? args[1].as_f64() : 0.0;
    double len = std::isnan(len_f64) ? 0.0 : std::min(len_f64, 0x7fffffff);

    if (len < 0.0) {
        if (std::isinf(len)) {
            len = 0.0;
        } else if (len <= -1.0) {
            double wrapped = static_cast<double>(this_str.len()) + len;
            if (static_cast<size_t>(wrapped) + start_index >= this_str.len()) {
                return Value<GC>::from_string(AvmString<GC>::empty());
            }
            len = wrapped;
        } else {
            len = static_cast<double>(static_cast<isize>(len));
        }
    }

    size_t end_index = std::min(this_str.len(), start_index + static_cast<size_t>(len));

    return Value<GC>::from_string(activation->strings()->substring(this_str, start_index, end_index));
}

template<typename GC>
Value<GC> substring(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_str = this_val.coerce_to_string(activation);

    double start_index_f64 = args.size() > 0 ? args[0].as_f64() : 0.0;
    size_t start_index = string_index(start_index_f64, this_str.len());

    double end_index_f64 = args.size() > 1 ? args[1].as_f64() : 0.0;
    size_t end_index = string_index(end_index_f64, this_str.len());

    if (end_index < start_index) {
        std::swap(start_index, end_index);
    }

    return Value<GC>::from_string(activation->strings()->substring(this_str, start_index, end_index));
}

template<typename GC>
Value<GC> to_lower_case(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_str = this_val.coerce_to_string(activation);

    string::WString result;
    for (auto c : this_str.iter()) {
        result.push_back(string::utils::swf_to_lowercase(c));
    }

    return Value<GC>::from_string(AvmString<GC>::new_string(activation->gc(), result));
}

template<typename GC>
Value<GC> to_upper_case(
    Activation<GC>* activation,
    Value<GC> this_val,
    const std::vector<Value<GC>>& args
) {
    auto this_str = this_val.coerce_to_string(activation);

    string::WString result;
    for (auto c : this_str.iter()) {
        result.push_back(string::utils::swf_to_uppercase(c));
    }

    return Value<GC>::from_string(AvmString<GC>::new_string(activation->gc(), result));
}

} // namespace string
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_STRING_H
