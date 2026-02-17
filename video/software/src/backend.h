// C++ translation of video/software/src/backend.rs
// Software video backend that proxies to CPU-only codec implementations

#ifndef RUFFLE_VIDEO_SOFTWARE_BACKEND_H
#define RUFFLE_VIDEO_SOFTWARE_BACKEND_H

#include <memory>
#include <vector>

// Forward declarations
namespace ruffle {
namespace render {
    class RenderBackend;
    struct BitmapInfo;
    class BitmapHandle;
}
namespace video {
    struct VideoStreamHandle;
    struct EncodedFrame;
    enum class FrameDependency;
    class VideoDecoder;
    class VideoBackend;

    namespace error {
        class Error;
    }
}
}

// Forward declaration for swf types
namespace swf {
    enum class VideoCodec : unsigned char;
    enum class VideoDeblocking;
}

namespace ruffle {
namespace video {
namespace software {

/// A single preloaded video stream.
struct VideoStream {
    std::shared_ptr<ruffle::render::BitmapHandle> bitmap;
    std::unique_ptr<VideoDecoder> decoder;

    VideoStream() = default;
    explicit VideoStream(std::unique_ptr<VideoDecoder> dec)
        : decoder(std::move(dec)) {}
};

/// Software video backend that proxies to CPU-only codec implementations that
/// ship with Ruffle.
class SoftwareVideoBackend : public VideoBackend {
public:
    SoftwareVideoBackend();
    ~SoftwareVideoBackend() override = default;

    /// Register a new video stream.
    ///
    /// @param num_frames Number of frames in the video
    /// @param size Video dimensions (width, height)
    /// @param codec Video codec to use
    /// @param filter Deblocking filter setting
    /// @return VideoStreamHandle on success, Error on failure
    Result<VideoStreamHandle> register_video_stream(
        uint32_t num_frames,
        std::pair<uint16_t, uint16_t> size,
        swf::VideoCodec codec,
        swf::VideoDeblocking filter
    ) override;

    /// Preload a frame of a given video stream.
    ///
    /// @param stream The video stream handle
    /// @param encoded_frame The encoded frame to preload
    /// @return FrameDependency indicating frame dependencies
    Result<FrameDependency> preload_video_stream_frame(
        VideoStreamHandle stream,
        const EncodedFrame& encoded_frame
    ) override;

    /// Configure the decoder of a given video stream.
    ///
    /// @param stream The video stream handle to configure
    /// @param configuration_data Codec-specific configuration data
    /// @return Result indicating success or error
    Result<void> configure_video_stream_decoder(
        VideoStreamHandle stream,
        const uint8_t* configuration_data,
        size_t configuration_data_size
    ) override;

    /// Decode a frame of a given video stream.
    ///
    /// @param stream The video stream handle
    /// @param encoded_frame The encoded frame to decode
    /// @param renderer The render backend to produce bitmaps for
    /// @return BitmapInfo for the decoded frame, or Error on failure
    Result<ruffle::render::BitmapInfo> decode_video_stream_frame(
        VideoStreamHandle stream,
        const EncodedFrame& encoded_frame,
        ruffle::render::RenderBackend* renderer
    ) override;

private:
    std::vector<VideoStream> streams_;
};

} // namespace software
} // namespace video
} // namespace ruffle

#endif // RUFFLE_VIDEO_SOFTWARE_BACKEND_H
