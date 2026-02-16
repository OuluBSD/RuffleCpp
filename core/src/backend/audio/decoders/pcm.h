// C++ translation of core/src/backend/audio/decoders/pcm.rs
/// Decoder for PCM audio data in a Flash file.
/// Flash exports this when you use the "Raw" compression setting.
/// 8-bit unsigned or 16-bit signed PCM.

#ifndef RUFFLE_CORE_BACKEND_AUDIO_DECODERS_PCM_H
#define RUFFLE_CORE_BACKEND_AUDIO_DECODERS_PCM_H

#include <cstdint>
#include <istream>

// Forward declarations
namespace ruffle {
namespace core {
namespace backend {
namespace audio {
    class Decoder;
    class SeekableDecoder;
}
}
}
}

namespace ruffle {
namespace core {
namespace backend {
namespace audio {
namespace decoders {

/// PCM audio decoder for 8-bit unsigned or 16-bit signed PCM data
template<typename R>
class PcmDecoder {
public:
    /// Constructs a new PCM decoder
    /// @param inner The input stream to read from
    /// @param is_stereo Whether the audio is stereo
    /// @param sample_rate The sample rate in Hz
    /// @param is_16_bit Whether the samples are 16-bit (true) or 8-bit (false)
    PcmDecoder(R& inner, bool is_stereo, uint16_t sample_rate, bool is_16_bit);

    /// Reads the next sample from the stream
    /// @return The sample as a signed 16-bit value, or std::nullopt if end of stream
    std::optional<int16_t> read_sample();

private:
    R& inner_;
    bool is_stereo_;
    uint16_t sample_rate_;
    bool is_16_bit_;
};

/// Iterator implementation for PcmDecoder
/// Returns stereo sample pairs [left, right]
template<typename R>
class PcmDecoderIterator {
public:
    using value_type = std::array<int16_t, 2>;
    using reference = const value_type&;
    using pointer = const value_type*;
    using iterator_category = std::input_iterator_tag;
    using difference_type = std::ptrdiff_t;

    PcmDecoderIterator(PcmDecoder<R>* decoder = nullptr, bool end = false);

    reference operator*() const;
    pointer operator->() const;
    PcmDecoderIterator& operator++();
    PcmDecoderIterator operator++(int);
    bool operator==(const PcmDecoderIterator& other) const;
    bool operator!=(const PcmDecoderIterator& other) const;

private:
    PcmDecoder<R>* decoder_;
    value_type current_;
    bool ended_;
};

/// Decoder trait implementation for PcmDecoder
template<typename R>
class PcmDecoderAsDecoder {
public:
    /// Returns the number of audio channels (1 for mono, 2 for stereo)
    uint8_t num_channels() const;

    /// Returns the sample rate in Hz
    uint16_t sample_rate() const;

private:
    const PcmDecoder<R>& decoder_;
};

/// SeekableDecoder trait implementation for PcmDecoder with Cursor
template<typename R>
class PcmDecoderAsSeekableDecoder {
public:
    /// Resets the decoder to the beginning of the stream
    void reset();

    /// Seeks to a specific sample frame
    /// @param frame The sample frame to seek to
    void seek_to_sample_frame(uint32_t frame);

private:
    PcmDecoder<std::istringstream>& decoder_;
};

} // namespace decoders
} // namespace audio
} // namespace backend
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_BACKEND_AUDIO_DECODERS_PCM_H
