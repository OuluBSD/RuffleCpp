// C++ translation of core/src/backend/audio.rs
//! Audio backend interface for Ruffle

#ifndef RUFFLE_CORE_BACKEND_AUDIO_H
#define RUFFLE_CORE_BACKEND_AUDIO_H

#include <cstdint>
#include <optional>
#include <vector>
#include <array>
#include <memory>
#include <any>
#include <chrono>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename T> class NativeObject;
    template<typename T> class Object;
}
namespace avm2 {
    template<typename T> class Avm2;
    template<typename T> class EventObject;
    template<typename T> class SoundChannelObject;
}
namespace display_object {
    template<typename T> class SoundTransform;
    template<typename T> class DisplayObject;
    template<typename T> class MovieClip;
}
namespace string {
    template<typename T> class AvmString;
}
namespace tag_utils {
    template<typename T> class SwfSlice;
}
template<typename GC> class UpdateContext;
template<typename GC> class Mutation;
}
}

namespace ruffle {
namespace swf {
    struct Sound;
    struct SoundFormat;
    struct SoundInfo;
    struct SoundStreamHead;
    enum class AudioCompression;
    using CharacterId = uint16_t;
}
}

namespace ruffle {
namespace common {
namespace buffer {
    template<typename T> class Substream;
}
}
}

namespace ruffle {
namespace core {
namespace backend {
namespace audio {

/// Sound handle - uniquely identifies a registered sound
using SoundHandle = uint64_t;

/// Sound instance handle - uniquely identifies a playing sound instance
using SoundInstanceHandle = uint64_t;

/// Null sound handle
constexpr SoundHandle NULL_SOUND_HANDLE = 0;

/// Null sound instance handle
constexpr SoundInstanceHandle NULL_SOUND_INSTANCE_HANDLE = 0;

/// Wrapping type for sound streams
enum class SoundStreamWrapping {
    /// Sound is being streamed from an SWF (MP3 chunks wrapped in header)
    Swf,

    /// Sound is being streamed from an unwrapped bitstream
    Unwrapped
};

/// Information about a sound stream
struct SoundStreamInfo {
    SoundStreamWrapping wrapping;
    swf::SoundFormat stream_format;
    uint16_t num_samples_per_block;
    int16_t latency_seek;
};

/// Convert swf::SoundStreamHead to SoundStreamInfo
inline SoundStreamInfo from_swf_sound_stream_head(const swf::SoundStreamHead& head) {
    return SoundStreamInfo{
        SoundStreamWrapping::Swf,
        head.stream_format,
        head.num_samples_per_block,
        head.latency_seek
    };
}

/// Error type for sound registration
enum class RegisterError {
    /// MP3 sound is too short
    ShortMp3
};

/// Error type for audio decoding
enum class DecodeError {
    /// Too many sounds are playing
    TooManySounds,

    /// Generic decode error
    DecodeFailed
};

/// Sound transform for volume/pan control
/// (Forward declared - actual definition in display_object)
template<typename GC>
using SoundTransform = display_object::SoundTransform<GC>;

/// Audio backend trait/interface
///
/// This is the interface that all audio backends must implement.
class AudioBackend {
public:
    virtual ~AudioBackend() = default;

    /// Start audio playback
    virtual void play() = 0;

    /// Pause audio playback
    virtual void pause() = 0;

    /// Register a sound embedded in an SWF
    ///
    /// @param swf_sound The SWF sound data
    /// @return Sound handle or error
    virtual std::optional<SoundHandle> register_sound(const swf::Sound& swf_sound) = 0;

    /// Register MP3 audio from an external source
    ///
    /// @param data The MP3 data
    /// @return Sound handle or error
    virtual std::optional<SoundHandle> register_mp3(const std::vector<uint8_t>& data) = 0;

    /// Play a sound
    ///
    /// @param sound The sound handle
    /// @param settings The sound playback settings
    /// @return Sound instance handle or error
    virtual std::optional<SoundInstanceHandle> start_sound(
        SoundHandle sound,
        const swf::SoundInfo& settings
    ) = 0;

    /// Start playing a stream sound from a MovieClip
    ///
    /// @param clip_data The SWF slice containing the stream data
    /// @param handle The sound stream header
    /// @return Sound instance handle or error
    virtual std::optional<SoundInstanceHandle> start_stream(
        tag_utils::SwfSlice<GC> clip_data,
        const swf::SoundStreamHead& handle
    ) = 0;

    /// Start playing a substream sound
    ///
    /// @param stream_data The substream data
    /// @param stream_info The stream information
    /// @return Sound instance handle or error
    virtual std::optional<SoundInstanceHandle> start_substream(
        common::buffer::Substream<GC> stream_data,
        const SoundStreamInfo& stream_info
    ) = 0;

    /// Stop a playing sound instance
    ///
    /// @param sound The sound instance handle
    virtual void stop_sound(SoundInstanceHandle sound) = 0;

    /// Stop all playing sounds (stopAllSounds)
    virtual void stop_all_sounds() = 0;

