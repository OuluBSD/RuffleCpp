/*
 * C++ header for AVM1 function functionality
 * This replaces the functionality of core/src/avm1/function.rs
 */

#ifndef AVM1_FUNCTION_H
#define AVM1_FUNCTION_H

#include "avm1.h"
#include "avm1/value.h"
#include "avm1/object.h"
#include "avm1/activation.h"
#include "avm1/error.h"
#include "avm1/scope.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace ruffle {

// Forward declarations
class Avm1Function;
class FunctionObject;

// Type for native functions in AVM1
using NativeFunction = std::function<std::shared_ptr<Value>(
    std::shared_ptr<Activation>,
    std::shared_ptr<Object>,
    const std::vector<std::shared_ptr<Value>>&)>;

// Type for table native functions (compatible with ASnative)
using TableNativeFunction = std::function<std::shared_ptr<Value>(
    std::shared_ptr<Activation>,
    std::shared_ptr<Object>,
    const std::vector<std::shared_ptr<Value>>&,
    uint16_t)>;

// Enum for execution reason
enum class ExecutionReason {
    // This execution is a "normal" function call from ActionScript bytecode.
    FUNCTION_CALL,
    // This execution is a "normal" constructor call from ActionScript bytecode.
    CONSTRUCTOR_CALL,
    // This execution is a "special" internal function call from the player,
    // such as getters, setters, `toString`, or event handlers.
    SPECIAL
};

// Enum for function flags
enum class FunctionFlags {
    NONE = 0,
    NEED_ARGUMENTS = 1 << 0,
    NEED_ACTIVATION = 1 << 1,
    NEED_REST = 1 << 2
};

// Execution name wrapper
class ExecutionName {
private:
    std::string name_;
    bool is_static_;

public:
    ExecutionName(const std::string& name, bool is_static = false) 
        : name_(name), is_static_(is_static) {}
    
    const std::string& name() const { return name_; }
    bool is_static() const { return is_static_; }
    
    static ExecutionName Static(const std::string& name) {
        return ExecutionName(name, true);
    }
};

// AVM1 function class
class Avm1Function {
private:
    std::string name_;
    std::vector<std::string> parameters_;
    std::shared_ptr<SwfSlice> action_data_;
    std::shared_ptr<Scope> scope_;
    FunctionFlags flags_;
    std::optional<NativeFunction> native_function_;
    std::shared_ptr<Object> constructor_;  // For constructor functions

public:
    Avm1Function(const std::string& name,
                 std::vector<std::string> parameters,
                 std::shared_ptr<SwfSlice> action_data,
                 std::shared_ptr<Scope> scope,
                 FunctionFlags flags = FunctionFlags::NONE,
                 std::optional<NativeFunction> native_func = std::nullopt,
                 std::shared_ptr<Object> constructor = nullptr)
        : name_(name), parameters_(std::move(parameters)), 
          action_data_(std::move(action_data)), scope_(std::move(scope)),
          flags_(flags), native_function_(std::move(native_func)),
          constructor_(std::move(constructor)) {}

    // Getters
    const std::string& name() const { return name_; }
    const std::vector<std::string>& parameters() const { return parameters_; }
    std::shared_ptr<SwfSlice> action_data() const { return action_data_; }
    std::shared_ptr<Scope> scope() const { return scope_; }
    FunctionFlags flags() const { return flags_; }
    bool has_native_function() const { return native_function_.has_value(); }
    std::shared_ptr<Object> constructor() const { return constructor_; }

    // Execute the function
    std::shared_ptr<Value> call(
        std::shared_ptr<Activation> activation,
        std::shared_ptr<Object> this_obj,
        const std::vector<std::shared_ptr<Value>>& args) const {
        
        if (native_function_) {
            // Call the native function
            return native_function_.value()(activation, this_obj, args);
        } else {
            // Execute the bytecode function
            return exec(ExecutionName(name_), activation, this_obj, args);
        }
    }

    // Execute constructor
    std::shared_ptr<Value> construct(
        std::shared_ptr<Activation> activation,
        std::shared_ptr<Object> callee,
        const std::vector<std::shared_ptr<Value>>& args) const {
        
        // Get the prototype
        auto prototype = callee->get("prototype", activation);
        
        // Create a new object with the prototype
        auto this_obj = std::make_shared<Object>(activation->context()->strings(), prototype);
        
        // Define constructor properties
        define_constructor_props(activation, this_obj, callee->as_value());
        
        // Execute the constructor
        bool propagate = constructor_ != nullptr;
        auto ret = exec_constructor(ExecutionName::Static("[ctor]"), 
                                   activation, this_obj, args);
        
        return propagate ? ret : this_obj->as_value();
    }

private:
    // Execute the function with bytecode
    std::shared_ptr<Value> exec(
        ExecutionName name,
        std::shared_ptr<Activation> activation,
        std::shared_ptr<Object> this_obj,
        const std::vector<std::shared_ptr<Value>>& args) const {
        
        // In a real implementation, this would execute the bytecode
        // For now, we'll return undefined
        return std::make_shared<Value>(Value::UNDEFINED);
    }

    // Execute constructor with bytecode
    std::shared_ptr<Value> exec_constructor(
        ExecutionName name,
        std::shared_ptr<Activation> activation,
        std::shared_ptr<Object> this_obj,
        const std::vector<std::shared_ptr<Value>>& args) const {
        
        // In a real implementation, this would execute the constructor bytecode
        // For now, we'll return undefined
        return std::make_shared<Value>(Value::UNDEFINED);
    }

    // Define constructor properties
    static void define_constructor_props(
        std::shared_ptr<Activation> activation,
        std::shared_ptr<Object> this_obj,
        std::shared_ptr<Value> callee) {
        
        this_obj->define_value(activation->context()->gc(), 
                              "__constructor__", 
                              callee, 
                              Attribute::DONT_ENUM);
        
        if (activation->context()->swf_version() < 7) {
            this_obj->define_value(activation->context()->gc(), 
                                  "constructor", 
                                  callee, 
                                  Attribute::DONT_ENUM);
        }
    }
};

// Function object class
class FunctionObject {
private:
    std::shared_ptr<Avm1Function> function_;
    std::shared_ptr<Object> proto_;
    std::shared_ptr<Object> constructor_;

public:
    FunctionObject(std::shared_ptr<Avm1Function> func,
                   std::shared_ptr<Object> proto = nullptr,
                   std::shared_ptr<Object> constructor = nullptr)
        : function_(std::move(func)), proto_(std::move(proto)), 
          constructor_(std::move(constructor)) {}

    std::shared_ptr<Avm1Function> function() const { return function_; }
    std::shared_ptr<Object> prototype() const { return proto_; }
    std::shared_ptr<Object> constructor() const { return constructor_; }

    // Call the function object
    std::shared_ptr<Value> call(
        const std::string& name,
        std::shared_ptr<Activation> activation,
        std::shared_ptr<Object> this_obj,
        const std::vector<std::shared_ptr<Value>>& args) const {
        
        return function_->call(activation, this_obj, args);
    }

    // Construct using the function object
    std::shared_ptr<Value> construct(
        std::shared_ptr<Activation> activation,
        std::shared_ptr<Object> callee,
        const std::vector<std::shared_ptr<Value>>& args) const {
        
        return function_->construct(activation, callee, args);
    }
};

} // namespace ruffle

#endif // AVM1_FUNCTION_H