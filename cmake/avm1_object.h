/*
 * C++ header for AVM1 object functionality
 * This replaces the functionality of core/src/avm1/object.rs
 */

#ifndef AVM1_OBJECT_H
#define AVM1_OBJECT_H

#include "avm1.h"
#include "avm1/value.h"
#include "avm1/activation.h"
#include "avm1/error.h"
#include "avm1/function.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <variant>
#include <functional>

namespace ruffle {

// Forward declarations
class Object;
class DisplayObject;
class MovieClip;
class EditText;
class Avm1Button;
class Video;
class NetStream;
class XmlNode;
class Xml;
class Sound;
class LocalConnection;
class SharedObject;
class NetConnection;
class TextFormat;
class ColorTransformObject;
class TransformObject;
class BitmapData;
class StyleSheetObject;
class TextSnapshotObject;
class BevelFilter;
class BlurFilter;
class ColorMatrixFilter;
class ConvolutionFilter;
class DisplacementMapFilter;
class DropShadowFilter;
class GlowFilter;
class GradientFilter;

// Enum for native objects in AVM1
enum class NativeObjectType {
    NONE,
    SUPER,
    DISPLAY_OBJECT,
    MOVIE_CLIP,
    EDIT_TEXT,
    BUTTON,
    VIDEO,
    NET_STREAM,
    XML_NODE,
    XML,
    SOUND,
    LOCAL_CONNECTION,
    SHARED_OBJECT,
    NET_CONNECTION,
    TEXT_FORMAT,
    COLOR_TRANSFORM,
    TRANSFORM,
    BITMAP_DATA,
    STYLE_SHEET,
    TEXT_SNAPSHOT,
    BEVEL_FILTER,
    BLUR_FILTER,
    COLOR_MATRIX_FILTER,
    CONVOLUTION_FILTER,
    DISPLACEMENT_MAP_FILTER,
    DROP_SHADOW_FILTER,
    GLOW_FILTER,
    GRADIENT_FILTER
};

// Variant to hold different native object types
using NativeObjectVariant = std::variant<
    std::monostate,  // None
    std::shared_ptr<DisplayObject>,  // DisplayObject
    std::shared_ptr<MovieClip>,      // MovieClip
    std::shared_ptr<EditText>,       // EditText
    std::shared_ptr<Avm1Button>,     // Button
    std::shared_ptr<Video>,          // Video
    std::shared_ptr<NetStream>,      // NetStream
    std::shared_ptr<XmlNode>,        // XmlNode
    std::shared_ptr<Xml>,            // Xml
    std::shared_ptr<Sound>,          // Sound
    std::shared_ptr<LocalConnection>, // LocalConnection
    std::shared_ptr<SharedObject>,    // SharedObject
    std::shared_ptr<NetConnection>,   // NetConnection
    std::shared_ptr<TextFormat>,      // TextFormat
    std::shared_ptr<ColorTransformObject>, // ColorTransform
    std::shared_ptr<TransformObject>, // Transform
    std::shared_ptr<BitmapData>,      // BitmapData
    std::shared_ptr<StyleSheetObject>, // StyleSheet
    std::shared_ptr<TextSnapshotObject>, // TextSnapshot
    std::shared_ptr<BevelFilter>,     // BevelFilter
    std::shared_ptr<BlurFilter>,      // BlurFilter
    std::shared_ptr<ColorMatrixFilter>, // ColorMatrixFilter
    std::shared_ptr<ConvolutionFilter>, // ConvolutionFilter
    std::shared_ptr<DisplacementMapFilter>, // DisplacementMapFilter
    std::shared_ptr<DropShadowFilter>, // DropShadowFilter
    std::shared_ptr<GlowFilter>,      // GlowFilter
    std::shared_ptr<GradientFilter>   // GradientFilter
>;

// Native object wrapper
class NativeObject {
private:
    NativeObjectVariant data_;

public:
    NativeObject() : data_(std::monostate{}) {}
    
    template<typename T>
    explicit NativeObject(std::shared_ptr<T> obj) : data_(obj) {}
    
