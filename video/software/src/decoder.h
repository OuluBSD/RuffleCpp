// C++ translation of video/software/src/decoder.rs
// Video decoder trait and modules

#ifndef RUFFLE_VIDEO_SOFTWARE_DECODER_H
#define RUFFLE_VIDEO_SOFTWARE_DECODER_H

#include <cstdint>
#include <vector>
#include <memory>
#include <optional>

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

/// Trait for video decoders.
/// This should be implemented for each video codec.
class VideoDecoder {
public:
    virtual ~VideoDecoder() = default;

    /// Configure the decoder.
    virtual std::optional<error::Error> configure_decoder(
        const std::vector<uint8_t>& configuration_data
    ) {
        return std::nullopt; // Ok(())
    }

    /// Preload a frame.
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
    virtual std::optional<FrameDependency> preload_frame(
        const EncodedFrame& encoded_frame
    ) = 0;

    /// Decode a frame of a given video stream.
    ///
    /// This function is provided the external index of the frame, the codec
    /// used to decode the data, and what codec to decode it with. The codec
    /// provided here must match the one used to register the video stream.
    ///
    /// Frames may be decoded in any order that does not violate the frame
    /// dependencies declared by the output of `preload_video_stream_frame`.
    ///
    /// The decoded frame should be returned. An `Error` can be returned if
    /// a drawable bitmap can not be produced.
    virtual std::optional<DecodedFrame> decode_frame(
        const EncodedFrame& encoded_frame
    ) = 0;
};

} // namespace software
} // namespace video
} // namespace ruffle

// Module declarations (submodules are in separate headers)
namespace ruffle {
namespace video {
namespace software {

// Submodule headers would be included here:
// #include "h263.h"      // H.263 decoder (conditional on feature)
// #include "vp6.h"       // VP6 decoder (conditional on feature)
// #include "screen.h"    // Screen video decoder (conditional on feature)

} // namespace software
} // namespace video
} // namespace ruffle

#endif // RUFFLE_VIDEO_SOFTWARE_DECODER_H
