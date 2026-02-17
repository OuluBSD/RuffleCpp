// C++ translation of video/software/src/decoder/screen.rs
// Screen Video (V1) decoder

#ifndef RUFFLE_VIDEO_SOFTWARE_DECODER_SCREEN_H
#define RUFFLE_VIDEO_SOFTWARE_DECODER_SCREEN_H

#include <vector>
#include <cstdint>
#include <memory>
#include <optional>

// Forward declarations
namespace ruffle {
namespace video {
    class VideoDecoder;
    namespace error {
        class Error;
    }
    namespace frame {
        struct EncodedFrame;
        struct DecodedFrame;
        enum class FrameDependency;
    }
}
namespace render {
namespace bitmap {
    enum class BitmapFormat;
}
}
}

namespace flate2 {
    class Decompress;
    class DecompressError;
}

namespace ruffle {
namespace video {
namespace software {
namespace decoder {
namespace screen {

/// Screen Video decoder error types
enum class ScreenError {
    /// Unexpected end of file
    UnexpectedEOF,

    /// Decompression error
    DecompressionError,

    /// Invalid frame type
    InvalidFrameType,

    /// Missing reference frame
    MissingReferenceFrame,

    /// Keyframe validation error
    KeyframeInvalid,
};

/// Byte reader for Screen Video bitstream
class ByteReader {
private:
    const uint8_t* data_;
    size_t pos_;
    size_t len_;

public:
    /// Create a new byte reader
    ///
    /// \param data Pointer to data buffer
    /// \param len Length of data buffer
    explicit ByteReader(const uint8_t* data, size_t len);

    /// Read a single byte
    ///
    /// \return The next byte
    /// \throws ScreenError::UnexpectedEOF if at end of data
    uint8_t read_byte();

    /// Read a big-endian 16-bit value
    ///
    /// \return The next 16-bit value
    /// \throws ScreenError::UnexpectedEOF if not enough data
    uint16_t read_u16be();

    /// Read a buffer reference (no copy)
    ///
    /// \param length Number of bytes to read
    /// \return Pointer to data buffer
    /// \throws ScreenError::UnexpectedEOF if not enough data
    const uint8_t* read_buf_ref(size_t length);

    /// Get current position
    size_t pos() const { return pos_; }

    /// Check if there is more data
    bool has_data() const { return pos_ < len_; }
};

/// Screen Video (V1 only) decoder
///
/// This decoder is heavily based on flashsv.rs from NihAV.
/// Decodes Screen Video format used in Flash SWF files.
class ScreenVideoDecoder : public VideoDecoder {
private:
    /// Frame width in pixels
    size_t w_;

    /// Frame height in pixels
    size_t h_;

    /// Block width in pixels
    size_t block_w_;

    /// Block height in pixels
    size_t block_h_;

    /// Tile buffer for decompression (scratch space)
    std::vector<uint8_t> tile_;

    /// Last decoded frame (for inter-frame compression)
    std::optional<std::vector<uint8_t>> last_frame_;

    /// Decode a V1 frame
    ///
    /// \param src Byte reader for compressed data
    /// \param data Output frame buffer
    /// \param stride Row stride in bytes
    /// \return true if this is an intra frame, false otherwise
    /// \throws Error on decode failure
    bool decode_v1(ByteReader& src, uint8_t* data, size_t stride);

    /// Flush decoder state (clear reference frame)
    void flush();

public:
    /// Create a new Screen Video decoder
    ScreenVideoDecoder();

    /// Preload a frame to determine its dependencies
    ///
    /// \param encoded_frame The encoded frame to analyze
    /// \return Frame dependency type
    /// \throws Error on parse failure
    frame::FrameDependency preload_frame(const frame::EncodedFrame& encoded_frame) override;

    /// Decode a video frame
    ///
    /// \param encoded_frame The encoded frame to decode
    /// \return The decoded frame
    /// \throws Error on decode failure
    frame::DecodedFrame decode_frame(const frame::EncodedFrame& encoded_frame) override;
};

} // namespace screen
} // namespace decoder
} // namespace software
} // namespace video
} // namespace ruffle

#endif // RUFFLE_VIDEO_SOFTWARE_DECODER_SCREEN_H
