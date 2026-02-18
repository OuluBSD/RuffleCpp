// C++ translation of core/src/backend/audio/mixer.rs
//! Audio mixer for Flash movie

#ifndef RUFFLE_CORE_BACKEND_AUDIO_MIXER_H
#define RUFFLE_CORE_BACKEND_AUDIO_MIXER_H

#include <vector>
#include <array>
#include <cstdint>
#include <memory>
#include <optional>
#include <mutex>
#include <shared_mutex>
#include <functional>

// Forward declarations
namespace ruffle {
namespace core {

namespace tag_utils {
    struct SwfSlice;
}

namespace backend {
namespace audio {
    class SoundTransform;
    struct SoundStreamInfo;
}
}

} // namespace core

namespace swf {
    struct Sound;
    struct SoundFormat;
    struct SoundInfo;
    struct SoundStreamHead;
    struct SoundEnvelopePoint;
    enum class AudioCompression;
}

namespace slotmap {
    // Simplified slotmap implementation
    template<typename T, typename Key>
    class SlotMap;
}

} // namespace ruffle

namespace ruffle {
namespace core {
namespace backend {
namespace audio {

using swf::SoundFormat;
using swf::SoundInfo;
using swf::SoundStreamHead;

// Type aliases for sound handles
using SoundHandle = uint32_t;
using SoundInstanceHandle = uint32_t;

// ============================================================================
// CircBuf - Circular buffer for audio samples
// ============================================================================

/// Holds the last 2048 output audio frames. Frames can be written to it one by
/// one, and the last completely filled 1024-wide window can be read from it.
class CircBuf {
public:
    std::array<std::array<float, 2>, 2048> samples;
    size_t pos;

    CircBuf() : pos(0) {
        for (auto& sample : samples) {
            sample[0] = 0.0f;
            sample[1] = 0.0f;
        }
    }

    /// Writes a value into the buffer, pushing the write position forward.
    void push(const std::array<float, 2>& sample) {
        samples[pos] = sample;
        pos = (pos + 1) % 2048;
    }

    /// Returns one half of the inner buffer, the one that is not currently
    /// being written to.
    const std::array<std::array<float, 2>, 1024>& get() const {
        static const std::array<std::array<float, 2>, 1024> empty{};
        if (pos < 1024) {
            // Return second half
            return *reinterpret_cast<const std::array<std::array<float, 2>, 1024>*>(
                samples.data() + 1024
            );
        } else {
            // Return first half
            return *reinterpret_cast<const std::array<std::array<float, 2>, 1024>*>(
                samples.data()
            );
        }
    }
};

// ============================================================================
// Sound - Registered sound data
// ============================================================================

/// Contains the data and metadata for a sound in an SWF file.
struct Sound {
    /// The format of the sound, including sample rate and compression format.
    SoundFormat format;

    /// The audio data of this sound.
    std::shared_ptr<const std::vector<uint8_t>> data;

    /// Number of samples in this audio.
    uint32_t num_sample_frames;

    /// The number of sample frames to skip to reach the start of the audio.
    uint16_t skip_sample_frames;
};

// ============================================================================
// SoundInstance - Actively playing sound
// ============================================================================

/// An actively playing instance of a sound.
struct SoundInstance {
    /// The handle the sound definition inside sounds.
    std::optional<SoundHandle> handle;

    /// Flag indicating whether this sound is still playing.
    bool active;

    /// The transform for the left channel of this sound instance.
    std::array<float, 2> left_transform;

    /// The transform for the right channel of this sound instance.
    std::array<float, 2> right_transform;

    /// Stores the per-channel "peak amplitude" (volume) of this sound
    /// over the last completely mixed 1024-frame long window.
    std::array<float, 2> peak;

    /// Accumulates the per-channel minimum and maximum sample values.
    std::pair<std::array<float, 2>, std::array<float, 2>> range;

    SoundInstance()
        : active(true)
        , left_transform{1.0f, 0.0f}
        , right_transform{0.0f, 1.0f}
        , peak{0.0f, 0.0f}
        , range({{std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()},
                 {std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest()}}) {}

    /// Creates a new SoundInstance from a Stream, with a SoundHandle.
    static SoundInstance create_sound(SoundHandle handle) {
        SoundInstance instance;
        instance.handle = handle;
        return instance;
    }

    /// Creates a new SoundInstance from a Stream, for stream sounds.
    static SoundInstance create_stream() {
        return SoundInstance();
    }

