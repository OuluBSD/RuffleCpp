// C++ translation of core/src/backend/audio/decoders/nellymoser.rs
// Nellymoser audio decoder

#ifndef RUFFLE_CORE_BACKEND_AUDIO_DECODERS_NELLYMOSER_H
#define RUFFLE_CORE_BACKEND_AUDIO_DECODERS_NELLYMOSER_H

#include <cstdint>
#include <memory>
#include <optional>

// Forward declarations
namespace ruffle {
namespace core {
namespace backend {
namespace audio {
namespace decoders {

/// Trait for audio decoders
template<typename R>
class Decoder {
public:
    virtual ~Decoder() = default;
    virtual uint8_t num_channels() const = 0;
    virtual uint16_t sample_rate() const = 0;
};

/// Trait for seekable decoders
template<typename R>
class SeekableDecoder : public Decoder<R> {
public:
    virtual void reset() = 0;
};

} // namespace decoders

/// Nellymoser audio decoder
template<typename R>
class NellymoserDecoder : public decoders::Decoder<R> {
private:
    // The underlying nellymoser decoder
    // In Rust: decoder: nellymoser_rs::Decoder<R>
    void* decoder_;  // Opaque pointer to nellymoser decoder
    uint32_t sample_rate_;

public:
    /// Create a new Nellymoser decoder
    /// @param reader The input reader
    /// @param sample_rate The sample rate in Hz
    NellymoserDecoder(R reader, uint32_t sample_rate)
        : decoder_(nullptr), sample_rate_(sample_rate)
    {
        // In Rust: decoder: nellymoser_rs::Decoder::new(reader, sample_rate)
        // This would call the nellymoser_rs library
    }

    ~NellymoserDecoder() override = default;

    /// Get the next stereo sample
    /// @return Optional stereo sample [left, right]
    std::optional<std::array<int16_t, 2>> next()
    {
        // In Rust: let sample = self.decoder.next()? as i16;
        // This would call the underlying decoder
        if (decoder_ == nullptr) {
            return std::nullopt;
        }

        // Placeholder - actual implementation would call nellymoser decoder
        int32_t sample = 0;  // Would come from decoder
        return std::array<int16_t, 2>{static_cast<int16_t>(sample), static_cast<int16_t>(sample)};
    }

    // Decoder trait implementation
    uint8_t num_channels() const override {
        return 1;  // Nellymoser is mono
    }

    uint16_t sample_rate() const override {
        return static_cast<uint16_t>(sample_rate_);
    }
};

/// Seekable Nellymoser decoder for cursor-based readers
template<typename R>
class SeekableNellymoserDecoder : public NellymoserDecoder<R>, public decoders::SeekableDecoder<R> {
public:
    SeekableNellymoserDecoder(R reader, uint32_t sample_rate)
        : NellymoserDecoder<R>(reader, sample_rate)
    {}

    /// Reset the decoder to the beginning
    void reset() override {
        // In Rust: self.decoder.reset();
        // This would call the underlying decoder's reset method
    }
};

} // namespace audio
} // namespace backend
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_BACKEND_AUDIO_DECODERS_NELLYMOSER_H
