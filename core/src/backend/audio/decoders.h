// C++ translation of core/src/backend/audio/decoders.rs
//! Audio decoders

#ifndef RUFFLE_CORE_BACKEND_AUDIO_DECODERS_H
#define RUFFLE_CORE_BACKEND_AUDIO_DECODERS_H

#include <cstdint>
#include <memory>
#include <optional>
#include <variant>
#include <string>
#include <vector>
#include <iterator>

// Forward declarations
namespace ruffle {
namespace core {
    namespace backend {
        namespace audio {
            struct SoundStreamInfo;
            enum class SoundStreamWrapping;
        }
    }
    namespace tag_utils {
        template<typename GC> class SwfSlice;
    }
    namespace ruffle_common {
        namespace buffer {
            class Slice;
            class Substream;
            template<typename T> class SubstreamChunksIter;
        }
    }
}
}

namespace swf {
    struct SoundFormat;
    struct SoundStreamHead;
    enum class AudioCompression;
    enum class TagCode;
}

namespace ruffle {
namespace core {
namespace backend {
namespace audio {

using tag_utils::SwfSlice;
using ruffle_common::buffer::Slice;
using ruffle_common::buffer::Substream;
using ruffle_common::buffer::SubstreamChunksIter;

/// Audio decoder error types
class DecoderError {
public:
    enum class Kind {
        InvalidMp3,
        InvalidAac,
        InvalidAdpcm,
        UnhandledCompression,
        TooManySounds
    };

    DecoderError() = default;
    explicit DecoderError(Kind kind) : kind_(kind) {}

    Kind kind() const { return kind_; }

private:
    Kind kind_ = Kind::InvalidAdpcm;
};

/// An audio decoder. Can be used as an `Iterator` to return stereo sample frames.
/// If the sound is mono, the sample is duplicated across both channels.
class Decoder {
public:
    virtual ~Decoder() = default;

    /// The number of channels of this audio decoder. Always 1 or 2.
    virtual uint8_t num_channels() const = 0;

    /// The sample rate of this audio decoder.
    virtual uint16_t sample_rate() const = 0;

    /// Get next sample frame [left, right]
    virtual std::optional<std::array<int16_t, 2>> next() = 0;
};

/// Instantiate a decoder for the compression that the sound data uses.
std::unique_ptr<Decoder> make_decoder(
    const swf::SoundFormat& format,
    std::unique_ptr<std::istream> data
);

/// Audio stream decoder.
///
/// A "stream" sound is a sound that has its data distributed across `SoundStreamBlock` tags,
/// one per each frame of a MovieClip. The sound is synced to the MovieClip's timeline, and will
/// stop/seek as the MovieClip stops/seeks.
///
/// In the Flash IDE, the is created by changing the "Sync" setting of the sound
/// to "Stream."
class StreamDecoder : public Decoder {
public:
    ~StreamDecoder() override = default;
};

/// The `StandardStreamDecoder` takes care of reading the audio data from `SoundStreamBlock` tags
/// and feeds it to the decoder.
class StandardStreamDecoder : public StreamDecoder {
private:
    /// The underlying decoder. The decoder will get its data from a `StreamTagReader`.
    std::unique_ptr<Decoder> decoder_;

public:
    /// Constructs a new `StandardStreamDecoder.
    /// `swf_data` should be the tag data of the MovieClip that contains the stream.
    static std::variant<std::unique_ptr<StandardStreamDecoder>, DecoderError> new_(
        const swf::SoundStreamHead& stream_info,
        SwfSlice swf_data
    );

    uint8_t num_channels() const override {
        return decoder_->num_channels();
    }

    uint16_t sample_rate() const override {
        return decoder_->sample_rate();
    }

    std::optional<std::array<int16_t, 2>> next() override {
        return decoder_->next();
    }
};

/// ADPCM stream decoder.
///
/// Stream sounds encoded with ADPCM have an ADPCM header in each `SoundStreamBlock` tag, unlike
/// other compression formats that remain the same as if they were a single sound clip.
/// Therefore, we must recreate the decoder with each `SoundStreamBlock` to parse the additional
/// headers.
class AdpcmStreamDecoder : public StreamDecoder {
private:
    swf::SoundFormat format_;
    // StreamTagReader would be here
    std::unique_ptr<Decoder> decoder_;

public:
    static std::variant<std::unique_ptr<AdpcmStreamDecoder>, DecoderError> new_(
        const swf::SoundStreamHead& stream_info,
        SwfSlice swf_data
    );

