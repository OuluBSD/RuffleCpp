// C++ translation of core/src/backend/audio/decoders/mp3.rs
// MP3 audio decoder using Symphonia

#ifndef RUFFLE_CORE_BACKEND_AUDIO_DECODERS_MP3_H
#define RUFFLE_CORE_BACKEND_AUDIO_DECODERS_MP3_H

#include <cstdint>
#include <memory>
#include <optional>
#include <array>
#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
namespace backend {
namespace audio {
namespace decoders {

/// MP3 metadata structure
struct Mp3Metadata {
    uint32_t num_sample_frames;
    uint16_t sample_rate;
};

/// Decoder trait interface
template<typename R>
class Decoder {
public:
    virtual ~Decoder() = default;
    virtual uint8_t num_channels() const = 0;
    virtual uint16_t sample_rate() const = 0;
};

/// Seekable decoder trait interface
template<typename R>
class SeekableDecoder : public Decoder<R> {
public:
    virtual void reset() = 0;
    virtual void seek_to_sample_frame(uint32_t frame) = 0;
};

} // namespace decoders

/// MP3 decoder using Symphonia library
template<typename R>
class Mp3Decoder : public decoders::Decoder<R>, public decoders::SeekableDecoder<R> {
private:
    // MP3 frames contain 1152 samples
    static constexpr uint64_t SAMPLE_BUFFER_DURATION = 1152;

    // Opaque pointers to Symphonia objects
    void* reader_;       // SymphoniaMpaReader
    void* decoder_;      // codecs::Decoder
    void* sample_buf_;   // audio::SampleBuffer<i16>
    
    size_t cur_sample_;
    uint16_t sample_rate_;
    uint8_t num_channels_;
    bool stream_ended_;

public:
    /// Create a new MP3 decoder from a reader
    /// @param reader The input reader (must be Read + Send + Sync)
    /// @return Result containing the decoder or error
    static std::optional<Mp3Decoder> new_from_reader(R reader);

    /// Create a new seekable MP3 decoder from a cursor
    /// @param reader The input cursor (must be AsRef<[u8]> + Send + Sync)
    /// @return Result containing the decoder or error
    static std::optional<Mp3Decoder> new_seekable(R reader);

    ~Mp3Decoder() override;

    /// Decode the next frame into the sample buffer
    void next_frame();

    /// Get the next stereo sample
    /// @return Optional stereo sample [left, right]
    std::optional<std::array<int16_t, 2>> next();

    // Decoder trait implementation
    uint8_t num_channels() const override {
        return num_channels_;
    }

    uint16_t sample_rate() const override {
        return sample_rate_;
    }

    // SeekableDecoder trait implementation
    void reset() override {
        seek_to_sample_frame(0);
    }

    void seek_to_sample_frame(uint32_t frame) override;
};

/// Returns the sample rate and length of the given MP3
/// @param data The MP3 data as a shared array
/// @return Optional Mp3Metadata or error
std::optional<Mp3Metadata> mp3_metadata(const std::shared_ptr<std::vector<uint8_t>>& data);

// Template method implementations

template<typename R>
std::optional<Mp3Decoder<R>> Mp3Decoder<R>::new_from_reader(R reader) {
    // In Rust:
    // let source = Box::new(io::ReadOnlySource::new(reader)) as Box<dyn io::MediaSource>;
    // let source = io::MediaSourceStream::new(source, Default::default());
    // let reader = SymphoniaMpaReader::try_new(source, &Default::default())?;
    // ...
    
    // Placeholder - actual implementation would use Symphonia library
    Mp3Decoder decoder;
    decoder.reader_ = nullptr;
    decoder.decoder_ = nullptr;
    decoder.sample_buf_ = nullptr;
    decoder.cur_sample_ = 0;
    decoder.sample_rate_ = 44100;  // Default sample rate
    decoder.num_channels_ = 2;     // Default stereo
    decoder.stream_ended_ = false;
    
    return decoder;
}

template<typename R>
std::optional<Mp3Decoder<R>> Mp3Decoder<R>::new_seekable(R reader) {
    // In Rust:
    // let source = Box::new(reader) as Box<dyn io::MediaSource>;
    // let source = io::MediaSourceStream::new(source, Default::default());
    // let reader = SymphoniaMpaReader::try_new(source, &Default::default())?;
    // ...
    
    // Placeholder - actual implementation would use Symphonia library
    Mp3Decoder decoder;
    decoder.reader_ = nullptr;
    decoder.decoder_ = nullptr;
    decoder.sample_buf_ = nullptr;
    decoder.cur_sample_ = 0;
    decoder.sample_rate_ = 44100;  // Default sample rate
    decoder.num_channels_ = 2;     // Default stereo
    decoder.stream_ended_ = false;
    
    return decoder;
}

template<typename R>
Mp3Decoder<R>::~Mp3Decoder() {
    // Clean up Symphonia resources
    // In Rust: dropped automatically
}

template<typename R>
void Mp3Decoder<R>::next_frame() {
    if (stream_ended_) {
        return;
    }

    cur_sample_ = 0;
    
    // In Rust:
    // while let Ok(packet) = self.reader.next_packet() {
    //     match self.decoder.decode(&packet) {
    //         Ok(decoded) => { ... }
    //         Err(errors::Error::DecodeError(_)) => ()
    //         Err(_) => break
    //     }
    // }
    // self.stream_ended = true;
    
    // Placeholder - actual implementation would use Symphonia library
    stream_ended_ = true;
}

template<typename R>
std::optional<std::array<int16_t, 2>> Mp3Decoder<R>::next() {
    if (cur_sample_ >= SAMPLE_BUFFER_DURATION) {
        next_frame();
        if (stream_ended_) {
            return std::nullopt;
        }
    }

    // Placeholder - actual implementation would read from sample_buf_
    if (num_channels_ == 2) {
        // Stereo: return [left, right]
        return std::array<int16_t, 2>{0, 0};
    } else {
        // Mono: duplicate to both channels
        return std::array<int16_t, 2>{0, 0};
    }
}

template<typename R>
void Mp3Decoder<R>::seek_to_sample_frame(uint32_t frame) {
    // In Rust:
    // let seek_result = self.reader.seek(
    //     formats::SeekMode::Accurate,
    //     formats::SeekTo::TimeStamp { track_id: 0, ts: frame.into() }
    // );
    // self.sample_buf.clear();
    // self.decoder.reset();
    // self.cur_sample = 0;
    // self.stream_ended = false;
    // ...
    
    // Placeholder - actual implementation would use Symphonia seek
    cur_sample_ = 0;
    stream_ended_ = false;
}

} // namespace audio
} // namespace backend
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_BACKEND_AUDIO_DECODERS_MP3_H
