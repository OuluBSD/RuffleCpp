// C++ translation of swf/src/types/rectangle.rs
// Rectangle type for rectangular regions

#ifndef RUFFLE_SWF_TYPES_RECTANGLE_H
#define RUFFLE_SWF_TYPES_RECTANGLE_H

#include <cstdint>
#include <fmt/format.h>
#include "point.h"

// Forward declarations
namespace ruffle {
namespace swf {
    class Twips;
}
}

namespace ruffle {
namespace swf {
namespace types {

/// Coordinate trait for Rectangle types
///
/// Extends Point::Coordinate with ordering operations.
template<typename T>
concept RectangleCoordinate = requires(T a, T b) {
    requires types::Coordinate<T>;
    { a < b } -> std::same_as<bool>;
    { a > b } -> std::same_as<bool>;
    { a <= b } -> std::same_as<bool>;
    { a >= b } -> std::same_as<bool>;
    { a.min(b) } -> std::same_as<T>;
    { a.max(b) } -> std::same_as<T>;
    { a.clamp(b, b) } -> std::same_as<T>;
    T::INVALID;
};

/// RectangleCoordinate concept specialization for Twips
template<>
inline constexpr bool RectangleCoordinate<Twips> = true;

/// A rectangular region defined by minimum and maximum x- and y-coordinate positions.
///
/// \tparam T The coordinate type (must satisfy RectangleCoordinate concept)
template<typename T>
struct Rectangle {
    /// The minimum x-position of the rectangle.
    T x_min;

    /// The maximum x-position of the rectangle.
    T x_max;

    /// The minimum y-position of the rectangle.
    T y_min;

    /// The maximum y-position of the rectangle.
    T y_max;

    /// Invalid rectangle constant
    static const Rectangle INVALID;

    /// Zero rectangle constant
    static const Rectangle ZERO;

    /// Default constructor - creates invalid rectangle
    Rectangle() : x_min(T::INVALID), x_max(T::INVALID), y_min(T::INVALID), y_max(T::INVALID) {}

    /// Constructor with all coordinates
    ///
    /// \param x_min_ The minimum x coordinate
    /// \param x_max_ The maximum x coordinate
    /// \param y_min_ The minimum y coordinate
    /// \param y_max_ The maximum y coordinate
    constexpr Rectangle(T x_min_, T x_max_, T y_min_, T y_max_)
        : x_min(x_min_), x_max(x_max_), y_min(y_min_), y_max(y_max_) {}

    /// Get the width of the rectangle
    ///
    /// \return The width (x_max - x_min)
    [[nodiscard]] inline T width() const {
        return x_max - x_min;
    }

    /// Set the width of the rectangle
    ///
    /// \param width The new width
    inline void set_width(T width) {
        x_max = x_min + width;
    }

    /// Create a rectangle with the given width
    ///
    /// \param width The new width
    /// \return A new rectangle with the specified width
    [[nodiscard]] inline Rectangle with_width(T width) const {
        Rectangle result = *this;
        result.set_width(width);
        return result;
    }

    /// Get the height of the rectangle
    ///
    /// \return The height (y_max - y_min)
    [[nodiscard]] inline T height() const {
        return y_max - y_min;
    }

    /// Set the height of the rectangle
    ///
    /// \param height The new height
    inline void set_height(T height) {
        y_max = y_min + height;
    }

    /// Create a rectangle with the given height
    ///
    /// \param height The new height
    /// \return A new rectangle with the specified height
    [[nodiscard]] inline Rectangle with_height(T height) const {
        Rectangle result = *this;
        result.set_height(height);
        return result;
    }

    /// Check if a point is contained within this rectangle
    ///
    /// \param point The point to check
    /// \return true if the point is inside or on the boundary of the rectangle
    [[nodiscard]] bool contains(const Point<T>& point) const {
        return point.x >= x_min && point.x <= x_max &&
               point.y >= y_min && point.y <= y_max;
    }
};

/// Rectangle specialization for RectangleCoordinate types
template<RectangleCoordinate T>
struct Rectangle<T> {
    /// The minimum x-position of the rectangle.
    T x_min;

    /// The maximum x-position of the rectangle.
    T x_max;

    /// The minimum y-position of the rectangle.
    T y_min;

    /// The maximum y-position of the rectangle.
    T y_max;

    /// Invalid rectangle constant
    static const Rectangle INVALID;

    /// Zero rectangle constant
    static const Rectangle ZERO;

    /// Default constructor - creates invalid rectangle
    Rectangle() : x_min(T::INVALID), x_max(T::INVALID), y_min(T::INVALID), y_max(T::INVALID) {}

    /// Constructor with all coordinates
    ///
    /// \param x_min_ The minimum x coordinate
    /// \param x_max_ The maximum x coordinate
    /// \param y_min_ The minimum y coordinate
    /// \param y_max_ The maximum y coordinate
    constexpr Rectangle(T x_min_, T x_max_, T y_min_, T y_max_)
        : x_min(x_min_), x_max(x_max_), y_min(y_min_), y_max(y_max_) {}

    /// Get the width of the rectangle
    ///
    /// \return The width (x_max - x_min)
    [[nodiscard]] inline T width() const {
        return x_max - x_min;
    }

