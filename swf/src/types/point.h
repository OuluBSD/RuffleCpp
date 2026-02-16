// C++ translation of swf/src/types/point.rs
// Point and PointDelta types for 2D coordinates

#ifndef RUFFLE_SWF_TYPES_POINT_H
#define RUFFLE_SWF_TYPES_POINT_H

#include <cstdint>
#include <fmt/format.h>

// Forward declarations
namespace ruffle {
namespace swf {
    class Twips;
}
}

namespace ruffle {
namespace swf {
namespace types {

/// Coordinate trait for Point types
///
/// Defines the operations required for a type to be used as a coordinate.
template<typename T>
concept Coordinate = requires(T a, T b, int32_t i) {
    { a + b } -> std::same_as<T>;
    { a - b } -> std::same_as<T>;
    { a * i } -> std::same_as<T>;
    { a / i } -> std::same_as<T>;
    { -a } -> std::same_as<T>;
    { a += b } -> std::same_as<T&>;
    { a -= b } -> std::same_as<T&>;
    { a *= i } -> std::same_as<T&>;
    { a /= i } -> std::same_as<T&>;
    { fmt::format("{}", a) } -> std::same_as<std::string>;
    T::ZERO;
};

/// Coordinate concept specialization for int32_t
template<>
inline constexpr bool Coordinate<int32_t> = true;

/// Coordinate concept specialization for Twips
template<>
inline constexpr bool Coordinate<Twips> = true;

/// A 2D position defined by x and y coordinates.
///
/// \tparam T The coordinate type (must satisfy Coordinate concept)
template<Coordinate T>
struct Point {
    /// X coordinate
    T x;

    /// Y coordinate
    T y;

    /// The Point object with a value of (0, 0).
    static const Point ZERO;

    /// Default constructor
    Point() : x(T::ZERO), y(T::ZERO) {}

    /// Constructor with coordinates
    ///
    /// \param x_ The x coordinate
    /// \param y_ The y coordinate
    constexpr Point(T x_, T y_) : x(x_), y(y_) {}

    /// Create a new Point with the given coordinates
    ///
    /// \param x_ The x coordinate
    /// \param y_ The y coordinate
    /// \return A new Point
    [[nodiscard]] static constexpr Point new_(T x_, T y_) {
        return Point(x_, y_);
    }
};

/// Point specialization for Twips with pixel conversion
template<>
struct Point<Twips> {
    /// X coordinate in twips
    Twips x;

    /// Y coordinate in twips
    Twips y;

    /// The Point object with a value of (0, 0).
    static const Point<Twips> ZERO;

    /// Default constructor
    Point() : x(Twips::ZERO), y(Twips::ZERO) {}

    /// Constructor with twips coordinates
    ///
    /// \param x_ The x coordinate in twips
    /// \param y_ The y coordinate in twips
    constexpr Point(Twips x_, Twips y_) : x(x_), y(y_) {}

    /// Create a new Point with the given coordinates
    ///
    /// \param x_ The x coordinate
    /// \param y_ The y coordinate
    /// \return A new Point
    [[nodiscard]] static constexpr Point new_(Twips x_, Twips y_) {
        return Point(x_, y_);
    }

    /// Converts the given number of pixels into twips.
    ///
    /// This may be a lossy conversion; any precision more than a twip (1/20 pixels) is truncated.
    ///
    /// \param x The x coordinate in pixels
    /// \param y The y coordinate in pixels
    /// \return A new Point with coordinates converted to twips
    [[nodiscard]] static Point from_pixels(double x, double y);
};

/// A difference between two 2D points.
///
/// \tparam T The coordinate type (must satisfy Coordinate concept)
template<Coordinate T>
struct PointDelta {
    /// Delta X coordinate
    T dx;

    /// Delta Y coordinate
    T y;

    /// The PointDelta object with a value of (0, 0).
    static const PointDelta ZERO;

    /// Default constructor
    PointDelta() : dx(T::ZERO), dy(T::ZERO) {}

    /// Constructor with delta coordinates
    ///
    /// \param dx_ The delta x coordinate
    /// \param dy_ The delta y coordinate
    constexpr PointDelta(T dx_, T dy_) : dx(dx_), dy(dy_) {}

    /// Create a new PointDelta with the given coordinates
    ///
    /// \param dx_ The delta x coordinate
    /// \param dy_ The delta y coordinate
    /// \return A new PointDelta
    [[nodiscard]] static constexpr PointDelta new_(T dx_, T dy_) {
        return PointDelta(dx_, dy_);
    }

    /// Multiply delta by scalar
    ///
    /// \param other The scalar multiplier
    /// \return The scaled delta
    [[nodiscard]] inline PointDelta operator*(int32_t other) const {
        return PointDelta(dx * other, dy * other);
    }

    /// Multiply-assign delta by scalar
    ///
    /// \param other The scalar multiplier
    /// \return Reference to this delta
    inline PointDelta& operator*=(int32_t other) {
        dx *= other;
        dy *= other;
        return *this;
    }

    /// Divide delta by scalar
    ///
    /// \param other The scalar divisor
    /// \return The scaled delta
    [[nodiscard]] inline PointDelta operator/(int32_t other) const {
        return PointDelta(dx / other, dy / other);
    }

