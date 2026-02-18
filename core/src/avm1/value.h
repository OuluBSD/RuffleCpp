// C++ translation of core/src/avm1/value.rs
//! AVM1 Value type and coercions

#ifndef RUFFLE_CORE_AVM1_VALUE_H
#define RUFFLE_CORE_AVM1_VALUE_H

#include <variant>
#include <string>
#include <cstdint>
#include <cmath>
#include <optional>
#include <memory>

// Forward declarations
namespace ruffle {
namespace core {

namespace string {
    class WStr;
    template<typename GC>
    class AvmString;
}

namespace avm1 {

template<typename GC>
class Object;
template<typename GC>
class Activation;
template<typename GC>
class Error;

namespace object_reference {
    template<typename GC>
    class MovieClipReference;
}

namespace function {
    enum class ExecutionReason;
}

namespace object {
    template<typename GC>
    class NativeObject;
}

namespace display_object {
    template<typename GC>
    class TDisplayObject;
}

} // namespace avm1

namespace swf {
    enum class BlendMode;
}

namespace ecma_conversions {
    uint8_t f64_to_wrapping_u8(double value);
    uint16_t f64_to_wrapping_u16(double value);
    int16_t f64_to_wrapping_i16(double value);
    int32_t f64_to_wrapping_i32(double value);
    uint32_t f64_to_wrapping_u32(double value);
}

} // namespace core
} // namespace ruffle

namespace ruffle {
namespace core {
namespace avm1 {

using string::AvmString;
using string::WStr;
using object_reference::MovieClipReference;

// ============================================================================
// Value - AVM1 value type
// ============================================================================

/// AVM1 Value type representing any ActionScript value.
/// This type is used very frequently, so it's kept small (<= 16 bytes).
template<typename GC>
class Value {
public:
    /// Underlying value variants
    struct UndefinedTag {};
    struct NullTag {};

private:
    std::variant<
        UndefinedTag,
        NullTag,
        bool,
        double,
        AvmString<GC>,
        Object<GC>,
        MovieClipReference<GC>
    > data_;

public:
    // ========================================================================
    // Constructors
    // ========================================================================

    Value() : data_(UndefinedTag{}) {}
    Value(UndefinedTag) : data_(UndefinedTag{}) {}
    Value(NullTag) : data_(NullTag{}) {}
    Value(bool value) : data_(value) {}
    Value(double value) : data_(value) {}
    Value(float value) : data_(static_cast<double>(value)) {}
    Value(int8_t value) : data_(static_cast<double>(value)) {}
    Value(uint8_t value) : data_(static_cast<double>(value)) {}
    Value(int16_t value) : data_(static_cast<double>(value)) {}
    Value(uint16_t value) : data_(static_cast<double>(value)) {}
    Value(int32_t value) : data_(static_cast<double>(value)) {}
    Value(uint32_t value) : data_(static_cast<double>(value)) {}
    Value(int64_t value) : data_(static_cast<double>(value)) {}
    Value(uint64_t value) : data_(static_cast<double>(value)) {}
    Value(AvmString<GC> string) : data_(std::move(string)) {}
    Value(Object<GC> object) : data_(std::move(object)) {}
    Value(MovieClipReference<GC> clip) : data_(std::move(clip)) {}

    // ========================================================================
    // Static factory methods
    // ========================================================================

    static Value<GC> undefined() { return Value<GC>(); }
    static Value<GC> null() { return Value<GC>(NullTag{}); }
    static Value<GC> from_bool(bool value) { return Value<GC>(value); }
    static Value<GC> from_number(double value) { return Value<GC>(value); }
    static Value<GC> from_string(AvmString<GC> string) { return Value<GC>(std::move(string)); }
    static Value<GC> from_object(Object<GC> object) { return Value<GC>(std::move(object)); }

    // ========================================================================
    // Type checks
    // ========================================================================

