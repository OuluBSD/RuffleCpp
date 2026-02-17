// C++ translation of flv/src/sound.rs
// FLV sound/sound data parsing

#ifndef RUFFLE_FLV_SOUND_H
#define RUFFLE_FLV_SOUND_H

#include <cstdint>
#include <variant>
#include <vector>

// Forward declarations
namespace ruffle {
namespace flv {
namespace error {
    class Error;
}
namespace reader {
    class FlvReader;
}
}
}

namespace ruffle {
namespace flv {
namespace sound {

/// Sound format enumeration.
enum class SoundFormat : uint8_t {
    LinearPCMPlatformEndian = 0,
    Adpcm = 1,
    MP3 = 2,
    LinearPCMLittleEndian = 3,
    Nellymoser16kHz = 4,
    Nellymoser8kHz = 5,
    Nellymoser = 6,
    G711ALawPCM = 7,
    G711MuLawPCM = 8,
    Aac = 10,
    Speex = 11,
    MP38kHz = 14,
    DeviceSpecific = 15,
};

/// Convert uint8_t to SoundFormat.
///
/// @param value The value to convert
/// @return The SoundFormat, or error if invalid
inline Result<SoundFormat> sound_format_from_u8(uint8_t value) {
    switch (value) {
        case 0: return SoundFormat::LinearPCMPlatformEndian;
        case 1: return SoundFormat::Adpcm;
        case 2: return SoundFormat::MP3;
        case 3: return SoundFormat::LinearPCMLittleEndian;
        case 4: return SoundFormat::Nellymoser16kHz;
        case 5: return SoundFormat::Nellymoser8kHz;
        case 6: return SoundFormat::Nellymoser;
        case 7: return SoundFormat::G711ALawPCM;
        case 8: return SoundFormat::G711MuLawPCM;
        case 10: return SoundFormat::Aac;
        case 11: return SoundFormat::Speex;
        case 14: return SoundFormat::MP38kHz;
        case 15: return SoundFormat::DeviceSpecific;
        default: return error::Error::unknown_audio_format_type(value);
    }
}

/// Sound rate enumeration.
enum class SoundRate : uint8_t {
    R5_500 = 0,
    R11_000 = 1,
    R22_000 = 2,
    R44_000 = 3,
};

/// Convert uint8_t to SoundRate.
///
/// @param value The value to convert
/// @return The SoundRate, or error if invalid
inline Result<SoundRate> sound_rate_from_u8(uint8_t value) {
    switch (value) {
        case 0: return SoundRate::R5_500;
        case 1: return SoundRate::R11_000;
        case 2: return SoundRate::R22_000;
        case 3: return SoundRate::R44_000;
        default: return error::Error::unknown_audio_rate(value);
    }
}

/// Sound size enumeration.
enum class SoundSize : uint8_t {
    Bits8 = 0,
    Bits16 = 1,
};

/// Convert uint8_t to SoundSize.
///
/// @param value The value to convert
/// @return The SoundSize, or error if invalid
inline Result<SoundSize> sound_size_from_u8(uint8_t value) {
    switch (value) {
        case 0: return SoundSize::Bits8;
        case 1: return SoundSize::Bits16;
        default: return error::Error::unknown_audio_sample_size(value);
    }
}

/// Sound type enumeration.
enum class SoundType : uint8_t {
    Mono = 0,
    Stereo = 1,
};

/// Convert uint8_t to SoundType.
///
/// @param value The value to convert
/// @return The SoundType, or error if invalid
inline Result<SoundType> sound_type_from_u8(uint8_t value) {
    switch (value) {
        case 0: return SoundType::Mono;
        case 1: return SoundType::Stereo;
        default: return error::Error::unknown_audio_channel_count(value);
    }
}

/// Audio data type.
///
/// For Aac variants, the discriminator byte is kept in the payload.
/// This is 0x00 for AacSequenceHeader and 0x01 for AacRaw.
/// While it's redundant, it's useful for the substream decoder.
struct AudioDataType {
    /// Raw audio data
    struct Raw {
        std::vector<uint8_t> data;
    };

    /// AAC sequence header
    struct AacSequenceHeader {
        std::vector<uint8_t> data;
    };

    /// AAC raw data
    struct AacRaw {
        std::vector<uint8_t> data;
    };

    std::variant<Raw, AacSequenceHeader, AacRaw> variant;

    /// Get the underlying data.
    const std::vector<uint8_t>& data() const {
        return std::visit([](const auto& v) -> const std::vector<uint8_t>& {
            return v.data;
        }, variant);
    }
};

/// Audio data structure.
struct AudioData {
    SoundFormat format;
    SoundRate rate;
    SoundSize size;
    SoundType sound_type;
    AudioDataType data;

    /// Parse audio data from a reader.
    ///
    /// This does not parse the actual audio data itself, which is instead
    /// returned as an array that must be provided to your audio decoder.
    ///
    /// @param reader The reader to parse from
    /// @param data_size The size of the entire audio data structure, including the header
    /// @return The parsed AudioData, or an error
    static Result<AudioData> parse(reader::FlvReader& reader, uint32_t data_size);
};

} // namespace sound

// Re-export for convenience
using sound::SoundFormat;
using sound::SoundRate;
using sound::SoundSize;
using sound::SoundType;
using sound::AudioDataType;
using sound::AudioData;

} // namespace flv
} // namespace ruffle

#endif // RUFFLE_FLV_SOUND_H
