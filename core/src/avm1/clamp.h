// C++ translation of core/src/avm1/clamp.rs
// Clamping utilities for AVM1

#ifndef RUFFLE_CORE_AVM1_CLAMP_H
#define RUFFLE_CORE_AVM1_CLAMP_H

#include <cstdint>
#include <cmath>
#include <limits>

namespace ruffle {
namespace core {
namespace avm1 {
namespace clamp {

/// Clamp trait for bounding values
///
/// Provides methods to clamp values while handling special cases like NaN.
template<typename T>
struct Clamp {
    /// Clamp a value also handling NaN
    ///
    /// Unlike standard clamp, this handles NaN by returning the smallest value
    /// from the numbers provided rather than propagating NaN.
    ///
    /// \param value The value to clamp
    /// \param min The minimum bound
    /// \param max The maximum bound
    /// \return The clamped value
    static T clamp_also_nan(T value, T min, T max) {
        return std::max(min, std::min(value, max));
    }

    /// Clamp a value to i32 range
    ///
    /// Clamps NaN and out-of-range (including infinite) values to i32::MIN.
    ///
    /// \param value The value to clamp
    /// \return The clamped i32 value
    static int32_t clamp_to_i32(T value) {
        constexpr int32_t i32_min = std::numeric_limits<int32_t>::min();
        constexpr int32_t i32_max = std::numeric_limits<int32_t>::max();

        // Clamp NaN and out-of-range (including infinite) values to i32::MIN
        if (value >= static_cast<T>(i32_min) && value <= static_cast<T>(i32_max)) {
            return static_cast<int32_t>(value);
        } else {
            return i32_min;
        }
    }
};

// Specialization for f64 (double)
template<>
struct Clamp<double> {
    /// Clamp a double value also handling NaN
    ///
    /// (f64::NAN).clamp(min, max) causes the code to propagate NaN rather
    /// than returning either max or min. Instead this function returns
    /// the smallest value from the numbers provided.
    ///
    /// \param value The value to clamp
    /// \param min The minimum bound
    /// \param max The maximum bound
    /// \return The clamped value
    static double clamp_also_nan(double value, double min, double max) {
        return std::max(min, std::min(value, max));
    }

    /// Clamp a double to i32 range
    ///
    /// Clamps NaN and out-of-range (including infinite) values to i32::MIN.
    ///
    /// \param value The value to clamp
    /// \return The clamped i32 value
    static int32_t clamp_to_i32(double value) {
        constexpr int32_t i32_min = std::numeric_limits<int32_t>::min();
        constexpr int32_t i32_max = std::numeric_limits<int32_t>::max();

        // Clamp NaN and out-of-range (including infinite) values to i32::MIN
        if (value >= static_cast<double>(i32_min) && value <= static_cast<double>(i32_max)) {
            return static_cast<int32_t>(value);
        } else {
            return i32_min;
        }
    }
};

} // namespace clamp
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_CLAMP_H