    bool is_undefined() const { return std::holds_alternative<UndefinedTag>(data_); }
    bool is_null() const { return std::holds_alternative<NullTag>(data_); }
    bool is_bool() const { return std::holds_alternative<bool>(data_); }
    bool is_number() const { return std::holds_alternative<double>(data_); }
    bool is_string() const { return std::holds_alternative<AvmString<GC>>(data_); }
    bool is_object() const { return std::holds_alternative<Object<GC>>(data_); }
    bool is_movie_clip() const { return std::holds_alternative<MovieClipReference<GC>>(data_); }

    /// Returns true if the value is a primitive (not an object or movie clip).
    bool is_primitive() const {
        return !is_object() && !is_movie_clip();
    }

    // ========================================================================
    // Accessors
    // ========================================================================

    std::optional<bool> as_bool() const {
        if (auto* v = std::get_if<bool>(&data_)) return *v;
        return std::nullopt;
    }

    std::optional<double> as_f64() const {
        if (auto* v = std::get_if<double>(&data_)) return *v;
        return std::nullopt;
    }

    std::optional<AvmString<GC>> as_string() const {
        if (auto* v = std::get_if<AvmString<GC>>(&data_)) return *v;
        return std::nullopt;
    }

    std::optional<Object<GC>> as_object() const {
        if (auto* v = std::get_if<Object<GC>>(&data_)) return *v;
        return std::nullopt;
    }

    std::optional<MovieClipReference<GC>> as_movie_clip() const {
        if (auto* v = std::get_if<MovieClipReference<GC>>(&data_)) return *v;
        return std::nullopt;
    }

    // ========================================================================
    // Coercions
    // ========================================================================

    /// Coerce to f64 (ToNumber)
    double coerce_to_f64(Activation<GC>* activation) const;

    /// Coerce to u8 with wrapping
    uint8_t coerce_to_u8(Activation<GC>* activation) const {
        return ecma_conversions::f64_to_wrapping_u8(coerce_to_f64(activation));
    }

    /// Coerce to u16 with wrapping (ToUInt16)
    uint16_t coerce_to_u16(Activation<GC>* activation) const {
        return ecma_conversions::f64_to_wrapping_u16(coerce_to_f64(activation));
    }

    /// Coerce to i16 with wrapping
    int16_t coerce_to_i16(Activation<GC>* activation) const {
        return ecma_conversions::f64_to_wrapping_i16(coerce_to_f64(activation));
    }

    /// Coerce to i32 with wrapping (ToInt32)
    int32_t coerce_to_i32(Activation<GC>* activation) const {
        return ecma_conversions::f64_to_wrapping_i32(coerce_to_f64(activation));
    }

    /// Coerce to u32 with wrapping (ToUInt32)
    uint32_t coerce_to_u32(Activation<GC>* activation) const {
        return ecma_conversions::f64_to_wrapping_u32(coerce_to_f64(activation));
    }

    /// Coerce to string (ToString)
    AvmString<GC> coerce_to_string(Activation<GC>* activation) const;

    /// Coerce to object, boxing primitives if necessary
    std::optional<Object<GC>> coerce_to_object(Activation<GC>* activation) const;

    /// Coerce to object or return a fresh bare object
    Object<GC> coerce_to_object_or_bare(Activation<GC>* activation) const;

    /// Convert to script object if possible (no coercion)
    std::optional<Object<GC>> as_object_ref(Activation<GC>* activation) const;

    /// Convert to boolean (ToBoolean)
    bool as_bool_value(uint8_t swf_version) const;

    /// Get blend mode from value
    std::optional<swf::BlendMode> as_blend_mode() const;

    // ========================================================================
    // Operators
    // ========================================================================

    /// ActionScript 2's typeof operator
    AvmString<GC> type_of(Activation<GC>* activation) const;

    /// Abstract less-than comparison (<)
    Value<GC> abstract_lt(const Value<GC>& other, Activation<GC>* activation) const;

