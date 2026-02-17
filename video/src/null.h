// C++ translation of video/src/null.rs
// Null video backend implementation

#ifndef RUFFLE_VIDEO_NULL_H
#define RUFFLE_VIDEO_NULL_H

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

// Forward declaration for slotmap
template<typename T, typename IndexType>
class SlotMap;

namespace ruffle {
namespace video {

/// Implementation of video that does not decode any video.
///
/// Specifically:
///
///  * Registering a video stream succeeds but does nothing
///  * All video frames are silently marked as keyframes (None dependency)
///  * Video stream decoding fails with an error that video decoding is
///    unimplemented
class NullVideoBackend {
private:
    SlotMap<VideoStreamHandle, void*> streams_;

public:
    /// Create a new null video backend
    NullVideoBackend();

    /// Default constructor
    NullVideoBackend() = default;

    /// Copy constructor
    NullVideoBackend(const NullVideoBackend&) = delete;
    NullVideoBackend& operator=(const NullVideoBackend&) = delete;

    /// Move constructor
    NullVideoBackend(NullVideoBackend&&) = default;
    NullVideoBackend& operator=(NullVideoBackend&&) = default;

    /// Register a new video stream.
    ///
    /// @param num_frames Number of frames in the video
    /// @param size Video dimensions (width, height)
    /// @param codec Video codec used
    /// @param filter Deblocking filter setting
    /// @return VideoStreamHandle on success
    Result<VideoStreamHandle> register_video_stream(
        uint32_t num_frames,
        std::pair<uint16_t, uint16_t> size,
        swf::VideoCodec codec,
        swf::VideoDeblocking filter
    );

    /// Configure the decoder of a given video stream.
    ///
    /// The configuration_data contains codec-specific parameters.
    ///
    /// @param stream The video stream handle to configure
    /// @param configuration_data Codec-specific configuration data
    /// @return Result indicating success
    Result<void> configure_video_stream_decoder(
        VideoStreamHandle stream,
        const uint8_t* configuration_data,
        size_t configuration_data_size
    );

    /// Preload a frame of a given video stream.
    ///
    /// No decoding is intended to happen at this point in time. Instead, the
    /// video data should be inspected to determine inter-frame dependencies
    /// between this and any previous frames in the stream.
    ///
    /// @param stream The video stream handle
    /// @param encoded_frame The encoded frame to preload
    /// @return FrameDependency indicating frame dependencies (always None)
    Result<FrameDependency> preload_video_stream_frame(
        VideoStreamHandle stream,
        const EncodedFrame& encoded_frame
    );

    /// Decode a frame of a given video stream.
    ///
    /// This always returns an error indicating that video decoding is not supported.
    ///
    /// @param stream The video stream handle
    /// @param encoded_frame The encoded frame to decode
    /// @param renderer The render backend to produce bitmaps for
    /// @return Error indicating decoding is not supported
    Result<ruffle::render::BitmapInfo> decode_video_stream_frame(
        VideoStreamHandle stream,
        const EncodedFrame& encoded_frame,
        ruffle::render::RenderBackend* renderer
    );
};

} // namespace video
} // namespace ruffle

#endif // RUFFLE_VIDEO_NULL_H
