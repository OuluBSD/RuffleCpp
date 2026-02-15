/*
 * C++ header for AVM1 value functionality
 * This replaces the functionality of core/src/avm1/value.rs
 */

#ifndef AVM1_VALUE_H
#define AVM1_VALUE_H

#include "avm1.h"
#include "avm1/object.h"
#include "avm1/activation.h"
#include "avm1/error.h"
#include <memory>
#include <string>
#include <variant>
#include <optional>
#include <cmath>
#include <limits>
#include <cstdint>

namespace ruffle {

// Forward declarations
class Object;
class Activation;
class MovieClipReference;

// Enum for value types
enum class ValueType {
    UNDEFINED,
    NULL_VAL,
    BOOLEAN,
    NUMBER,
    STRING,
    OBJECT,
    MOVIE_CLIP
};

// Value class for AVM1
class Value {
private:
    std::variant<std::monostate,        // Undefined
                 std::monostate,        // Null
                 bool,                  // Boolean
                 double,                // Number
                 std::string,           // String
                 std::shared_ptr<Object>, // Object
                 std::shared_ptr<MovieClipReference>> data_; // MovieClip

public:
    // Constructors
    Value() : data_(std::monostate{}) {}  // Undefined by default
    Value(std::monostate) : data_(std::monostate{}) {}  // Undefined
    Value(ValueType type) {
        switch (type) {
            case ValueType::UNDEFINED: data_ = std::monostate{}; break;
            case ValueType::NULL_VAL: data_ = std::monostate{}; break;
            case ValueType::BOOLEAN: data_ = false; break;
            case ValueType::NUMBER: data_ = 0.0; break;
            case ValueType::STRING: data_ = std::string{}; break;
            case ValueType::OBJECT: data_ = std::shared_ptr<Object>{}; break;
            case ValueType::MOVIE_CLIP: data_ = std::shared_ptr<MovieClipReference>{}; break;
        }
    }
    
    explicit Value(bool b) : data_(b) {}
    explicit Value(double d) : data_(d) {}
    explicit Value(const std::string& s) : data_(s) {}
    explicit Value(std::shared_ptr<Object> obj) : data_(std::move(obj)) {}
    explicit Value(std::shared_ptr<MovieClipReference> mc) : data_(std::move(mc)) {}

    // Static constructors for specific types
    static Value undefined() { return Value(std::monostate{}); }
    static Value null() { return Value(); }  // Using second monostate for null
    static Value boolean(bool b) { return Value(b); }
    static Value number(double d) { return Value(d); }
    static Value string(const std::string& s) { return Value(s); }
    static Value object(std::shared_ptr<Object> obj) { return Value(std::move(obj)); }
    static Value movie_clip(std::shared_ptr<MovieClipReference> mc) { return Value(std::move(mc)); }

    // Type checking methods
    bool is_undefined() const { return std::holds_alternative<std::monostate>(data_) && index() == 0; }
    bool is_null() const { return std::holds_alternative<std::monostate>(data_) && index() == 1; }
    bool is_boolean() const { return std::holds_alternative<bool>(data_); }
    bool is_number() const { return std::holds_alternative<double>(data_); }
    bool is_string() const { return std::holds_alternative<std::string>(data_); }
    bool is_object() const { return std::holds_alternative<std::shared_ptr<Object>>(data_); }
    bool is_movie_clip() const { return std::holds_alternative<std::shared_ptr<MovieClipReference>>(data_); }
    bool is_primitive() const { return !is_object() && !is_movie_clip(); }
    bool is_falsy() const { return to_boolean() == false; }

    // Getters
    bool as_bool() const { 
        if (is_boolean()) return std::get<bool>(data_);
        if (is_number()) return std::get<double>(data_) != 0.0 && !std::isnan(std::get<double>(data_));
        if (is_string()) return !std::get<std::string>(data_).empty();
        if (is_null() || is_undefined()) return false;
        return true; // Objects and movie clips are truthy
    }

    double as_number() const {
        if (is_number()) return std::get<double>(data_);
        if (is_boolean()) return std::get<bool>(data_) ? 1.0 : 0.0;
        if (is_string()) {
            try {
                return std::stod(std::get<std::string>(data_));
            } catch (...) {
                return std::numeric_limits<double>::quiet_NaN();
            }
        }
        if (is_null() || is_undefined()) return std::numeric_limits<double>::quiet_NaN();
        return 0.0; // Objects convert to 0
    }

    std::string as_string() const {
        if (is_string()) return std::get<std::string>(data_);
        if (is_number()) {
            double num = std::get<double>(data_);
            if (std::isnan(num)) return "NaN";
            if (std::isinf(num)) return num > 0 ? "Infinity" : "-Infinity";
            if (num == static_cast<int64_t>(num)) {
                return std::to_string(static_cast<int64_t>(num));
            }
            return std::to_string(num);
        }
        if (is_boolean()) return std::get<bool>(data_) ? "true" : "false";
        if (is_null()) return "null";
        if (is_undefined()) return "undefined";
        return "[type Object]"; // Objects convert to this string
    }

    std::shared_ptr<Object> as_object() const {
        if (is_object()) return std::get<std::shared_ptr<Object>>(data_);
        return nullptr;
    }

    std::shared_ptr<MovieClipReference> as_movie_clip() const {
        if (is_movie_clip()) return std::get<std::shared_ptr<MovieClipReference>>(data_);
        return nullptr;
    }