    /// Get the position of a sound instance in milliseconds
    ///
    /// @param instance The sound instance handle
    /// @return Position in milliseconds, or nullopt if not playing
    virtual std::optional<double> get_sound_position(SoundInstanceHandle instance) = 0;

    /// Get the duration of a sound in milliseconds
    ///
    /// @param sound The sound handle
    /// @return Duration in milliseconds, or nullopt if not registered
    virtual std::optional<double> get_sound_duration(SoundHandle sound) = 0;

    /// Get the size of the data stored within a given sound (compressed bytes)
    ///
    /// @param sound The sound handle
    /// @return Size in bytes, or nullopt if not registered
    virtual std::optional<uint32_t> get_sound_size(SoundHandle sound) = 0;

    /// Get the sound format that a given sound was added with
    ///
    /// @param sound The sound handle
    /// @return Sound format, or nullopt if not registered
    virtual std::optional<swf::SoundFormat> get_sound_format(SoundHandle sound) = 0;

    /// Set the volume transform for a sound instance
    ///
    /// @param instance The sound instance handle
    /// @param transform The sound transform
    virtual void set_sound_transform(
        SoundInstanceHandle instance,
        const SoundTransform<GC>& transform
    ) = 0;

    /// Get the sound peak for a sound instance
    ///
    /// @param instance The sound instance handle
    /// @return Peak values [left, right], or nullopt if not available
    virtual std::optional<std::array<float, 2>> get_sound_peak(SoundInstanceHandle instance) = 0;

    /// Allow the audio backend to update (runs once per event loop iteration)
    virtual void tick() {}

    /// Inform the audio backend of the current stage frame rate
    ///
    /// @param frame_rate The frame rate in Hz
    virtual void set_frame_rate(double frame_rate) { (void)frame_rate; }

    /// The approximate interval that this backend updates a sound's position value
    ///
    /// @return Duration resolution, or nullopt if unknown
    virtual std::optional<std::chrono::duration<double>> position_resolution() const {
        return std::nullopt;
    }

    /// Get the master volume of the audio backend
    ///
    /// @return Volume in range [0.0, 1.0]
    virtual float volume() const = 0;

    /// Set the master volume of the audio backend
    ///
    /// @param volume Volume in range [0.0, 1.0]
    virtual void set_volume(float volume) = 0;

    /// Get the last whole window of output samples
    ///
    /// @return Array of 1024 stereo samples [left, right]
    virtual std::array<std::array<float, 2>, 1024> get_sample_history() const = 0;

    /// Determine if a sound is still playing
    ///
    /// @param instance The sound instance handle
    /// @return True if playing
    virtual bool is_sound_playing(SoundInstanceHandle instance) const {
        return get_sound_position(instance).has_value();
    }
};

/// Null audio backend that ignores all audio
class NullAudioBackend : public AudioBackend {
public:
    NullAudioBackend();

    void play() override {}
    void pause() override override {}

    std::optional<SoundHandle> register_sound(const swf::Sound& swf_sound) override;
    std::optional<SoundHandle> register_mp3(const std::vector<uint8_t>& data) override;

    std::optional<SoundInstanceHandle> start_sound(
        SoundHandle sound,
        const swf::SoundInfo& settings
    ) override;

    std::optional<SoundInstanceHandle> start_stream(
        tag_utils::SwfSlice<GC> clip_data,
        const swf::SoundStreamHead& handle
    ) override;

    std::optional<SoundInstanceHandle> start_substream(
        common::buffer::Substream<GC> stream_data,
        const SoundStreamInfo& stream_info
    ) override;

    void stop_sound(SoundInstanceHandle sound) override {}
    void stop_all_sounds() override {}

    std::optional<double> get_sound_position(SoundInstanceHandle instance) override;
    std::optional<double> get_sound_duration(SoundHandle sound) override;
    std::optional<uint32_t> get_sound_size(SoundHandle sound) override;
    std::optional<swf::SoundFormat> get_sound_format(SoundHandle sound) override;

    void set_sound_transform(
        SoundInstanceHandle instance,
        const SoundTransform<GC>& transform
    ) override { (void)instance; (void)transform; }

    std::optional<std::array<float, 2>> get_sound_peak(SoundInstanceHandle instance) override {
        (void)instance;
        return std::nullopt;
    }

    float volume() const override { return volume_; }
    void set_volume(float volume) override { volume_ = volume; }

    std::array<std::array<float, 2>, 1024> get_sample_history() const override;

private:
    struct NullSound {
        double duration;
        uint32_t size;
        swf::SoundFormat format;
    };

    // Map of sound handles to sound info
    std::unordered_map<SoundHandle, NullSound> sounds_;
    float volume_;
    SoundHandle next_handle_;
};

/// Sound instance information
template<typename GC>
struct SoundInstance {
    /// The registered sound (nullopt for stream sounds)
    std::optional<SoundHandle> sound;

    /// The playing instance handle
    SoundInstanceHandle instance;

