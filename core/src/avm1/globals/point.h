// C++ translation of core/src/avm1/globals/point.rs
// flash.geom.Point object for AVM1

#ifndef RUFFLE_CORE_AVM1_GLOBALS_POINT_H
#define RUFFLE_CORE_AVM1_GLOBALS_POINT_H

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
namespace point {

/// Property declarations for Point prototype
extern const StaticDeclarations PROTO_DECLS;

/// Property declarations for Point object (static properties)
extern const StaticDeclarations OBJECT_DECLS;

/// Create the Point class
///
/// \param context The declaration context for creating the class
/// \param super_proto The prototype object for the superclass
/// \return A SystemClass containing the created Point class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// Creates a Point object from x,y coordinates.
///
/// \param point The x,y coordinates as a pair
/// \param activation The current activation context
/// \return The created Point object
template<typename GC>
Value<GC> point_to_object(
    const std::pair<double, double>& point,
    Activation<GC>* activation
);

/// Constructs a new Point object with the given arguments.
///
/// \param args Arguments (x, y coordinates)
/// \param arg_count Number of arguments
/// \param activation The current activation context
/// \return The constructed Point object
template<typename GC>
Value<GC> construct_new_point(
    const Value<GC>* args,
    size_t arg_count,
    Activation<GC>* activation
);

/// Extracts x,y coordinates from a Value (object or primitive).
///
/// \param value The value to extract coordinates from
/// \param activation The current activation context
/// \return The x,y coordinates as a pair
template<typename GC>
std::pair<double, double> value_to_point(
    const Value<GC>& value,
    Activation<GC>* activation
);

/// Extracts x,y coordinates from an Object.
///
/// \param object The object to extract coordinates from
/// \param activation The current activation context
/// \return The x,y coordinates as a pair
template<typename GC>
std::pair<double, double> object_to_point(
    Object<GC>* object,
    Activation<GC>* activation
);

/// Point constructor
///
/// Initializes the Point object with x,y coordinates.
///
/// \param activation The current activation context
/// \param this_obj The Point object being constructed
/// \param args Arguments for the constructor (x, y coordinates)
/// \param arg_count Number of arguments
/// \return Value::Undefined
template<typename GC>
Value<GC> constructor(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Point.clone method
///
/// Creates a copy of this Point object.
///
/// \param activation The current activation context
/// \param this_obj The Point object
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return A new Point object with the same coordinates
template<typename GC>
Value<GC> clone(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Point.equals method
///
/// Compares this Point with another Point for equality.
///
/// \param activation The current activation context
/// \param this_obj The Point object
/// \param args Arguments (other Point to compare)
/// \param arg_count Number of arguments
/// \return true if points are equal, false otherwise
template<typename GC>
Value<GC> equals(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Point.add method
///
/// Adds another Point to this Point.
///
/// \param activation The current activation context
/// \param this_obj The Point object
/// \param args Arguments (other Point to add)
/// \param arg_count Number of arguments
/// \return A new Point object with the sum
template<typename GC>
Value<GC> add(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Point.subtract method
///
/// Subtracts another Point from this Point.
///
/// \param activation The current activation context
/// \param this_obj The Point object
/// \param args Arguments (other Point to subtract)
/// \param arg_count Number of arguments
/// \return A new Point object with the difference
template<typename GC>
Value<GC> subtract(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Point.distance static method
///
/// Calculates the distance between two Points.
///
/// \param activation The current activation context
/// \param this_obj The Point object (unused for static method)
/// \param args Arguments (point1, point2)
/// \param arg_count Number of arguments
/// \return The distance between the points
template<typename GC>
Value<GC> distance(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Point.polar static method
///
/// Creates a Point from polar coordinates.
///
/// \param activation The current activation context
/// \param this_obj The Point object (unused for static method)
/// \param args Arguments (length, angle in radians)
/// \param arg_count Number of arguments
/// \return A new Point object
template<typename GC>
Value<GC> polar(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Point.interpolate static method
///
/// Interpolates between two Points by a factor.
///
/// \param activation The current activation context
/// \param this_obj The Point object (unused for static method)
/// \param args Arguments (point1, point2, factor)
/// \param arg_count Number of arguments
/// \return A new interpolated Point object, or NaN point if insufficient args
template<typename GC>
Value<GC> interpolate(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Point.toString method
///
/// Returns a string representation of the Point.
///
/// \param activation The current activation context
/// \param this_obj The Point object
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return String in format "(x=X, y=Y)"
template<typename GC>
Value<GC> to_string(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Point.length getter
///
/// Gets the length (magnitude) of the Point vector.
///
/// \param activation The current activation context
/// \param this_obj The Point object
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return The length of the vector
template<typename GC>
Value<GC> length(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Point.normalize method
///
/// Scales the Point to the specified length.
///
/// \param activation The current activation context
/// \param this_obj The Point object
/// \param args Arguments (new length)
/// \param arg_count Number of arguments
/// \return Value::Undefined
template<typename GC>
Value<GC> normalize(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Point.offset method
///
/// Offsets the Point by the specified dx, dy values.
///
/// \param activation The current activation context
/// \param this_obj The Point object
/// \param args Arguments (dx, dy)
/// \param arg_count Number of arguments
/// \return Value::Undefined
template<typename GC>
Value<GC> offset(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

} // namespace point
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_POINT_H
