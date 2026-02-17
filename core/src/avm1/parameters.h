// C++ translation of core/src/avm1/parameters.rs
//! Parameter extension traits for AVM1

#ifndef RUFFLE_CORE_AVM1_PARAMETERS_H
#define RUFFLE_CORE_AVM1_PARAMETERS_H

#include <cstdint>
#include <optional>
#include <span>
#include <format>

// Forward declarations
namespace ruffle {
namespace core {
    namespace avm1 {
        template<typename T> class Activation;
        template<typename T> class Error;
        template<typename T> class Object;
        template<typename T> class Value;
    }
    namespace string {
        class AvmString;
    }
}
}

namespace ruffle {
namespace core {
namespace avm1 {

/// Controls the behavior of explicit `undefined` when passed to a `try_get_*` method.
enum class UndefinedAs {
    /// An explicit `undefined` will be treated the same as an implicit `undefined` - the result will be `None`.
    None,

    /// An explicit `undefined` will be treated as if the value did exist - the result will be `Some`.
    Some
};

/// ParametersExt - Extension trait for parameter access
///
/// Provides methods for accessing and coercing parameter values in AVM1.
/// This is implemented for std::span<Value<GC>> to provide parameter access.
template<typename GC>
class ParametersExt {
public:
    ParametersExt() = delete;
    explicit ParametersExt(std::span<Value<GC>> values) : values_(values) {}

    /// Gets the value at the given index.
    /// If the value does not exist, it will return Undefined.
    Value<GC> get_value(size_t index) const {
        if (index < values_.size()) {
            return values_[index];
        }
        return Value<GC>::undefined();
    }

    /// Gets the value at the given index, if it exists.
    std::optional<Value<GC>> get_optional(size_t index) const {
        if (index < values_.size()) {
            return std::make_optional(values_[index]);
        }
        return std::nullopt;
    }

    /// Gets the value at the given index as an Object.
    /// The value will be coerced to an Object, even if it's undefined/missing.
    Result<Object<GC>, Error<GC>> get_object(
        Activation<GC>* activation,
        size_t index
    ) const {
        return get_value(index).coerce_to_object_or_bare(activation);
    }

    /// Tries to get the value at the given index as an Object.
    /// The value will be coerced to an Object if it exists and is coercible.
    Result<std::optional<Object<GC>>, Error<GC>> try_get_object(
        Activation<GC>* activation,
        size_t index
    ) const {
        if (auto value = get_optional(index)) {
            return value->coerce_to_object(activation);
        }
        return std::make_optional<Object<GC>>(std::nullopt);
    }

    /// Get the value at the given index as a String.
    /// The value will be coerced to a String, even if it's undefined/missing.
    Result<string::AvmString, Error<GC>> get_string(
        Activation<GC>* activation,
        size_t index
    ) const {
        return get_value(index).coerce_to_string(activation);
    }

    /// Tries to get the value at the given index as a String.
    /// The value will be coerced to a String if it exists.
    Result<std::optional<string::AvmString>, Error<GC>> try_get_string(
        Activation<GC>* activation,
        size_t index,
        UndefinedAs undefined_behaviour
    ) const {
        if (auto value = get_optional(index)) {
            if (undefined_behaviour == UndefinedAs::None && *value == Value<GC>::undefined()) {
                return std::make_optional<string::AvmString>(std::nullopt);
            }
            return value->coerce_to_string(activation);
        }
        return std::make_optional<string::AvmString>(std::nullopt);
    }

    /// Get the value at the given index as a bool.
    /// The value will be coerced to a bool, even if it's undefined/missing.
    bool get_bool(Activation<GC>* activation, size_t index) const {
        return get_value(index).as_bool(activation->swf_version());
    }

    /// Tries to get the value at the given index as a bool.
    /// The value will be coerced to a bool if it exists.
    std::optional<bool> try_get_bool(
        Activation<GC>* activation,
        size_t index,
        UndefinedAs undefined_behaviour
    ) const {
        if (auto value = get_optional(index)) {
            if (undefined_behaviour == UndefinedAs::None && *value == Value<GC>::undefined()) {
                return std::nullopt;
            }
            return std::make_optional(value->as_bool(activation->swf_version()));
        }
        return std::nullopt;
    }

    /// Gets the value at the given index as an u16.
    /// The value will be coerced to an u16, even if it's undefined/missing.
    Result<uint16_t, Error<GC>> get_u16(
        Activation<GC>* activation,
        size_t index
    ) const {
        return get_value(index).coerce_to_u16(activation);
    }

    /// Gets the value at the given index as an u16.
    /// The value will be coerced to an u16 if it exists.
    Result<std::optional<uint16_t>, Error<GC>> try_get_u16(
        Activation<GC>* activation,
        size_t index,
        UndefinedAs undefined_behaviour
    ) const {
        if (auto value = get_optional(index)) {
            if (undefined_behaviour == UndefinedAs::None && *value == Value<GC>::undefined()) {
                return std::make_optional<uint16_t>(std::nullopt);
            }
            auto result = value->coerce_to_u16(activation);
            if (result.has_value()) {
                return std::make_optional(result.value());
            }
            return result;
        }
        return std::make_optional<uint16_t>(std::nullopt);
    }