    /// The display object associated with this sound (AVM1 concept)
    std::optional<display_object::DisplayObject<GC>> display_object;

    /// The local sound transform
    SoundTransform<GC> transform;

    /// The AVM1 sound object (for onSoundComplete events)
    std::optional<avm1::Object<GC>> avm1_object;

    /// The AVM2 sound channel object
    std::optional<avm2::SoundChannelObject<GC>> avm2_object;

    /// The starting frame for stream sounds
    std::optional<uint16_t> stream_start_frame;
};

/// Audio manager for tracking and updating sound instances
template<typename GC>
class AudioManager {
public:
    /// Maximum number of sound instances that can play at once
    static constexpr size_t MAX_SOUNDS = 32;

    /// Default timeline stream buffer time in seconds
    static constexpr int32_t DEFAULT_STREAM_BUFFER_TIME = 5;

    /// Threshold where audio stream is considered too out-of-sync
    static constexpr double STREAM_RESTART_THRESHOLD = 1.0;

    /// Minimum audio syncing threshold in seconds
    static constexpr double STREAM_DEFAULT_SYNC_THRESHOLD = 0.2;

    AudioManager();

    /// Update state of active sounds (should be called once per frame)
    static void update_sounds(UpdateContext<GC>* context);

    /// Start a sound and optionally associate it with a Display Object
    std::optional<SoundInstanceHandle> start_sound(
        AudioBackend* audio,
        SoundHandle sound,
        const swf::SoundInfo& settings,
        std::optional<SoundTransform<GC>> transform,
        std::optional<display_object::DisplayObject<GC>> display_object,
        std::optional<avm1::Object<GC>> avm1_object
    );

    /// Attach an AVM2 sound channel object to a sound instance
    void attach_avm2_sound_channel(
        SoundInstanceHandle instance,
        avm2::SoundChannelObject<GC> avm2_object
    );

    /// Stop a specific sound instance
    void stop_sound(AudioBackend* audio, SoundInstanceHandle instance);

    /// Stop all sounds with a given sound handle
    void stop_sounds_with_handle(AudioBackend* audio, SoundHandle sound);

    /// Stop any sound associated with a given Display Object
    void stop_sounds_with_display_object(
        AudioBackend* audio,
        display_object::DisplayObject<GC> display_object
    );

    /// Stop any sound associated with a Display Object and its children
    void stop_sounds_on_parent_and_children(
        AudioBackend* audio,
        display_object::DisplayObject<GC> display_object
    );

    /// Stop all sounds
    void stop_all_sounds(AudioBackend* audio);

    /// Check if a sound instance is playing
    bool is_sound_playing(SoundInstanceHandle sound) const;

    /// Check if a sound with given handle is playing
    bool is_sound_playing_with_handle(SoundHandle sound) const;

    /// Start a stream sound
    std::optional<SoundInstanceHandle> start_stream(
        AudioBackend* audio,
        display_object::MovieClip<GC> movie_clip,
        uint16_t clip_frame,
        tag_utils::SwfSlice<GC> data,
        const swf::SoundStreamHead& stream_info
    );

    /// Start a substream sound
    std::optional<SoundInstanceHandle> start_substream(
        AudioBackend* audio,
        common::buffer::Substream<GC> stream_data,
        display_object::MovieClip<GC> movie_clip,
        const SoundStreamInfo& stream_info
    );

    /// Calculate audio skew time for syncing
    double audio_skew_time(AudioBackend* audio, double offset_ms);

    /// Get the global sound transform
    const SoundTransform<GC>& global_sound_transform() const {
        return global_sound_transform_;
    }

    /// Set the global sound transform
    void set_global_sound_transform(const SoundTransform<GC>& sound_transform);

    /// Get the local sound transform of a single sound instance
    std::optional<const SoundTransform<GC>&> local_sound_transform(
        SoundInstanceHandle instance
    ) const;

    /// Set the local sound transform of a single sound instance
    void set_local_sound_transform(
        SoundInstanceHandle instance,
        const SoundTransform<GC>& sound_transform
    );

    /// Get stream buffer time
    int32_t stream_buffer_time() const {
        return stream_buffer_time_;
    }

    /// Set stream buffer time
    void set_stream_buffer_time(int32_t time) {
        stream_buffer_time_ = time;
    }

    /// Update sound transforms if dirty
    void update_sound_transforms(AudioBackend* audio);

    /// Get the transform for a sound (combining global and local transforms)
    SoundTransform<GC> transform_for_sound(const SoundInstance<GC>* instance) const;

private:
    /// List of actively playing sounds
    std::vector<SoundInstance<GC>> sounds_;

    /// Global sound transform applied to all sounds
    SoundTransform<GC> global_sound_transform_;

    /// Stream buffer time in seconds
    int32_t stream_buffer_time_;

    /// Whether sound transforms are dirty
    bool transforms_dirty_;
};

} // namespace audio
} // namespace backend
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_BACKEND_AUDIO_H
