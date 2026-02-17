// C++ translation of web/src/audio.rs
// WebAudio backend for Web platform

#ifndef RUFFLE_WEB_AUDIO_H
#define RUFFLE_WEB_AUDIO_H

#include <cstdint>
#include <vector>
#include <memory>
#include <atomic>
#include <chrono>
#include <optional>
#include <functional>

// Forward declarations
namespace ruffle {
namespace core {
namespace backend {
namespace audio {
    class AudioBackend;
    class AudioMixer;
    template<typename T> class AudioMixerProxy;
    class DecodeError;
    class RegisterError;
    using SoundHandle = uint32_t;
    using SoundInstanceHandle = uint32_t;
    struct SoundStreamInfo;
    struct SoundTransform;
    namespace swf {
        struct SoundFormat;
    }
}
}
}
}

// Forward declarations for Web APIs (would be implemented with Emscripten/wasm-bindgen)
namespace web_sys {
    class AudioContext;
    class AudioBuffer;
    class AudioBufferSourceNode;
    class AudioScheduledSourceNode;
    class AudioNode;
}

namespace wasm_bindgen {
    class JsValue;
    template<typename T> class Ref;
}

using JsError = wasm_bindgen::JsValue;

namespace ruffle {
namespace web {

/// WebAudio backend for browser-based audio playback
///
/// Uses the Web Audio API to play audio through the browser's audio system.
/// Implements adaptive buffer sizing to minimize audio underruns while
/// maintaining low latency.
class WebAudioBackend {
private:
    // Audio mixer for mixing multiple sound sources
    std::shared_ptr<core::backend::audio::AudioMixer> mixer_;

    // Web Audio API context
    std::shared_ptr<web_sys::AudioContext> context_;

    /// The current length of both buffers, in frames (pairs of left/right samples).
    std::shared_ptr<std::atomic<uint32_t>> buffer_size_;

    /// Ping-pong buffers for audio playback
    std::vector<std::shared_ptr<class Buffer>> buffers_;

    /// When the last submitted buffer is expected to play out completely, in seconds.
    std::shared_ptr<std::atomic<double>> time_;

    /// For how many seconds were we able to continuously fill the next buffer "at a sufficiently early time".
    std::shared_ptr<std::atomic<float>> probation_elapsed_;

    // Buffer size configuration constants (in frames)
    static constexpr uint32_t INITIAL_BUFFER_SIZE = 2048;   // 46.44 ms at 44.1 kHz
    static constexpr uint32_t MIN_BUFFER_SIZE = 1024;       // 23.22 ms at 44.1 kHz
    static constexpr uint32_t MAX_BUFFER_SIZE = 16384;      // 371.52 ms at 44.1 kHz

    /// Buffer size will not be increased until this many seconds have elapsed after startup
    static constexpr float WARMUP_PERIOD = 2.0f;

    /// For how long we need to fill every single buffer "quickly enough" to decrease buffer size
    static constexpr float PROBATION_LENGTH = 10.0f;

    /// The limit of playout ratio when filling the next buffer, under which it is considered "quick"
    static constexpr double NORMAL_PROGRESS_RANGE_MIN = 0.25;

    /// The limit of playout ratio when filling the next buffer, over which buffer size is increased
    static constexpr double NORMAL_PROGRESS_RANGE_MAX = 0.75;

public:
    /// Create a new WebAudio backend
    ///
    /// \param log_subscriber Logging subscriber for tracing
    /// \return New WebAudioBackend or error
    static std::optional<JsError> create();

    /// Returns the JavaScript AudioContext
    web_sys::AudioContext* audio_context() const {
        return context_.get();
    }

    /// Get sample rate
    uint32_t sample_rate() const;

    /// Get number of channels
    uint32_t channels() const { return 2; }
};

/// Internal buffer class for ping-pong audio playback
class Buffer {
private:
    std::shared_ptr<web_sys::AudioContext> context_;
    std::shared_ptr<core::backend::audio::AudioMixerProxy> mixer_proxy_;
    std::shared_ptr<std::atomic<uint32_t>> buffer_size_;

    /// Audio buffer data (interleaved stereo)
    std::vector<float> audio_buffer_;

    /// JavaScript AudioBuffer
    std::shared_ptr<web_sys::AudioBuffer> js_buffer_;

    /// Audio node for playback
    std::optional<std::shared_ptr<web_sys::AudioBufferSourceNode>> audio_node_;

    /// Handler for buffer end event
    std::function<void()> on_ended_handler_;

    /// Shared timing state
    std::shared_ptr<std::atomic<double>> time_;
    std::shared_ptr<std::atomic<float>> probation_elapsed_;

public:
    /// Create a new buffer
    ///
    /// \param audio The WebAudioBackend this buffer belongs to
    /// \return New Buffer or error
    static std::optional<JsError> create(std::shared_ptr<WebAudioBackend> audio);

    /// Play the buffer
    ///
    /// Mixes new audio data and schedules the buffer for playback.
    /// \return Error if playback fails
    std::optional<JsError> play();

    /// Get the audio buffer data
    std::vector<float>& data() { return audio_buffer_; }
    const std::vector<float>& data() const { return audio_buffer_; }
};

/// Copy interleaved audio data to a JavaScript AudioBuffer
///
/// This function is imported from JavaScript via wasm-bindgen.
///
/// \param audio_buffer The JavaScript AudioBuffer to copy to
/// \param interleaved_data The interleaved audio data (LRLRLR...)
void copy_to_audio_buffer_interleaved(
    web_sys::AudioBuffer* audio_buffer,
    const std::vector<float>& interleaved_data
);

} // namespace web
} // namespace ruffle

// Implementation of AudioBackend trait methods would be in a separate header
// or inline in the implementation file. For now, we provide the interface.

namespace ruffle {
namespace core {
namespace backend {
namespace audio {

/// Mix audio into output buffer
///
/// This would be implemented using the impl_audio_mixer_backend! macro in Rust.
/// In C++, we provide the methods directly.
template<typename Backend>
class AudioBackendImpl {
public:
    /// Register a sound
    static std::optional<SoundHandle> register_sound(
        Backend* backend,
        const swf::SoundFormat& format,
        const std::vector<uint8_t>& data
    );

    /// Play a sound
    static std::optional<SoundInstanceHandle> play_sound(
        Backend* backend,
        SoundHandle handle,
        const SoundTransform& transform
    );

    /// Play a sound stream
    static std::optional<SoundInstanceHandle> play_sound_stream(
        Backend* backend,
        const SoundStreamInfo& info,
        double start_time,
        const SoundTransform& transform
    );

    /// Stop a sound instance
    static void stop_sound(Backend* backend, SoundInstanceHandle handle);

    /// Set sound transform
    static void set_sound_transform(
        Backend* backend,
        SoundInstanceHandle handle,
        const SoundTransform& transform
    );

    /// Get sound position
    static std::optional<double> get_sound_position(
        Backend* backend,
        SoundInstanceHandle handle
    );
};

} // namespace audio
} // namespace backend
} // namespace core
} // namespace ruffle

#endif // RUFFLE_WEB_AUDIO_H
