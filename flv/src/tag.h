// C++ translation of flv/src/tag.rs
//! FLV Tag Structures

#ifndef RUFFLE_FLV_TAG_H
#define RUFFLE_FLV_TAG_H

#include <cstdint>
#include <vector>
#include <variant>
#include <optional>

namespace ruffle {
namespace flv {

// Forward declarations
class FlvReader;
class Error;
struct AudioData;
struct VideoData;
struct ScriptData;

/// Tag data variants
///
/// The tag data was recognized but could not be parsed due to an error.
/// The error contained will never be EndOfData; this should only be used
/// to flag unparsable data within an otherwise complete tag.
struct TagData {
    struct AudioTag {
        AudioData data;
    };

    struct VideoTag {
        VideoData data;
    };

    struct ScriptTag {
        ScriptData data;
    };

    struct InvalidTag {
        Error error;
    };

    std::variant<AudioTag, VideoTag, ScriptTag, InvalidTag> data;
};

/// FLV Tag structure
///
/// FLV files are constructed as a list of tags. Back pointers to prior
/// tags are provided to allow reverse seeking.
struct Tag {
    /// Timestamp in milliseconds
    int32_t timestamp;

    /// Stream ID (24 bits max)
    uint32_t stream_id;

    /// Tag data
    TagData data;

    /// Parse a single FLV tag structure.
    ///
    /// FLV files are constructed as a list of tags. Back pointers to prior
    /// tags are provided to allow reverse seeking. This function ignores the
    /// back pointer and parses the tag at the current location. At the end of
    /// parsing, the reader will be pointing to the start of the next tag. Thus,
    /// repeated calls to `parse` will yield further tags until the end of the
    /// file.
    ///
    /// Errors can be reported in one of two ways. If the header cannot be read
    /// then this function returns the error normally. However, if the header
    /// can be read, but the data inside the tag is corrupt, then a
    /// TagData::Invalid will be returned with the inner error. EndOfData will
    /// always be reported as a normal error and not as an invalid tag.
    ///
    /// In the event of an invalid header or end-of-data error, the reader
    /// position will be unchanged. Valid headers, with or without valid tag
    /// data, will seek the reader to the start of the next tag. This allows
    /// skipping past invalid tags.
    ///
    /// \param reader The FLV reader
    /// \return Parsed tag or an error
    static std::optional<Tag> parse(FlvReader* reader);

    /// Skip back to the prior tag in the FLV.
    ///
    /// FLV files are constructed as a list of tags. Back pointers to prior
    /// tags are provided to allow reverse seeking. This function ignores the
    /// tag at the current location and skips back to prior data in the file.
    ///
    /// \param reader The FLV reader
    /// \return Success or an error
    static std::optional<void> skip_back(FlvReader* reader);
};

} // namespace flv
} // namespace ruffle

#endif // RUFFLE_FLV_TAG_H
