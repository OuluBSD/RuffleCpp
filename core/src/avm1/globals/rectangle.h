// C++ translation of core/src/avm1/globals/rectangle.rs
//! flash.geom.Rectangle object for AVM1

#ifndef RUFFLE_CORE_AVM1_GLOBALS_RECTANGLE_H
#define RUFFLE_CORE_AVM1_GLOBALS_RECTANGLE_H

#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Error;
    template<typename T> class Object;
    template<typename T> class Value;
    struct DeclContext;
    struct StaticDeclarations;
    template<typename T> class SystemClass;
}
namespace string {
    template<typename T> class AvmString;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace rectangle {

/// Property declarations for Rectangle prototype
extern const StaticDeclarations PROTO_DECLS;

/// Create the Rectangle class
///
/// \param context The declaration context for creating the class
/// \param super_proto The prototype object for the superclass
/// \return A SystemClass containing the created Rectangle class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// Rectangle constructor
///
/// Initializes the Rectangle object with x, y, width, height.
/// If no arguments, sets all to 0.
///
/// \param activation The current activation context
/// \param this_obj The Rectangle object being constructed
/// \param args Arguments (x, y, width, height)
/// \return Value::Undefined
template<typename GC>
Value<GC> constructor(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Rectangle.toString method
///
/// Returns a string representation of the Rectangle.
///
/// \param activation The current activation context
/// \param this_obj The Rectangle object
/// \param args Arguments (unused)
/// \return String in format "(x=X, y=Y, w=W, h=H)"
template<typename GC>
Value<GC> to_string(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Rectangle.isEmpty method
///
/// Returns true if the rectangle is empty (width or height <= 0 or NaN).
///
/// \param activation The current activation context
/// \param this_obj The Rectangle object
/// \param args Arguments (unused)
/// \return true if empty, false otherwise
template<typename GC>
Value<GC> is_empty(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Rectangle.setEmpty method
///
/// Sets the rectangle to empty (x=0, y=0, width=0, height=0).
///
/// \param activation The current activation context
/// \param this_obj The Rectangle object
/// \param args Arguments (unused)
/// \return Value::Undefined
template<typename GC>
Value<GC> set_empty(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Rectangle.clone method
///
/// Creates a copy of this Rectangle object.
///
/// \param activation The current activation context
/// \param this_obj The Rectangle object
/// \param args Arguments (unused)
/// \return A new Rectangle object with the same values
template<typename GC>
Value<GC> clone(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Rectangle.contains method
///
/// Checks if a point (x, y) is inside this rectangle.
///
/// \param activation The current activation context
/// \param this_obj The Rectangle object
/// \param args Arguments (x, y coordinates)
/// \return true if point is inside, false otherwise, undefined for NaN
template<typename GC>
Value<GC> contains(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Rectangle.containsPoint method
///
/// Checks if a Point object is inside this rectangle.
///
/// \param activation The current activation context
/// \param this_obj The Rectangle object
/// \param args Arguments (Point object)
/// \return true if point is inside, false otherwise, undefined for NaN
template<typename GC>
Value<GC> contains_point(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Rectangle.containsRectangle method
///
/// Checks if another rectangle is completely inside this rectangle.
///
/// \param activation The current activation context
/// \param this_obj The Rectangle object
/// \param args Arguments (other Rectangle object)
/// \return true if other is inside, false otherwise, undefined for NaN
template<typename GC>
Value<GC> contains_rectangle(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Rectangle.intersects method
///
/// Checks if this rectangle intersects another rectangle.
///
/// \param activation The current activation context
/// \param this_obj The Rectangle object
/// \param args Arguments (other Rectangle object)
/// \return true if rectangles intersect, false otherwise
template<typename GC>
Value<GC> intersects(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Rectangle.union method
///
/// Returns the union of this rectangle and another rectangle.
///
/// \param activation The current activation context
/// \param this_obj The Rectangle object
/// \param args Arguments (other Rectangle object)
/// \return A new Rectangle object representing the union
template<typename GC>
Value<GC> union_(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Rectangle.intersection method
///
/// Returns the intersection of this rectangle and another rectangle.
///
/// \param activation The current activation context
/// \param this_obj The Rectangle object
/// \param args Arguments (other Rectangle object)
/// \return A new Rectangle object representing the intersection
template<typename GC>
Value<GC> intersection(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Rectangle.equals method
///
/// Checks if this rectangle equals another rectangle.
///
/// \param activation The current activation context
/// \param this_obj The Rectangle object
/// \param args Arguments (other Rectangle object)
/// \return true if rectangles are equal, false otherwise
template<typename GC>
Value<GC> equals(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Rectangle.inflate method
///
/// Inflates the rectangle by the specified horizontal and vertical amounts.
///
/// \param activation The current activation context
/// \param this_obj The Rectangle object
/// \param args Arguments (horizontal, vertical amounts)
/// \return Value::Undefined
template<typename GC>
Value<GC> inflate(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Rectangle.inflatePoint method
///
/// Inflates the rectangle by a Point's x and y values.
///
/// \param activation The current activation context
/// \param this_obj The Rectangle object
/// \param args Arguments (Point object with horizontal, vertical amounts)
/// \return Value::Undefined
template<typename GC>
Value<GC> inflate_point(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Rectangle.offset method
///
/// Offsets the rectangle by the specified horizontal and vertical amounts.
///
/// \param activation The current activation context
/// \param this_obj The Rectangle object
/// \param args Arguments (horizontal, vertical amounts)
/// \return Value::Undefined
template<typename GC>
Value<GC> offset(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Rectangle.offsetPoint method
///
/// Offsets the rectangle by a Point's x and y values.
///
/// \param activation The current activation context
/// \param this_obj The Rectangle object
/// \param args Arguments (Point object with horizontal, vertical amounts)
/// \return Value::Undefined
template<typename GC>
Value<GC> offset_point(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Rectangle.left getter
///
/// Gets the x coordinate of the left edge.
///
/// \param activation The current activation context
/// \param this_obj The Rectangle object
/// \param args Arguments (unused)
/// \return The x coordinate
template<typename GC>
Value<GC> get_left(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Rectangle.left setter
///
/// Sets the x coordinate, adjusting width to maintain right edge.
///
/// \param activation The current activation context
/// \param this_obj The Rectangle object
/// \param args Arguments (new left value)
/// \return Value::Undefined
template<typename GC>
Value<GC> set_left(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Rectangle.right getter
///
/// Gets the x coordinate of the right edge (x + width).
///
/// \param activation The current activation context
/// \param this_obj The Rectangle object
/// \param args Arguments (unused)
/// \return The right edge x coordinate
template<typename GC>
Value<GC> get_right(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Rectangle.right setter
///
/// Sets the right edge by adjusting width.
///
/// \param activation The current activation context
/// \param this_obj The Rectangle object
/// \param args Arguments (new right value)
/// \return Value::Undefined
template<typename GC>
Value<GC> set_right(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Rectangle.top getter
///
/// Gets the y coordinate of the top edge.
///
/// \param activation The current activation context
/// \param this_obj The Rectangle object
/// \param args Arguments (unused)
/// \return The y coordinate
template<typename GC>
Value<GC> get_top(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Rectangle.top setter
///
/// Sets the y coordinate, adjusting height to maintain bottom edge.
///
/// \param activation The current activation context
/// \param this_obj The Rectangle object
/// \param args Arguments (new top value)
/// \return Value::Undefined
template<typename GC>
Value<GC> set_top(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Rectangle.bottom getter
///
/// Gets the y coordinate of the bottom edge (y + height).
///
/// \param activation The current activation context
/// \param this_obj The Rectangle object
/// \param args Arguments (unused)
/// \return The bottom edge y coordinate
template<typename GC>
Value<GC> get_bottom(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Rectangle.bottom setter
///
/// Sets the bottom edge by adjusting height.
///
/// \param activation The current activation context
/// \param this_obj The Rectangle object
/// \param args Arguments (new bottom value)
/// \return Value::Undefined
template<typename GC>
Value<GC> set_bottom(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Rectangle.topLeft getter
///
/// Gets a Point object representing the top-left corner.
///
/// \param activation The current activation context
/// \param this_obj The Rectangle object
/// \param args Arguments (unused)
/// \return A new Point object with (x, y)
template<typename GC>
Value<GC> get_top_left(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Rectangle.topLeft setter
///
/// Sets the top-left corner, adjusting width and height.
///
/// \param activation The current activation context
/// \param this_obj The Rectangle object
/// \param args Arguments (Point object with new x, y)
/// \return Value::Undefined
template<typename GC>
Value<GC> set_top_left(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Rectangle.bottomRight getter
///
/// Gets a Point object representing the bottom-right corner.
///
/// \param activation The current activation context
/// \param this_obj The Rectangle object
/// \param args Arguments (unused)
/// \return A new Point object with (right, bottom)
template<typename GC>
Value<GC> get_bottom_right(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Rectangle.bottomRight setter
///
/// Sets the bottom-right corner, adjusting width and height.
///
/// \param activation The current activation context
/// \param this_obj The Rectangle object
/// \param args Arguments (Point object with new right, bottom)
/// \return Value::Undefined
template<typename GC>
Value<GC> set_bottom_right(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Rectangle.size getter
///
/// Gets a Point object representing the width and height.
///
/// \param activation The current activation context
/// \param this_obj The Rectangle object
/// \param args Arguments (unused)
/// \return A new Point object with (width, height)
template<typename GC>
Value<GC> get_size(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Rectangle.size setter
///
/// Sets the size from a Point object.
///
/// \param activation The current activation context
/// \param this_obj The Rectangle object
/// \param args Arguments (Point object with new width, height)
/// \return Value::Undefined
template<typename GC>
Value<GC> set_size(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

} // namespace rectangle
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_RECTANGLE_H
