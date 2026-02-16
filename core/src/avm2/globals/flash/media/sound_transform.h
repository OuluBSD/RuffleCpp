// C++ translation of core/src/avm2/globals/flash/media/sound_transform.rs
// flash.media.SoundTransform builtin/prototype

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_MEDIA_SOUND_TRANSFORM_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_MEDIA_SOUND_TRANSFORM_H

#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class TObject;
    namespace object {
        template<typename T> class SoundTransformObject;
    }
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace media {
namespace sound_transform {

/// Re-export sound_transform_allocator from the object module
template<typename GC>
using sound_transform_allocator = ruffle::core::avm2::object::sound_transform_allocator<GC>;

/// Implements SoundTransform.leftToRight getter
/// Returns the left-to-right mixing value
template<typename GC>
Value<GC> get_left_to_left(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements SoundTransform.leftToRight setter
/// Sets the left-to-right mixing value
template<typename GC>
Value<GC> set_left_to_left(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements SoundTransform.leftToRight getter
/// Returns the left-to-right mixing value
template<typename GC>
Value<GC> get_left_to_right(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements SoundTransform.leftToRight setter
/// Sets the left-to-right mixing value
template<typename GC>
Value<GC> set_left_to_right(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements SoundTransform.rightToLeft getter
/// Returns the right-to-left mixing value
template<typename GC>
Value<GC> get_right_to_left(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements SoundTransform.rightToLeft setter
/// Sets the right-to-left mixing value
template<typename GC>
Value<GC> set_right_to_left(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements SoundTransform.rightToRight getter
/// Returns the right-to-right mixing value
template<typename GC>
Value<GC> get_right_to_right(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements SoundTransform.rightToRight setter
/// Sets the right-to-right mixing value
template<typename GC>
Value<GC> set_right_to_right(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements SoundTransform.volume getter
/// Returns the volume value
template<typename GC>
Value<GC> get_volume(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements SoundTransform.volume setter
/// Sets the volume value
template<typename GC>
Value<GC> set_volume(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements SoundTransform.pan getter
/// Returns the pan value calculated from mixing coefficients
template<typename GC>
Value<GC> get_pan(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements SoundTransform.pan setter
/// Sets the pan value by adjusting left-to-left and right-to-right mixing
template<typename GC>
Value<GC> set_pan(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace sound_transform
} // namespace media
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_MEDIA_SOUND_TRANSFORM_H
