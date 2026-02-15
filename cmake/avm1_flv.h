/*
 * C++ header for AVM1 FLV functionality
 * This replaces the functionality of core/src/avm1/flv.rs
 */

#ifndef AVM1_FLV_H
#define AVM1_FLV_H

#include "avm1.h"
#include "avm1/value.h"
#include "avm1/object.h"
#include "avm1/activation.h"
#include "avm1/globals/array_builder.h"
#include <vector>
#include <string>
#include <memory>
#include <variant>

namespace ruffle {

// Forward declarations for FLV-related types
enum class FlvValueType {
    ECMA_ARRAY,
    OBJECT,
    STRICT_ARRAY,
    STRING,
    LONG_STRING,
    DATE,
    NUMBER,
    BOOLEAN,
    NULL_VAL,
    UNDEFINED
};

struct FlvVariable {
    std::string name;
    class FlvValue data;
};

class FlvValue {
private:
    FlvValueType type_;
    std::variant<std::vector<FlvVariable>,           // For ECMA_ARRAY/OBJECT
                 std::vector<FlvValue>,             // For STRICT_ARRAY
                 std::string,                       // For STRING/LONG_STRING
                 double,                            // For NUMBER/DATE
                 bool,                              // For BOOLEAN
                 std::monostate>                    // For NULL/UNDEFINED
        data_;

public:
    // Constructors for different value types
    static FlvValue ecma_array(const std::vector<FlvVariable>& vars) {
        FlvValue val;
        val.type_ = FlvValueType::ECMA_ARRAY;
        val.data_ = vars;
        return val;
    }

    static FlvValue object(const std::vector<FlvVariable>& vars) {
        FlvValue val;
        val.type_ = FlvValueType::OBJECT;
        val.data_ = vars;
        return val;
    }

    static FlvValue strict_array(const std::vector<FlvValue>& values) {
        FlvValue val;
        val.type_ = FlvValueType::STRICT_ARRAY;
        val.data_ = values;
        return val;
    }

    static FlvValue string(const std::string& str) {
        FlvValue val;
        val.type_ = FlvValueType::STRING;
        val.data_ = str;
        return val;
    }

    static FlvValue long_string(const std::string& str) {
        FlvValue val;
        val.type_ = FlvValueType::LONG_STRING;
        val.data_ = str;
        return val;
    }

    static FlvValue date(double unix_time, int16_t local_offset) {
        // For simplicity, we'll store the unix_time in the double field
        // and ignore local_offset for now
        FlvValue val;
        val.type_ = FlvValueType::DATE;
        val.data_ = unix_time;
        return val;
    }

    static FlvValue number(double value) {
        FlvValue val;
        val.type_ = FlvValueType::NUMBER;
        val.data_ = value;
        return val;
    }

    static FlvValue boolean(bool value) {
        FlvValue val;
        val.type_ = FlvValueType::BOOLEAN;
        val.data_ = value;
        return val;
    }

    static FlvValue null() {
        FlvValue val;
        val.type_ = FlvValueType::NULL_VAL;
        val.data_ = std::monostate{};
        return val;
    }

    static FlvValue undefined() {
        FlvValue val;
        val.type_ = FlvValueType::UNDEFINED;
        val.data_ = std::monostate{};
        return val;
    }

    // Getters
    FlvValueType type() const { return type_; }
    
    const std::vector<FlvVariable>& as_ecma_array() const {
        return std::get<std::vector<FlvVariable>>(data_);
    }
    
    const std::vector<FlvValue>& as_strict_array() const {
        return std::get<std::vector<FlvValue>>(data_);
    }
    
    const std::string& as_string() const {
        return std::get<std::string>(data_);
    }
    
    double as_number() const {
        return std::get<double>(data_);
    }
    
    bool as_boolean() const {
        return std::get<bool>(data_);
    }
};

// Extension class for converting FLV values to AVM1 values
class FlvValueAvm1Ext {
public:
    // Convert an FLV value to an AVM1 value
    static std::shared_ptr<Value> to_avm1_value(FlvValue flv_value, 
                                               std::shared_ptr<Activation> activation) {
        switch (flv_value.type()) {
            case FlvValueType::ECMA_ARRAY:
            case FlvValueType::OBJECT:
                return avm1_object_from_flv_variables(activation, flv_value.as_ecma_array());
                
            case FlvValueType::STRICT_ARRAY: {
                auto values = flv_value.as_strict_array();
                std::vector<std::shared_ptr<Value>> avm1_values;
                for (const auto& val : values) {
                    avm1_values.push_back(to_avm1_value(val, activation));
                }
                return avm1_array_from_flv_values(activation, avm1_values);
            }
                
            case FlvValueType::STRING:
            case FlvValueType::LONG_STRING:
                return std::make_shared<Value>(flv_value.as_string());
                
            case FlvValueType::DATE: {
                double unix_time = flv_value.as_number();
                return avm1_date_from_flv_date(activation, unix_time);
            }
                
            case FlvValueType::NUMBER:
                return std::make_shared<Value>(flv_value.as_number());
                
            case FlvValueType::BOOLEAN:
                return std::make_shared<Value>(flv_value.as_boolean());
                
            case FlvValueType::NULL_VAL:
                return std::make_shared<Value>(Value::NULL_VAL);
                
            case FlvValueType::UNDEFINED:
                return std::make_shared<Value>(Value::UNDEFINED);
                
            default:
                // Handle unimplemented cases
                throw std::runtime_error("FLV data to AVM1 data conversion unimplemented for this type");
        }
    }

private:
    // Convert FLV variables to an AVM1 object
    static std::shared_ptr<Value> avm1_object_from_flv_variables(
        std::shared_ptr<Activation> activation,
        const std::vector<FlvVariable>& variables) {
        
        // Create a new object
        auto object_proto = activation->context()->prototypes().object;
        auto info_object = std::make_shared<Object>(activation->context()->strings(), object_proto);
        
        // Set properties from variables
        for (const auto& var : variables) {
            auto property_name = var.name;  // In a real implementation, this would be an AvmString
            auto value = to_avm1_value(var.data, activation);
            info_object->set(property_name, value, activation);
        }
        
        return std::make_shared<Value>(info_object);
    }

    // Convert FLV date to AVM1 date
    static std::shared_ptr<Value> avm1_date_from_flv_date(
        std::shared_ptr<Activation> activation,
        double unix_time) {
        
        // In a real implementation, this would create a Date object
        // For now, we'll just return the unix timestamp as a number
        return std::make_shared<Value>(unix_time);
    }

    // Convert FLV values to an AVM1 array
    static std::shared_ptr<Value> avm1_array_from_flv_values(
        std::shared_ptr<Activation> activation,
        const std::vector<std::shared_ptr<Value>>& values) {
        
        auto array_builder = ArrayBuilder::create(activation);
        for (const auto& value : values) {
            array_builder.add(value);
        }
        return array_builder.build();
    }
};

} // namespace ruffle

#endif // AVM1_FLV_H