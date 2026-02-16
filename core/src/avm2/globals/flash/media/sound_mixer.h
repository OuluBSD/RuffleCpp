// C++ translation of core/src/avm2/globals/flash/media/sound_mixer.rs
// flash.media.SoundMixer builtin/prototype

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_MEDIA_SOUND_MIXER_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_MEDIA_SOUND_MIXER_H

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace media {
namespace sound_mixer {

/// Implements soundTransform's getter
///
/// This also implements SimpleButton's soundTransform property, as per
/// Flash Player behavior.
///
/// \param activation The current activation context
/// \param this_value The this value for the call
/// \param args Arguments passed to the getter
/// \return The current global sound transform as an AVM2 object
template<typename GC>
Value<GC> get_sound_transform(
    Activation<GC>* activation,
    Value<GC> this_value,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements soundTransform's setter
///
/// This also implements SimpleButton's soundTransform property, as per
/// Flash Player behavior.
///
/// \param activation The current activation context
/// \param this_value The this value for the call
/// \param args Arguments passed to the setter (first arg is the SoundTransform)
/// \return Value::Undefined
template<typename GC>
Value<GC> set_sound_transform(
    Activation<GC>* activation,
    Value<GC> this_value,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements SoundMixer.stopAll
///
/// Stops all currently playing sounds.
///
/// \param activation The current activation context
/// \param this_value The this value for the call
/// \param args Arguments (unused)
/// \return Value::Undefined
template<typename GC>
Value<GC> stop_all(
    Activation<GC>* activation,
    Value<GC> this_value,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements bufferTime's getter
///
/// Returns the current stream buffer time.
///
/// \param activation The current activation context
/// \param this_value The this value for the call
/// \param args Arguments (unused)
/// \return The buffer time in milliseconds
template<typename GC>
Value<GC> get_buffer_time(
    Activation<GC>* activation,
    Value<GC> this_value,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements bufferTime's setter
///
/// Sets the stream buffer time.
///
/// \param activation The current activation context
/// \param this_value The this value for the call
/// \param args Arguments (first arg is the buffer time in ms)
/// \return Value::Undefined
template<typename GC>
Value<GC> set_buffer_time(
    Activation<GC>* activation,
    Value<GC> this_value,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements SoundMixer.areSoundsInaccessible
///
/// Returns whether sounds are inaccessible due to security restrictions.
/// Currently returns false as a stub.
///
/// \param activation The current activation context
/// \param this_value The this value for the call
/// \param args Arguments (unused)
/// \return false (stub implementation)
template<typename GC>
Value<GC> are_sounds_inaccessible(
    Activation<GC>* activation,
    Value<GC> this_value,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements SoundMixer.computeSpectrum
///
/// Computes the frequency spectrum of the currently playing audio.
/// Writes the spectrum data to a ByteArray.
///
/// \param activation The current activation context
/// \param this_value The this value for the call
/// \param args Arguments:
///   - args[0]: ByteArray to write spectrum data to
///   - args[1]: FFT flag (boolean)
///   - args[2]: Stretch factor (int)
/// \return Value::Undefined
template<typename GC>
Value<GC> compute_spectrum(
    Activation<GC>* activation,
    Value<GC> this_value,
    const Value<GC>* args,
    size_t arg_count
);

} // namespace sound_mixer
} // namespace media
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_MEDIA_SOUND_MIXER_H
