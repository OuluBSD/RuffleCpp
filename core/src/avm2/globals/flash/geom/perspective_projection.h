// C++ translation of core/src/avm2/globals/flash/geom/perspective_projection.rs
// flash.geom.PerspectiveProjection implementation

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_GEOM_PERSPECTIVE_PROJECTION_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_GEOM_PERSPECTIVE_PROJECTION_H

#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class Object;
    namespace parameters {
        class ParametersExt;
    }
}
namespace render {
namespace perspective_projection {
    class PerspectiveProjection;
}
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace geom {
namespace perspective_projection {

/// Gets the width for perspective projection calculations
/// @param activation The current AVM2 activation
/// @param this_object The PerspectiveProjection object
template<typename GC>
double get_width(
    Activation<GC>* activation,
    Object<GC> this_object
);

/// Implements flash.geom.PerspectiveProjection.focalLength getter
/// Returns the focal length of the perspective projection
/// @param activation The current AVM2 activation
/// @param this_value The PerspectiveProjection object
template<typename GC>
Value<GC> get_focal_length(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.geom.PerspectiveProjection.focalLength setter
/// Sets the focal length of the perspective projection
/// @param activation The current AVM2 activation
/// @param this_value The PerspectiveProjection object
/// @param args Method arguments: focal_length (Number, must be > 0)
template<typename GC>
Value<GC> set_focal_length(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.geom.PerspectiveProjection.fieldOfView getter
/// Returns the field of view angle in degrees
/// @param activation The current AVM2 activation
/// @param this_value The PerspectiveProjection object
template<typename GC>
Value<GC> get_field_of_view(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.geom.PerspectiveProjection.fieldOfView setter
/// Sets the field of view angle in degrees (must be > 0 and < 180)
/// @param activation The current AVM2 activation
/// @param this_value The PerspectiveProjection object
/// @param args Method arguments: field_of_view (Number)
template<typename GC>
Value<GC> set_field_of_view(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.geom.PerspectiveProjection.projectionCenter getter
/// Returns the center point of the projection as a Point object
/// @param activation The current AVM2 activation
/// @param this_value The PerspectiveProjection object
template<typename GC>
Value<GC> get_projection_center(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.geom.PerspectiveProjection.projectionCenter setter
/// Sets the center point of the projection
/// @param activation The current AVM2 activation
/// @param this_value The PerspectiveProjection object
/// @param args Method arguments: center (Point)
template<typename GC>
Value<GC> set_projection_center(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.geom.PerspectiveProjection.toMatrix3D
/// Converts the perspective projection to a Matrix3D
/// @param activation The current AVM2 activation
/// @param this_value The PerspectiveProjection object
template<typename GC>
Value<GC> to_matrix_3d(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Syncs the perspective projection from the associated display object
/// @param activation The current AVM2 activation
/// @param this_object The PerspectiveProjection object
template<typename GC>
void sync_from_display_object(
    Activation<GC>* activation,
    Object<GC> this_object
);

/// Syncs the perspective projection to the associated display object
/// @param this_object The PerspectiveProjection object
template<typename GC>
void sync_to_display_object(
    Object<GC> this_object
);

} // namespace perspective_projection
} // namespace geom
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_GEOM_PERSPECTIVE_PROJECTION_H
