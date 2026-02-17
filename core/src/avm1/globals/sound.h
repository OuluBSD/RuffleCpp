// C++ translation of core/src/avm1/globals/sound.rs
//! AVM1 Sound object

#ifndef RUFFLE_CORE_AVM1_GLOBALS_SOUND_H
#define RUFFLE_CORE_AVM1_GLOBALS_SOUND_H

#include <cstdint>
#include <vector>
#include <optional>
#include <memory>
#include <string>
#include <cell>
#include <mutex>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC>
    class AvmString;
    template<typename GC>
    class Value;
    template<typename GC>
    class Object;
    template<typename GC>
    class NativeObject;
    template<typename GC>
    class Activation;
    template<typename GC>
    class Error;
    template<typename GC>
    class FunctionObject;
    class UpdateContext;

    namespace avm1 {
        enum class Attribute;
        enum class ExecutionReason;
        template<typename GC>
        class ArrayBuilder;
    }

    namespace backend {
        namespace audio {
            struct SoundHandle;
            struct SoundInstanceHandle;
        }
    }

    namespace display_object {
        template<typename GC>
        class DisplayObject;
        template<typename GC>
        class TDisplayObject;
        struct SoundTransform;
    }

    namespace character {
        enum class Character;
    }

    namespace swf {
        struct SoundInfo;
        enum class SoundEvent;
    }

    namespace gc_arena {
        template<typename T>
        class Gc;
        template<typename T>
        class RefLock;
        class Mutation;
    }
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace sound {

using gc_arena::Gc;
using gc_arena::RefLock;
using gc_arena::Mutation;
using backend::audio::SoundHandle;
using backend::audio::SoundInstanceHandle;
using display_object::DisplayObject;
using display_object::SoundTransform;

/// Queued play request for loading sounds
template<typename GC>
struct QueuedPlay {
    Object<GC> sound_object;
    double start_offset;
    uint16_t loops;
};

/// Sound state enum
enum class SoundState {
    /// Empty sound object, no sound playback allowed.
    Empty,

    /// Sound is loading, plays can be queued.
    Loading,

    /// Sound is loaded, plays can be started immediately.
    Loaded
};

/// A Sound object that is tied to a sound from the AudioBackend.
template<typename GC>
class Sound {
private:
    Gc<GC, SoundData<GC>> data_;

public:
    explicit Sound(Gc<GC, SoundData<GC>> data)
        : data_(data)
    {}

    /// Create an empty sound
    static Sound empty(Mutation<GC>* mc, std::optional<DisplayObject<GC>> owner) {
        SoundData<GC> data;
        data.state = RefLock::new(SoundState::Empty);
        data.sound_instance = std::nullopt;
        data.owner = owner;
        data.position = 0;
        data.duration = std::nullopt;
        data.is_streaming = false;
        return Sound(Gc<GC, SoundData<GC>>::new(mc, data));
    }

    /// Get duration
    std::optional<uint32_t> duration() const {
        return data_->duration;
    }

    /// Set duration
    void set_duration(std::optional<uint32_t> duration) {
        data_->duration = duration;
    }

    /// Get sound handle
    std::optional<SoundHandle> sound() const {
        if (data_->state.get() == SoundState::Loaded) {
            return data_->sound_handle;
        }
        return std::nullopt;
    }

    /// Get sound instance handle
    std::optional<SoundInstanceHandle> sound_instance() const {
        return data_->sound_instance;
    }

    /// Set sound instance handle
    void set_sound_instance(std::optional<SoundInstanceHandle> handle) {
        data_->sound_instance = handle;
    }

    /// Get owner display object
    std::optional<DisplayObject<GC>> owner() const {
        return data_->owner;
    }

    /// Get position in milliseconds
    uint32_t position() const {
        return data_->position;
    }

    /// Set position in milliseconds
    void set_position(uint32_t position) {
        data_->position = position;
    }

    /// Check if sound is streaming
    bool is_streaming() const {
        return data_->is_streaming;
    }

    /// Set streaming flag
    void set_is_streaming(bool is_streaming) {
        data_->is_streaming = is_streaming;
    }

    /// Play the sound
    void play(QueuedPlay<GC> play, UpdateContext<GC>* context);

    /// Set loading state
    void set_is_loading(UpdateContext<GC>* context);

    /// Load sound from handle
    void load_sound(
        Activation<GC>* activation,
        Object<GC> sound_object,
        SoundHandle sound
    );

    /// Load ID3 tags from bytes
    Result<void, Error<GC>> load_id3(
        Activation<GC>* activation,
        Object<GC> sound_object,
        const std::vector<uint8_t>& bytes
    );
};

/// Internal data for Sound
template<typename GC>
struct SoundData {
    RefLock<SoundState> state;
    std::optional<SoundInstanceHandle> sound_instance;
    std::optional<DisplayObject<GC>> owner;
    uint32_t position;
    std::optional<uint32_t> duration;
    bool is_streaming;

    // For loaded state
    std::optional<SoundHandle> sound_handle;
};

// Property declarations for Sound class
constexpr const char* PROTO_DECLS[] = {
    // Note: id3 is not a built-in property. See Sound::load_id3.
    // Note: duration is defined later. See Sound::load_sound.
    // Note: position is defined later. See Sound::load_sound.
    "getPan",
    "getTransform",
    "getVolume",
    "setPan",
    "setTransform",
    "setVolume",
    "stop",
    "attachSound",
    "start",
    "getDuration",
    "setDuration",
    "getPosition",
    "setPosition",
    "loadSound",
    "getBytesLoaded",
    "getBytesTotal"
};

/// Create the Sound class
template<typename GC>
struct SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC> super_proto
);

