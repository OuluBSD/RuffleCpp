// C++ translation of core/src/backend/audio/decoders/aac.rs
// AAC audio decoder

#ifndef RUFFLE_CORE_BACKEND_AUDIO_DECODERS_AAC_H
#define RUFFLE_CORE_BACKEND_AUDIO_DECODERS_AAC_H

#include <cstdint>
#include <array>
#include <optional>
#include <memory>

// Forward declarations
namespace ruffle {
namespace core {
namespace backend {
namespace audio {

struct SoundStreamInfo;
class Substream;

/// Audio decoder trait
class Decoder {
public:
    virtual ~Decoder() = default;

    /// The number of channels of this audio decoder. Always 1 or 2.
    virtual uint8_t num_channels() const = 0;

    /// The sample rate of this audio decoder.
    virtual uint16_t sample_rate() const = 0;
};

/// Error type for audio decoding
class Error : public std::exception {
public:
    explicit Error(std::string message) : message_(std::move(message)) {}
    const char* what() const noexcept override { return message_.c_str(); }

private:
    std::string message_;
};

} // namespace audio
} // namespace backend
} // namespace core
} // namespace ruffle

namespace ruffle {
namespace core {
namespace backend {
namespace audio {
namespace decoders {

/// Substream tag reader for AAC
/// Reads through data chunks of a Substream with audio data
class SubstreamTagReader {
    // Implementation would iterate through Substream chunks
    // and return audio data slices
};

/// AAC substream decoder
///
/// Unlike MP3, AAC can only be in FLV, not SWF. Therefore, no need for an
/// AacStreamDecoder, nor for our own AacDecoder type wrapping Symphonia's.
class AacSubstreamDecoder : public Decoder {
private:
    SubstreamTagReader tag_reader_;
    // Symphonia AAC decoder would be wrapped here
    void* decoder_;  // Placeholder for SymphoniaAacDecoder
    std::vector<int16_t> sample_buf_;
    size_t cur_sample_;
    bool stream_ended_;

public:
    /// Create a new AAC substream decoder
    ///
    /// \param stream_info The sound stream info
    /// \param data_stream The substream containing audio data
    /// \return A new AacSubstreamDecoder
    static std::unique_ptr<AacSubstreamDecoder> create(
        const SoundStreamInfo& stream_info,
        Substream data_stream
    );

    /// Get the number of channels
    uint8_t num_channels() const override;

    /// Get the sample rate
    uint16_t sample_rate() const override;

    /// Get the next stereo sample frame
    ///
    /// \return Optional stereo sample pair, or nullopt if no more samples
    std::optional<std::array<int16_t, 2>> next();
};

} // namespace decoders
} // namespace audio
} // namespace backend
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_BACKEND_AUDIO_DECODERS_AAC_H