    NativeObjectType type() const {
        return std::visit([](const auto& value) -> NativeObjectType {
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, std::monostate>) {
                return NativeObjectType::NONE;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<DisplayObject>>) {
                return NativeObjectType::DISPLAY_OBJECT;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<MovieClip>>) {
                return NativeObjectType::MOVIE_CLIP;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<EditText>>) {
                return NativeObjectType::EDIT_TEXT;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<Avm1Button>>) {
                return NativeObjectType::BUTTON;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<Video>>) {
                return NativeObjectType::VIDEO;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<NetStream>>) {
                return NativeObjectType::NET_STREAM;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<XmlNode>>) {
                return NativeObjectType::XML_NODE;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<Xml>>) {
                return NativeObjectType::XML;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<Sound>>) {
                return NativeObjectType::SOUND;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<LocalConnection>>) {
                return NativeObjectType::LOCAL_CONNECTION;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<SharedObject>>) {
                return NativeObjectType::SHARED_OBJECT;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<NetConnection>>) {
                return NativeObjectType::NET_CONNECTION;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<TextFormat>>) {
                return NativeObjectType::TEXT_FORMAT;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<ColorTransformObject>>) {
                return NativeObjectType::COLOR_TRANSFORM;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<TransformObject>>) {
                return NativeObjectType::TRANSFORM;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<BitmapData>>) {
                return NativeObjectType::BITMAP_DATA;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<StyleSheetObject>>) {
                return NativeObjectType::STYLE_SHEET;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<TextSnapshotObject>>) {
                return NativeObjectType::TEXT_SNAPSHOT;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<BevelFilter>>) {
                return NativeObjectType::BEVEL_FILTER;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<BlurFilter>>) {
                return NativeObjectType::BLUR_FILTER;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<ColorMatrixFilter>>) {
                return NativeObjectType::COLOR_MATRIX_FILTER;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<ConvolutionFilter>>) {
                return NativeObjectType::CONVOLUTION_FILTER;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<DisplacementMapFilter>>) {
                return NativeObjectType::DISPLACEMENT_MAP_FILTER;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<DropShadowFilter>>) {
                return NativeObjectType::DROP_SHADOW_FILTER;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<GlowFilter>>) {
                return NativeObjectType::GLOW_FILTER;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<GradientFilter>>) {
                return NativeObjectType::GRADIENT_FILTER;
            } else {
                return NativeObjectType::NONE;
            }
        }, data_);
    }
    
    template<typename T>
    std::shared_ptr<T> get() const {
        if (auto ptr = std::get_if<std::shared_ptr<T>>(&data_)) {
            return *ptr;
        }
        return nullptr;
    }
};

// Object class for AVM1
class Object {
private:
    std::unordered_map<std::string, std::shared_ptr<Value>> properties_;
    std::shared_ptr<Object> prototype_;
    std::shared_ptr<FunctionObject> constructor_;
    NativeObject native_object_;
    std::string type_name_;

public:
    Object(std::shared_ptr<Object> prototype = nullptr, 
           const std::string& type_name = "Object")
        : prototype_(std::move(prototype)), type_name_(type_name) {}