    // Type getter
    ValueType type() const {
        switch (index()) {
            case 0: return is_undefined() ? ValueType::UNDEFINED : ValueType::NULL_VAL;
            case 2: return ValueType::BOOLEAN;
            case 3: return ValueType::NUMBER;
            case 4: return ValueType::STRING;
            case 5: return ValueType::OBJECT;
            case 6: return ValueType::MOVIE_CLIP;
            default: return ValueType::UNDEFINED;
        }
    }

    // Index of the variant (for internal use)
    size_t index() const { return data_.index(); }

    // Equality operator
    bool operator==(const Value& other) const {
        if (index() != other.index()) return false;
        
        if (is_number()) {
            double a = as_number();
            double b = other.as_number();
            // Handle NaN specially (NaN != NaN)
            if (std::isnan(a) && std::isnan(b)) return true;
            if (std::isnan(a) || std::isnan(b)) return false;
            return a == b;
        }
        
        return data_ == other.data_;
    }

    bool operator!=(const Value& other) const {
        return !(*this == other);
    }

    // Coercion methods
    bool to_boolean() const { return as_bool(); }

    double to_number(std::shared_ptr<Activation> activation) const {
        // In a real implementation, this would use the activation for more complex conversions
        return as_number();
    }

    std::string to_string(std::shared_ptr<Activation> activation) const {
        // In a real implementation, this would use the activation for more complex conversions
        return as_string();
    }

    std::shared_ptr<Object> to_object(std::shared_ptr<Activation> activation) const {
        if (is_object()) {
            return as_object();
        } else if (is_null() || is_undefined()) {
            // In a real implementation, this would throw an error
            return nullptr;
        } else {
            // Convert primitive to object wrapper
            // This would create a wrapper object in a real implementation
            return nullptr;
        }
    }

    // Coercion to specific types
    double coerce_to_number(std::shared_ptr<Activation> activation) const {
        return to_number(activation);
    }

    std::string coerce_to_string(std::shared_ptr<Activation> activation) const {
        return to_string(activation);
    }

    std::shared_ptr<Object> coerce_to_object(std::shared_ptr<Activation> activation) const {
        return to_object(activation);
    }

    std::shared_ptr<Object> coerce_to_object_or_bare(std::shared_ptr<Activation> activation) const {
        if (is_object()) {
            return as_object();
        }
        // For non-objects, return a wrapper or the activation's default object
        return activation->context()->global_obj();
    }

    bool coerce_to_boolean(std::shared_ptr<Activation> activation) const {
        return to_boolean();
    }

    // Numeric coercion helpers
    uint8_t coerce_to_u8(std::shared_ptr<Activation> activation) const {
        double val = coerce_to_number(activation);
        if (std::isnan(val) || std::isinf(val)) return 0;
        return static_cast<uint8_t>(static_cast<int64_t>(val) & 0xFF);
    }

    int8_t coerce_to_i8(std::shared_ptr<Activation> activation) const {
        double val = coerce_to_number(activation);
        if (std::isnan(val) || std::isinf(val)) return 0;
        return static_cast<int8_t>(static_cast<int64_t>(val) & 0xFF);
    }

    uint16_t coerce_to_u16(std::shared_ptr<Activation> activation) const {
        double val = coerce_to_number(activation);
        if (std::isnan(val) || std::isinf(val)) return 0;
        return static_cast<uint16_t>(static_cast<int64_t>(val) & 0xFFFF);
    }

    int16_t coerce_to_i16(std::shared_ptr<Activation> activation) const {
        double val = coerce_to_number(activation);
        if (std::isnan(val) || std::isinf(val)) return 0;
        return static_cast<int16_t>(static_cast<int64_t>(val) & 0xFFFF);
    }

    uint32_t coerce_to_u32(std::shared_ptr<Activation> activation) const {
        double val = coerce_to_number(activation);
        if (std::isnan(val) || std::isinf(val)) return 0;
        return static_cast<uint32_t>(static_cast<int64_t>(val));
    }

    int32_t coerce_to_i32(std::shared_ptr<Activation> activation) const {
        double val = coerce_to_number(activation);
        if (std::isnan(val) || std::isinf(val)) return 0;
        return static_cast<int32_t>(static_cast<int64_t>(val));
    }

    // Constants
    static const Value UNDEFINED;
    static const Value NULL_VAL;
};

// Define static constants
const Value Value::UNDEFINED = Value(std::monostate{});
const Value Value::NULL_VAL = Value(); // Using the null constructor

// Helper function to convert float to string in AVM1 style
inline std::string f64_to_string(std::shared_ptr<Activation> activation, double value) {
    if (std::isnan(value)) return "NaN";
    if (std::isinf(value)) return value > 0 ? "Infinity" : "-Infinity";
    
    // Format the number according to AVM1 rules
    // This is a simplified version - a full implementation would handle
    // all the edge cases mentioned in the original Rust tests
    if (value == static_cast<long long>(value)) {
        return std::to_string(static_cast<long long>(value));
    } else {
        // Use scientific notation for very large or very small numbers
        if (value > 1e15 || (value > 0 && value < 1e-5) || (value < 0 && value > -1e-5)) {
            char buffer[32];
            snprintf(buffer, sizeof(buffer), "%.6g", value);
            return std::string(buffer);
        } else {
            return std::to_string(value);
        }
    }
}

} // namespace ruffle

#endif // AVM1_VALUE_H