    /// Set the width of the rectangle
    ///
    /// \param width The new width
    inline void set_width(T width) {
        x_max = x_min + width;
    }

    /// Create a rectangle with the given width
    ///
    /// \param width The new width
    /// \return A new rectangle with the specified width
    [[nodiscard]] inline Rectangle with_width(T width) const {
        Rectangle result = *this;
        result.set_width(width);
        return result;
    }

    /// Get the height of the rectangle
    ///
    /// \return The height (y_max - y_min)
    [[nodiscard]] inline T height() const {
        return y_max - y_min;
    }

    /// Set the height of the rectangle
    ///
    /// \param height The new height
    inline void set_height(T height) {
        y_max = y_min + height;
    }

    /// Create a rectangle with the given height
    ///
    /// \param height The new height
    /// \return A new rectangle with the specified height
    [[nodiscard]] inline Rectangle with_height(T height) const {
        Rectangle result = *this;
        result.set_height(height);
        return result;
    }

    /// Check if a point is contained within this rectangle
    ///
    /// \param point The point to check
    /// \return true if the point is inside or on the boundary of the rectangle
    [[nodiscard]] bool contains(const Point<T>& point) const {
        return point.x >= x_min && point.x <= x_max &&
               point.y >= y_min && point.y <= y_max;
    }

    /// Check if the rectangle is valid
    ///
    /// \return true if the rectangle is valid (not INVALID)
    [[nodiscard]] inline bool is_valid() const {
        return x_min != T::INVALID;
    }

    /// Clamp a given point inside this rectangle.
    ///
    /// \param point The point to clamp
    /// \return The clamped point, or the original point if rectangle is invalid
    [[nodiscard]] Point<T> clamp(const Point<T>& point) const {
        if (is_valid()) {
            return Point<T>::new_(
                std::clamp(point.x, x_min, x_max),
                std::clamp(point.y, y_min, y_max)
            );
        }
        return point;
    }

    /// Encompass a point in this rectangle
    ///
    /// Expands the rectangle to include the given point.
    ///
    /// \param point The point to encompass
    /// \return A new rectangle that encompasses the point
    [[nodiscard]] Rectangle encompass(const Point<T>& point) const {
        Rectangle result = *this;
        if (result.is_valid()) {
            result.x_min = std::min(result.x_min, point.x);
            result.x_max = std::max(result.x_max, point.x);
            result.y_min = std::min(result.y_min, point.y);
            result.y_max = std::max(result.y_max, point.y);
        } else {
            result.x_min = point.x;
            result.x_max = point.x;
            result.y_min = point.y;
            result.y_max = point.y;
        }
        return result;
    }

    /// Union with another rectangle
    ///
    /// Creates a rectangle that encompasses both this and the other rectangle.
    ///
    /// \param other The other rectangle
    /// \return A new rectangle that is the union of both
    [[nodiscard]] Rectangle union_(const Rectangle& other) const {
        if (!is_valid()) {
            return other;
        }
        if (!other.is_valid()) {
            return *this;
        }
        Rectangle result = *this;
        result.x_min = std::min(result.x_min, other.x_min);
        result.x_max = std::max(result.x_max, other.x_max);
        result.y_min = std::min(result.y_min, other.y_min);
        result.y_max = std::max(result.y_max, other.y_max);
        return result;
    }

    /// Check if this rectangle intersects another
    ///
    /// \param other The other rectangle
    /// \return true if the rectangles intersect
    [[nodiscard]] bool intersects(const Rectangle& other) const {
        return is_valid() &&
               x_min <= other.x_max &&
               x_max >= other.x_min &&
               y_min <= other.y_max &&
               y_max >= other.y_min;
    }

    /// Grow the rectangle by a given amount
    ///
    /// Expands the rectangle uniformly in all directions.
    ///
    /// \param amount The amount to grow by
    /// \return A new grown rectangle
    [[nodiscard]] Rectangle grow(T amount) const {
        Rectangle result = *this;
        if (result.is_valid()) {
            result.x_min -= amount;
            result.x_max += amount;
            result.y_min -= amount;
            result.y_max += amount;
        }
        return result;
    }

    /// Grow the rectangle in the x direction only
    ///
    /// \param amount The amount to grow by
    /// \return A new grown rectangle
    [[nodiscard]] Rectangle grow_x(T amount) const {
        Rectangle result = *this;
        if (result.is_valid()) {
            result.x_min -= amount;
            result.x_max += amount;
        }
        return result;
    }

    /// Check if the rectangle is a point (zero width and height)
    ///
    /// \return true if x_min == x_max and y_min == y_max
    [[nodiscard]] bool is_point() const {
        return x_min == x_max && y_min == y_max;
    }

    /// Equality comparison
    bool operator==(const Rectangle& other) const {
        return x_min == other.x_min &&
               x_max == other.x_max &&
               y_min == other.y_min &&
               y_max == other.y_max;
    }

    /// Inequality comparison
    bool operator!=(const Rectangle& other) const {
        return !(*this == other);
    }
};

} // namespace types

// Re-export at swf namespace level for convenience
template<typename T>
using Rectangle = types::Rectangle<T>;

} // namespace swf
} // namespace ruffle

#endif // RUFFLE_SWF_TYPES_RECTANGLE_H
