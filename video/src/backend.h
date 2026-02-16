// C++ translation of video/src/backend.rs
// Video backend trait for decoder access

#ifndef RUFFLE_VIDEO_BACKEND_H
#define RUFFLE_VIDEO_BACKEND_H

#include <cstdint>
#include <memory>

// Forward declarations
namespace ruffle {
namespace video {
    struct VideoStreamHandle;
    struct EncodedFrame;
    enum class FrameDependency;

    namespace error {
        class Error;
    }
}
namespace render {
    class RenderBackend;
    struct BitmapInfo;
}
}

// Forward declaration for swf types
namespace swf {
    enum class VideoCodec : uint8_t;
    enum class VideoDeblocking;
}

namespace ruffle {
namespace video {

/// A backend that provides access to some number of video decoders.
///
/// Implementations of VideoBackend are not required to actually support
/// decoding any video formats. However, they must interoperate with at least
/// one RenderBackend so that renderable video frames may be passed from the
/// decoder to the renderer.
class VideoBackend {
public:
    virtual ~VideoBackend() = default;

    /// Register a new video stream.
    ///
    /// Most of the parameters provided to this function are advisory: the
    /// actual video data stream provided to the decoder may vary in size or
    /// number of frames. This function should return an Error if it is not
    /// possible to decode video with the given parameters.
    ///
    /// @param num_frames Number of frames in the video
    /// @param size Video dimensions (width, height)
    /// @param codec Video codec used
    /// @param filter Deblocking filter setting
    /// @return VideoStreamHandle on success, Error on failure
    virtual Result<VideoStreamHandle> register_video_stream(
        uint32_t num_frames,
        std::pair<uint16_t, uint16_t> size,
        swf::VideoCodec codec,
        swf::VideoDeblocking filter
    ) = 0;

    /// Configure the decoder of a given video stream.
    ///
    /// The configuration_data contains codec-specific parameters.
    ///
    /// @param stream The video stream handle to configure
    /// @param configuration_data Codec-specific configuration data
    /// @return Result indicating success or error
    virtual Result<void> configure_video_stream_decoder(
        VideoStreamHandle stream,
        const uint8_t* configuration_data,
        size_t configuration_data_size
    ) = 0;

    /// Preload a frame of a given video stream.
    ///
    /// No decoding is intended to happen at this point in time. Instead, the
    /// video data should be inspected to determine inter-frame dependencies
    /// between this and any previous frames in the stream.
    ///
    /// Frames should be preloaded in the order that they are received.
    ///
    /// Any dependencies listed here are inherent to the video bitstream. The
    /// containing video stream is also permitted to introduce additional
    /// interframe dependencies.
    ///
    /// @param stream The video stream handle
    /// @param encoded_frame The encoded frame to preload
    /// @return FrameDependency indicating frame dependencies
    virtual Result<FrameDependency> preload_video_stream_frame(
        VideoStreamHandle stream,
        const EncodedFrame& encoded_frame
    ) = 0;

    /// Decode a frame of a given video stream.
    ///
    /// This function is provided the external index of the frame, the codec
    /// used to decode the data, and what codec to decode it with. The codec
    /// provided here must match the one used to register the video stream.
    ///
    /// Frames may be decoded in any order that does not violate the frame
    /// dependencies declared by the output of preload_video_stream_frame.
    ///
    /// The resulting BitmapInfo will be renderable only on the given
    /// RenderBackend. VideoBackend implementations are allowed to return
    /// an error if a drawable bitmap cannot be produced for the given
    /// renderer.
    ///
    /// Any previously returned bitmaps may be updated, invalidated, or
    /// reclaimed by whatever means the decoder implementation chooses.
    ///
    /// @param stream The video stream handle
    /// @param encoded_frame The encoded frame to decode
    /// @param renderer The render backend to produce bitmaps for
    /// @return BitmapInfo for the decoded frame, or Error on failure
    virtual Result<ruffle::render::BitmapInfo> decode_video_stream_frame(
        VideoStreamHandle stream,
        const EncodedFrame& encoded_frame,
        ruffle::render::RenderBackend* renderer
    ) = 0;
};

} // namespace video
} // namespace ruffle

#endif // RUFFLE_VIDEO_BACKEND_H
