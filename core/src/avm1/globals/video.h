// C++ translation of core/src/avm1/globals/video.rs
// Video class implementation for AVM1

#ifndef RUFFLE_CORE_AVM1_GLOBALS_VIDEO_H
#define RUFFLE_CORE_AVM1_GLOBALS_VIDEO_H

#include <cstddef>
#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Error;
    template<typename T> class NativeObject;
    template<typename T> class Object;
    template<typename T> class Value;
    struct DeclContext;
    struct StaticDeclarations;
    template<typename T> class SystemClass;
}
namespace display_object {
    template<typename T> class Video;
}
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace video {

// Property declaration indices for Video prototype
namespace method {
    constexpr uint16_t ATTACH_VIDEO = 0;
}

/// Property declarations for Video prototype
extern const StaticDeclarations PROTO_DECLS;

/// Create the Video class
/// 
/// \param context The declaration context for creating the class
/// \param super_proto The prototype object for the superclass
/// \return A SystemClass containing the created Video class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// Implements Video.attachVideo method
/// 
/// Attaches a NetStream to the video object for playback.
/// Warns if the source object is not a NetStream.
/// 
/// \param video The Video display object
/// \param activation The current activation context
/// \param args Arguments passed to the method (first arg should be NetStream)
/// \return Value::Undefined
template<typename GC>
Value<GC> attach_video(
    display_object::Video<GC> video,
    Activation<GC>* activation,
    const Value<GC>* args
);

} // namespace video
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_VIDEO_H
