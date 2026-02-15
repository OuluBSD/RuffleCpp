/*
 * C++ header for AVM1 parameters functionality
 * This replaces the functionality of core/src/avm1/parameters.rs
 */

#ifndef AVM1_PARAMETERS_H
#define AVM1_PARAMETERS_H

#include "avm1.h"
#include "avm1/value.h"
#include "avm1/object.h"
#include "avm1/activation.h"
#include "avm1/error.h"
#include <vector>
#include <optional>

namespace ruffle {

// Enum to control the behavior of explicit `undefined` when passed to parameter methods
enum class UndefinedAs {
    // An explicit `undefined` will be treated the same as an implicit `undefined` - the result will be `None`.
    NONE,
    // An explicit `undefined` will be treated as if the value did exist - the result will be `Some`.
    SOME
};

// Interface for parameter extension functionality
class ParametersExt {
public:
    // Virtual destructor for interface
    virtual ~ParametersExt() = default;

    // Gets the value at the given index.
    // If the value does not exist, it will return Undefined.
    virtual std::shared_ptr<Value> get_value(size_t index) const = 0;

    // Gets the value at the given index, if it exists.
    virtual std::optional<std::shared_ptr<Value>> get_optional(size_t index) const = 0;

    // Gets the value at the given index as an Object.
    // The value will be coerced to an Object, even if it's undefined/missing.
    std::shared_ptr<Object> get_object(std::shared_ptr<Activation> activation, size_t index) const {
        auto value = get_value(index);
        return value->coerce_to_object_or_bare(activation);
    }

    // Tries to get the value at the given index as an Object.
    // The value will be coerced to an Object if it exists and is coercible.
    std::optional<std::shared_ptr<Object>> try_get_object(
        std::shared_ptr<Activation> activation, size_t index) const {
        
        auto opt_value = get_optional(index);
        if (opt_value.has_value() && opt_value.value()) {
            auto obj = opt_value.value()->coerce_to_object(activation);
            return obj;
        }
        return std::nullopt;
    }

    // Get the value at the given index as a String.
    // The value will be coerced to a String, even if it's undefined/missing.
    std::string get_string(std::shared_ptr<Activation> activation, size_t index) const {
        auto value = get_value(index);
        return value->coerce_to_string(activation);
    }

    // Tries to get the value at the given index as a String.
    // The value will be coerced to a String if it exists.
    std::optional<std::string> try_get_string(
        std::shared_ptr<Activation> activation, 
        size_t index, 
        UndefinedAs undefined_behavior) const {
        
        auto opt_value = get_optional(index);
        if (opt_value.has_value() && opt_value.value()) {
            if (undefined_behavior == UndefinedAs::NONE && 
                *opt_value.value() == Value::UNDEFINED) {
                return std::nullopt;
            }
            auto str = opt_value.value()->coerce_to_string(activation);
            return str;
        }
        return std::nullopt;
    }

    // Get the value at the given index as a bool.
    // The value will be coerced to a bool, even if it's undefined/missing.
    bool get_bool(std::shared_ptr<Activation> activation, size_t index) const {
        auto value = get_value(index);
        return value->as_bool(activation->swf_version());
    }

    // Tries to get the value at the given index as a bool.
    // The value will be coerced to a bool if it exists.
    std::optional<bool> try_get_bool(
        std::shared_ptr<Activation> activation, 
        size_t index, 
        UndefinedAs undefined_behavior) const {
        
        auto opt_value = get_optional(index);
        if (opt_value.has_value() && opt_value.value()) {
            if (undefined_behavior == UndefinedAs::NONE && 
                *opt_value.value() == Value::UNDEFINED) {
                return std::nullopt;
            }
            bool result = opt_value.value()->as_bool(activation->swf_version());
            return result;
        }
        return std::nullopt;
    }

    // Gets the value at the given index as a u16.
    uint16_t get_u16(std::shared_ptr<Activation> activation, size_t index) const {
        auto value = get_value(index);
        return value->coerce_to_u16(activation);
    }

    // Gets the value at the given index as a u16 if it exists.
    std::optional<uint16_t> try_get_u16(
        std::shared_ptr<Activation> activation, 
        size_t index, 
        UndefinedAs undefined_behavior) const {
        
        auto opt_value = get_optional(index);
        if (opt_value.has_value() && opt_value.value()) {
            if (undefined_behavior == UndefinedAs::NONE && 
                *opt_value.value() == Value::UNDEFINED) {
                return std::nullopt;
            }
            uint16_t result = opt_value.value()->coerce_to_u16(activation);
            return result;
        }
        return std::nullopt;
    }

    // Gets the value at the given index as an i16.
    int16_t get_i16(std::shared_ptr<Activation> activation, size_t index) const {
        auto value = get_value(index);
        return value->coerce_to_i16(activation);
    }

