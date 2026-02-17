// C++ translation of video/external/src/decoder.rs
// Video decoder module for external decoders

#ifndef RUFFLE_VIDEO_EXTERNAL_DECODER_H
#define RUFFLE_VIDEO_EXTERNAL_DECODER_H

// Forward declarations
namespace ruffle {
namespace video {
namespace software {
namespace decoder {
    class VideoDecoder;
}
}
}
}

namespace ruffle {
namespace video {
namespace external {
namespace decoder {

// Re-export VideoDecoder from video_software
using VideoDecoder = ruffle::video::software::decoder::VideoDecoder;

// OpenH264 decoder module (conditionally compiled with OPENH264 feature)
#ifdef RUFFLE_OPENH264
namespace openh264 {
    // OpenH264 decoder functionality
}
#endif

// WebCodecs decoder module (conditionally compiled with WEBCODECS feature)
#ifdef RUFFLE_WEBCODECS
namespace webcodecs {
    // WebCodecs decoder functionality
}
#endif

} // namespace decoder
} // namespace external
} // namespace video
} // namespace ruffle

#endif // RUFFLE_VIDEO_EXTERNAL_DECODER_H