    uint8_t num_channels() const override {
        return decoder_->num_channels();
    }

    uint16_t sample_rate() const override {
        return decoder_->sample_rate();
    }

    std::optional<std::array<int16_t, 2>> next() override;
};

/// Makes a `StreamDecoder` for the given stream. `swf_data` should be the MovieClip's tag data.
/// Generally this will return a `StandardStreamDecoder`, except for ADPCM streams.
std::variant<std::unique_ptr<StreamDecoder>, DecoderError> make_stream_decoder(
    const swf::SoundStreamHead& stream_info,
    SwfSlice swf_data
);

/// Adds seeking ability to decoders where the underline stream is `std::io::Seek`.
class SeekableDecoder : public Decoder {
public:
    ~SeekableDecoder() override = default;

    /// Resets the decoder to the beginning of the stream.
    virtual void reset() = 0;

    /// Seeks to a specific sample frame.
    virtual void seek_to_sample_frame(uint32_t frame) {
        // The default implementation simply resets the stream and steps through
        // until the desired position.
        // This will be slow for long sounds on heavy decoders.
        reset();
        for (uint32_t i = 0; i < frame; ++i) {
            next();
        }
    }
};

/// `StreamTagReader` reads through the SWF tag data of a `MovieClip`, extracting
/// audio data from the `SoundStreamBlock` tags. It can be used as an `Iterator` that
/// will return consecutive slices of the underlying audio data.
class StreamTagReader {
private:
    /// The tag data of the `MovieClip` that contains the streaming audio track.
    SwfSlice swf_data_;

    /// The audio playback position inside `swf_data`.
    size_t pos_;

    /// The compressed audio data in the most recent `SoundStreamBlock` we've seen, returned by `Iterator::next`.
    SwfSlice current_audio_data_;

    /// The compression used by the audio data.
    swf::AudioCompression compression_;

    /// The number of audio samples for use in future animation frames.
    ///
    /// Only used in MP3 encoding to properly handle gaps in the audio track.
    int32_t mp3_samples_buffered_;

    /// The ideal number of audio samples in each animation frame, i.e. the sample rate divided by frame rate.
    ///
    /// Only used in MP3 encoding to properly handle gaps in the audio track.
    uint16_t mp3_samples_per_block_;

public:
    /// Builds a new `StreamTagReader` from the given SWF data.
    /// `swf_data` should be the tag data of a MovieClip.
    static StreamTagReader new_(
        const swf::SoundStreamHead& stream_info,
        SwfSlice swf_data
    );

    /// Get next audio data slice
    std::optional<SwfSlice> next();
};

/// MP3 metadata
struct Mp3Metadata {
    uint16_t sample_rate;
    uint32_t num_sample_frames;
};

/// `SubstreamTagReader` reads through the data chunks of a `Substream` with
/// audio data in it. Data is assumed to have already been extracted from its
/// container.
///
/// MP3 data will be further joined into properly-sized chunks.
class SubstreamTagReader {
private:
    /// The tag data of the `MovieClip` that contains the streaming audio track.
    SubstreamChunksIter<Slice> data_stream_;

    /// The compressed audio data in the most recent `SoundStreamBlock` we've seen, returned by `Iterator::next`.
    std::optional<Slice> current_audio_data_;

    /// The compression used by the audio data.
    swf::AudioCompression compression_;

    /// The wrapping used by audio chunks in the stream.
    SoundStreamWrapping wrapping_;

    /// The number of audio samples for use in future animation frames.
    ///
    /// Only used in MP3 encoding to properly handle gaps in the audio track.
    int32_t mp3_samples_buffered_;