    // Get a property value
    std::shared_ptr<Value> get(const std::string& name, 
                              std::shared_ptr<Activation> activation) const {
        auto it = properties_.find(name);
        if (it != properties_.end()) {
            return it->second;
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
             std::shared_ptr<Activation> activation) {
        properties_[name] = std::move(value);
    }

    // Define a value property
    void define_value(const std::string& name, 
                     std::shared_ptr<Value> value, 
                     int attributes = 0) {
        properties_[name] = std::move(value);
    }

    // Get all property names
    std::vector<std::string> get_keys(std::shared_ptr<Activation> activation, 
                                     bool include_prototype = true) const {
        std::vector<std::string> keys;
        
        // Add own properties
        for (const auto& [key, value] : properties_) {
            keys.push_back(key);
        }
        
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
                    std::shared_ptr<Activation> activation) const {
        if (properties_.find(name) != properties_.end()) {
            return true;
        }
        
        if (prototype_) {
            return prototype_->has_property(name, activation);
        }
        
        return false;
    }

    // Call a method on this object
    std::shared_ptr<Value> call(const std::string& name,
                               std::shared_ptr<Activation> activation,
                               std::shared_ptr<Object> this_obj,
                               const std::vector<std::shared_ptr<Value>>& args) {
        auto method = get(name, activation);
        if (method && method->is_object()) {
            auto func_obj = method->as_object();
            if (func_obj) {
                return func_obj->call(name, activation, this_obj, args);
            }
        }
        
        // If not found or not callable, return undefined
        return std::make_shared<Value>(Value::UNDEFINED);
    }

    // Construct an object using this as constructor
    std::shared_ptr<Value> construct(std::shared_ptr<Activation> activation,
                                    std::shared_ptr<Object> callee,
                                    const std::vector<std::shared_ptr<Value>>& args) {
        // Create a new object with this object as prototype
        auto new_obj = std::make_shared<Object>(prototype_);
        
        // Call the constructor function
        if (constructor_) {
            return constructor_->construct(activation, callee, args);
        }
        
        return new_obj->as_value();
    }

    // Getters
    std::shared_ptr<Object> proto() const { return prototype_; }
    std::shared_ptr<FunctionObject> constr() const { return constructor_; }
    NativeObject& native() { return native_object_; }
    const NativeObject& native() const { return native_object_; }
    const std::string& type_name() const { return type_name_; }

    // Setters
    void set_proto(std::shared_ptr<Object> proto) { prototype_ = std::move(proto); }
    void set_constr(std::shared_ptr<FunctionObject> constr) { constructor_ = std::move(constr); }

    // Convert to value
    std::shared_ptr<Value> as_value() const {
        return std::make_shared<Value>(const_cast<Object*>(this));
    }

    // Check if this is a function
    bool is_function() const {
        return constructor_ != nullptr;
    }

    // Get as function object (if it is one)
    std::shared_ptr<FunctionObject> as_function() const {
        return is_function() ? constructor_ : nullptr;
    }
};

// Object handle for weak references
class ObjectHandle {
private:
    std::weak_ptr<Object> object_ref_;

public:
    explicit ObjectHandle(std::shared_ptr<Object> obj) 
        : object_ref_(obj) {}

    std::shared_ptr<Object> upgrade() const {
        return object_ref_.lock();
    }

    bool is_valid() const {
        return !object_ref_.expired();
    }
};

// Helper function to find a property in the prototype chain
std::pair<std::shared_ptr<Value>, int> find_property(
    std::shared_ptr<Object> this_obj,
    const std::string& name,
    std::shared_ptr<Activation> activation,
    bool call_resolve_fn = true) {
    
    auto proto = this_obj;
    int depth = 0;
    auto orig_proto = this_obj;
    
    while (proto) {
        if (depth == 255) {
            throw Avm1Exception(Avm1Error::prototype_recursion_limit());
        }
        
        // Check for local stored property
        if (proto->has_property(name, activation)) {
            auto value = proto->get(name, activation);
            return std::make_pair(value, depth);
        }
        
        proto = proto->proto();
        depth++;
    }
    
    // If resolve function should be called and we found one
    if (call_resolve_fn) {
        auto resolve_method = find_resolve_method(orig_proto, activation);
        if (resolve_method) {
            auto result = resolve_method->call("__resolve", activation, this_obj, 
                                            {std::make_shared<Value>(name)});
            return std::make_pair(result, 0);
        }
    }
    
    return std::make_pair(nullptr, -1);
}

// Helper function to find the resolve method in the prototype chain
std::shared_ptr<Object> find_resolve_method(
    std::shared_ptr<Object> proto,
    std::shared_ptr<Activation> activation) {
    
    int depth = 0;
    
    while (proto) {
        if (depth == 255) {
            throw Avm1Exception(Avm1Error::prototype_recursion_limit());
        }
        
        auto resolve_prop = proto->get("__resolve", activation);
        if (resolve_prop && resolve_prop->is_object()) {
            return resolve_prop->as_object();
        }
        
        proto = proto->proto();
        depth++;
    }
    
    return nullptr;
}

} // namespace ruffle

#endif // AVM1_OBJECT_H