    /// Updates peak from range, and resets the latter to default.
    void update_peak() {
        peak[0] = (range.second[0] - range.first[0]) / 2.0f;
        peak[1] = (range.second[1] - range.first[1]) / 2.0f;
        range = {{std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()},
                 {std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest()}};
    }
};

// ============================================================================
// AudioMixerProxy - Thread-safe proxy to AudioMixer
// ============================================================================

/// A thread-safe proxy to the main AudioMixer, allowing for mixing audio from a different thread.
class AudioMixerProxy {
private:
    std::shared_ptr<std::mutex> sound_instances_mutex_;
    std::shared_ptr<std::shared_mutex> volume_mutex_;
    uint8_t num_output_channels_;
    std::shared_ptr<CircBuf> output_memory_;

public:
    AudioMixerProxy(
        std::shared_ptr<std::mutex> sound_instances_mutex,
        std::shared_ptr<std::shared_mutex> volume_mutex,
        uint8_t num_output_channels,
        std::shared_ptr<CircBuf> output_memory
    )
        : sound_instances_mutex_(std::move(sound_instances_mutex))
        , volume_mutex_(std::move(volume_mutex))
        , num_output_channels_(num_output_channels)
        , output_memory_(std::move(output_memory)) {}

    float get_volume() const;
    void set_volume(float volume);

    /// Mixes audio into the given output_buffer.
    template<typename T>
    void mix(std::vector<T>& output_buffer);

    std::array<std::array<float, 2>, 1024> get_sample_history() const;
};

// ============================================================================
// AudioMixer - Audio mixer for Flash movie
// ============================================================================

/// An audio mixer for a Flash movie.
///
/// AudioMixer manages the audio state for a Flash movie. This can be used by any backend that
/// can output a raw audio stream.
class AudioMixer {
private:
    /// The currently registered sounds.
    std::unordered_map<SoundHandle, Sound> sounds_;
    SoundHandle next_sound_handle_ = 0;

    /// The list of actively playing sound instances.
    std::shared_ptr<std::mutex> sound_instances_mutex_;
    std::unordered_map<SoundInstanceHandle, SoundInstance> sound_instances_;
    SoundInstanceHandle next_instance_handle_ = 0;

    /// The master volume of the audio from [0.0, 1.0].
    std::shared_ptr<std::shared_mutex> volume_mutex_;
    float volume_;

    /// The number of channels in the output stream. Must be 1 or 2.
    uint8_t num_output_channels_;

    /// The sample rate of the output stream in Hz.
    uint32_t output_sample_rate_;

    /// The last two windows of output samples.
    std::shared_ptr<CircBuf> output_memory_;

public:
    /// Creates a new AudioMixer with the given number of channels and sample rate.
    AudioMixer(uint8_t num_output_channels, uint32_t output_sample_rate)
        : sound_instances_mutex_(std::make_shared<std::mutex>())
        , volume_mutex_(std::make_shared<std::shared_mutex>())
        , volume_(1.0f)
        , num_output_channels_(num_output_channels)
        , output_sample_rate_(output_sample_rate)
        , output_memory_(std::make_shared<CircBuf>()) {}

    /// Creates a proxy that may be sent to a different thread.
    AudioMixerProxy proxy() const {
        return AudioMixerProxy(
            sound_instances_mutex_,
            volume_mutex_,
            num_output_channels_,
            output_memory_
        );
    }

    /// Mixes audio into the given output_buffer.
    template<typename T>
    void mix(std::vector<T>& output_buffer);

    /// Registers an embedded SWF sound with the audio mixer.
    SoundHandle register_sound(const swf::Sound& swf_sound);

    /// Registers an external MP3 with the audio mixer.
    SoundHandle register_mp3(const std::vector<uint8_t>& data);

    /// Starts a timeline audio stream.
    SoundInstanceHandle start_stream(
        const tag_utils::SwfSlice& clip_data,
        const SoundStreamHead& stream_info
    );

    /// Starts a sound.
    SoundInstanceHandle start_sound(
        SoundHandle sound_handle,
        const SoundInfo& settings
    );

    /// Starts a Substream backed audio stream.
    SoundInstanceHandle start_substream(
        const std::vector<uint8_t>& stream_data,
        const SoundStreamInfo& stream_info
    );

    /// Stops a playing sound instance.
    void stop_sound(SoundInstanceHandle instance);

    /// Stops all playing sounds.
    void stop_all_sounds();

    /// Returns the position of a playing sound in milliseconds.
    std::optional<double> get_sound_position(SoundInstanceHandle instance) const;

