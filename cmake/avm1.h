/*
 * C++ header for AVM1 functionality
 * This replaces the functionality of core/src/avm1.rs
 */

#ifndef AVM1_H
#define AVM1_H

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <iostream>

// Forward declarations for AVM1 components
namespace ruffle {

class Activation;
class Avm1;
class Value;
class Object;
class PropertyMap;
template<typename T>
class ArrayBuilder;

// Enum for execution reason
enum class ExecutionReason {
    CALL_METHOD,
    CONSTRUCT,
    INITIALIZATION
};

// Attribute for properties
enum class Attribute {
    READ_ONLY = 0x1,
    DONT_ENUM = 0x2,
    DONT_DELETE = 0x4
};

// AVM1 error class
class Avm1Error {
private:
    std::string message_;
    
public:
    explicit Avm1Error(const std::string& message) : message_(message) {}
    
    const std::string& message() const { return message_; }
    
    // Print error to stderr
    void print() const {
        std::cerr << "AVM1 Error: " << message_ << std::endl;
    }
};

// Activation identifier
struct ActivationIdentifier {
    size_t id;
    std::string description;
    
    ActivationIdentifier(size_t activation_id, const std::string& desc) 
        : id(activation_id), description(desc) {}
};

// Activation class for AVM1 execution context
class Activation {
private:
    ActivationIdentifier id_;
    // Context would contain references to other AVM1 components
    // For now, we'll use a simplified approach
    
public:
    Activation(ActivationIdentifier id) : id_(id) {}
    
    const ActivationIdentifier& id() const { return id_; }
    
    // Method to track stubs (for compatibility)
    void encounter_stub(const std::string& stub_info) {
        // In a real implementation, this would track encountered stubs
        // For now, we'll just output a warning
        #ifdef AVM_DEBUG
        std::cout << "Encountered stub in activation " << id_.id << ": " << stub_info << std::endl;
        #endif
    }
};

// AVM1 runtime class
class Avm1 {
private:
    // State for the AVM1 runtime
    std::vector<std::shared_ptr<Object>> objects_;
    std::shared_ptr<Value> global_scope_;
    
public:
    Avm1();
    
    // Execute a script or function
    std::shared_ptr<Value> execute(const std::string& code, 
                                  std::shared_ptr<Activation> activation);
    
    // Get the global scope
    std::shared_ptr<Value> get_global_scope() const { return global_scope_; }
    
    // Set the global scope
    void set_global_scope(std::shared_ptr<Value> scope) { global_scope_ = scope; }
};

// Macro for AVM warnings (equivalent to avm_warn!)
#define AVM_WARN(activation, ...) \
    do { \
        if constexpr (true) { /* Enable warnings by default */ \
            std::cout << "AVM1 Warning: "; \
            printf(__VA_ARGS__); \
            std::cout << " -- in activation " << activation->id().id << std::endl; \
        } \
    } while(0)

// Macro for AVM errors (equivalent to avm_error!)
#define AVM_ERROR(activation, ...) \
    do { \
        std::cerr << "AVM1 Error: "; \
        printf(__VA_ARGS__); \
        std::cerr << " -- in activation " << activation->id().id << std::endl; \
    } while(0)

// Macro for AVM1 stubs (equivalent to avm1_stub!)
#define AVM1_STUB(activation, ...) \
    do { \
        activation->encounter_stub("AVM1 stub encountered"); \
    } while(0)

// Include headers for the various AVM1 components
#include "avm1/activation.h"
#include "avm1/value.h"
#include "avm1/object.h"
#include "avm1/property_map.h"
#include "avm1/runtime.h"
#include "avm1/globals/array_builder.h"

} // namespace ruffle

#endif // AVM1_H