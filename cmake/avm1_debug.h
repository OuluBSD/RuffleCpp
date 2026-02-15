/*
 * C++ header for AVM1 debug functionality
 * This replaces the functionality of core/src/avm1/debug.rs
 */

#ifndef AVM1_DEBUG_H
#define AVM1_DEBUG_H

#include "avm1/value.h"
#include "avm1/object.h"
#include "avm1/activation.h"
#include <vector>
#include <string>
#include <memory>
#include <unordered_set>

namespace ruffle {

// Variable dumper for debugging AVM1 values
class VariableDumper {
private:
    std::vector<const Object*> objects_;
    uint32_t depth_;
    std::string output_;
    std::string indent_;

public:
    explicit VariableDumper(const std::string& indent = "  ")
        : depth_(0), indent_(indent) {}

    // Create a new variable dumper
    static VariableDumper create(const std::string& indent = "  ") {
        return VariableDumper(indent);
    }

    // Dump a value to string
    template<typename GCContext>
    static std::string dump(const std::shared_ptr<Value>& value, 
                           const std::string& indent,
                           std::shared_ptr<Activation> activation) {
        VariableDumper dumper(indent);
        dumper.print_value(value, activation);
        return dumper.output();
    }

    // Get the output string
    const std::string& output() const { return output_; }

    // Get object ID for circular reference detection
    std::pair<size_t, bool> object_id(std::shared_ptr<Object> object) {
        const Object* ptr = object.get();

        for (size_t i = 0; i < objects_.size(); ++i) {
            if (objects_[i] == ptr) {
                return {i, false};  // Object already seen
            }
        }

        size_t id = objects_.size();
        objects_.push_back(ptr);
        return {id, true};  // New object
    }

    // Add indentation to output
    void indent() {
        for (uint32_t i = 0; i < depth_; ++i) {
            output_ += indent_;
        }
    }

    // Print a string value with proper escaping
    void print_string(const std::string& str) {
        output_ += '"';
        
        for (char c : str) {
            switch (c) {
                case '"':
                    output_ += "\\\"";
                    break;
                case '\\':
                    output_ += "\\\\";
                    break;
                case '\n':
                    output_ += "\\n";
                    break;
                case '\r':
                    output_ += "\\r";
                    break;
                case '\t':
                    output_ += "\\t";
                    break;
                case '\b':  // Backspace
                    output_ += "\\b";
                    break;
                case '\f':  // Form feed
                    output_ += "\\f";
                    break;
                default:
                    output_ += c;
                    break;
            }
        }
        
        output_ += '"';
    }

    // Print an object value
    void print_object(std::shared_ptr<Object> object, 
                     std::shared_ptr<Activation> activation) {
        auto [id, is_new] = object_id(object);
        output_ += "[object #";
        output_ += std::to_string(id);
        output_ += "]";
        
        if (is_new) {
            print_properties(object, activation);
        }
    }

    // Print a property of an object
    void print_property(std::shared_ptr<Object> object,
                       const std::string& key,
                       std::shared_ptr<Activation> activation) {
        try {
            auto value = object->get(key, activation);
            print_value(value, activation);
        } catch (const std::exception& e) {
            output_ += "Error: \"";
            output_ += e.what();
            output_ += "\"";
        }
    }

    // Print all properties of an object
    void print_properties(std::shared_ptr<Object> object,
                         std::shared_ptr<Activation> activation) {
        auto keys = object->get_keys(activation, false);
        
        if (keys.empty()) {
            output_ += " {}";
        } else {
            output_ += " {\n";
            depth_++;
            
            for (const auto& key : keys) {
                indent();
                output_ += key;
                output_ += ": ";
                print_property(object, key, activation);
                output_ += "\n";
            }
            
            depth_--;
            indent();
            output_ += "}";
        }
    }

    // Print a value based on its type
    void print_value(std::shared_ptr<Value> value,
                    std::shared_ptr<Activation> activation) {
        if (!value) {
            output_ += "undefined";
            return;
        }
        
        switch (value->type()) {
            case ValueType::UNDEFINED:
                output_ += "undefined";
                break;
            case ValueType::NULL_VAL:
                output_ += "null";
                break;
            case ValueType::BOOLEAN:
                output_ += value->as_bool() ? "true" : "false";
                break;
            case ValueType::NUMBER:
                output_ += std::to_string(value->as_number());
                break;
            case ValueType::STRING:
                print_string(value->as_string());
                break;
            case ValueType::OBJECT:
                print_object(value->as_object(), activation);
                break;
            default:
                output_ += "unknown";
                break;
        }
    }

    // Print variables from an object with a header
    void print_variables(const std::string& header,
                        const std::string& name,
                        std::shared_ptr<Object> object,
                        std::shared_ptr<Activation> activation) {
        auto keys = object->get_keys(activation, false);
        if (keys.empty()) {
            return;
        }

        output_ += header;
        output_ += "\n";
        depth_++;

        for (const auto& key : keys) {
            output_ += name;
            output_ += ".";
            output_ += key;
            output_ += " = ";
            print_property(object, key, activation);
            output_ += "\n";
        }

        depth_--;
        output_ += "\n";
    }
};

} // namespace ruffle

#endif // AVM1_DEBUG_H