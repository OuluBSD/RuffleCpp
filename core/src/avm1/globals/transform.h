// C++ translation of core/src/avm1/globals/transform.rs
//! flash.geom.Transform

#ifndef RUFFLE_CORE_AVM1_GLOBALS_TRANSFORM_H
#define RUFFLE_CORE_AVM1_GLOBALS_TRANSFORM_H

#include <cstdint>
#include <vector>
#include <optional>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename T> class AvmString;
namespace avm1 {
    class Activation;
    class Error;
    template<typename T> class Object;
    template<typename T> class Value;
    enum class NativeObject;
namespace object_reference {
    template<typename T> class MovieClipReference;
}
namespace property_decl {
    class DeclContext;
    class StaticDeclarations;
    template<typename T> class SystemClass;
}
}
namespace display_object {
    template<typename T> class DisplayObject;
    template<typename T> TDisplayObject;
    enum class BoundsMode;
}
namespace swf {
    class Rectangle;
    class Twips;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace transform {

/// Transform object for flash.geom.Transform
///
/// Holds a reference to a movie clip whose transform properties
/// can be accessed and modified.
template<typename GC>
class TransformObject {
public:
    TransformObject()
        : clip_(std::nullopt)
    {}

    /// Create a new Transform object
    ///
    /// \param activation The current activation context
    /// \param args Constructor arguments (expects 1 MovieClip argument)
    /// \return Optional TransformObject if successful
    static std::optional<TransformObject> create(
        Activation* activation,
        const std::vector<Value<GC>>& args
    );

    /// Get the clip this transform is associated with
    ///
    /// \param activation The current activation context
    /// \return The display object, or nullopt if unavailable
    std::optional<display_object::DisplayObject<GC>> clip(Activation* activation) const;

private:
    std::optional<MovieClipReference<GC>> clip_;
};

/// Property declaration indices for Transform prototype
namespace method {
    constexpr uint16_t CONSTRUCTOR = 0;
    constexpr uint16_t GET_MATRIX = 101;
    constexpr uint16_t SET_MATRIX = 102;
    constexpr uint16_t GET_CONCATENATED_MATRIX = 103;
    constexpr uint16_t GET_COLOR_TRANSFORM = 105;
    constexpr uint16_t SET_COLOR_TRANSFORM = 106;
    constexpr uint16_t GET_CONCATENATED_COLOR_TRANSFORM = 107;
    constexpr uint16_t GET_PIXEL_BOUNDS = 109;
}

/// Create the Transform class
///
/// \param context The declaration context
/// \param super_proto The superclass prototype
/// \return The created system class
template<typename GC>
property_decl::SystemClass<GC> create_class(
    property_decl::DeclContext* context,
    Object<GC> super_proto
);

/// Method handler for Transform properties
///
/// Handles all Transform methods and properties based on the index.
///
/// \param activation The current activation context
/// \param this_object The this object
/// \param args Method arguments
/// \param index The method/property index
/// \return The result value
template<typename GC>
Value<GC> method(
    Activation* activation,
    Object<GC> this_object,
    const std::vector<Value<GC>>& args,
    uint16_t index
);

} // namespace transform
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_TRANSFORM_H
