// C++ translation of core/src/avm1/globals/color.rs
// Color object for AVM1 color transformations

#ifndef RUFFLE_CORE_AVM1_GLOBALS_COLOR_H
#define RUFFLE_CORE_AVM1_GLOBALS_COLOR_H

#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Error;
    template<typename T> class DisplayObject;
    template<typename T> class NativeObject;
    template<typename T> class Object;
    template<typename T> class Value;
    struct DeclContext;
    struct StaticDeclarations;
    template<typename T> class SystemClass;
}
}
namespace swf {
    struct ColorTransform;
    class Fixed8;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace color {

/// Property declarations for Color prototype
extern const StaticDeclarations PROTO_DECLS;

/// Create the Color class
///
/// \param context The declaration context for creating the class
/// \param super_proto The prototype object for the superclass
/// \return A SystemClass containing the created Color class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// Color constructor
///
/// Initializes the Color object with a target display object.
///
/// \param activation The current activation context
/// \param this_obj The Color object being constructed
/// \param args Arguments for the constructor (target display object)
/// \param arg_count Number of arguments
/// \return Value::Undefined
template<typename GC>
Value<GC> constructor(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Gets the target display object of this color transform.
///
/// \param activation The current activation context
/// \param this_obj The Color object
/// \return The target display object, or nullptr if none
template<typename GC>
DisplayObject<GC>* target(
    Activation<GC>* activation,
    Object<GC>* this_obj
);

/// Implements Color.getRGB method
///
/// Gets the RGB color value from the target's color transform.
///
/// \param activation The current activation context
/// \param this_obj The Color object
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return RGB value as integer, or Value::Undefined if no target
template<typename GC>
Value<GC> get_rgb(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Color.getTransform method
///
/// Gets the full color transform as an object with ra, ga, ba, aa, rb, gb, bb, ab properties.
///
/// \param activation The current activation context
/// \param this_obj The Color object
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return Object with transform properties, or Value::Undefined if no target
template<typename GC>
Value<GC> get_transform(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Color.setRGB method
///
/// Sets the RGB color of the target display object.
///
/// \param activation The current activation context
/// \param this_obj The Color object
/// \param args Arguments (RGB value as integer)
/// \param arg_count Number of arguments
/// \return Value::Undefined
template<typename GC>
Value<GC> set_rgb(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Color.setTransform method
///
/// Sets the full color transform from an object with ra, ga, ba, aa, rb, gb, bb, ab properties.
///
/// \param activation The current activation context
/// \param this_obj The Color object
/// \param args Arguments (transform object)
/// \param arg_count Number of arguments
/// \return Value::Undefined
template<typename GC>
Value<GC> set_transform(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Helper function to set color multiplier from transform object
///
/// \param activation The current activation context
/// \param transform The transform object
/// \param property The property name to read from
/// \param out Output value to set
/// \return Error if any
template<typename GC>
void set_color_mult(
    Activation<GC>* activation,
    Object<GC>* transform,
    const char* property,
    swf::Fixed8* out
);

/// Helper function to set color adder from transform object
///
/// \param activation The current activation context
/// \param transform The transform object
/// \param property The property name to read from
/// \param out Output value to set
/// \return Error if any
template<typename GC>
void set_color_add(
    Activation<GC>* activation,
    Object<GC>* transform,
    const char* property,
    int16_t* out
);

} // namespace color
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_COLOR_H
