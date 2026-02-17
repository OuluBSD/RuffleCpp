// C++ translation of core/src/avm2/globals/flash/geom/transform.rs
// flash.geom.Transform implementation

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_GEOM_TRANSFORM_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_GEOM_TRANSFORM_H

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
    namespace vector {
        template<typename T> class VectorStorage;
    }
}
namespace prelude {
    class DisplayObject;
    class Matrix;
    class Twips;
}
namespace render {
namespace matrix3d {
    class Matrix3D;
}
namespace perspective_projection {
    class PerspectiveProjection;
}
}
}
namespace swf {
    class ColorTransform;
    class Rectangle;
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace geom {
namespace transform {

/// Gets the display object from a transform object
/// @param this_object The Transform object
template<typename GC>
DisplayObject get_display_object(
    Object<GC> this_object
);

/// Implements flash.geom.Transform.colorTransform getter
/// Returns the color transform of the display object
/// @param activation The current AVM2 activation
/// @param this_value The Transform object
template<typename GC>
Value<GC> get_color_transform(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.geom.Transform.colorTransform setter
/// Sets the color transform of the display object
/// @param activation The current AVM2 activation
/// @param this_value The Transform object
/// @param args Method arguments: colorTransform (ColorTransform)
template<typename GC>
Value<GC> set_color_transform(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.geom.Transform.matrix getter
/// Returns the transformation matrix of the display object
/// @param activation The current AVM2 activation
/// @param this_value The Transform object
template<typename GC>
Value<GC> get_matrix(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.geom.Transform.matrix setter
/// Sets the transformation matrix of the display object
/// @param activation The current AVM2 activation
/// @param this_value The Transform object
/// @param args Method arguments: matrix (Matrix)
template<typename GC>
Value<GC> set_matrix(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.geom.Transform.concatenatedMatrix getter
/// Returns the concatenated transformation matrix from root to this object
/// @param activation The current AVM2 activation
/// @param this_value The Transform object
template<typename GC>
Value<GC> get_concatenated_matrix(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Checks if a transform object has a Matrix3D stub
/// @param transform_object The Transform object
template<typename GC>
bool has_matrix3d_from_transform_object(
    Object<GC> transform_object
);

/// Gets the matrix from a transform object
/// @param transform_object The Transform object
template<typename GC>
Matrix matrix_from_transform_object(
    Object<GC> transform_object
);

/// Gets the color transform from a transform object
/// @param transform_object The Transform object
template<typename GC>
ColorTransform color_transform_from_transform_object(
    Object<GC> transform_object
);

/// Converts an Object to a ColorTransform
/// @param object The object to convert
/// @param activation The current AVM2 activation
template<typename GC>
ColorTransform object_to_color_transform(
    Object<GC> object,
    Activation<GC>* activation
);

/// Converts a ColorTransform to an Object
/// @param color_transform The ColorTransform to convert
/// @param activation The current AVM2 activation
template<typename GC>
Value<GC> color_transform_to_object(
    const ColorTransform& color_transform,
    Activation<GC>* activation
);

/// Converts a Matrix3D to an Object
/// @param matrix The Matrix3D to convert
/// @param activation The current AVM2 activation
template<typename GC>
Value<GC> matrix3d_to_object(
    const Matrix3D& matrix,
    Activation<GC>* activation
);

/// Converts an Object to a Matrix3D
/// @param object The object to convert
/// @param activation The current AVM2 activation
template<typename GC>
Matrix3D object_to_matrix3d(
    Object<GC> object,
    Activation<GC>* activation
);

/// Converts an Object to a PerspectiveProjection
/// @param object The object to convert
/// @param activation The current AVM2 activation
template<typename GC>
PerspectiveProjection object_to_perspective_projection(
    Object<GC> object,
    Activation<GC>* activation
);

/// Converts a Matrix to an Object
/// @param matrix The Matrix to convert
/// @param activation The current AVM2 activation
template<typename GC>
Value<GC> matrix_to_object(
    const Matrix& matrix,
    Activation<GC>* activation
);

/// Converts an Object to a Matrix
/// @param object The object to convert
/// @param activation The current AVM2 activation
template<typename GC>
Matrix object_to_matrix(
    Object<GC> object,
    Activation<GC>* activation
);

/// Implements flash.geom.Transform.pixelBounds getter
/// Returns the pixel bounds of the display object as a Rectangle
/// @param activation The current AVM2 activation
/// @param this_value The Transform object
template<typename GC>
Value<GC> get_pixel_bounds(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Converts a Rectangle to an Object
/// @param rectangle The Rectangle to convert
/// @param activation The current AVM2 activation
template<typename GC>
Value<GC> rectangle_to_object(
    const Rectangle& rectangle,
    Activation<GC>* activation
);

/// Implements flash.geom.Transform.matrix3D getter
/// Returns the 3D transformation matrix of the display object
/// @param activation The current AVM2 activation
/// @param this_value The Transform object
template<typename GC>
Value<GC> get_matrix_3d(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.geom.Transform.matrix3D setter
/// Sets the 3D transformation matrix of the display object
/// @param activation The current AVM2 activation
/// @param this_value The Transform object
/// @param args Method arguments: matrix3D (Matrix3D)
template<typename GC>
Value<GC> set_matrix_3d(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.geom.Transform.perspectiveProjection getter
/// Returns the perspective projection of the display object
/// @param activation The current AVM2 activation
/// @param this_value The Transform object
template<typename GC>
Value<GC> get_perspective_projection(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.geom.Transform.perspectiveProjection setter
/// Sets the perspective projection of the display object
/// @param activation The current AVM2 activation
/// @param this_value The Transform object
/// @param args Method arguments: perspectiveProjection (PerspectiveProjection)
template<typename GC>
Value<GC> set_perspective_projection(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.geom.Transform.getRelativeMatrix3D
/// Returns the relative 3D transformation matrix
/// @param activation The current AVM2 activation
/// @param this_value The Transform object
/// @param args Method arguments: relativeTo (Transform)
template<typename GC>
Value<GC> get_relative_matrix_3d(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace transform
} // namespace geom
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_GEOM_TRANSFORM_H