/// Implements Sound constructor
template<typename GC>
Result<Value<GC>, Error<GC>> constructor(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
);

/// Implements Sound.attachSound
template<typename GC>
Result<Value<GC>, Error<GC>> attach_sound(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
);

/// Implements Sound.duration getter
template<typename GC>
Result<Value<GC>, Error<GC>> duration(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
);

/// Implements Sound.setDuration
template<typename GC>
Result<Value<GC>, Error<GC>> set_duration(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
);

/// Implements Sound.getBytesLoaded
template<typename GC>
Result<Value<GC>, Error<GC>> get_bytes_loaded(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
);

/// Implements Sound.getBytesTotal
template<typename GC>
Result<Value<GC>, Error<GC>> get_bytes_total(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
);

/// Implements Sound.getPan
template<typename GC>
Result<Value<GC>, Error<GC>> get_pan(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
);

/// Implements Sound.getTransform
template<typename GC>
Result<Value<GC>, Error<GC>> get_transform(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
);

/// Implements Sound.getVolume
template<typename GC>
Result<Value<GC>, Error<GC>> get_volume(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
);

/// Implements Sound.loadSound
template<typename GC>
Result<Value<GC>, Error<GC>> load_sound(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
);

/// Implements Sound.position getter
template<typename GC>
Result<Value<GC>, Error<GC>> position(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
);

/// Implements Sound.setPan
template<typename GC>
Result<Value<GC>, Error<GC>> set_pan(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
);

/// Implements Sound.setTransform
template<typename GC>
Result<Value<GC>, Error<GC>> set_transform(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
);

/// Implements Sound.setVolume
template<typename GC>
Result<Value<GC>, Error<GC>> set_volume(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
);

/// Implements Sound.start
template<typename GC>
Result<Value<GC>, Error<GC>> start(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
);

/// Implements Sound.stop
template<typename GC>
Result<Value<GC>, Error<GC>> stop(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
);

/// Implements Sound.setPosition
template<typename GC>
Result<Value<GC>, Error<GC>> set_position(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
);

/// Implements Sound.getPosition
template<typename GC>
Result<Value<GC>, Error<GC>> get_position(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::vector<Value<GC>>& args
);

} // namespace sound
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_SOUND_H
