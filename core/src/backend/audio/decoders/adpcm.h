// C++ translation of core/src/backend/audio/decoders/adpcm.rs
// ADPCM audio decoder

#ifndef RUFFLE_CORE_BACKEND_AUDIO_DECODERS_ADPCM_H
#define RUFFLE_CORE_BACKEND_AUDIO_DECODERS_ADPCM_H

#include <cstdint>
#include <vector>
#include <array>
#include <optional>
#include <functional>

// Forward declarations
namespace ruffle {
namespace core {
namespace backend {
namespace audio {
namespace decoders {

// Forward declare from parent module
template<typename R>
class Decoder;
template<typename R>
class SeekableDecoder;

struct SoundStreamInfo;
template<typename GC>
class Substream;
template<typename GC>
class SubstreamTagReader;

} // namespace decoders
} // namespace audio
} // namespace backend

namespace swf {
struct SoundFormat;
}

} // namespace core
} // namespace ruffle

namespace ruffle {
namespace core {
namespace backend {
namespace audio {
namespace decoders {

/// ADPCM decoder error types
enum class AdpcmError {
    Invalid
};

/// ADPCM channel state
struct Channel {
    int16_t sample = 0;
    int16_t step_index = 0;
};

/// Index table for ADPCM step adjustment
constexpr const int16_t* INDEX_TABLE[4] = {
    new int16_t[2]{-1, 2},
    new int16_t[4]{-1, -1, 2, 4},
    new int16_t[8]{-1, -1, -1, -1, 2, 4, 6, 8},
    new int16_t[16]{-1, -1, -1, -1, -1, -1, -1, -1, 1, 2, 4, 6, 8, 10, 13, 16}
};

/// Step table for ADPCM decoding
constexpr uint16_t STEP_TABLE[89] = {
    7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31, 34, 37, 41, 45, 50, 55, 60, 66,
    73, 80, 88, 97, 107, 118, 130, 143, 157, 173, 190, 209, 230, 253, 279, 307, 337, 371, 408, 449,
    494, 544, 598, 658, 724, 796, 876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066, 2272,
    2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358, 5894, 6484, 7132, 7845, 8630, 9493,
    10442, 11487, 12635, 13899, 15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
};

/// Sample delta calculator function type
using SampleDeltaCalculator = std::function<uint16_t(uint16_t, uint32_t)>;

/// ADPCM decoder for streaming audio
template<typename R>
class AdpcmDecoder : public Decoder<R> {
private:
    R inner_;                    // BitReader wrapping the input
    uint16_t sample_rate_;
    size_t bits_per_sample_;
    uint16_t sample_num_;
    std::vector<Channel> channels_;
    SampleDeltaCalculator decoder_;

    /// Sample delta calculator for 2 bits
    static uint16_t delta_2bits(uint16_t step, uint32_t magnitude) {
        uint16_t delta = step >> 1;
        if (magnitude & 1) delta += step;
        return delta;
    }

    /// Sample delta calculator for 3 bits
    static uint16_t delta_3bits(uint16_t step, uint32_t magnitude) {
        uint16_t delta = step >> 2;
        if (magnitude & 1) delta += step >> 1;
        if (magnitude & 2) delta += step;
        return delta;
    }

    /// Sample delta calculator for 4 bits
    static uint16_t delta_4bits(uint16_t step, uint32_t magnitude) {
        uint16_t delta = step >> 3;
        if (magnitude & 1) delta += step >> 2;
        if (magnitude & 2) delta += step >> 1;
        if (magnitude & 4) delta += step;
        return delta;
    }

    /// Sample delta calculator for 5 bits
    static uint16_t delta_5bits(uint16_t step, uint32_t magnitude) {
        uint16_t delta = step >> 4;
        if (magnitude & 1) delta += step >> 3;
        if (magnitude & 2) delta += step >> 2;
        if (magnitude & 4) delta += step >> 1;
        if (magnitude & 8) delta += step;
        return delta;
    }

    static const SampleDeltaCalculator SAMPLE_DELTA_CALCULATOR[4];

public:
    /// Create a new ADPCM decoder
    /// @param inner The input reader
    /// @param is_stereo Whether the audio is stereo
    /// @param sample_rate The sample rate in Hz
    /// @return Optional decoder or error
    static std::optional<AdpcmDecoder> new_from_reader(R inner, bool is_stereo, uint16_t sample_rate);

    ~AdpcmDecoder() = default;

    // Decoder trait implementation
    uint8_t num_channels() const override {
        return static_cast<uint8_t>(channels_.size());
    }

    uint16_t sample_rate() const override {
        return sample_rate_;
    }

    /// Get the next stereo sample
    std::optional<std::array<int16_t, 2>> next();
};

// Static member initialization
template<typename R>
const SampleDeltaCalculator AdpcmDecoder<R>::SAMPLE_DELTA_CALCULATOR[4] = {
    delta_2bits,
    delta_3bits,
    delta_4bits,
    delta_5bits
};

/// Seekable ADPCM decoder for cursor-based readers
template<typename R>
class SeekableAdpcmDecoder : public AdpcmDecoder<R>, public SeekableDecoder<R> {
public:
    SeekableAdpcmDecoder(R inner, bool is_stereo, uint16_t sample_rate)
        : AdpcmDecoder<R>(inner, is_stereo, sample_rate) {}