    /// Returns the peak amplitude of a playing sound.
    std::optional<std::array<float, 2>> get_sound_peak(SoundInstanceHandle instance) const;

    /// Returns the duration of a registered sound in milliseconds.
    std::optional<double> get_sound_duration(SoundHandle sound) const;

    /// Returns the size of a registered sound in bytes.
    std::optional<uint32_t> get_sound_size(SoundHandle sound) const;

    /// Returns the format of a registered sound.
    const SoundFormat* get_sound_format(SoundHandle sound) const;

    /// Sets the sound transform for the given playing sound.
    void set_sound_transform(SoundInstanceHandle instance, const SoundTransform& transform);

    /// Returns the master volume.
    float volume() const;

    /// Sets the master volume.
    void set_volume(float volume);

    /// Returns the sample history.
    std::array<std::array<float, 2>, 1024> get_sample_history() const;

private:
    /// Internal mixing implementation.
    template<typename T>
    static void mix_audio(
        std::unordered_map<SoundInstanceHandle, SoundInstance>& sound_instances,
        float volume,
        uint8_t num_channels,
        std::vector<T>& output_buffer,
        CircBuf& output_memory
    );
};

// ============================================================================
// Template Implementations
// ============================================================================

template<typename T>
void AudioMixer::mix(std::vector<T>& output_buffer) {
    std::lock_guard<std::mutex> lock(*sound_instances_mutex_);
    std::shared_lock<std::shared_mutex> volume_lock(*volume_mutex_);
    std::unique_lock<CircBuf> memory_lock(*output_memory_);  // Note: needs proper locking

    mix_audio(
        sound_instances_,
        volume_,
        num_output_channels_,
        output_buffer,
        *output_memory_
    );
}

template<typename T>
void AudioMixer::mix_audio(
    std::unordered_map<SoundInstanceHandle, SoundInstance>& sound_instances,
    float volume,
    uint8_t num_channels,
    std::vector<T>& output_buffer,
    CircBuf& output_memory
) {
    // Adapt the volume for logarithmic hearing.
    float adapted_volume = (std::pow(10.0f, std::log10(81.0f) * volume) - 1.0f) / 80.0f;

    // For each sample, mix the samples from all active sound instances.
    for (size_t i = 0; i < output_buffer.size(); i += num_channels) {
        float left = 0.0f;
        float right = 0.0f;

        for (auto& [handle, sound] : sound_instances) {
            if (sound.active) {
                // Apply transforms
                float sample_left = sound.left_transform[0] * left + sound.left_transform[1] * right;
                float sample_right = sound.right_transform[0] * left + sound.right_transform[1] * right;

                // Apply volume
                sample_left *= adapted_volume;
                sample_right *= adapted_volume;

                // Update range for peak calculation
                sound.range.first[0] = std::min(sound.range.first[0], sample_left);
                sound.range.first[1] = std::min(sound.range.first[1], sample_right);
                sound.range.second[0] = std::max(sound.range.second[0], sample_left);
                sound.range.second[1] = std::max(sound.range.second[1], sample_right);

                left += sample_left;
                right += sample_right;
            } else {
                sound.active = false;
            }
        }

        output_memory.push({left, right});

        if (output_memory.pos == 0 || output_memory.pos == 1024) {
            for (auto& [handle, sound] : sound_instances) {
                sound.update_peak();
            }
        }

        if (i < output_buffer.size()) {
            output_buffer[i] = static_cast<T>(left);
        }
        if (i + 1 < output_buffer.size() && num_channels >= 2) {
            output_buffer[i + 1] = static_cast<T>(right);
        }
    }

    // Remove all dead sounds.
    for (auto it = sound_instances.begin(); it != sound_instances.end(); ) {
        if (!it->second.active) {
            it = sound_instances.erase(it);
        } else {
            ++it;
        }
    }
}

// ============================================================================
// AudioMixerProxy Implementations
// ============================================================================

inline float AudioMixerProxy::get_volume() const {
    // Would need proper locking in real implementation
    return 1.0f;
}

inline void AudioMixerProxy::set_volume(float volume) {
    // Would need proper locking in real implementation
}

template<typename T>
void AudioMixerProxy::mix(std::vector<T>& output_buffer) {
    // Would delegate to AudioMixer::mix_audio in real implementation
}

inline std::array<std::array<float, 2>, 1024> AudioMixerProxy::get_sample_history() const {
    return output_memory_->get();
}

} // namespace audio
} // namespace backend
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_BACKEND_AUDIO_MIXER_H
