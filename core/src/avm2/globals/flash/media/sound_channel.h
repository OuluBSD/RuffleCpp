// C++ translation of core/src/avm2/globals/flash/media/sound_channel.rs
//! flash.media.SoundChannel builtin/prototype

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_MEDIA_SOUND_CHANNEL_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_MEDIA_SOUND_CHANNEL_H

#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class Object;

    namespace object {
        template<typename GC>
        using sound_channel_allocator = /* would come from object module */ void;
    }
}
}
}

namespace ruffle {
namespace core {
namespace display_object {
    struct SoundTransform;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace media {
namespace sound_channel {

/// Re-export sound_channel_allocator from object module
template<typename GC>
using sound_channel_allocator = ruffle::core::avm2::object::sound_channel_allocator<GC>;

/// Implements SoundChannel.leftPeak
template<typename GC>
Value<GC> get_left_peak(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements SoundChannel.rightPeak
template<typename GC>
Value<GC> get_right_peak(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Impl SoundChannel.position
template<typename GC>
Value<GC> get_position(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements soundTransform's getter
template<typename GC>
Value<GC> get_sound_transform(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements soundTransform's setter
template<typename GC>
Value<GC> set_sound_transform(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Impl SoundChannel.stop
template<typename GC>
Value<GC> stop(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace sound_channel
} // namespace media
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_MEDIA_SOUND_CHANNEL_H