    /// Gets the value at the given index as an i16.
    /// The value will be coerced to an i16, even if it's undefined/missing.
    Result<int16_t, Error<GC>> get_i16(
        Activation<GC>* activation,
        size_t index
    ) const {
        return get_value(index).coerce_to_i16(activation);
    }

    /// Gets the value at the given index as an i16.
    /// The value will be coerced to an i16 if it exists.
    Result<std::optional<int16_t>, Error<GC>> try_get_i16(
        Activation<GC>* activation,
        size_t index,
        UndefinedAs undefined_behaviour
    ) const {
        if (auto value = get_optional(index)) {
            if (undefined_behaviour == UndefinedAs::None && *value == Value<GC>::undefined()) {
                return std::make_optional<int16_t>(std::nullopt);
            }
            auto result = value->coerce_to_i16(activation);
            if (result.has_value()) {
                return std::make_optional(result.value());
            }
            return result;
        }
        return std::make_optional<int16_t>(std::nullopt);
    }

    /// Gets the value at the given index as an u8.
    /// The value will be coerced to an u8, even if it's undefined/missing.
    Result<uint8_t, Error<GC>> get_u8(
        Activation<GC>* activation,
        size_t index
    ) const {
        return get_value(index).coerce_to_u8(activation);
    }

    /// Gets the value at the given index as an u8.
    /// The value will be coerced to an u8 if it exists.
    Result<std::optional<uint8_t>, Error<GC>> try_get_u8(
        Activation<GC>* activation,
        size_t index,
        UndefinedAs undefined_behaviour
    ) const {
        if (auto value = get_optional(index)) {
            if (undefined_behaviour == UndefinedAs::None && *value == Value<GC>::undefined()) {
                return std::make_optional<uint8_t>(std::nullopt);
            }
            auto result = value->coerce_to_u8(activation);
            if (result.has_value()) {
                return std::make_optional(result.value());
            }
            return result;
        }
        return std::make_optional<uint8_t>(std::nullopt);
    }

    /// Gets the value at the given index as an i32.
    /// The value will be coerced to an i32, even if it's undefined/missing.
    Result<int32_t, Error<GC>> get_i32(
        Activation<GC>* activation,
        size_t index
    ) const {
        return get_value(index).coerce_to_i32(activation);
    }

    /// Gets the value at the given index as an i32.
    /// The value will be coerced to an i32 if it exists.
    Result<std::optional<int32_t>, Error<GC>> try_get_i32(
        Activation<GC>* activation,
        size_t index,
        UndefinedAs undefined_behaviour
    ) const {
        if (auto value = get_optional(index)) {
            if (undefined_behaviour == UndefinedAs::None && *value == Value<GC>::undefined()) {
                return std::make_optional<int32_t>(std::nullopt);
            }
            auto result = value->coerce_to_i32(activation);
            if (result.has_value()) {
                return std::make_optional(result.value());
            }
            return result;
        }
        return std::make_optional<int32_t>(std::nullopt);
    }

    /// Gets the value at the given index as an u32.
    /// The value will be coerced to an u32, even if it's undefined/missing.
    Result<uint32_t, Error<GC>> get_u32(
        Activation<GC>* activation,
        size_t index
    ) const {
        return get_value(index).coerce_to_u32(activation);
    }

    /// Gets the value at the given index as an u32.
    /// The value will be coerced to an u32 if it exists.
    Result<std::optional<uint32_t>, Error<GC>> try_get_u32(
        Activation<GC>* activation,
        size_t index,
        UndefinedAs undefined_behaviour
    ) const {
        if (auto value = get_optional(index)) {
            if (undefined_behaviour == UndefinedAs::None && *value == Value<GC>::undefined()) {
                return std::make_optional<uint32_t>(std::nullopt);
            }
            auto result = value->coerce_to_u32(activation);
            if (result.has_value()) {
                return std::make_optional(result.value());
            }
            return result;
        }
        return std::make_optional<uint32_t>(std::nullopt);
    }

    /// Gets the value at the given index as an f64.
    /// The value will be coerced to an f64, even if it's undefined/missing.
    Result<double, Error<GC>> get_f64(
        Activation<GC>* activation,
        size_t index
    ) const {
        return get_value(index).coerce_to_f64(activation);
    }

    /// Gets the value at the given index as an f64.
    /// The value will be coerced to an f64 if it exists.
    Result<std::optional<double>, Error<GC>> try_get_f64(
        Activation<GC>* activation,
        size_t index,
        UndefinedAs undefined_behaviour
    ) const {
        if (auto value = get_optional(index)) {
            if (undefined_behaviour == UndefinedAs::None && *value == Value<GC>::undefined()) {
                return std::make_optional<double>(std::nullopt);
            }
            auto result = value->coerce_to_f64(activation);
            if (result.has_value()) {
                return std::make_optional(result.value());
            }
            return result;
        }
        return std::make_optional<double>(std::nullopt);
    }

private:
    std::span<Value<GC>> values_;
};

} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_PARAMETERS_H
