// C++ translation of core/src/html/dimensions.rs
//! CSS dimension types for HTML layout

#ifndef RUFFLE_CORE_HTML_DIMENSIONS_H
#define RUFFLE_CORE_HTML_DIMENSIONS_H

#include <cstdint>
#include <algorithm>
#include <type_traits>

// Forward declarations
namespace ruffle {
namespace core {
    namespace swf {
        struct Rectangle;
        struct Twips;
        template<typename T>
        struct PointDelta;
    }
}
}

namespace ruffle_render {
namespace matrix {
    class Matrix;
}
}

namespace ruffle {
namespace core {
namespace html {
namespace dimensions {

using swf::Twips;
using swf::Rectangle;
using swf::PointDelta;

/// A type which represents the top-left position of a layout box.
template<typename T>
class Position {
private:
    T x_;
    T y_;

public:
    Position() : x_(T{}), y_(T{}) {}

    Position(T x, T y) : x_(x), y_(y) {}

    // Default constructor
    static Position zero() {
        return Position();
    }

    // Create from pair
    static Position from_pair(std::pair<T, T> pair) {
        return Position(pair.first, pair.second);
    }

    // Getters
    T x() const { return x_; }
    T y() const { return y_; }

    // Setters
    void set_x(T x) { x_ = x; }
    void set_y(T y) { y_ = y; }

    // Operators
    Position operator+(const Position& other) const {
        return Position(x_ + other.x_, y_ + other.y_);
    }

    Position& operator+=(const Position& other) {
        x_ += other.x_;
        y_ += other.y_;
        return *this;
    }

    Position operator-(const Position& other) const {
        return Position(x_ - other.x_, y_ - other.y_);
    }

    Position& operator-=(const Position& other) {
        x_ -= other.x_;
        y_ -= other.y_;
        return *this;
    }

    bool operator==(const Position& other) const {
        return x_ == other.x_ && y_ == other.y_;
    }

    bool operator!=(const Position& other) const {
        return !(*this == other);
    }
};

/// A type which represents the size of a layout box.
template<typename T>
class Size {
private:
    T width_;
    T height_;

public:
    Size() : width_(T{}), height_(T{}) {}

    Size(T width, T height) : width_(width), height_(height) {}

    // Default constructor
    static Size zero() {
        return Size();
    }

    // Create from pair
    static Size from_pair(std::pair<T, T> pair) {
        return Size(pair.first, pair.second);
    }

    // Create from Position
    static Size from_position(const Position<T>& pos) {
        return Size(pos.x(), pos.y());
    }

    // Getters
    T width() const { return width_; }
    T height() const { return height_; }

    bool operator==(const Size& other) const {
        return width_ == other.width_ && height_ == other.height_;
    }

    bool operator!=(const Size& other) const {
        return !(*this == other);
    }
};

// Specialization for Twips conversion from PointDelta
template<>
inline Size<Twips> Size<Twips>::from_pair(std::pair<Twips, Twips> pair) {
    return Size(pair.first, pair.second);
}

// Conversion from PointDelta<Twips> to Size<Twips>
inline Size<Twips> size_from_point_delta(const PointDelta<Twips>& delta) {
    return Size<Twips>(delta.dx, delta.dy);
}

// Conversion from Size<Twips> to PointDelta<Twips>
inline PointDelta<Twips> point_delta_from_size(const Size<Twips>& size) {
    return PointDelta<Twips>{size.width(), size.height()};
}

// Matrix multiplication for Size<Twips>
inline Size<Twips> operator*(const ruffle_render::matrix::Matrix& matrix, const Size<Twips>& size) {
    // Size has the same semantics as PointDelta when applying a matrix
    PointDelta<Twips> delta = point_delta_from_size(size);
    PointDelta<Twips> result = matrix * delta;
    return size_from_point_delta(result);
}

/// A type which represents the offset and size of a text box.
template<typename T>
class BoxBounds {
private:
    T offset_x_;
    T extent_x_;
    T offset_y_;
    T extent_y_;

public:
    BoxBounds()
        : offset_x_(T{})
        , extent_x_(T{})
        , offset_y_(T{})
        , extent_y_(T{})
    {}

    BoxBounds(T offset_x, T extent_x, T offset_y, T extent_y)
        : offset_x_(offset_x)
        , extent_x_(extent_x)
        , offset_y_(offset_y)
        , extent_y_(extent_y)
    {}

    // Create from position and size
    static BoxBounds from_position_and_size(const Position<T>& pos, const Size<T>& size) {
        return BoxBounds(
            pos.x(),
            pos.x() + size.width(),
            pos.y(),
            pos.y() + size.height()
        );
    }