    /// Divide-assign delta by scalar
    ///
    /// \param other The scalar divisor
    /// \return Reference to this delta
    inline PointDelta& operator/=(int32_t other) {
        dx /= other;
        dy /= other;
        return *this;
    }

    /// Negate delta
    ///
    /// \return The negated delta
    [[nodiscard]] inline PointDelta operator-() const {
        return PointDelta(-dx, -dy);
    }

    /// Equality comparison
    bool operator==(const PointDelta& other) const {
        return dx == other.dx && dy == other.dy;
    }

    /// Inequality comparison
    bool operator!=(const PointDelta& other) const {
        return !(*this == other);
    }
};

/// PointDelta specialization for Twips with pixel conversion
template<>
struct PointDelta<Twips> {
    /// Delta X coordinate in twips
    Twips dx;

    /// Delta Y coordinate in twips
    Twips dy;

    /// The PointDelta object with a value of (0, 0).
    static const PointDelta<Twips> ZERO;

    /// Default constructor
    PointDelta() : dx(Twips::ZERO), dy(Twips::ZERO) {}

    /// Constructor with twips delta coordinates
    ///
    /// \param dx_ The delta x coordinate in twips
    /// \param dy_ The delta y coordinate in twips
    constexpr PointDelta(Twips dx_, Twips dy_) : dx(dx_), dy(dy_) {}

    /// Create a new PointDelta with the given coordinates
    ///
    /// \param dx_ The delta x coordinate
    /// \param dy_ The delta y coordinate
    /// \return A new PointDelta
    [[nodiscard]] static constexpr PointDelta new_(Twips dx_, Twips dy_) {
        return PointDelta(dx_, dy_);
    }

    /// Converts the given number of pixels into twips.
    ///
    /// This may be a lossy conversion; any precision more than a twip (1/20 pixels) is truncated.
    ///
    /// \param dx The delta x coordinate in pixels
    /// \param dy The delta y coordinate in pixels
    /// \return A new PointDelta with coordinates converted to twips
    [[nodiscard]] static PointDelta from_pixels(double dx, double dy);

    /// Multiply delta by scalar
    ///
    /// \param other The scalar multiplier
    /// \return The scaled delta
    [[nodiscard]] inline PointDelta operator*(int32_t other) const {
        return PointDelta(dx * other, dy * other);
    }

    /// Multiply-assign delta by scalar
    ///
    /// \param other The scalar multiplier
    /// \return Reference to this delta
    inline PointDelta& operator*=(int32_t other) {
        dx *= other;
        dy *= other;
        return *this;
    }

    /// Divide delta by scalar
    ///
    /// \param other The scalar divisor
    /// \return The scaled delta
    [[nodiscard]] inline PointDelta operator/(int32_t other) const {
        return PointDelta(dx / other, dy / other);
    }

    /// Divide-assign delta by scalar
    ///
    /// \param other The scalar divisor
    /// \return Reference to this delta
    inline PointDelta& operator/=(int32_t other) {
        dx /= other;
        dy /= other;
        return *this;
    }

    /// Negate delta
    ///
    /// \return The negated delta
    [[nodiscard]] inline PointDelta operator-() const {
        return PointDelta(-dx, -dy);
    }

    /// Equality comparison
    bool operator==(const PointDelta& other) const {
        return dx == other.dx && dy == other.dy;
    }

    /// Inequality comparison
    bool operator!=(const PointDelta& other) const {
        return !(*this == other);
    }
};

/// Add delta to point
template<Coordinate T>
inline Point<T> operator+(Point<T> point, const PointDelta<T>& delta) {
    return Point<T>(point.x + delta.dx, point.y + delta.dy);
}

/// Add-assign delta to point
template<Coordinate T>
inline Point<T>& operator+=(Point<T>& point, const PointDelta<T>& delta) {
    point.x += delta.dx;
    point.y += delta.dy;
    return point;
}

/// Subtract delta from point
template<Coordinate T>
inline Point<T> operator-(Point<T> point, const PointDelta<T>& delta) {
    return Point<T>(point.x - delta.dx, point.y - delta.dy);
}

/// Subtract-assign delta from point
template<Coordinate T>
inline Point<T>& operator-=(Point<T>& point, const PointDelta<T>& delta) {
    point.x -= delta.dx;
    point.y -= delta.dy;
    return point;
}

/// Subtract point from point to get delta
template<Coordinate T>
inline PointDelta<T> operator-(const Point<T>& a, const Point<T>& b) {
    return PointDelta<T>(a.x - b.x, a.y - b.y);
}

} // namespace types

// Re-export at swf namespace level for convenience
template<typename T>
using Point = types::Point<T>;

template<typename T>
using PointDelta = types::PointDelta<T>;

} // namespace swf
} // namespace ruffle

// Define static constants
namespace ruffle {
namespace swf {
namespace types {

template<Coordinate T>
const Point<T> Point<T>::ZERO = Point<T>(T::ZERO, T::ZERO);

template<Coordinate T>
const PointDelta<T> PointDelta<T>::ZERO = PointDelta<T>(T::ZERO, T::ZERO);

} // namespace types
} // namespace swf
} // namespace ruffle

#endif // RUFFLE_SWF_TYPES_POINT_H
