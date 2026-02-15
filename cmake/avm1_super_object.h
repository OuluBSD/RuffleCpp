/*
 * C++ header for AVM1 super object functionality
 * This replaces the functionality of core/src/avm1/object/super_object.rs
 */

#ifndef AVM1_SUPER_OBJECT_H
#define AVM1_SUPER_OBJECT_H

#include "avm1.h"
#include "avm1/value.h"
#include "avm1/object.h"
#include "avm1/activation.h"
#include "avm1/error.h"
#include "avm1/function.h"
#include <memory>
#include <string>

namespace ruffle {

// Forward declaration
class SuperObject;

// Implementation of the `super` object in AS2.
//
// A `SuperObject` references all data from another object, but with one layer
// of prototyping removed. It's as if the given object had been constructed
// with its parent class.
class SuperObject {
private:
    std::shared_ptr<Object> this_;
    uint8_t depth_;
    // Adds a niche, so that enums containing this type can use it for their discriminant.
    uint8_t niche_;  // Using uint8_t instead of ZeroU8 for simplicity

public:
    // Constructor
    SuperObject(std::shared_ptr<Object> this_obj, uint8_t depth)
        : this_(std::move(this_obj)), depth_(depth), niche_(0) {}

    // Create a new SuperObject
    static std::shared_ptr<SuperObject> create(std::shared_ptr<Object> this_obj, uint8_t depth) {
        return std::make_shared<SuperObject>(std::move(this_obj), depth);
    }

    // Get the 'this' object
    std::shared_ptr<Object> get_this() const { return this_; }

    // Get the prototype depth
    uint8_t get_depth() const { return depth_; }

    // Get the base prototype
    std::shared_ptr<Object> base_proto(std::shared_ptr<Activation> activation) const {
        auto proto = this_;
        for (int i = 0; i < depth_; ++i) {
            auto proto_val = proto->proto(activation);
            if (proto_val && proto_val->is_object()) {
                proto = proto_val->as_object();
            } else {
                return nullptr; // No more prototypes
            }
        }
        return proto;
    }

    // Get the prototype
    std::shared_ptr<Value> proto(std::shared_ptr<Activation> activation) const {
        auto base = base_proto(activation);
        if (base) {
            return base->proto(activation);
        }
        return std::make_shared<Value>(Value::UNDEFINED);
    }

    // Call a constructor using super
    std::shared_ptr<Value> call(const std::string& name,
                               std::shared_ptr<Activation> activation,
                               const std::vector<std::shared_ptr<Value>>& args) const {
        auto base = base_proto(activation);
        if (!base) {
            return std::make_shared<Value>(Value::UNDEFINED);
        }

        // Get the constructor
        auto constructor_val = base->get("__constructor__", activation);
        if (!constructor_val || !constructor_val->is_object()) {
            return std::make_shared<Value>(Value::UNDEFINED);
        }

        auto constructor_obj = constructor_val->as_object();
        if (!constructor_obj) {
            return std::make_shared<Value>(Value::UNDEFINED);
        }

        // Check if it's a function
        auto func_obj = constructor_obj->as_function();
        if (!func_obj) {
            return std::make_shared<Value>(Value::UNDEFINED);
        }

        // Execute the constructor
        return func_obj->exec_constructor(name, activation, this_, depth_ + 1, args);
    }

    // Call a method using super
    std::shared_ptr<Value> call_method(const std::string& name,
                                      const std::vector<std::shared_ptr<Value>>& args,
                                      std::shared_ptr<Activation> activation,
                                      ExecutionReason reason) const {
        // 'special' method calls appear to skip the `__resolve` fallback logic
        bool call_resolve_fn = reason != ExecutionReason::SPECIAL;
        
        auto this_obj = this_;
        auto proto_val = proto(activation);
        
        // Search for the method in the prototype chain
        auto search_result = search_prototype(proto_val, name, activation, this_obj, call_resolve_fn);
        
        if (!search_result.first || !search_result.first->is_object()) {
            return std::make_shared<Value>(Value::UNDEFINED);
        }

        auto method_obj = search_result.first->as_object();
        int depth = search_result.second;

        // Execute the method
        if (auto func_obj = method_obj->as_function()) {
            return func_obj->exec(name, activation, this_obj, depth_ + depth + 1, args, reason, method_obj);
        } else {
            // If it's not a function, try to call it directly
            return method_obj->call(name, activation, this_obj, args);
        }
    }
};

// Helper function to search for a property in the prototype chain
inline std::pair<std::shared_ptr<Value>, int> search_prototype(
    std::shared_ptr<Value> proto,
    const std::string& name,
    std::shared_ptr<Activation> activation,
    std::shared_ptr<Object> this_obj,
    bool call_resolve_fn) {
    
    int depth = 0;
    auto current_proto = proto;

    while (current_proto && current_proto->is_object()) {
        if (depth == 255) {
            // Throw prototype recursion limit error
            throw Avm1Exception(Avm1Error::prototype_recursion_limit());
        }

        auto obj = current_proto->as_object();
        if (obj->has_property(name, activation)) {
            auto value = obj->get(name, activation);
            return std::make_pair(value, depth);
        }

        current_proto = obj->proto(activation);
        depth++;
    }

    // If resolve function should be called and we found one
    if (call_resolve_fn) {
        auto resolve_method = find_resolve_method(this_obj, activation);
        if (resolve_method) {
            auto result = resolve_method->call("__resolve", activation, this_obj, 
                                            {std::make_shared<Value>(name)});
            return std::make_pair(result, 0);
        }
    }

    return std::make_pair(nullptr, -1); // Not found
}

// Helper function to find the resolve method
inline std::shared_ptr<Object> find_resolve_method(
    std::shared_ptr<Object> obj,
    std::shared_ptr<Activation> activation) {
    
    int depth = 0;
    auto current_obj = obj;

    while (current_obj) {
        if (depth == 255) {
            // Throw prototype recursion limit error
            throw Avm1Exception(Avm1Error::prototype_recursion_limit());
        }

        auto resolve_prop = current_obj->get("__resolve", activation);
        if (resolve_prop && resolve_prop->is_object()) {
            return resolve_prop->as_object();
        }

        current_obj = current_obj->proto(activation);
        depth++;
    }

    return nullptr; // Not found
}

} // namespace ruffle

#endif // AVM1_SUPER_OBJECT_H