    // Getters
    T offset_x() const { return offset_x_; }
    T offset_y() const { return offset_y_; }
    T extent_x() const { return extent_x_; }
    T extent_y() const { return extent_y_; }

    // Get origin position
    Position<T> origin() const {
        return Position<T>(offset_x_, offset_y_);
    }

    // Get extent position
    Position<T> extent() const {
        return Position<T>(extent_x_, extent_y_);
    }

    // Get width
    T width() const {
        return extent_x_ - offset_x_;
    }

    // Get height
    T height() const {
        return extent_y_ - offset_y_;
    }

    // Convert to position and size
    std::pair<Position<T>, Size<T>> into_position_and_size() const {
        T width = extent_x_ - offset_x_;
        T height = extent_y_ - offset_y_;
        return std::make_pair(
            Position<T>(offset_x_, offset_y_),
            Size<T>(width, height)
        );
    }

    // Check if position is contained
    bool contains(const Position<T>& local_position) const {
        return local_position.x() >= offset_x_ &&
               local_position.x() <= extent_x_ &&
               local_position.y() >= offset_y_ &&
               local_position.y() <= extent_y_;
    }

    // Create with new size
    BoxBounds with_size(const Size<T>& new_size) const {
        return BoxBounds(
            offset_x_,
            offset_x_ + new_size.width(),
            offset_y_,
            offset_y_ + new_size.height()
        );
    }

    // Create with new width
    BoxBounds with_width(T new_width) const {
        return BoxBounds(
            offset_x_,
            offset_x_ + new_width,
            offset_y_,
            extent_y_
        );
    }

    // Union with another bounds
    BoxBounds operator+(const BoxBounds& other) const {
        return BoxBounds(
            std::min(offset_x_, other.offset_x_),
            std::max(extent_x_, other.extent_x_),
            std::min(offset_y_, other.offset_y_),
            std::max(extent_y_, other.extent_y_)
        );
    }

    BoxBounds& operator+=(const BoxBounds& other) {
        offset_x_ = std::min(offset_x_, other.offset_x_);
        extent_x_ = std::max(extent_x_, other.extent_x_);
        offset_y_ = std::min(offset_y_, other.offset_y_);
        extent_y_ = std::max(extent_y_, other.extent_y_);
        return *this;
    }

    // Translate by position
    BoxBounds operator+(const Position<T>& rhs) const {
        return BoxBounds(
            offset_x_ + rhs.x(),
            extent_x_ + rhs.x(),
            offset_y_ + rhs.y(),
            extent_y_ + rhs.y()
        );
    }

    BoxBounds& operator+=(const Position<T>& rhs) {
        offset_x_ += rhs.x();
        extent_x_ += rhs.x();
        offset_y_ += rhs.y();
        extent_y_ += rhs.y();
        return *this;
    }

    // Expand by size
    BoxBounds operator+(const Size<T>& rhs) const {
        return BoxBounds(
            offset_x_,
            extent_x_ + rhs.width(),
            offset_y_,
            extent_y_ + rhs.height()
        );
    }

    BoxBounds& operator+=(const Size<T>& rhs) {
        extent_x_ += rhs.width();
        extent_y_ += rhs.height();
        return *this;
    }

    bool operator==(const BoxBounds& other) const {
        return offset_x_ == other.offset_x_ &&
               extent_x_ == other.extent_x_ &&
               offset_y_ == other.offset_y_ &&
               extent_y_ == other.extent_y_;
    }

    bool operator!=(const BoxBounds& other) const {
        return !(*this == other);
    }
};

// Conversion from BoxBounds<T> to Rectangle<Twips>
template<typename T>
inline Rectangle rectangle_from_box_bounds(const BoxBounds<T>& bounds) {
    return Rectangle{
        static_cast<Twips>(bounds.offset_x()),
        static_cast<Twips>(bounds.extent_x()),
        static_cast<Twips>(bounds.offset_y()),
        static_cast<Twips>(bounds.extent_y())
    };
}

// Conversion from Rectangle<Twips> to BoxBounds<T>
template<typename T>
inline BoxBounds<T> box_bounds_from_rectangle(const Rectangle& rect) {
    return BoxBounds<T>(
        T::from(rect.x_min),
        T::from(rect.x_max),
        T::from(rect.y_min),
        T::from(rect.y_max)
    );
}

// Matrix multiplication for BoxBounds<Twips>
inline BoxBounds<Twips> operator*(const ruffle_render::matrix::Matrix& matrix, const BoxBounds<Twips>& bounds) {
    // BoxBounds have the same semantics as Rectangle when applying a matrix
    Rectangle rect = rectangle_from_box_bounds(bounds);
    Rectangle result = matrix * rect;
    return box_bounds_from_rectangle<Twips>(result);
}

} // namespace dimensions
} // namespace html
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_HTML_DIMENSIONS_H
