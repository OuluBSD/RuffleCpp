// C++ translation of core/src/avm2/globals/flash/display/shape.rs
// flash.display.Shape builtin/prototype

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_SHAPE_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_SHAPE_H

#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class ClassObject;
    template<typename T> class Object;
}
namespace display_object {
    class Graphic;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace display {
namespace shape {

/// Shape allocator function
/// 
/// Creates a new Shape display object.
/// 
/// \param class The class object for Shape
/// \param activation The current activation context
/// \return A new Shape object
template<typename GC>
Object<GC> shape_allocator(
    ClassObject<GC> class_obj,
    Activation<GC>* activation
);

/// Implements the graphics property getter
/// 
/// Returns the Graphics object associated with this Shape.
/// Lazily initializes the Graphics object if it doesn't exist.
/// 
/// \param activation The current activation context
/// \param this_value The Shape instance
/// \param args Function arguments (unused)
/// \return The Graphics object, or undefined if not a valid Shape
template<typename GC>
Value<GC> get_graphics(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace shape
} // namespace display
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_SHAPE_H
