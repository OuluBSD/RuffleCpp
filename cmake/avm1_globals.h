/*
 * C++ header for AVM1 globals functionality
 * This replaces the functionality of core/src/avm1/globals.rs
 */

#ifndef AVM1_GLOBALS_H
#define AVM1_GLOBALS_H

#include "avm1.h"
#include "avm1/value.h"
#include "avm1/object.h"
#include "avm1/activation.h"
#include "avm1/error.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace ruffle {

// Global functions for AVM1
class Avm1Globals {
public:
    // Trace function - outputs a value to the trace output
    static std::shared_ptr<Value> trace(std::shared_ptr<Activation> activation,
                                       const std::vector<std::shared_ptr<Value>>& args) {
        if (args.empty()) {
            activation->context()->avm_trace("");
        } else {
            auto str_value = args[0]->coerce_to_string(activation);
            activation->context()->avm_trace(str_value);
        }
        return std::make_shared<Value>(Value::UNDEFINED);
    }

    // Check if a value is finite
    static std::shared_ptr<Value> is_finite(std::shared_ptr<Activation> activation,
                                           const std::vector<std::shared_ptr<Value>>& args) {
        if (args.empty()) {
            return std::make_shared<Value>(false);
        }
        
        double num = args[0]->coerce_to_number(activation);
        return std::make_shared<Value>(std::isfinite(num));
    }

    // Check if a value is NaN
    static std::shared_ptr<Value> is_nan(std::shared_ptr<Activation> activation,
                                        const std::vector<std::shared_ptr<Value>>& args) {
        if (args.empty()) {
            return std::make_shared<Value>(true);
        }
        
        double num = args[0]->coerce_to_number(activation);
        return std::make_shared<Value>(std::isnan(num));
    }

    // Parse integer from string
    static std::shared_ptr<Value> parse_int(std::shared_ptr<Activation> activation,
                                          const std::vector<std::shared_ptr<Value>>& args) {
        if (args.empty()) {
            return std::make_shared<Value>(Value::UNDEFINED);
        }

        std::string str = args[0]->coerce_to_string(activation);
        
        // Get radix if provided
        int radix = 10;
        if (args.size() > 1) {
            radix = static_cast<int>(args[1]->coerce_to_number(activation));
            if (radix < 2 || radix > 36) {
                return std::make_shared<Value>(std::numeric_limits<double>::quiet_NaN());
            }
        }

        // Handle hex prefix
        size_t start_pos = 0;
        if (radix == 0) {
            if (str.length() >= 2 && str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
                radix = 16;
                start_pos = 2;
            } else {
                radix = 10; // Default
            }
        } else if (radix == 16) {
            if (str.length() >= 2 && str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
                start_pos = 2;
            }
        }

        // Skip whitespace
        while (start_pos < str.length() && isspace(str[start_pos])) {
            start_pos++;
        }

        // Handle sign
        bool negative = false;
        if (start_pos < str.length() && (str[start_pos] == '+' || str[start_pos] == '-')) {
            negative = (str[start_pos] == '-');
            start_pos++;
        }

        // Parse digits
        double result = 0.0;
        bool has_digits = false;
        for (size_t i = start_pos; i < str.length(); ++i) {
            char c = str[i];
            int digit;
            
            if (c >= '0' && c <= '9') {
                digit = c - '0';
            } else if (c >= 'a' && c <= 'z') {
                digit = c - 'a' + 10;
            } else if (c >= 'A' && c <= 'Z') {
                digit = c - 'A' + 10;
            } else {
                break; // Stop at first invalid character
            }
            
            if (digit >= radix) {
                break; // Stop at first invalid digit for this radix
            }
            
            result = result * radix + digit;
            has_digits = true;
        }

        if (!has_digits) {
            return std::make_shared<Value>(std::numeric_limits<double>::quiet_NaN());
        }

        return std::make_shared<Value>(negative ? -result : result);
    }

    // Get infinity value based on SWF version
    static std::shared_ptr<Value> get_infinity(std::shared_ptr<Activation> activation,
                                              const std::vector<std::shared_ptr<Value>>& args) {
        if (activation->context()->swf_version() > 4) {
            return std::make_shared<Value>(std::numeric_limits<double>::infinity());
        } else {
            return std::make_shared<Value>(Value::UNDEFINED);
        }
    }

    // Get NaN value based on SWF version
    static std::shared_ptr<Value> get_nan(std::shared_ptr<Activation> activation,
                                         const std::vector<std::shared_ptr<Value>>& args) {
        if (activation->context()->swf_version() > 4) {
            return std::make_shared<Value>(std::numeric_limits<double>::quiet_NaN());
        } else {
            return std::make_shared<Value>(Value::UNDEFINED);
        }
    }

    // Parse float from string
    static std::shared_ptr<Value> parse_float(std::shared_ptr<Activation> activation,
                                            const std::vector<std::shared_ptr<Value>>& args) {
        if (args.empty()) {
            return std::make_shared<Value>(Value::UNDEFINED);
        }
        
        std::string str = args[0]->coerce_to_string(activation);
        
        // Simple float parsing
        try {
            size_t pos;
            double result = std::stod(str, &pos);
            // If we consumed the entire string, return the result
            if (pos == str.length()) {
                return std::make_shared<Value>(result);
            } else {
                // If there are remaining characters, return NaN
                return std::make_shared<Value>(std::numeric_limits<double>::quiet_NaN());
            }
        } catch (...) {
            return std::make_shared<Value>(std::numeric_limits<double>::quiet_NaN());
        }
    }

    // Set interval function (for timers)
    static std::shared_ptr<Value> set_interval(std::shared_ptr<Activation> activation,
                                             const std::vector<std::shared_ptr<Value>>& args) {
        return create_timer(activation, args, false);
    }