    /// The ideal number of audio samples in each animation frame, i.e. the sample rate divided by frame rate.
    ///
    /// Only used in MP3 encoding to properly handle gaps in the audio track.
    uint16_t mp3_samples_per_block_;

public:
    /// Builds a new `SubstreamTagReader` from the given `Substream`.
    ///
    /// `Substream` should reference audio data.
    static SubstreamTagReader new_(
        const SoundStreamInfo& stream_info,
        Substream data_stream
    );

    /// Get next audio data slice
    std::optional<Slice> next();
};

/// Create a new decoder that reads data from a shared `Substream` instance.
///
/// This works similarly to `make_stream_decoder` but using the new buffer
/// infrastructure that will eventually replace SWF-specific streaming.
///
/// The substream is shared in order to allow appending additional data into
/// the stream.
std::variant<std::unique_ptr<Decoder>, DecoderError> make_substream_decoder(
    const SoundStreamInfo& stream_info,
    Substream data_stream
);

/// The `StandardSubstreamDecoder` takes care of reading the audio data from
/// the chunks of a `Substream` and feeding it to the decoder.
class StandardSubstreamDecoder : public Decoder {
private:
    /// The underlying decoder. The decoder will get its data from a `Substream`.
    std::unique_ptr<Decoder> decoder_;

public:
    /// Constructs a new `StandardSubstreamDecoder`.
    /// `swf_data` should be the tag data of the MovieClip that contains the stream.
    static std::variant<std::unique_ptr<StandardSubstreamDecoder>, DecoderError> new_(
        const SoundStreamInfo& stream_info,
        Substream data_stream
    );

    uint8_t num_channels() const override {
        return decoder_->num_channels();
    }

    uint16_t sample_rate() const override {
        return decoder_->sample_rate();
    }

    std::optional<std::array<int16_t, 2>> next() override {
        return decoder_->next();
    }
};

// Implementation

inline StreamTagReader StreamTagReader::new_(
    const swf::SoundStreamHead& stream_info,
    SwfSlice swf_data
) {
    return StreamTagReader{
        swf_data,
        0,
        SwfSlice::empty(swf_data.movie()),
        stream_info.stream_format.compression,
        0,
        stream_info.num_samples_per_block
    };
}

inline SubstreamTagReader SubstreamTagReader::new_(
    const SoundStreamInfo& stream_info,
    Substream data_stream
) {
    return SubstreamTagReader{
        data_stream.iter_chunks(),
        std::nullopt,
        stream_info.stream_format.compression,
        stream_info.wrapping,
        0,
        stream_info.num_samples_per_block
    };
}

inline std::variant<std::unique_ptr<StreamDecoder>, DecoderError> make_stream_decoder(
    const swf::SoundStreamHead& stream_info,
    SwfSlice swf_data
) {
    if (stream_info.stream_format.compression == swf::AudioCompression::Adpcm) {
        auto result = AdpcmStreamDecoder::new_(stream_info, swf_data);
        if (std::holds_alternative<DecoderError>(result)) {
            return std::get<DecoderError>(result);
        }
        return std::variant<std::unique_ptr<StreamDecoder>, DecoderError>(
            std::move(std::get<std::unique_ptr<AdpcmStreamDecoder>>(result))
        );
    } else {
        auto result = StandardStreamDecoder::new_(stream_info, swf_data);
        if (std::holds_alternative<DecoderError>(result)) {
            return std::get<DecoderError>(result);
        }
        return std::variant<std::unique_ptr<StreamDecoder>, DecoderError>(
            std::move(std::get<std::unique_ptr<StandardStreamDecoder>>(result))
        );
    }
}

inline std::variant<std::unique_ptr<Decoder>, DecoderError> make_substream_decoder(
    const SoundStreamInfo& stream_info,
    Substream data_stream
) {
    switch (stream_info.stream_format.compression) {
        case swf::AudioCompression::Adpcm:
            // Would create AdpcmSubstreamDecoder
            return DecoderError(DecoderError::Kind::InvalidAdpcm);
        default:
            return StandardSubstreamDecoder::new_(stream_info, data_stream);
    }
}

} // namespace audio
} // namespace backend
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_BACKEND_AUDIO_DECODERS_H
