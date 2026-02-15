/*
 * C++ header for AVM1 script object functionality
 * This replaces the functionality of core/src/avm1/object/script_object.rs
 */

#ifndef AVM1_SCRIPT_OBJECT_H
#define AVM1_SCRIPT_OBJECT_H

#include "avm1.h"
#include "avm1/value.h"
#include "avm1/object.h"
#include "avm1/activation.h"
#include "avm1/error.h"
#include "avm1/function.h"
#include "avm1/property.h"
#include "avm1/property_map.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>

namespace ruffle {

// Forward declarations
class ScriptObject;
class Watcher;

// Watcher class for property watching
class Watcher {
private:
    std::shared_ptr<Object> callback_;
    std::shared_ptr<Value> user_data_;

public:
    Watcher(std::shared_ptr<Object> callback, std::shared_ptr<Value> user_data)
        : callback_(std::move(callback)), user_data_(std::move(user_data)) {}

    // Call the watcher callback
    std::shared_ptr<Value> call(std::shared_ptr<Activation> activation,
                               const std::string& name,
                               std::shared_ptr<Value> old_value,
                               std::shared_ptr<Value> new_value,
                               std::shared_ptr<Object> this_obj) {
        // Prepare arguments for the callback
        std::vector<std::shared_ptr<Value>> args;
        args.push_back(std::make_shared<Value>(name));  // Property name
        args.push_back(old_value);  // Old value
        args.push_back(new_value);  // New value
        args.push_back(user_data_); // User data

        // Call the callback function
        if (callback_ && callback_->is_function()) {
            auto func_obj = callback_->as_function();
            if (func_obj) {
                return func_obj->call(name, activation, this_obj, args);
            }
        }

        return std::make_shared<Value>(Value::UNDEFINED);
    }
};

// Script object class for AVM1
class ScriptObject : public Object {
private:
    std::shared_ptr<PropertyMap> properties_;
    std::shared_ptr<Object> prototype_;
    std::shared_ptr<FunctionObject> constructor_;
    std::unordered_map<std::string, std::shared_ptr<Watcher>> watchers_;
    std::shared_ptr<NativeObject> native_object_;
    std::string type_name_;
    bool is_array_like_;  // Flag for array-like behavior

public:
    ScriptObject(std::shared_ptr<Object> prototype = nullptr,
                 const std::string& type_name = "Object")
        : Object(prototype, type_name), prototype_(std::move(prototype)),
          type_name_(type_name), is_array_like_(false) {
        properties_ = std::make_shared<PropertyMap>();
    }

    // Create a new script object
    static std::shared_ptr<ScriptObject> create(std::shared_ptr<Object> prototype = nullptr,
                                               const std::string& type_name = "Object") {
        return std::make_shared<ScriptObject>(std::move(prototype), type_name);
    }

    // Get a property value
    std::shared_ptr<Value> get(const std::string& name,
                              std::shared_ptr<Activation> activation) const override {
        // First check for watched properties
        auto watcher_it = watchers_.find(name);
        if (watcher_it != watchers_.end()) {
            // For watched properties, we might need special handling
            // For now, just return the value from the property map
        }

        // Check the property map
        auto prop = properties_->get(name);
        if (prop) {
            return prop->get_value();
        }

        // Search in prototype chain
        if (prototype_) {
            return prototype_->get(name, activation);
        }

        return std::make_shared<Value>(Value::UNDEFINED);
    }

    // Set a property value
    void set(const std::string& name,
             std::shared_ptr<Value> value,
             std::shared_ptr<Activation> activation) override {
        // Check if there's a watcher for this property
        auto watcher_it = watchers_.find(name);
        if (watcher_it != watchers_.end()) {
            // Get the old value for the watcher
            auto old_value = get(name, activation);
            // Call the watcher
            watcher_it->second->call(activation, name, old_value, value, shared_from_this());
        }

        // Set the property in the property map
        properties_->set(name, std::move(value));
    }

    // Define a property with attributes
    void define_value(const std::string& name,
                     std::shared_ptr<Value> value,
                     int attributes = 0) override {
        properties_->define_value(name, std::move(value), attributes);
    }

    // Watch a property
    void watch(const std::string& name,
               std::shared_ptr<Object> callback,
               std::shared_ptr<Value> user_data) {
        auto watcher = std::make_shared<Watcher>(std::move(callback), std::move(user_data));
        watchers_[name] = std::move(watcher);
    }

    // Unwatch a property
    void unwatch(const std::string& name) {
        watchers_.erase(name);
    }