    // Set timeout function (for timers)
    static std::shared_ptr<Value> set_timeout(std::shared_ptr<Activation> activation,
                                            const std::vector<std::shared_ptr<Value>>& args) {
        return create_timer(activation, args, true);
    }

    // Clear interval function
    static std::shared_ptr<Value> clear_interval(std::shared_ptr<Activation> activation,
                                               const std::vector<std::shared_ptr<Value>>& args) {
        if (args.empty()) {
            return std::make_shared<Value>(Value::UNDEFINED);
        }
        
        int32_t id = static_cast<int32_t>(args[0]->coerce_to_number(activation));
        activation->context()->timers.remove(id);
        
        return std::make_shared<Value>(Value::UNDEFINED);
    }

    // Update after event function
    static std::shared_ptr<Value> update_after_event(std::shared_ptr<Activation> activation,
                                                   const std::vector<std::shared_ptr<Value>>& args) {
        activation->context()->needs_render = true;
        return std::make_shared<Value>(Value::UNDEFINED);
    }

    // Escape function for URL encoding
    static std::shared_ptr<Value> escape(std::shared_ptr<Activation> activation,
                                        const std::vector<std::shared_ptr<Value>>& args) {
        if (args.empty()) {
            return std::make_shared<Value>(Value::UNDEFINED);
        }
        
        std::string str = args[0]->coerce_to_string(activation);
        std::string result;
        
        for (unsigned char c : str) {
            // Characters that don't need escaping
            if ((c >= '0' && c <= '9') || 
                (c >= 'A' && c <= 'Z') || 
                (c >= 'a' && c <= 'z')) {
                result += static_cast<char>(c);
            } else {
                // Escape the character
                char buffer[4];
                snprintf(buffer, sizeof(buffer), "%%%02X", c);
                result += buffer;
            }
        }
        
        return std::make_shared<Value>(result);
    }

    // Unescape function for URL decoding
    static std::shared_ptr<Value> unescape(std::shared_ptr<Activation> activation,
                                          const std::vector<std::shared_ptr<Value>>& args) {
        if (args.empty()) {
            return std::make_shared<Value>(Value::UNDEFINED);
        }
        
        std::string str = args[0]->coerce_to_string(activation);
        std::string result;
        
        for (size_t i = 0; i < str.length(); ++i) {
            if (str[i] == '%' && i + 2 < str.length()) {
                // Try to parse the hex value
                char hex_str[3] = {str[i+1], str[i+2], '\0'};
                char* end_ptr;
                long val = strtol(hex_str, &end_ptr, 16);
                
                if (*end_ptr == '\0') {  // Successfully parsed
                    result += static_cast<char>(val);
                    i += 2;  // Skip the next two characters
                } else {
                    result += str[i];  // Not a valid hex sequence
                }
            } else if (str[i] == '+') {
                result += ' ';  // Convert + to space
            } else {
                result += str[i];
            }
        }
        
        return std::make_shared<Value>(result);
    }

private:
    // Helper function to create timers
    static std::shared_ptr<Value> create_timer(std::shared_ptr<Activation> activation,
                                             const std::vector<std::shared_ptr<Value>>& args,
                                             bool is_timeout) {
        if (args.size() < 2) {
            return std::make_shared<Value>(Value::UNDEFINED);
        }
        
        // In a real implementation, this would create a timer and return its ID
        // For now, we'll return a dummy ID
        int32_t interval = static_cast<int32_t>(args[1]->coerce_to_number(activation));
        
        // Create a timer callback based on the first argument
        // This would be more complex in a real implementation
        int32_t timer_id = activation->context()->timers.add_timer(interval, is_timeout);
        
        return std::make_shared<Value>(static_cast<double>(timer_id));
    }
};

// System prototypes structure
struct SystemPrototypes {
    std::shared_ptr<Object> button;
    std::shared_ptr<Object> object;
    std::shared_ptr<Object> object_constructor;
    std::shared_ptr<Object> function;
    std::shared_ptr<Object> movie_clip;
    std::shared_ptr<Object> text_field;
    std::shared_ptr<Object> text_format;
    std::shared_ptr<Object> array;
    std::shared_ptr<Object> array_constructor;
    std::shared_ptr<Object> xml_node_constructor;
    std::shared_ptr<Object> xml_constructor;
    std::shared_ptr<Object> matrix_constructor;
    std::shared_ptr<Object> point_constructor;
    std::shared_ptr<Object> rectangle;
    std::shared_ptr<Object> rectangle_constructor;
    std::shared_ptr<Object> transform_constructor;
    std::shared_ptr<Object> shared_object_constructor;
    std::shared_ptr<Object> color_transform_constructor;
    std::shared_ptr<Object> context_menu_constructor;
    std::shared_ptr<Object> context_menu_item_constructor;
    std::shared_ptr<Object> date_constructor;
    std::shared_ptr<Object> bitmap_data;
    std::shared_ptr<Object> video;
    std::shared_ptr<Object> blur_filter;
    std::shared_ptr<Object> bevel_filter;
    std::shared_ptr<Object> glow_filter;
    std::shared_ptr<Object> drop_shadow_filter;
    std::shared_ptr<Object> color_matrix_filter;
    std::shared_ptr<Object> displacement_map_filter;
    std::shared_ptr<Object> convolution_filter;
    std::shared_ptr<Object> gradient_bevel_filter;
    std::shared_ptr<Object> gradient_glow_filter;
};

// Constants for AVM depth handling
constexpr int32_t AVM_DEPTH_BIAS = 16384;
constexpr int32_t AVM_MAX_DEPTH = 2130706428;
constexpr int32_t AVM_MAX_REMOVE_DEPTH = 2130706416;

} // namespace ruffle

#endif // AVM1_GLOBALS_H