/*
 * C++ header for AVM1 error functionality
 * This replaces the functionality of core/src/avm1/error.rs
 */

#ifndef AVM1_ERROR_H
#define AVM1_ERROR_H

#include "avm1/value.h"
#include <stdexcept>
#include <string>
#include <memory>
#include <variant>

namespace ruffle {

// Base class for AVM1 errors
class Avm1ErrorBase : public std::exception {
protected:
    std::string message_;

public:
    explicit Avm1ErrorBase(const std::string& message) : message_(message) {}
    
    const char* what() const noexcept override {
        return message_.c_str();
    }
    
    const std::string& message() const { return message_; }
};

// Enum class for AVM1 error types
enum class Avm1ErrorType {
    PROTOTYPE_RECURSION_LIMIT,
    EXECUTION_TIMEOUT,
    FUNCTION_RECURSION_LIMIT,
    SPECIAL_RECURSION_LIMIT,
    INVALID_SWF,
    THROWN_VALUE
};

// AVM1 error class
class Avm1Error : public Avm1ErrorBase {
private:
    Avm1ErrorType type_;
    std::shared_ptr<Value> thrown_value_;  // For THROWN_VALUE type
    std::string swf_error_message_;       // For INVALID_SWF type
    uint16_t recursion_limit_;            // For FUNCTION_RECURSION_LIMIT type

public:
    // Constructor for prototype recursion limit error
    static Avm1Error prototype_recursion_limit() {
        return Avm1Error(Avm1ErrorType::PROTOTYPE_RECURSION_LIMIT,
                        "Prototype recursion limit has been exceeded");
    }

    // Constructor for execution timeout error
    static Avm1Error execution_timeout() {
        return Avm1Error(Avm1ErrorType::EXECUTION_TIMEOUT,
                        "A script in this movie has taken too long to execute and has been terminated.");
    }

    // Constructor for function recursion limit error
    static Avm1Error function_recursion_limit(uint16_t limit) {
        return Avm1Error(Avm1ErrorType::FUNCTION_RECURSION_LIMIT,
                        std::to_string(limit) + " levels of function recursion were exceeded in one action list. This is probably an infinite loop.",
                        limit);
    }

    // Constructor for special recursion limit error
    static Avm1Error special_recursion_limit() {
        return Avm1Error(Avm1ErrorType::SPECIAL_RECURSION_LIMIT,
                        "66 levels of special recursion were exceeded in one action list. This is probably an infinite loop.");
    }

    // Constructor for invalid SWF error
    static Avm1Error invalid_swf(const std::string& swf_error_msg) {
        return Avm1Error(Avm1ErrorType::INVALID_SWF,
                        "Couldn't parse SWF: " + swf_error_msg,
                        0, swf_error_msg);
    }

    // Constructor for thrown value error
    static Avm1Error thrown_value(std::shared_ptr<Value> value) {
        return Avm1Error(Avm1ErrorType::THROWN_VALUE,
                        "A script has thrown a custom error.",
                        0, "", value);
    }

    // Getters
    Avm1ErrorType type() const { return type_; }
    std::shared_ptr<Value> thrown_value() const { return thrown_value_; }
    const std::string& swf_error_message() const { return swf_error_message_; }
    uint16_t recursion_limit() const { return recursion_limit_; }

    // Check error type
    bool is_prototype_recursion_limit() const { return type_ == Avm1ErrorType::PROTOTYPE_RECURSION_LIMIT; }
    bool is_execution_timeout() const { return type_ == Avm1ErrorType::EXECUTION_TIMEOUT; }
    bool is_function_recursion_limit() const { return type_ == Avm1ErrorType::FUNCTION_RECURSION_LIMIT; }
    bool is_special_recursion_limit() const { return type_ == Avm1ErrorType::SPECIAL_RECURSION_LIMIT; }
    bool is_invalid_swf() const { return type_ == Avm1ErrorType::INVALID_SWF; }
    bool is_thrown_value() const { return type_ == Avm1ErrorType::THROWN_VALUE; }

private:
    Avm1Error(Avm1ErrorType type, const std::string& msg, 
              uint16_t limit = 0, const std::string& swf_msg = "", 
              std::shared_ptr<Value> value = nullptr)
        : Avm1ErrorBase(msg), type_(type), thrown_value_(value), 
          swf_error_message_(swf_msg), recursion_limit_(limit) {}
};

// Exception class for AVM1 errors that can be thrown
class Avm1Exception : public std::exception {
private:
    Avm1Error error_;

public:
    explicit Avm1Exception(const Avm1Error& error) : error_(error) {}
    
    const char* what() const noexcept override {
        return error_.what();
    }
    
    const Avm1Error& get_error() const { return error_; }
};

} // namespace ruffle

#endif // AVM1_ERROR_H