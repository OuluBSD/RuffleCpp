// C++ translation of flv/src/video.rs
//! FLV Video Data Structures

#ifndef RUFFLE_FLV_VIDEO_H
#define RUFFLE_FLV_VIDEO_H

#include <cstdint>
#include <vector>
#include <variant>
#include <optional>

namespace ruffle {
namespace flv {

// Forward declarations
class FlvReader;
class Error;

/// Video frame types
enum class FrameType : uint8_t {
    Keyframe = 1,
    Interframe = 2,
    InterframeDisposable = 3,
    Generated = 4,
    CommandFrame = 5,
};

/// Video codec IDs
enum class CodecId : uint8_t {
    Jpeg = 1,
    SorensonH263 = 2,
    ScreenVideo = 3,
    On2Vp6 = 4,
    On2Vp6Alpha = 5,
    ScreenVideo2 = 6,
    Avc = 7,
};

/// Command frame types
enum class CommandFrame : uint8_t {
    StartOfClientSideSeek = 0,
    EndOfClientSideSeek = 1,
};

/// Video packet data variants
struct VideoPacket {
    /// Raw video data
    struct Data {
        std::vector<uint8_t> bytes;
    };

    /// VP6 video data with adjustment values
    struct Vp6Data {
        uint8_t hadjust;
        uint8_t vadjust;
        std::vector<uint8_t> data;
    };

    /// AVC sequence header
    struct AvcSequenceHeader {
        std::vector<uint8_t> data;
    };

    /// AVC NALU with composition time offset
    struct AvcNalu {
        int32_t composition_time_offset;
        std::vector<uint8_t> data;
    };

    /// AVC end of sequence marker
    struct AvcEndOfSequence {};

    /// Command frame
    struct CommandFrameData {
        ruffle::flv::CommandFrame command;
    };

    std::variant<
        Data,
        Vp6Data,
        AvcSequenceHeader,
        AvcNalu,
        AvcEndOfSequence,
        CommandFrameData
    > data;
};

/// Video data structure
///
/// Contains the frame type, codec ID, and packet data.
/// The actual video data is returned as a byte array that must be
/// provided to your video decoder.
struct VideoData {
    FrameType frame_type;
    CodecId codec_id;
    VideoPacket data;

    /// Parse a video data structure.
    ///
    /// This does not parse the actual video data itself, which is instead
    /// returned as an array that must be provided to your video decoder.
    ///
    /// \param reader The FLV reader
    /// \param data_size The size of the entire video data structure, including
    ///                  the header. Errors are yielded if the data_size is too
    ///                  small for the video data present in the tag.
    /// \return Parsed video data or an error
    static std::optional<VideoData> parse(FlvReader* reader, uint32_t data_size);
};

} // namespace flv
} // namespace ruffle

#endif // RUFFLE_FLV_VIDEO_H
