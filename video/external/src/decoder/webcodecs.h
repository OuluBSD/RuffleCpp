// C++ translation of video/external/src/decoder/webcodecs.rs
//! WebCodecs H.264 video decoder
//!
//! This module provides H.264 video decoding using the WebCodecs API.
//! It is used in web environments where WebCodecs is available.

#ifndef RUFFLE_VIDEO_EXTERNAL_DECODER_WEBCODECS_H
#define RUFFLE_VIDEO_EXTERNAL_DECODER_WEBCODECS_H

#include <cstdint>
#include <vector>
#include <memory>
#include <optional>
#include <functional>

// Forward declarations
namespace ruffle {
namespace video {
namespace error {
    class Error;
}
namespace frame {
    class DecodedFrame;
    class EncodedFrame;
    enum class FrameDependency;
}
namespace external {
namespace decoder {

/// Video decoder interface
class VideoDecoder {
public:
    virtual ~VideoDecoder() = default;

    /// Configure the decoder with codec configuration data
    virtual bool configure_decoder(const std::vector<uint8_t>& configuration_data) = 0;

    /// Preload a frame to determine its dependency
    virtual FrameDependency preload_frame(const EncodedFrame& encoded_frame) = 0;

    /// Decode a frame
    virtual DecodedFrame decode_frame(const EncodedFrame& encoded_frame) = 0;
};

} // namespace decoder
} // namespace external
}
}

namespace ruffle {
namespace render {
namespace bitmap {
    enum class BitmapFormat {
        Yuv420p,
        Rgba,
        // Other formats...
    };
}
}
}

namespace ruffle {
namespace video {
namespace external {
namespace decoder {
namespace webcodecs {

// NALU type constants
// NALU type 5 means IDR frame - basically a keyframe.
constexpr uint8_t NALU_TYPE_IDR = 5;

/// H.264 decoder using WebCodecs API
///
/// Abbreviations:
/// - NAL: Network Abstraction Layer
/// - NALU: NAL unit
/// - VCL: Video Coding Layer
/// - SPS: Sequence Parameter Set
/// - PPS: Picture Parameter Set
/// - IDR: Instantaneous Decoding Refresh
/// - SEI: Supplemental enhancement information
class H264Decoder : public VideoDecoder {
public:
    H264Decoder() = default;
    ~H264Decoder() override = default;

    /// Create a new H264Decoder
    ///
    /// `configuration_data` should hold "AVCC (MP4) format" decoder configuration,
    /// including PPS and SPS. Make sure it has any start code emulation prevention
    /// "three bytes" removed.
    ///
    /// @param log_subscriber Logging subscriber for callback logging
    /// @return New H264Decoder instance, or error on failure
    static std::unique_ptr<H264Decoder> create();

    /// Configure the decoder with codec configuration data
    ///
    /// extradata[0]: configuration version, always 1
    /// extradata[1]: profile
    /// extradata[2]: compatibility
    /// extradata[3]: level
    /// extradata[4]: 6 reserved bits | NALU length size - 1
    ///
    /// @param configuration_data Codec configuration data
    /// @return true on success, false on error
    bool configure_decoder(const std::vector<uint8_t>& configuration_data) override;

    /// Preload a frame to determine its dependency
    ///
    /// @param encoded_frame The encoded frame to preload
    /// @return FrameDependency indicating frame dependencies
    FrameDependency preload_frame(const frame::EncodedFrame& encoded_frame) override;

    /// Decode a frame
    ///
    /// @param encoded_frame The encoded frame to decode
    /// @return DecodedFrame on success, Error on failure
    frame::DecodedFrame decode_frame(const frame::EncodedFrame& encoded_frame) override;

private:
    /// How many bytes are used to store the length of the NALU (1, 2, 3, or 4).
    uint8_t length_size_ = 0;

    /// The decoder output callback writes this, and the decode_frame method reads it.
    ///
    /// This in itself results in one frame of delay (because we can't block decode_frame
    /// until the callback is invoked), but it shouldn't matter in practice.
    std::optional<frame::DecodedFrame> last_frame_;

    /// Convert JavaScript error to decoder error
    ///
    /// @param js_error JavaScript error value
    /// @return Video error
    static error::Error js_error_to_decoder_error(const void* js_error);
};

/// Provides an iterator for individual consecutive NALUs in a byte stream,
/// also providing the type of each NALU for easier usage.
///
/// @param data The byte stream containing NALUs
/// @param length_size Size of length prefix for each NALU
/// @return Iterator yielding (nalu_type, nalu_data) pairs
class NaluIterator {
public:
    NaluIterator(const std::vector<uint8_t>& data, size_t length_size);

    /// Get the next NALU
    ///
    /// @return Optional pair of (nalu_type, nalu_data), or nullopt if no more NALUs
    std::optional<std::pair<uint8_t, std::vector<uint8_t>>> next();

private:
    const std::vector<uint8_t>& data_;
    size_t length_size_;
    size_t position_ = 0;
};

/// Pixel format enumeration matching WebCodecs VideoPixelFormat
enum class VideoPixelFormat {
    I420,
    Bgrx,
    // Other formats...
};

/// Encoded video chunk type
enum class EncodedVideoChunkType {
    Key,
    Delta,
};

} // namespace webcodecs
} // namespace decoder
} // namespace external
} // namespace video
} // namespace ruffle

#endif // RUFFLE_VIDEO_EXTERNAL_DECODER_WEBCODECS_H