    // Gets the value at the given index as an i16 if it exists.
    std::optional<int16_t> try_get_i16(
        std::shared_ptr<Activation> activation, 
        size_t index, 
        UndefinedAs undefined_behavior) const {
        
        auto opt_value = get_optional(index);
        if (opt_value.has_value() && opt_value.value()) {
            if (undefined_behavior == UndefinedAs::NONE && 
                *opt_value.value() == Value::UNDEFINED) {
                return std::nullopt;
            }
            int16_t result = opt_value.value()->coerce_to_i16(activation);
            return result;
        }
        return std::nullopt;
    }

    // Gets the value at the given index as a u8.
    uint8_t get_u8(std::shared_ptr<Activation> activation, size_t index) const {
        auto value = get_value(index);
        return value->coerce_to_u8(activation);
    }

    // Gets the value at the given index as a u8 if it exists.
    std::optional<uint8_t> try_get_u8(
        std::shared_ptr<Activation> activation, 
        size_t index, 
        UndefinedAs undefined_behavior) const {
        
        auto opt_value = get_optional(index);
        if (opt_value.has_value() && opt_value.value()) {
            if (undefined_behavior == UndefinedAs::NONE && 
                *opt_value.value() == Value::UNDEFINED) {
                return std::nullopt;
            }
            uint8_t result = opt_value.value()->coerce_to_u8(activation);
            return result;
        }
        return std::nullopt;
    }

    // Gets the value at the given index as an i32.
    int32_t get_i32(std::shared_ptr<Activation> activation, size_t index) const {
        auto value = get_value(index);
        return value->coerce_to_i32(activation);
    }

    // Gets the value at the given index as an i32 if it exists.
    std::optional<int32_t> try_get_i32(
        std::shared_ptr<Activation> activation, 
        size_t index, 
        UndefinedAs undefined_behavior) const {
        
        auto opt_value = get_optional(index);
        if (opt_value.has_value() && opt_value.value()) {
            if (undefined_behavior == UndefinedAs::NONE && 
                *opt_value.value() == Value::UNDEFINED) {
                return std::nullopt;
            }
            int32_t result = opt_value.value()->coerce_to_i32(activation);
            return result;
        }
        return std::nullopt;
    }

    // Gets the value at the given index as a u32.
    uint32_t get_u32(std::shared_ptr<Activation> activation, size_t index) const {
        auto value = get_value(index);
        return value->coerce_to_u32(activation);
    }

    // Gets the value at the given index as a u32 if it exists.
    std::optional<uint32_t> try_get_u32(
        std::shared_ptr<Activation> activation, 
        size_t index, 
        UndefinedAs undefined_behavior) const {
        
        auto opt_value = get_optional(index);
        if (opt_value.has_value() && opt_value.value()) {
            if (undefined_behavior == UndefinedAs::NONE && 
                *opt_value.value() == Value::UNDEFINED) {
                return std::nullopt;
            }
            uint32_t result = opt_value.value()->coerce_to_u32(activation);
            return result;
        }
        return std::nullopt;
    }

    // Gets the value at the given index as an f64.
    double get_f64(std::shared_ptr<Activation> activation, size_t index) const {
        auto value = get_value(index);
        return value->coerce_to_number(activation);
    }

    // Gets the value at the given index as an f64 if it exists.
    std::optional<double> try_get_f64(
        std::shared_ptr<Activation> activation, 
        size_t index, 
        UndefinedAs undefined_behavior) const {
        
        auto opt_value = get_optional(index);
        if (opt_value.has_value() && opt_value.value()) {
            if (undefined_behavior == UndefinedAs::NONE && 
                *opt_value.value() == Value::UNDEFINED) {
                return std::nullopt;
            }
            double result = opt_value.value()->coerce_to_number(activation);
            return result;
        }
        return std::nullopt;
    }
};

// Concrete implementation of ParametersExt for a vector of values
class VectorParameters : public ParametersExt {
private:
    std::vector<std::shared_ptr<Value>> values_;

public:
    explicit VectorParameters(std::vector<std::shared_ptr<Value>> values)
        : values_(std::move(values)) {}

    // Gets the value at the given index.
    // If the value does not exist, it will return Undefined.
    std::shared_ptr<Value> get_value(size_t index) const override {
        if (index < values_.size()) {
            return values_[index];
        }
        return std::make_shared<Value>(Value::UNDEFINED);
    }

    // Gets the value at the given index, if it exists.
    std::optional<std::shared_ptr<Value>> get_optional(size_t index) const override {
        if (index < values_.size()) {
            return values_[index];
        }
        return std::nullopt;
    }

    // Static factory method
    static std::shared_ptr<VectorParameters> create(const std::vector<std::shared_ptr<Value>>& values) {
        return std::make_shared<VectorParameters>(values);
    }
};

} // namespace ruffle

#endif // AVM1_PARAMETERS_H