// C++ translation of core/src/avm2/globals/flash/media/video.rs
// flash.media.Video implementation

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_MEDIA_VIDEO_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_MEDIA_VIDEO_H

#include <vector>
#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class Object;
    template<typename T> class ClassObject;
    namespace parameters {
        class ParametersExt;
    }
}
namespace display_object {
    template<typename T> class Video;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace media {
namespace video {

/// Allocator function for flash.media.Video objects
/// Creates a new Video display object or instantiates from a class symbol
/// @param class The class object to allocate
/// @param activation The current AVM2 activation
template<typename GC>
Object<GC> video_allocator(
    ClassObject<GC> class_,
    Activation<GC>* activation
);

/// Implements flash.media.Video.init method, called from the constructor
/// Initializes the video object with the specified dimensions
/// @param activation The current AVM2 activation
/// @param this_value The Video object
/// @param args Method arguments: width (int), height (int)
template<typename GC>
Value<GC> init(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.media.Video.attachNetStream
/// Attaches a NetStream to the video object for playback
/// @param activation The current AVM2 activation
/// @param this_value The Video object
/// @param args Method arguments: net_stream (NetStream)
template<typename GC>
Value<GC> attach_net_stream(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace video
} // namespace media
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_MEDIA_VIDEO_H
