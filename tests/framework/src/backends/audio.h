// C++ translation of tests/framework/src/backends/audio.rs
// Test audio backend for test framework

#ifndef RUFFLE_TESTS_FRAMEWORK_BACKENDS_AUDIO_H
#define RUFFLE_TESTS_FRAMEWORK_BACKENDS_AUDIO_H

#include <vector>
#include <cstdint>
#include <memory>

// Forward declarations
namespace ruffle {
namespace core {
namespace backend {
namespace audio {
    class AudioBackend;
    class AudioMixer;
    template<typename T>
    struct SoundHandle;
    template<typename T>
    struct SoundInstanceHandle;
    struct SoundStreamInfo;
    struct SoundTransform;
    namespace swf {
        // SWF audio types
    }
    class DecodeError;
    class RegisterError;
}
}
}
}

namespace ruffle {
namespace tests {
namespace framework {
namespace backends {
namespace audio {

/// Test audio backend implementation
///
/// A simple audio backend for testing purposes that uses an AudioMixer
/// to mix audio samples into a buffer.
class TestAudioBackend : public core::backend::audio::AudioBackend {
private:
    /// Audio mixer for mixing audio samples
    core::backend::audio::AudioMixer mixer_;

    /// Buffer for mixed audio samples
    std::vector<float> buffer_;

    /// Number of audio channels (stereo)
    static constexpr uint8_t NUM_CHANNELS = 2;

    /// Sample rate in Hz
    static constexpr uint32_t SAMPLE_RATE = 44100;

public:
    /// Create a new test audio backend with default settings
    TestAudioBackend();

    /// Get the audio mixer
    ///
    /// \return Reference to the audio mixer
    core::backend::audio::AudioMixer& get_mixer() override;

    /// Get the audio mixer (const version)
    ///
    /// \return Const reference to the audio mixer
    const core::backend::audio::AudioMixer& get_mixer() const override;

    /// Register a sound for playback
    ///
    /// \param swf_sound The SWF sound data
    /// \return Handle to the registered sound or error
    Result<SoundHandle<void>, RegisterError> register_sound(
        const core::backend::audio::swf::Sound& swf_sound) override;

    /// Register a sound stream for playback
    ///
    /// \param info The sound stream information
    /// \return Handle to the registered sound stream or error
    Result<SoundHandle<void>, RegisterError> register_sound_stream(
        const SoundStreamInfo& info) override;

    /// Start audio playback
    void play() override;

    /// Pause audio playback
    void pause() override;

    /// Set the frame rate for audio mixing
    ///
    /// \param frame_rate The frame rate in frames per second
    void set_frame_rate(double frame_rate) override;

    /// Mix audio for the current frame
    ///
    /// This method mixes audio samples into the internal buffer
    /// using the AudioMixer.
    void tick() override;

    /// Get the number of audio channels
    ///
    /// \return Number of channels (2 for stereo)
    static uint8_t num_channels() { return NUM_CHANNELS; }

    /// Get the sample rate
    ///
    /// \return Sample rate in Hz (44100)
    static uint32_t sample_rate() { return SAMPLE_RATE; }
};

} // namespace audio
} // namespace backends
} // namespace framework
} // namespace tests
} // namespace ruffle

#endif // RUFFLE_TESTS_FRAMEWORK_BACKENDS_AUDIO_H
