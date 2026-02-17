// C++ translation of video/software/src/decoder/vp6.rs
//! VP6 video decoder

#ifndef RUFFLE_VIDEO_SOFTWARE_DECODER_VP6_H
#define RUFFLE_VIDEO_SOFTWARE_DECODER_VP6_H

#include <vector>
#include <cstdint>
#include <optional>
#include <memory>

// Forward declarations
namespace ruffle {
namespace video {
    struct EncodedFrame;
    struct DecodedFrame;
    enum class FrameDependency;
    namespace error {
        class Error;
    }
}
}

namespace ruffle {
namespace video {
namespace software {
namespace decoder {

/// VP6 decoder error types
enum class Vp6ErrorType {
    DecoderError,
    UnexpectedSkipFrame,
    InvalidBufferType
};

/// VP6 decoder error
class Vp6Error {
private:
    Vp6ErrorType type_;
    std::string message_;

public:
    explicit Vp6Error(Vp6ErrorType type, const std::string& message = "")
        : type_(type), message_(message) {}

    static Vp6Error decoder_error(const std::string& msg) {
        return Vp6Error(Vp6ErrorType::DecoderError, msg);
    }

    static Vp6Error unexpected_skip_frame() {
        return Vp6Error(Vp6ErrorType::UnexpectedSkipFrame, "Unexpected skip frame");
    }

    static Vp6Error invalid_buffer_type() {
        return Vp6Error(Vp6ErrorType::InvalidBufferType, "Invalid buffer type");
    }

    Vp6ErrorType type() const { return type_; }
    const std::string& message() const { return message_; }
};

/// Convert Vp6Error to generic video Error
video::error::Error to_video_error(const Vp6Error& error);

/// VP6 video decoder.
///
/// Implements the VideoDecoder trait for VP6 codec.
class Vp6Decoder {
private:
    bool with_alpha_;
    std::pair<uint16_t, uint16_t> bounds_;
    
    // Internal decoder state (would use Nihav libraries in real implementation)
    bool init_called_ = false;
    std::optional<std::vector<uint8_t>> last_frame_;

public:
    /// Create a new VP6 decoder.
    ///
    /// @param with_alpha Whether the video stream includes an alpha channel (VP6A)
    /// @param bounds The declared bounds of the video (width, height)
    Vp6Decoder(bool with_alpha, std::pair<uint16_t, uint16_t> bounds)
        : with_alpha_(with_alpha)
        , bounds_(bounds)
        , init_called_(false)
        , last_frame_(std::nullopt)
    {
        // Note: The decoder cannot be fully initialized yet because `bounds`
        // is only the declared size of the video, to which it will be cropped.
        // This can be (rarely) even much smaller than the actual encoded size.
        // The encoded frame size will be parsed from the header of the first
        // encoded frame passed to decode_frame().
    }

    /// Create a default VP6 decoder (no alpha, zero bounds)
    Vp6Decoder() : Vp6Decoder(false, {0, 0}) {}

    /// Preload a frame to determine its dependencies.
    ///
    /// No decoding happens at this point. The video data is inspected to
    /// determine inter-frame dependencies.
    ///
    /// @param encoded_frame The encoded frame data
    /// @return FrameDependency indicating what frames this depends on
    std::optional<FrameDependency> preload_frame(const EncodedFrame& encoded_frame);

    /// Decode a frame.
    ///
    /// @param encoded_frame The encoded frame data
    /// @return The decoded frame or an error
    std::optional<DecodedFrame> decode_frame(const EncodedFrame& encoded_frame);

private:
    /// Initialize the decoder from the first frame's header
    std::optional<Vp6Error> initialize_decoder(const EncodedFrame& encoded_frame);

    /// Crop image data to the specified bounds
    static std::vector<uint8_t> crop(
        const std::vector<uint8_t>& data,
        size_t width,
        std::pair<uint16_t, uint16_t> to_size
    );
};

} // namespace decoder
} // namespace software
} // namespace video
} // namespace ruffle

#endif // RUFFLE_VIDEO_SOFTWARE_DECODER_VP6_H
