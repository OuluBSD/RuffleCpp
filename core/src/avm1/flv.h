// C++ translation of core/src/avm1/flv.rs
// FLV value extensions for AVM1

#ifndef RUFFLE_CORE_AVM1_FLV_H
#define RUFFLE_CORE_AVM1_FLV_H

#include <cstdint>
#include <vector>
#include <string>
#include <memory>

// Forward declarations
namespace ruffle {
namespace core {
namespace string {
    template<typename T> class AvmString;
}
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Object;
    template<typename T> class Value;
    template<typename T> class ArrayBuilder;
    namespace object {
        template<typename T> class Object;
    }
}
}
}

namespace flv_rs {

/// FLV Variable structure
struct Variable {
    std::string name;
    class Value data;
};

/// FLV Value types
class Value {
public:
    enum class Kind {
        EcmaArray,
        Object,
        StrictArray,
        String,
        LongString,
        Date,
        Number,
        Boolean,
        Null,
        Undefined,
        // Other FLV types...
    };

private:
    Kind kind_;

    // Data for different variants
    std::vector<Variable> variables_;  // For EcmaArray/Object
    std::vector<Value> values_;        // For StrictArray
    std::string string_data_;          // For String/LongString
    double unix_time_;                 // For Date
    int16_t local_offset_;             // For Date
    double number_;                    // For Number
    bool boolean_;                     // For Boolean

public:
    // Getters for variant data
    const std::vector<Variable>& variables() const { return variables_; }
    const std::vector<Value>& values() const { return values_; }
    const std::string& string_data() const { return string_data_; }
    double unix_time() const { return unix_time_; }
    int16_t local_offset() const { return local_offset_; }
    double number() const { return number_; }
    bool boolean() const { return boolean_; }

    Kind kind() const { return kind_; }

    bool is_ecma_array() const { return kind_ == Kind::EcmaArray; }
    bool is_object() const { return kind_ == Kind::Object; }
    bool is_strict_array() const { return kind_ == Kind::StrictArray; }
    bool is_string() const { return kind_ == Kind::String || kind_ == Kind::LongString; }
    bool is_date() const { return kind_ == Kind::Date; }
    bool is_number() const { return kind_ == Kind::Number; }
    bool is_boolean() const { return kind_ == Kind::Boolean; }
    bool is_null() const { return kind_ == Kind::Null; }
    bool is_undefined() const { return kind_ == Kind::Undefined; }
};

} // namespace flv_rs

namespace ruffle {
namespace core {
namespace avm1 {
namespace flv {

/// Convert FLV variables to an AVM1 object
///
/// Creates an AVM1 object with properties from FLV variables.
///
/// \param activation The current activation context
/// \param variables The FLV variables to convert
/// \return An AVM1 Value containing the object
template<typename GC>
Value<GC> avm1_object_from_flv_variables(
    Activation<GC>* activation,
    const std::vector<flv_rs::Variable>& variables
) {
    auto* object_proto = activation->prototypes().object;
    auto info_object = object::Object<GC>::new_object(activation->strings(), object_proto);

    for (const auto& value : variables) {
        std::string property_name = value.name;

        // Convert FLV value to AVM1 value and set property
        auto avm_value = value.data.to_avm1_value(activation);
        auto name = string::AvmString<GC>::new_utf8_bytes(activation->gc(), property_name);

        info_object->set(name, avm_value, activation);
    }

    return Value<GC>::object(&info_object);
}

/// Convert FLV date to AVM1 Date object
///
/// Creates an AVM1 Date object from FLV date data.
///
/// \param activation The current activation context
/// \param unix_time The Unix timestamp
/// \param local_offset The local timezone offset
/// \return An AVM1 Value containing the Date object
template<typename GC>
Value<GC> avm1_date_from_flv_date(
    Activation<GC>* activation,
    double unix_time,
    int16_t local_offset
) {
    auto* constructor = activation->prototypes().date_constructor;
    std::vector<Value<GC>> args = { Value<GC>::number(unix_time) };

    auto result = constructor->construct(activation, args);

    // In production, this would handle the error properly
    // For now, assume construction succeeds
    return result.value_or(Value<GC>::undefined());
}

/// Convert FLV values to an AVM1 array
///
/// Creates an AVM1 array from FLV values.
///
/// \param activation The current activation context
/// \param values The FLV values to convert
/// \return An AVM1 Value containing the array
template<typename GC>
Value<GC> avm1_array_from_flv_values(
    Activation<GC>* activation,
    const std::vector<flv_rs::Value>& values
) {
    ArrayBuilder<GC> builder(activation);

    for (const auto& v : values) {
        builder.push(v.to_avm1_value(activation));
    }

    return builder.into();
}

/// Extension trait for converting FLV values to AVM1 values
///
/// Provides the to_avm1_value method for FLV Value types.
template<typename GC>
class FlvValueAvm1Ext {
public:
    /// Convert an FLV value to an AVM1 value
    ///
    /// Handles all FLV value types:
    /// - Objects/Arrays -> AVM1 objects/arrays
    /// - Strings -> AVM1 strings
    /// - Dates -> AVM1 Date objects
    /// - Numbers/Booleans -> AVM1 primitives
    /// - Null/Undefined -> AVM1 null/undefined
    ///
    /// \param activation The current activation context
    /// \return The converted AVM1 value
    static Value<GC> to_avm1_value(const flv_rs::Value& self, Activation<GC>* activation) {
        switch (self.kind()) {
            case flv_rs::Value::Kind::EcmaArray:
            case flv_rs::Value::Kind::Object:
                return avm1_object_from_flv_variables(activation, self.variables());

            case flv_rs::Value::Kind::StrictArray:
                return avm1_array_from_flv_values(activation, self.values());

            case flv_rs::Value::Kind::String:
            case flv_rs::Value::Kind::LongString:
                return Value<GC>::string(string::AvmString<GC>::new_utf8_bytes(
                    activation->gc(), self.string_data()));

            case flv_rs::Value::Kind::Date:
                return avm1_date_from_flv_date(activation, self.unix_time(), self.local_offset());

            case flv_rs::Value::Kind::Number:
                return Value<GC>::number(self.number());

            case flv_rs::Value::Kind::Boolean:
                return Value<GC>::boolean(self.boolean());

            case flv_rs::Value::Kind::Null:
                return Value<GC>::null();

            case flv_rs::Value::Kind::Undefined:
                return Value<GC>::undefined();

            default:
                // Unimplemented FLV type conversion
                // In production, this would panic or return a default value
                return Value<GC>::undefined();
        }
    }
};

} // namespace flv
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_FLV_H
