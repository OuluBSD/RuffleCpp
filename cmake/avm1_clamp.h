/*
 * C++ header for AVM1 clamp functionality
 * This replaces the functionality of core/src/avm1/clamp.rs
 */

#ifndef AVM1_CLAMP_H
#define AVM1_CLAMP_H

#include <cmath>
#include <limits>
#include <algorithm>

namespace ruffle {

// Clamp functionality for floating-point values
class Clamp {
public:
    // Clamp a value with special handling for NaN
    // Returns the value clamped between min and max, with special handling for NaN
    static double clamp_also_nan(double value, double min_val, double max_val) {
        // Handle NaN case: return the smallest value among value, min_val, max_val
        if (std::isnan(value)) {
            // For NaN, return the smallest of the three values
            double result = min_val;
            if (min_val > max_val) result = max_val;
            return result;
        }
        
        // Standard clamping: max(min(value, max_val), min_val)
        return std::min(std::max(value, min_val), max_val);
    }

    // Clamp a floating-point value to i32 range
    static int32_t clamp_to_i32(double value) {
        // Clamp NaN and out-of-range (including infinite) values to i32::MIN
        if (std::isnan(value) || value < static_cast<double>(std::numeric_limits<int32_t>::min()) || 
            value > static_cast<double>(std::numeric_limits<int32_t>::max())) {
            return std::numeric_limits<int32_t>::min();
        }
        
        // Value is in range, convert to i32
        return static_cast<int32_t>(value);
    }

    // Clamp a floating-point value to u32 range
    static uint32_t clamp_to_u32(double value) {
        // Clamp NaN and out-of-range (including infinite) values to 0
        if (std::isnan(value) || value < 0.0 || 
            value > static_cast<double>(std::numeric_limits<uint32_t>::max())) {
            return 0;
        }
        
        // Value is in range, convert to u32
        return static_cast<uint32_t>(value);
    }

    // Clamp a floating-point value to i16 range
    static int16_t clamp_to_i16(double value) {
        // Clamp NaN and out-of-range values to i16::MIN
        if (std::isnan(value) || value < static_cast<double>(std::numeric_limits<int16_t>::min()) || 
            value > static_cast<double>(std::numeric_limits<int16_t>::max())) {
            return std::numeric_limits<int16_t>::min();
        }
        
        // Value is in range, convert to i16
        return static_cast<int16_t>(value);
    }

    // Clamp a floating-point value to u16 range
    static uint16_t clamp_to_u16(double value) {
        // Clamp NaN and out-of-range values to 0
        if (std::isnan(value) || value < 0.0 || 
            value > static_cast<double>(std::numeric_limits<uint16_t>::max())) {
            return 0;
        }
        
        // Value is in range, convert to u16
        return static_cast<uint16_t>(value);
    }
};

// Convenience function for clamping any floating-point value
template<typename T>
T clamp_value(T value, T min_val, T max_val) {
    return Clamp::clamp_also_nan(static_cast<double>(value), 
                                static_cast<double>(min_val), 
                                static_cast<double>(max_val));
}

} // namespace ruffle

#endif // AVM1_CLAMP_H