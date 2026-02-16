// C++ translation of core/src/avm2/parameters.rs
//! Parameter extensions for AVM2 method calls

#ifndef RUFFLE_CORE_AVM2_PARAMETERS_H
#define RUFFLE_CORE_AVM2_PARAMETERS_H

#include <cstdint>
#include <optional>
#include <string_view>
#include <span>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Object;
    template<typename T> class FunctionObject;
    template<typename T> class Error;
    template<typename T> class AvmString;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Extensions over parameters that are passed into AS-defined, Rust-implemented methods.
///
/// It is expected that the AS signature is correct and you only operate on values defined from it.
/// These values will be `expect()`ed to exist, and any method here will panic if they're missing.
///
/// The rules for ActionScript type coercion may be surprising. Here is a table mapping
/// ParametersExt functions to the corresponding ActionScript types:
///
/// get_value: All parameter types work
/// get_f64: Number, int, or uint type
/// get_i32: Number, int, or uint type
/// get_u32: Number, int, or uint type
/// get_bool: Boolean type only
/// get_string and family: String type only
/// get_function and family: Function type only
/// get_object and family: Any non-primitive type; i.e. any type *except* the following:
///   - * (aka "any") type
///   - Object type (as Object can represent any primitive value except undefined)
///   - Boolean type
///   - int type
///   - uint type
///   - Number type
///   - String type
template<typename GC>
class ParametersExt {
private:
    std::span<Value<GC>> values_;

public:
    explicit ParametersExt(std::span<Value<GC>> values) : values_(values) {}

    /// Gets the value at the given index.
    Value<GC> get_value(size_t index) const {
        return values_[index];
    }

    /// Gets the value at the given index, if it exists.
    std::optional<Value<GC>> get_optional(size_t index) const {
        if (index < values_.size()) {
            return values_[index];
        }
        return std::nullopt;
    }

    /// Gets the value at the given index as an Object. It is expected that the
    /// value is either Object or Null.
    ///
    /// If the value is null, a TypeError 2007 is raised.
    ///
    /// @param activation The current activation context
    /// @param index The parameter index
    /// @param name The parameter name for error messages
    /// @return The object value
    /// @throws Error 2007 if the value is null
    Object<GC> get_object(
        Activation<GC>* activation,
        size_t index,
        const char* name
    ) const {
        auto obj = try_get_object(index);
        if (!obj.has_value()) {
            throw make_error_2007(activation, name);
        }
        return obj.value();
    }

    /// Gets the value at the given index as an Object. It is expected that the
    /// value is either Object or Null.
    ///
    /// If the value is null, None is returned.
    ///
    /// @param index The parameter index
    /// @return The object value, or nullopt if null
    std::optional<Object<GC>> try_get_object(size_t index) const {
        auto val = get_value(index);
        if (val.is_null()) {
            return std::nullopt;
        }
        if (val.is_object()) {
            return val.as_object();
        }
        // Panic on unexpected types
        throw std::runtime_error("Expected Object or null as parameter");
    }

    /// Gets the value at the given index as a FunctionObject. It is expected
    /// that the value is either FunctionObject or Null.
    ///
    /// If the value is null, a TypeError 2007 is raised.
    ///
    /// @param activation The current activation context
    /// @param index The parameter index
    /// @param name The parameter name for error messages
    /// @return The function object
    /// @throws Error 2007 if the value is null
    FunctionObject<GC> get_function(
        Activation<GC>* activation,
        size_t index,
        const char* name
    ) const {
        auto func = try_get_function(index);
        if (!func.has_value()) {
            throw make_error_2007(activation, name);
        }
        return func.value();
    }

    /// Gets the value at the given index as a FunctionObject. It is expected
    /// that the value is either FunctionObject or Null.
    ///
    /// If the value is null, None is returned.
    ///
    /// @param index The parameter index
    /// @return The function object, or nullopt if null
    std::optional<FunctionObject<GC>> try_get_function(size_t index) const {
        auto val = get_value(index);
        if (val.is_null()) {
            return std::nullopt;
        }
        if (val.is_object()) {
            auto obj = val.as_object();
            if (obj.is_function_object()) {
                return obj.as_function_object();
            }
        }
        // Panic on unexpected types
        throw std::runtime_error("Expected FunctionObject or null as parameter");
    }

    /// Gets the Number-typed value at the given index. It is expected that the
    /// value is numerical.
    double get_f64(size_t index) const {
        return get_value(index).as_f64();
    }

    /// Gets the uint-typed value at the given index. It is expected that the
    /// value is numerical.
    uint32_t get_u32(size_t index) const {
        return get_value(index).as_u32();
    }

    /// Gets the int-typed value at the given index. It is expected that the
    /// value is numerical.
    int32_t get_i32(size_t index) const {
        return get_value(index).as_i32();
    }

    /// Gets the Boolean-typed value at the given index. It is expected that the
    /// value is of the Boolean type.
    bool get_bool(size_t index) const {
        auto val = get_value(index);
        if (val.is_bool()) {
            return val.as_bool();
        }
        // Panic on unexpected types
        throw std::runtime_error("Expected Boolean-typed parameter");
    }

    /// Gets the String-typed value at the given index. It is expected that the
    /// value is either String or Null.
    ///
    /// If the value is null, None is returned.
    ///
    /// @param index The parameter index
    /// @return The string value, or nullopt if null
    std::optional<AvmString<GC>> try_get_string(size_t index) const {
        auto val = get_value(index);
        if (val.is_null()) {
            return std::nullopt;
        }
        if (val.is_string()) {
            return val.as_string();
        }
        // Panic on unexpected types
        throw std::runtime_error("Expected String-typed parameter");
    }

    /// Like try_get_string, but returns "null" for null values instead
    /// of returning None.
    ///
    /// @param activation The current activation context
    /// @param index The parameter index
    /// @return The string value, or "null" if the value was null
    AvmString<GC> get_string(Activation<GC>* activation, size_t index) const {
        auto str = try_get_string(index);
        if (str.has_value()) {
            return str.value();
        }
        return AvmString<GC>::from_utf8(activation->gc(), "null");
    }

    /// Like try_get_string, but throws TypeError 2007 for null values instead
    /// of returning None.
    ///
    /// @param activation The current activation context
    /// @param index The parameter index
    /// @param name The parameter name for error messages
    /// @return The string value
    /// @throws Error 2007 if the value is null
    AvmString<GC> get_string_non_null(
        Activation<GC>* activation,
        size_t index,
        const char* name
    ) const {
        auto str = try_get_string(index);
        if (!str.has_value()) {
            throw make_error_2007(activation, name);
        }
        return str.value();
    }
};

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_PARAMETERS_H
