// C++ translation of core/src/types.rs
// Percent and Degrees type wrappers

#ifndef RUFFLE_CORE_TYPES_H
#define RUFFLE_CORE_TYPES_H

#include <cstdint>
#include <cmath>

namespace ruffle {
namespace core {

/// Percent units for things that need to be stored as percentages.
///
/// This type-safe wrapper ensures that percentage values are handled
/// consistently throughout the codebase.
class Percent {
private:
    double percent_;

public:
    /// Default constructor - 0%
    constexpr Percent() : percent_(0.0) {}

    /// Constructor from percentage value
    ///
    /// \param percent The percentage value (e.g., 50.0 for 50%)
    explicit constexpr Percent(double percent) : percent_(percent) {}

    /// Return as percentage in [0.0, 100.0].
    ///
    /// \return The percentage value
    constexpr double percent() const { return percent_; }

    /// Convert a unit proportion in [0.0, 1.0] to percentage.
    ///
    /// \param unit The unit proportion (0.0 to 1.0)
    /// \return Percent with the converted value
    static constexpr Percent from_unit(double unit) {
        return Percent(unit * 100.0);
    }

    /// Return as unit proportion in [0.0, 1.0].
    ///
    /// \return The unit proportion
    constexpr double unit() const { return percent_ / 100.0; }

    /// Equality comparison
    constexpr bool operator==(const Percent& other) const {
        return percent_ == other.percent_;
    }

    /// Inequality comparison
    constexpr bool operator!=(const Percent& other) const {
        return percent_ != other.percent_;
    }

    /// Less than comparison
    constexpr bool operator<(const Percent& other) const {
        return percent_ < other.percent_;
    }

    /// Less than or equal comparison
    constexpr bool operator<=(const Percent& other) const {
        return percent_ <= other.percent_;
    }

    /// Greater than comparison
    constexpr bool operator>(const Percent& other) const {
        return percent_ > other.percent_;
    }

    /// Greater than or equal comparison
    constexpr bool operator>=(const Percent& other) const {
        return percent_ >= other.percent_;
    }

    /// Partial ordering
    constexpr auto operator<=>(const Percent& other) const {
        return percent_ <=> other.percent_;
    }
};

/// Degree units for things that need to be stored as degrees.
///
/// Actual degrees (0-360, or -179-180) can be stored in here by From and
/// Into coercions. No wrapping is done on the type to keep the conversion
/// lossless. To convert into radians (0-2π, or -π-π), use the from_radians
/// and into_radians methods.
///
/// No arithmetic operators are provided on degrees as most of the math they
/// are involved in should be done in unit proportions rather than percentages.
class Degrees {
private:
    double degrees_;

public:
    /// Default constructor - 0 degrees
    constexpr Degrees() : degrees_(0.0) {}

    /// Constructor from degree value
    ///
    /// \param degrees The degree value
    explicit constexpr Degrees(double degrees) : degrees_(degrees) {}

    /// Convert a radian value into degrees.
    ///
    /// \param rads The radian value
    /// \return Degrees with the converted value
    static constexpr Degrees from_radians(double rads) {
        constexpr double RAD2DEG = 180.0 / 3.14159265358979323846;
        return Degrees(rads * RAD2DEG);
    }

    /// Convert a degree value into radians.
    ///
    /// \return The radian value
    constexpr double into_radians() const {
        constexpr double DEG2RAD = 3.14159265358979323846 / 180.0;
        return degrees_ * DEG2RAD;
    }

    /// Get the degree value
    ///
    /// \return The degree value
    constexpr double degrees() const { return degrees_; }

    /// Equality comparison
    constexpr bool operator==(const Degrees& other) const {
        return degrees_ == other.degrees_;
    }

    /// Inequality comparison
    constexpr bool operator!=(const Degrees& other) const {
        return degrees_ != other.degrees_;
    }

    /// Less than comparison
    constexpr bool operator<(const Degrees& other) const {
        return degrees_ < other.degrees_;
    }

    /// Less than or equal comparison
    constexpr bool operator<=(const Degrees& other) const {
        return degrees_ <= other.degrees_;
    }

    /// Greater than comparison
    constexpr bool operator>(const Degrees& other) const {
        return degrees_ > other.degrees_;
    }

    /// Greater than or equal comparison
    constexpr bool operator>=(const Degrees& other) const {
        return degrees_ >= other.degrees_;
    }

    /// Partial ordering
    constexpr auto operator<=>(const Degrees& other) const {
        return degrees_ <=> other.degrees_;
    }
};

} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_TYPES_H