    /// Abstract equality comparison (==)
    bool abstract_eq(const Value<GC>& other, Activation<GC>* activation) const;

    /// instanceof operator
    bool instance_of(const Value<GC>& class_val, Activation<GC>* activation) const;

    // ========================================================================
    // Equality
    // ========================================================================

    bool operator==(const Value<GC>& other) const {
        return data_ == other.data_;
    }

    bool operator!=(const Value<GC>& other) const {
        return !(*this == other);
    }
};

// ============================================================================
// Helper Functions
// ============================================================================

/// Calculate value * 10^exp through repeated multiplication or division.
inline double decimal_shift(double value, int32_t exp) {
    double base = 10.0;
    if (exp > 0) {
        while (exp > 0) {
            if (exp & 1) value *= base;
            exp >>= 1;
            base *= base;
        }
    } else {
        uint32_t uexp = static_cast<uint32_t>(-exp);
        while (uexp > 0) {
            if (uexp & 1) value /= base;
            uexp >>= 1;
            base *= base;
        }
    }
    return value;
}

/// Converts a double to string with Flash AVM1 behavior.
template<typename GC>
AvmString<GC> f64_to_string(Activation<GC>* activation, double n);

/// Converts a string to f64 based on SWF version.
double string_to_f64(const WStr* s, uint8_t swf_version);

/// Parse float from string (strict mode controls trailing garbage handling).
double parse_float_impl(const WStr* s, bool strict);

// ============================================================================
// Template Implementations
// ============================================================================

template<typename GC>
double Value<GC>::coerce_to_f64(Activation<GC>* activation) const {
    // Simplified implementation - full version would handle object coercion
    if (is_number()) {
        return *as_f64();
    } else if (is_bool()) {
        return *as_bool() ? 1.0 : 0.0;
    } else if (is_string()) {
        return string_to_f64(as_string()->as_wstr(), activation->swf_version());
    } else if (is_undefined() || is_null()) {
        return activation->swf_version() < 7 ? 0.0 : std::numeric_limits<double>::quiet_NaN();
    }
    return std::numeric_limits<double>::quiet_NaN();
}

template<typename GC>
AvmString<GC> Value<GC>::coerce_to_string(Activation<GC>* activation) const {
    if (is_string()) {
        return *as_string();
    } else if (is_bool()) {
        return *as_bool() ? AvmString<GC>::from_utf8(activation->gc(), "true")
                          : AvmString<GC>::from_utf8(activation->gc(), "false");
    } else if (is_number()) {
        return f64_to_string(activation, *as_f64());
    } else if (is_undefined()) {
        return AvmString<GC>::from_utf8(activation->gc(), "undefined");
    } else if (is_null()) {
        return AvmString<GC>::from_utf8(activation->gc(), "null");
    }
    // Objects would call toString() here
    return AvmString<GC>::from_utf8(activation->gc(), "[object Object]");
}

template<typename GC>
bool Value<GC>::as_bool_value(uint8_t swf_version) const {
    if (is_bool()) {
        return *as_bool();
    } else if (is_number()) {
        double n = *as_f64();
        return !std::isnan(n) && n != 0.0;
    } else if (is_string()) {
        if (swf_version >= 7) {
            return !as_string()->is_empty();
        } else {
            double n = string_to_f64(as_string()->as_wstr(), swf_version);
            return !std::isnan(n) && n != 0.0;
        }
    } else if (is_object() || is_movie_clip()) {
        return true;
    }
    return false;
}

template<typename GC>
AvmString<GC> Value<GC>::type_of(Activation<GC>* activation) const {
    if (is_undefined()) {
        return AvmString<GC>::from_utf8(activation->gc(), "undefined");
    } else if (is_null()) {
        return AvmString<GC>::from_utf8(activation->gc(), "null");
    } else if (is_number()) {
        return AvmString<GC>::from_utf8(activation->gc(), "number");
    } else if (is_bool()) {
        return AvmString<GC>::from_utf8(activation->gc(), "boolean");
    } else if (is_string()) {
        return AvmString<GC>::from_utf8(activation->gc(), "string");
    } else if (is_movie_clip()) {
        return AvmString<GC>::from_utf8(activation->gc(), "movieclip");
    } else if (is_object()) {
        // Would check if it's a function
        return AvmString<GC>::from_utf8(activation->gc(), "object");
    }
    return AvmString<GC>::from_utf8(activation->gc(), "unknown");
}

template<typename GC>
Value<GC> Value<GC>::abstract_lt(const Value<GC>& other, Activation<GC>* activation) const {
    // Simplified implementation
    double a = coerce_to_f64(activation);
    double b = other.coerce_to_f64(activation);

    if (std::isnan(a) || std::isnan(b)) {
        return Value<GC>::undefined();
    }

    return Value<GC>(a < b);
}

template<typename GC>
bool Value<GC>::abstract_eq(const Value<GC>& other, Activation<GC>* activation) const {
    if (is_undefined() && other.is_undefined()) return true;
    if (is_null() && other.is_null()) return true;
    if (is_bool() && other.is_bool()) return *as_bool() == *other.as_bool();
    if (is_string() && other.is_string()) return *as_string() == *other.as_string();
    if (is_number() && other.is_number()) {
        double a = *as_f64();
        double b = *other.as_f64();
        return a == b || (std::isnan(a) && std::isnan(b));
    }
    // Object comparison would use pointer equality
    return false;
}

template<typename GC>
bool Value<GC>::instance_of(const Value<GC>& class_val, Activation<GC>* activation) const {
    if (!is_primitive()) {
        // Would check prototype chain
        return false;
    }
    return false;
}

template<typename GC>
std::optional<Object<GC>> Value<GC>::as_object_ref(Activation<GC>* activation) const {
    if (is_object()) {
        return as_object();
    } else if (is_movie_clip()) {
        // Would coerce movie clip to object
        return std::nullopt;
    }
    return std::nullopt;
}

template<typename GC>
Object<GC> Value<GC>::coerce_to_object_or_bare(Activation<GC>* activation) const {
    if (auto obj = coerce_to_object(activation)) {
        return *obj;
    }
    // Return fresh bare object
    return Object<GC>::new_without_proto(activation->gc());
}

template<typename GC>
std::optional<Object<GC>> Value<GC>::coerce_to_object(Activation<GC>* activation) const {
    if (is_object()) {
        return as_object();
    } else if (is_movie_clip()) {
        // Would coerce movie clip
        return std::nullopt;
    } else if (is_undefined() || is_null()) {
        return std::nullopt;
    }
    // Would box primitive using constructor
    return std::nullopt;
}

template<typename GC>
AvmString<GC> f64_to_string(Activation<GC>* activation, double n) {
    if (std::isnan(n)) {
        return AvmString<GC>::from_utf8(activation->gc(), "NaN");
    } else if (n == std::numeric_limits<double>::infinity()) {
        return AvmString<GC>::from_utf8(activation->gc(), "Infinity");
    } else if (n == std::numeric_limits<double>::neg_infinity()) {
        return AvmString<GC>::from_utf8(activation->gc(), "-Infinity");
    } else if (n == 0.0) {
        return AvmString<GC>::from_utf8(activation->gc(), "0");
    } else if (n >= -2147483648.0 && n <= 2147483647.0 && std::floor(n) == n) {
        // Fast path for integers
        int32_t i = static_cast<int32_t>(n);
        return AvmString<GC>::from_utf8(activation->gc(), std::to_string(i));
    }
    // Full implementation would handle exponential notation
    return AvmString<GC>::from_utf8(activation->gc(), std::to_string(n));
}

} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_VALUE_H