    /// Reset the decoder to the beginning
    void reset() override;
};

/// ADPCM substream decoder for sound streams
template<typename GC>
class AdpcmSubstreamDecoder : public Decoder<void*> {
private:
    swf::SoundFormat format_;
    SubstreamTagReader<GC> tag_reader_;
    AdpcmDecoder<void*> decoder_;  // Using void* as placeholder

public:
    /// Create a new ADPCM substream decoder
    /// @param stream_info The sound stream info
    /// @param data_stream The substream data
    /// @return Optional decoder or error
    static std::optional<AdpcmSubstreamDecoder> new_from_stream(
        const SoundStreamInfo* stream_info,
        Substream<GC> data_stream
    );

    uint8_t num_channels() const override {
        return decoder_.num_channels();
    }

    uint16_t sample_rate() const override {
        return decoder_.sample_rate();
    }

    /// Get the next stereo sample
    std::optional<std::array<int16_t, 2>> next();
};

// Template method implementations

template<typename R>
std::optional<AdpcmDecoder<R>> AdpcmDecoder<R>::new_from_reader(R inner, bool is_stereo, uint16_t sample_rate) {
    // In Rust:
    // let mut reader = BitReader::new(inner);
    // let bits_per_sample = reader.read::<2, u8>().map_err(|_| Error::Invalid)? as usize + 2;
    
    AdpcmDecoder decoder;
    decoder.inner_ = inner;
    decoder.sample_rate_ = sample_rate;
    decoder.bits_per_sample_ = 2;  // Default, would be read from stream
    decoder.sample_num_ = 0;
    
    size_t num_channels = is_stereo ? 2 : 1;
    decoder.channels_.resize(num_channels);
    
    // Select the appropriate delta calculator based on bits_per_sample
    decoder.decoder_ = SAMPLE_DELTA_CALCULATOR[decoder.bits_per_sample_ - 2];
    
    return decoder;
}

template<typename R>
std::optional<std::array<int16_t, 2>> AdpcmDecoder<R>::next() {
    // In Rust:
    // if self.sample_num == 0 {
    //     // Read initial sample values (NOT byte-aligned)
    //     for channel in &mut self.channels {
    //         channel.sample = self.inner.read_signed::<16, i16>().ok()?;
    //         channel.step_index = self.inner.read::<6, u16>().ok()? as i16;
    //     }
    // }
    // self.sample_num = (self.sample_num + 1) % 4095;
    
    // For each channel, decode the next sample
    for (auto& channel : channels_) {
        uint16_t step = STEP_TABLE[channel.step_index];
        
        // Read data (sign-magnitude, NOT two's complement)
        uint32_t data = 0;  // Would be read from bit stream
        
        uint32_t sign_mask = 1 << (bits_per_sample_ - 1);
        uint32_t magnitude = data & ~sign_mask;
        
        // (data + 0.5) * step / 2^(bits_per_sample - 2)
        uint16_t delta = decoder_(step, magnitude);
        
        if (data & sign_mask) {
            channel.sample = static_cast<int16_t>(
                std::max(static_cast<int32_t>(channel.sample) - static_cast<int32_t>(delta), 
                         static_cast<int32_t>(INT16_MIN))
            );
        } else {
            channel.sample = static_cast<int16_t>(
                std::min(static_cast<int32_t>(channel.sample) + static_cast<int32_t>(delta), 
                         static_cast<int32_t>(INT16_MAX))
            );
        }
        
        // Adjust step index
        channel.step_index += INDEX_TABLE[bits_per_sample_ - 2][magnitude];
        channel.step_index = std::clamp(channel.step_index, 
                                        static_cast<int16_t>(0), 
                                        static_cast<int16_t>(88));
    }
    
    int16_t left = channels_[0].sample;
    int16_t right = channels_.size() > 1 ? channels_[1].sample : left;
    
    return std::array<int16_t, 2>{left, right};
}

template<typename R>
void SeekableAdpcmDecoder<R>::reset() {
    // In Rust:
    // let bit_stream = std::mem::replace(&mut self.inner, BitReader::new(Default::default()));
    // let mut cursor = bit_stream.into_reader();
    // cursor.set_position(0);
    // *self = AdpcmDecoder::new(cursor, self.num_channels() == 2, self.sample_rate())
    //     .expect("Existing valid decoder should be valid when recreated");
    
    // Placeholder - would recreate decoder with reset cursor
}

template<typename GC>
std::optional<AdpcmSubstreamDecoder<GC>> AdpcmSubstreamDecoder<GC>::new_from_stream(
    const SoundStreamInfo* stream_info,
    Substream<GC> data_stream
) {
    // In Rust:
    // let empty_buffer = data_stream.buffer().to_empty_slice();
    // let mut tag_reader = SubstreamTagReader::new(stream_info, data_stream);
    // let audio_data = tag_reader.next().unwrap_or(empty_buffer);
    // let decoder = AdpcmDecoder::new(
    //     audio_data.as_cursor(),
    //     stream_info.stream_format.is_stereo,
    //     stream_info.stream_format.sample_rate,
    // )?;
    
    return std::nullopt;  // Placeholder
}

template<typename GC>
std::optional<std::array<int16_t, 2>> AdpcmSubstreamDecoder<GC>::next() {
    // In Rust:
    // if let Some(sample_frame) = self.decoder.next() {
    //     Some(sample_frame)
    // } else if let Some(audio_data) = self.tag_reader.next() {
    //     // Recreate decoder with new audio data
    //     self.decoder = AdpcmDecoder::new(...).ok()?;
    //     self.decoder.next()
    // } else {
    //     None
    // }
    
    return std::nullopt;  // Placeholder
}

} // namespace decoders
} // namespace audio
} // namespace backend
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_BACKEND_AUDIO_DECODERS_ADPCM_H
