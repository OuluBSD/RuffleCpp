// C++ translation of video/software/src/decoder/h263.rs
// H263 video decoder

#ifndef RUFFLE_VIDEO_SOFTWARE_DECODER_H263_H
#define RUFFLE_VIDEO_SOFTWARE_DECODER_H263_H

#include <vector>
#include <cstdint>
#include <memory>
#include <optional>
#include <stdexcept>

// Forward declarations
namespace ruffle {
namespace render {
namespace bitmap {
    enum class BitmapFormat;
}
}

namespace video {
namespace error {
    class Error;
}
namespace frame {
    class DecodedFrame;
    class EncodedFrame;
    enum class FrameDependency;
}
}

namespace swf {
    enum class VideoDeblocking;
}
}

// Forward declare h263_rs types (would be provided by external library)
namespace h263_rs {
    class Error;
    enum class PictureTypeCode;
    struct PictureOption;
    struct DecoderOption;
    class H263State;
    class H263Reader;
}

namespace h263_rs_deblock {
namespace deblock {
    extern const std::vector<double> QUANT_TO_STRENGTH;
    std::vector<uint8_t> deblock(const std::vector<uint8_t>& samples, size_t width, double strength);
}
}

namespace ruffle {
namespace video {
namespace software {
namespace decoder {

/// H263 decoder errors
enum class H263Error {
    NoPictureInVideoStream,
    DecoderError,
    InvalidPictureType,
    MissingWidthHeight
};

/// H263 video decoder
class H263Decoder {
private:
    std::unique_ptr<h263_rs::H263State> state_;
    swf::VideoDeblocking deblocking_;

public:
    /// Create a new H263 decoder with the specified deblocking setting
    /// @param deblock The video deblocking level to use
    explicit H263Decoder(swf::VideoDeblocking deblock);

    /// Default constructor with default deblocking setting
    H263Decoder();

    /// Preload a frame to determine its dependency
    /// @param encoded_frame The encoded frame data
    /// @return The frame dependency (None or Past)
    /// @throws Error if the frame cannot be preloaded
    frame::FrameDependency preload_frame(const frame::EncodedFrame& encoded_frame);

    /// Decode a frame
    /// @param encoded_frame The encoded frame data
    /// @return The decoded frame
    /// @throws Error if the frame cannot be decoded
    frame::DecodedFrame decode_frame(const frame::EncodedFrame& encoded_frame);

private:
    /// Helper to convert H263Error to Error
    static error::Error convert_error(H263Error err);
    static error::Error convert_error(const h263_rs::Error& err);
};

} // namespace decoder
} // namespace software
} // namespace video
} // namespace ruffle

#endif // RUFFLE_VIDEO_SOFTWARE_DECODER_H263_H