    // Get all property names
    std::vector<std::string> get_keys(std::shared_ptr<Activation> activation,
                                     bool include_prototype = true) const override {
        std::vector<std::string> keys;

        // Add own properties
        auto own_keys = properties_->get_keys();
        keys.insert(keys.end(), own_keys.begin(), own_keys.end());

        // Optionally add prototype properties
        if (include_prototype && prototype_) {
            auto proto_keys = prototype_->get_keys(activation, true);
            for (const auto& key : proto_keys) {
                if (std::find(keys.begin(), keys.end(), key) == keys.end()) {
                    keys.push_back(key);
                }
            }
        }

        return keys;
    }

    // Check if this object has a specific property
    bool has_property(const std::string& name,
                    std::shared_ptr<Activation> activation) const override {
        if (properties_->has_property(name)) {
            return true;
        }

        if (prototype_) {
            return prototype_->has_property(name, activation);
        }

        return false;
    }

    // Get element by index (array-like access)
    std::shared_ptr<Value> get_element(std::shared_ptr<Activation> activation,
                                      int32_t index) const {
        if (is_array_like_ || type_name_ == "Array") {
            // For array-like objects, treat index as string property
            std::string index_str = std::to_string(index);
            return get(index_str, activation);
        }

        // For non-array objects, return undefined
        return std::make_shared<Value>(Value::UNDEFINED);
    }

    // Set element by index (array-like access)
    void set_element(std::shared_ptr<Activation> activation,
                    int32_t index,
                    std::shared_ptr<Value> value) {
        if (is_array_like_ || type_name_ == "Array") {
            // For array-like objects, treat index as string property
            std::string index_str = std::to_string(index);
            set(index_str, std::move(value), activation);
        }
    }

    // Delete element by index
    bool delete_element(std::shared_ptr<Activation> activation, int32_t index) {
        if (is_array_like_ || type_name_ == "Array") {
            std::string index_str = std::to_string(index);
            return delete_property(activation, index_str);
        }
        return false;
    }

    // Delete a property
    bool delete_property(std::shared_ptr<Activation> activation,
                        const std::string& name) {
        return properties_->delete_property(name);
    }

    // Get the length of the object (for array-like objects)
    int32_t length(std::shared_ptr<Activation> activation) const {
        if (is_array_like_ || type_name_ == "Array") {
            // Find the highest numeric index and return length
            int32_t max_index = -1;
            auto keys = get_keys(activation, false);
            for (const auto& key : keys) {
                try {
                    int32_t index = std::stoi(key);
                    if (index > max_index) {
                        max_index = index;
                    }
                } catch (...) {
                    // Not a numeric key, continue
                }
            }
            return max_index + 1;
        }
        return 0;
    }

    // Set the length of the object (for array-like objects)
    void set_length(std::shared_ptr<Activation> activation, int32_t new_length) {
        if (is_array_like_ || type_name_ == "Array") {
            // Remove elements with indices >= new_length
            auto keys = get_keys(activation, false);
            for (const auto& key : keys) {
                try {
                    int32_t index = std::stoi(key);
                    if (index >= new_length) {
                        delete_property(activation, key);
                    }
                } catch (...) {
                    // Not a numeric key, continue
                }
            }
        }
    }

    // Getters
    std::shared_ptr<PropertyMap> properties() const { return properties_; }
    std::shared_ptr<Object> proto() const override { return prototype_; }
    std::shared_ptr<FunctionObject> constr() const override { return constructor_; }
    std::shared_ptr<NativeObject> native() const { return native_object_; }
    const std::string& type_name() const override { return type_name_; }

    // Setters
    void set_proto(std::shared_ptr<Object> proto) override { prototype_ = std::move(proto); }
    void set_constr(std::shared_ptr<FunctionObject> constr) override { constructor_ = std::move(constr); }
    void set_native(std::shared_ptr<NativeObject> native) { native_object_ = std::move(native); }
    void set_array_like(bool array_like) { is_array_like_ = array_like; }

    // Convert to value
    std::shared_ptr<Value> as_value() const override {
        return std::make_shared<Value>(const_cast<ScriptObject*>(this));
    }

    // Check if this is a function
    bool is_function() const override {
        return constructor_ != nullptr;
    }

    // Get as function object (if it is one)
    std::shared_ptr<FunctionObject> as_function() const override {
        return is_function() ? constructor_ : nullptr;
    }
};

// Helper function to create a script object
inline std::shared_ptr<ScriptObject> create_script_object(std::shared_ptr<Object> prototype = nullptr,
                                                        const std::string& type_name = "Object") {
    return ScriptObject::create(std::move(prototype), type_name);
}

} // namespace ruffle

#endif // AVM1_SCRIPT_OBJECT_H