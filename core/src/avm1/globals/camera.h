// C++ translation of core/src/avm1/globals/camera.rs
// Camera object

#ifndef RUFFLE_CORE_AVM1_GLOBALS_CAMERA_H
#define RUFFLE_CORE_AVM1_GLOBALS_CAMERA_H

#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Object;
    template<typename T> class Value;
    template<typename T> class Error;
    struct DeclContext;
    struct StaticDeclarations;
    template<typename T> class SystemClass;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace camera {

// Method indices for Camera
namespace method {
    constexpr uint16_t SET_MODE = 0;
    constexpr uint16_t SET_QUALITY = 1;
    constexpr uint16_t SET_KEY_FRAME_INTERVAL = 2;
    constexpr uint16_t SET_MOTION_LEVEL = 3;
    constexpr uint16_t SET_LOOPBACK = 4;
    constexpr uint16_t SET_CURSOR = 5;

    constexpr uint16_t INTERNAL_GET = 200;
    constexpr uint16_t GET_NAMES = 201;
}

// Property declarations for Camera prototype
extern const StaticDeclarations PROTO_DECLS;

// Property declarations for Camera object
extern const StaticDeclarations OBJECT_DECLS;

/// Create the Camera class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// Camera method handler
template<typename GC>
Value<GC> method(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count,
    uint16_t index
);

} // namespace camera
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_CAMERA_H
