/*
 * C++ header for AVM1 Callable Value functionality
 * This replaces the functionality of core/src/avm1/callable_value.rs
 */

#ifndef AVM1_CALLABLE_VALUE_H
#define AVM1_CALLABLE_VALUE_H

#include "avm1/value.h"
#include "avm1/object.h"
#include <variant>
#include <memory>

namespace ruffle {

// Enum for callable values in AVM1
class CallableValue {
private:
    using UnCallableType = std::shared_ptr<Value>;
    using CallableType = std::pair<std::shared_ptr<Object>, std::shared_ptr<Value>>;
    std::variant<UnCallableType, CallableType> value_;

public:
    // Constructors
    explicit CallableValue(std::shared_ptr<Value> uncallable_value) 
        : value_(uncallable_value) {}
    
    CallableValue(std::shared_ptr<Object> obj, std::shared_ptr<Value> callable_value) 
        : value_(std::make_pair(std::move(obj), std::move(callable_value))) {}

    // Check if the value is callable
    bool is_callable() const {
        return std::holds_alternative<CallableType>(value_);
    }

    // Get the underlying value regardless of callable status
    std::shared_ptr<Value> get_value() const {
        if (std::holds_alternative<UnCallableType>(value_)) {
            return std::get<UnCallableType>(value_);
        } else {
            return std::get<CallableType>(value_).second;
        }
    }

    // Get the object if callable
    std::shared_ptr<Object> get_object() const {
        if (std::holds_alternative<CallableType>(value_)) {
            return std::get<CallableType>(value_).first;
        }
        return nullptr; // Not callable
    }

    // Call with default this
    std::shared_ptr<Value> call_with_default_this(
        std::shared_ptr<Value> default_this,
        const std::string& name,
        std::shared_ptr<Activation> activation,
        const std::vector<std::shared_ptr<Value>>& args) {
        
        if (std::holds_alternative<CallableType>(value_)) {
            // Callable case: use the object from the pair
            auto [this_obj, val] = std::get<CallableType>(value_);
            if (val->is_object()) {
                return val->as_object()->call(name, activation, this_obj, args);
            }
        } else {
            // Uncallable case: use default this
            auto val = std::get<UnCallableType>(value_);
            if (val->is_object()) {
                return val->as_object()->call(name, activation, default_this, args);
            }
        }
        
        // Return undefined if not callable
        return std::make_shared<Value>(Value::UNDEFINED);
    }

    // Conversion operator to Value
    operator std::shared_ptr<Value>() const {
        return get_value();
    }
};

} // namespace ruffle

#endif // AVM1_CALLABLE_VALUE_H