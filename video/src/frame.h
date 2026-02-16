// C++ translation of video/src/frame.rs
// Management of video frames

#ifndef RUFFLE_VIDEO_FRAME_H
#define RUFFLE_VIDEO_FRAME_H

#include <cstdint>
#include <cstddef>
#include <memory>

// Forward declaration - will be defined in ruffle_render/bitmap.h
namespace ruffle {
namespace render {
    class Bitmap;
}
}

// Forward declaration - will be defined in swf types
namespace swf {
    enum class VideoCodec : uint8_t;
}

namespace ruffle {
namespace video {

/// An encoded video frame of some video codec.
struct EncodedFrame {
    /// The codec used to encode the frame.
    VideoCodec codec;

    /// The raw bitstream data to funnel into the codec.
    const uint8_t* data;

    /// The size of the data buffer.
    size_t data_size;

    /// A caller-specified frame ID. Frame IDs must be consistent between
    /// subsequent uses of the same data stream.
    uint32_t frame_id;

    /// Borrow this frame's data.
    const uint8_t* get_data() const { return data; }

    /// Get the size of the data.
    size_t get_data_size() const { return data_size; }
};

/// A decoded frame of video. It can be in whichever format the decoder chooses.
using DecodedFrame = std::shared_ptr<ruffle::render::Bitmap>;

/// What dependencies a given video frame has on any previous frames.
enum class FrameDependency {
    /// This frame has no reference frames and can be seeked to at any time.
    None,

    /// This frame has some number of reference frames that prohibit any
    /// out-of-order decoding.
    ///
    /// The only legal way to decode a `Past` frame is to decode every prior
    /// frame from the last `None` frame. In the event that there is no prior
    /// `None` frame, then video decoding should start from the beginning.
    Past,
};

/// Determine if this given frame is a keyframe.
///
/// A keyframe is a frame that can be independently seeked to without
/// decoding any prior or future frames.
inline bool is_keyframe(FrameDependency dependency) {
    return dependency == FrameDependency::None;
}

} // namespace video
} // namespace ruffle

#endif // RUFFLE_VIDEO_FRAME_H
