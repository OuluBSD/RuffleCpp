// C++ translation of web/src/external_interface.rs
// External interface for JavaScript integration

#ifndef RUFFLE_WEB_EXTERNAL_INTERFACE_H
#define RUFFLE_WEB_EXTERNAL_INTERFACE_H

#include <cstdint>
#include <string>
#include <map>
#include <vector>
#include <optional>

// Forward declarations
namespace ruffle {
namespace web {
    struct JavascriptPlayer;
}
namespace core {
    namespace context {
        template<typename T> class UpdateContext;
    }
    namespace external {
        class ExternalInterfaceProvider;
        class FsCommandProvider;
        
        /// External interface value types
        enum class ValueType {
            Undefined,
            Null,
            Bool,
            Number,
            String,
            Object,
            List,
        };
        
        /// External interface value
        struct Value {
            ValueType type;
            bool bool_value;
            double number_value;
            std::string string_value;
            std::map<std::string, Value> object_value;
            std::vector<Value> list_value;
            
            static Value Undefined() { return {ValueType::Undefined}; }
            static Value Null() { return {ValueType::Null}; }
            static Value Bool(bool v) { return {ValueType::Bool, v}; }
            static Value Number(double v) { return {ValueType::Number, .number_value = v}; }
            static Value String(const std::string& v) { return {ValueType::String, .string_value = v}; }
            static Value Object(const std::map<std::string, Value>& v) { return {ValueType::Object, .object_value = v}; }
            static Value List(const std::vector<Value>& v) { return {ValueType::List, .list_value = v}; }
        };
    }
}
}

namespace ruffle {
namespace web {
namespace external_interface {

/// JavaScript interface for external communication
class JavascriptInterface {
public:
    /// Create a new JavaScript interface
    explicit JavascriptInterface(JavascriptPlayer js_player);
    
    /// Get the JavaScript player reference
    [[nodiscard]] const JavascriptPlayer& get_js_player() const { return js_player_; }

private:
    JavascriptPlayer js_player_;
};

/// Convert a JavaScript value to an external value
ExternalInterfaceValue js_to_external_value(const JsValue& js);

/// Convert an external value to a JavaScript value
JsValue external_to_js_value(const ExternalInterfaceValue& external);

} // namespace external_interface
} // namespace web
} // namespace ruffle

#endif // RUFFLE_WEB_EXTERNAL_INTERFACE_H
