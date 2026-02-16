// C++ translation of frontend-utils/src/backends/audio.rs
// CPAL audio backend for desktop audio playback

#ifndef RUFFLE_FRONTEND_UTILS_BACKENDS_AUDIO_H
#define RUFFLE_FRONTEND_UTILS_BACKENDS_AUDIO_H

#include <string>
#include <optional>
#include <memory>
#include <vector>
#include <cstdint>

// Forward declarations for ruffle_core types
namespace ruffle {
namespace core {
namespace backend {
namespace audio {
    class AudioBackend;
    class AudioMixer;
    struct SoundHandle;
    struct SoundInstanceHandle;
    struct SoundStreamInfo;
    struct SoundTransform;

    enum class DecodeError;
    enum class RegisterError;

    namespace swf {
        // SWF audio types
    }
}
}
}
}

// Forward declaration for cpal types (would be included from cpal library)
namespace cpal {
    class Host;
    class Device;
    struct StreamConfig;
    class Stream;
    enum class SampleFormat;
}

namespace ruffle {
namespace frontend {
namespace utils {
namespace backends {
namespace audio {

/// CPAL audio backend error types.
enum class CpalError {
    NoDevices,                    ///< No audio devices available
    DefaultStream,                ///< Failed to get default output config
    UnsupportedSampleFormat,      ///< Unsupported sample format
    Play,                         ///< Couldn't play the audio stream
    Build                         ///< Failed to construct audio stream
};

/// CPAL-based audio backend.
///
/// This backend uses the cpal library to provide audio output on desktop platforms.
/// It supports multiple sample formats (F32, I16, U16) and handles audio mixing.
class CpalAudioBackend {
private:
    cpal::Device device_;
    cpal::StreamConfig config_;
    cpal::Stream stream_;
    std::shared_ptr<ruffle::core::backend::audio::AudioMixer> mixer_;

    /// Error handler for audio stream errors.
    /// @param error The error message
    static void error_handler(const std::string& error) {
        // tracing::error!("Audio stream error: {}", error);
    }

public:
    /// Create a new CPAL audio backend.
    ///
    /// @param preferred_device_name Optional name of preferred audio device
    /// @return Result containing the backend or CpalError
    static std::optional<CpalAudioBackend> new_with_device(
        const std::optional<std::string>& preferred_device_name
    ) {
        // Create CPAL audio device.
        // This would use cpal::default_host() and get_suitable_output_device()
        // Implementation would go here
        return std::nullopt; // Placeholder
    }

    /// Create a new CPAL audio backend with default device.
    /// @return Result containing the backend or CpalError
    static std::optional<CpalAudioBackend> create() {
        return new_with_device(std::nullopt);
    }

    /// Play the audio stream.
    ///
    /// Resumes audio playback if it was paused.
    void play() {
        // self.stream.play().expect("Error trying to resume CPAL audio stream...");
    }

    /// Pause the audio stream.
    ///
    /// Pauses audio playback.
    void pause() {
        // self.stream.pause().expect("Error trying to pause CPAL audio stream...");
    }

    /// Get the audio mixer.
    /// @return Reference to the audio mixer
    ruffle::core::backend::audio::AudioMixer& mixer() {
        return *mixer_;
    }

    /// Get the audio mixer (const version).
    /// @return Const reference to the audio mixer
    const ruffle::core::backend::audio::AudioMixer& mixer() const {
        return *mixer_;
    }
};

/// Get a suitable output device.
///
/// First checks for a user-preferred device, then falls back to system default.
/// @param preferred_device_name Optional preferred device name
/// @param host The CPAL host
/// @return Optional device if found
std::optional<cpal::Device> get_suitable_output_device(
    const std::optional<std::string>& preferred_device_name,
    const cpal::Host& host
);

} // namespace audio
} // namespace backends
} // namespace utils
} // namespace frontend
} // namespace ruffle

#endif // RUFFLE_FRONTEND_UTILS_BACKENDS_AUDIO_H
