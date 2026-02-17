// C++ translation of core/common/src/tag_utils.rs
// SWF decoding support

#ifndef RUFFLE_CORE_COMMON_TAG_UTILS_H
#define RUFFLE_CORE_COMMON_TAG_UTILS_H

#include <cstdint>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>
#include <optional>
#include <functional>
#include <variant>

// Forward declarations
namespace swf {
    enum class TagCode : uint16_t;
    using CharacterId = uint16_t;
    using Fixed8 = uint16_t;
    using Twips = int32_t;

    struct Rectangle {
        Twips x_min;
        Twips x_max;
        Twips y_min;
        Twips y_max;

        Twips width() const { return x_max - x_min; }
        Twips height() const { return y_max - y_min; }
    };

    struct HeaderExt {
        uint8_t version;
        uint32_t uncompressed_len;
        Rectangle stage_size;
        uint16_t num_frames;
        Fixed8 frame_rate;
        bool is_action_script_3;

        static HeaderExt default_with_swf_version(uint8_t version) {
            return HeaderExt{
                version,
                0,
                Rectangle{0, 0, 0, 0},
                0,
                0,
                false
            };
        }

        static HeaderExt default_with_uncompressed_len(int32_t len) {
            return HeaderExt{
                1,
                static_cast<uint32_t>(len),
                Rectangle{0, 0, 0, 0},
                0,
                0,
                false
            };
        }

        static HeaderExt default_error_header() {
            return HeaderExt{
                0,
                0,
                Rectangle{0, 0, 0, 0},
                0,
                0,
                false
            };
        }

        uint8_t version() const { return version; }
        int32_t uncompressed_len() const { return static_cast<int32_t>(uncompressed_len); }
        const Rectangle& stage_size() const { return stage_size; }
        uint16_t num_frames() const { return num_frames; }
        Fixed8 frame_rate() const { return frame_rate; }
        bool is_action_script_3() const { return is_action_script_3; }
    };

    namespace error {
        class Error;
    }

    namespace read {
        template<typename T>
        class Reader;
    }

    // Encoding types
    inline constexpr const char* UTF_8 = "UTF-8";
}

namespace url {
    class Url;
}

namespace ruffle {
namespace common {
namespace sandbox {
    enum class SandboxType {
        LocalTrusted,
        LocalWithFile,
        LocalWithNetwork,
        Remote
    };

    inline SandboxType infer(const std::string& url, const swf::HeaderExt* header) {
        // Implementation would determine sandbox type based on URL
        return SandboxType::Remote;
    }
}
}
}

namespace ruffle {
namespace common {
namespace tag_utils {

using SwfStream = swf::read::Reader<const uint8_t*>;

/// An open, fully parsed SWF movie ready to play back, either in a Player or a
/// MovieClip.
class SwfMovie {
private:
    /// The SWF header parsed from the data stream.
    swf::HeaderExt header_;

    /// Uncompressed SWF data.
    std::vector<uint8_t> data_;

    /// The URL the SWF was downloaded from.
    std::string url_;

    /// The URL that triggered the SWF load.
    std::optional<std::string> loader_url_;

    /// Any parameters provided when loading this movie (also known as 'flashvars'),
    /// as a list of key-value pairs.
    std::vector<std::pair<std::string, std::string>> parameters_;

    /// The suggested encoding for this SWF.
    const char* encoding_;

    /// The compressed length of the entire datastream
    size_t compressed_len_;

    /// Whether this SwfMovie actually represents a loaded movie or fills in for
    /// something else, like a loaded image, filler movie, or error state.
    bool is_movie_;

    /// Security sandbox type enforced for this movie.
    ///
    /// It absolutely cannot be changed after constructing
    /// the object in order to ensure proper sandboxing.
    sandbox::SandboxType sandbox_type_;

public:
    /// Construct an empty movie.
    static SwfMovie empty(uint8_t swf_version, std::optional<std::string> loader_url) {
        std::string url = "file:///";
        swf::HeaderExt header = swf::HeaderExt::default_with_swf_version(swf_version);

        // TODO What sandbox type should we use here?
        auto sandbox_type = sandbox::SandboxType::infer(url, &header);

        return SwfMovie{
            header,
            {},
            url,
            loader_url,
            {},
            swf::UTF_8,
            0,
            false,
            sandbox_type
        };
    }

    /// Construct an empty movie with a fake `compressed_len`.
    /// This is used by `Loader` when firing an initial `progress` event:
    /// `LoaderInfo.bytesTotal` is set to the actual value, but no data is available,
    /// and `LoaderInfo.parameters` is empty.
    static SwfMovie fake_with_compressed_len(
        uint8_t swf_version,
        std::optional<std::string> loader_url,
        size_t compressed_len
    ) {
        std::string url = "file:///";
        swf::HeaderExt header = swf::HeaderExt::default_with_swf_version(swf_version);

        // TODO What sandbox type should we use here?
        auto sandbox_type = sandbox::SandboxType::infer(url, &header);

        return SwfMovie{
            header,
            {},
            url,
            loader_url,
            {},
            swf::UTF_8,
            compressed_len,
            false,
            sandbox_type
        };
    }

    /// Like `fake_with_compressed_len`, but uses actual data.
    /// This is used when loading a Bitmap to expose the underlying content
    static SwfMovie fake_with_compressed_data(
        uint8_t swf_version,
        std::optional<std::string> loader_url,
        std::vector<uint8_t> compressed_data
    ) {
        std::string url = "file:///";
        swf::HeaderExt header = swf::HeaderExt::default_with_swf_version(swf_version);

        // TODO What sandbox type should we use here?
        auto sandbox_type = sandbox::SandboxType::infer(url, &header);

        SwfMovie movie{
            header,
            std::move(compressed_data),
            url,
            loader_url,
            {},
            swf::UTF_8,
            movie.compressed_len_,
            false,
            sandbox_type
        };
        movie.append_parameters_from_url();
        return movie;
    }

    /// Constructs the error state movie stub in which some attributes have certain
    /// error values to signal that no valid file could be loaded.
    ///
    /// This happens if no file could be loaded or if the loaded content is no valid
    /// supported content.
    static SwfMovie error_movie(std::string movie_url) {
        swf::HeaderExt header = swf::HeaderExt::default_error_header();

        // TODO What sandbox type should we use here?
        auto sandbox_type = sandbox::SandboxType::infer(movie_url, &header);

        return SwfMovie{
            header,
            {},
            movie_url,
            std::nullopt,
            {},
            swf::UTF_8,
            0,
            false,
            sandbox_type
        };
    }

    /// Construct a movie based on the contents of the SWF datastream.
    static std::variant<SwfMovie, swf::error::Error> from_data(
        const std::vector<uint8_t>& swf_data,
        std::string url,
        std::optional<std::string> loader_url
    ) {
        size_t compressed_len = swf_data.size();

        // Decompress SWF data (placeholder - would use swf library)
        // auto swf_buf = swf::read::decompress_swf(swf_data);

        const char* encoding = swf::UTF_8; // swf::SwfStr::encoding_for_version(swf_buf.header.version())
        auto sandbox_type = sandbox::SandboxType::infer(url, nullptr);

        SwfMovie movie{
            swf::HeaderExt{}, // swf_buf.header
            {},               // swf_buf.data
            url,
            loader_url,
            {},
            encoding,
            compressed_len,
            true,
            sandbox_type
        };
        movie.append_parameters_from_url();

        return movie;
    }

    /// Construct a movie based on a loaded image (JPEG, GIF or PNG).
    static SwfMovie from_loaded_image(std::string url, size_t length) {
        swf::HeaderExt header = swf::HeaderExt::default_with_uncompressed_len(static_cast<int32_t>(length));

        // TODO What sandbox type should we use here?
        auto sandbox_type = sandbox::SandboxType::infer(url, &header);

        SwfMovie movie{
            header,
            {},
            url,
            std::nullopt,
            {},
            swf::UTF_8,
            length,
            false,
            sandbox_type
        };
        movie.append_parameters_from_url();
        return movie;
    }

    /// Get the SWF header
    const swf::HeaderExt& header() const {
        return header_;
    }

    /// Get the version of the SWF.
    uint8_t version() const {
        return header_.version();
    }

    /// Get the SWF data
    const std::vector<uint8_t>& data() const {
        return data_;
    }

    /// Returns the suggested string encoding for the given SWF version.
    /// For SWF version 6 and higher, this is always UTF-8.
    /// For SWF version 5 and lower, this is locale-dependent,
    /// and we default to WINDOWS-1252.
    const char* encoding() const {
        return encoding_;
    }

    /// The width of the movie in twips.
    swf::Twips width() const {
        return header_.stage_size().width();
    }

    /// The height of the movie in twips.
    swf::Twips height() const {
        return header_.stage_size().height();
    }

    /// Get the URL this SWF was fetched from.
    const std::string& url() const {
        return url_;
    }

    /// Set the URL
    void set_url(std::string url) {
        url_ = std::move(url);
    }

    /// Get the URL that triggered the fetch of this SWF.
    std::optional<std::string> loader_url() const {
        return loader_url_;
    }

    /// Get the parameters
    const std::vector<std::pair<std::string, std::string>>& parameters() const {
        return parameters_;
    }

    /// Append parameters
    void append_parameters(const std::vector<std::pair<std::string, std::string>>& params) {
        parameters_.insert(parameters_.end(), params.begin(), params.end());
    }

    /// Get compressed length
    size_t compressed_len() const {
        return compressed_len_;
    }

    /// Get uncompressed length
    int32_t uncompressed_len() const {
        return header_.uncompressed_len();
    }

    /// Check if this is ActionScript 3
    bool is_action_script_3() const {
        return header_.is_action_script_3();
    }

    /// Get stage size
    const swf::Rectangle& stage_size() const {
        return header_.stage_size();
    }

    /// Get number of frames
    uint16_t num_frames() const {
        return header_.num_frames();
    }

    /// Get frame rate
    swf::Fixed8 frame_rate() const {
        return header_.frame_rate();
    }

    /// Check if this is a movie (vs image or error state)
    bool is_movie() const {
        return is_movie_;
    }

    /// Get sandbox type
    sandbox::SandboxType sandbox_type() const {
        return sandbox_type_;
    }

private:
    void append_parameters_from_url() {
        // Parse URL and extract query parameters
        // Implementation would use url::Url parser
        // for (const auto& [key, value] : parsed_url.query_pairs()) {
        //     parameters_.emplace_back(key, value);
        // }
    }
};

/// A shared-ownership reference to some portion of an SWF datastream.
class SwfSlice {
public:
    std::shared_ptr<SwfMovie> movie;
    size_t start;
    size_t end;

    /// Create from shared movie pointer
    SwfSlice(std::shared_ptr<SwfMovie> movie_ptr)
        : movie(std::move(movie_ptr))
        , start(0)
        , end(movie->data().size())
    {}

    /// Creates an empty SwfSlice.
    static SwfSlice empty(std::shared_ptr<SwfMovie> movie) {
        return SwfSlice{std::move(movie), 0, 0};
    }

    /// Creates an empty SwfSlice of the same movie.
    SwfSlice copy_empty() const {
        return empty(movie);
    }

    /// Construct a new SwfSlice from a regular slice.
    ///
    /// This function returns an empty slice if the given slice is not a subslice of the
    /// current slice.
    SwfSlice to_subslice(const std::vector<uint8_t>& slice) const {
        size_t self_pval = reinterpret_cast<size_t>(movie->data().data());
        size_t slice_pval = reinterpret_cast<size_t>(slice.data());

        if ((self_pval + start) <= slice_pval && slice_pval < (self_pval + end)) {
            return SwfSlice{
                movie,
                slice_pval - self_pval,
                (slice_pval - self_pval) + slice.size()
            };
        } else {
            return copy_empty();
        }
    }

    /// Construct a new SwfSlice from a movie subslice.
    ///
    /// This function allows subslices outside the current slice to be formed,
    /// as long as they are valid subslices of the movie itself.
    SwfSlice to_unbounded_subslice(const std::vector<uint8_t>& slice) const {
        size_t self_pval = reinterpret_cast<size_t>(movie->data().data());
        size_t self_len = movie->data().size();
        size_t slice_pval = reinterpret_cast<size_t>(slice.data());

        if (self_pval <= slice_pval && slice_pval < (self_pval + self_len)) {
            return SwfSlice{
                movie,
                slice_pval - self_pval,
                (slice_pval - self_pval) + slice.size()
            };
        } else {
            return copy_empty();
        }
    }

    /// Construct a new SwfSlice from a start and an end.
    ///
    /// The start and end values will be relative to the current slice.
    /// Furthermore, this function will yield an empty slice if the calculated slice
    /// would be invalid (e.g. negative length) or would extend past the end of
    /// the current slice.
    SwfSlice to_start_and_end(size_t start_offset, size_t end_offset) const {
        size_t new_start = start + start_offset;
        size_t new_end = start + end_offset;

        if (new_start <= new_end) {
            if (new_end <= movie->data().size()) {
                return SwfSlice{movie, new_start, new_end};
            }
        }
        return copy_empty();
    }

    /// Convert the SwfSlice into a standard data slice.
    const std::vector<uint8_t>& data() const {
        // Return reference to the underlying data slice
        // In practice, would return a span or view
        static std::vector<uint8_t> empty;
        if (start >= end || end > movie->data().size()) {
            return empty;
        }
        // Would need to return a view/span here
        return movie->data();
    }

    /// Get the version of the SWF this data comes from.
    uint8_t version() const {
        return movie->header().version();
    }

    /// Check if this slice is empty
    bool is_empty() const {
        return end == start;
    }

    /// Get the length of the SwfSlice.
    size_t len() const {
        return end - start;
    }
};

/// Error types for tag decoding
enum class ErrorType {
    InvalidSwf,
    InvalidBitmap,
    InvalidFont,
    PreloadVideoIntoInvalidCharacter,
    IOError,
    InvalidSwfUrl,
};

/// Tag decoding error
class Error {
private:
    ErrorType type_;
    std::string message_;
    swf::CharacterId character_id_;

public:
    explicit Error(ErrorType t, std::string msg = "")
        : type_(t), message_(std::move(msg)), character_id_(0) {}

    explicit Error(ErrorType t, swf::CharacterId id)
        : type_(t), character_id_(id) {
        message_ = "Character ID: " + std::to_string(id);
    }

    const char* what() const noexcept {
        return message_.c_str();
    }

    ErrorType type() const { return type_; }
};

/// Whether or not to end tag decoding.
enum class ControlFlow {
    /// Stop decoding after this tag.
    Exit,

    /// Continue decoding the next tag.
    Continue,
};

/// Decode tags from a SWF stream reader.
///
/// The given tag_callback will be called for each decoded tag. It will be
/// provided with the stream to read from, the tag code read, and the tag's
/// size. The callback is responsible for (optionally) parsing the contents of
/// the tag; otherwise, it will be skipped.
///
/// Decoding will terminate when the following conditions occur:
///
///  * The tag_callback calls for the decoding to finish.
///  * The decoder encounters a tag longer than the underlying SWF slice
///    (indicated by returning false)
///  * The SWF stream is otherwise corrupt or unreadable (indicated as an error
///    result)
///
/// Decoding will also log tags longer than the SWF slice, error messages
/// yielded from the tag callback, and unknown tags. It will *only* return an
/// error message if the SWF tag itself could not be parsed. Other forms of
/// irregular decoding will be signalled by returning false.
template<typename StreamType>
bool decode_tags(
    StreamType* reader,
    std::function<std::variant<ControlFlow, Error>(StreamType*, swf::TagCode, size_t)> tag_callback
) {
    // Implementation would go here
    // This is a placeholder for the actual decoding logic
    while (true) {
        // Read tag code and length
        // auto [tag_code, tag_len] = reader->read_tag_code_and_length();

        // Check if tag length exceeds remaining data
        // if (tag_len > reader->get_ref().size()) {
        //     tracing::error("Unexpected EOF when reading tag");
        //     return false;
        // }

        // Get tag slice
        // auto tag_slice = reader->get_ref().subspan(0, tag_len);
        // auto end_slice = reader->get_ref().subspan(tag_len);

        // Check if tag code is known
        // if (auto tag = swf::TagCode::from_u16(tag_code)) {
        //     // Call callback
        //     auto result = tag_callback(reader, tag, tag_len);
        //
        //     if (std::holds_alternative<Error>(result)) {
        //         tracing::error("Error running definition tag: {}, got {}", tag, std::get<Error>(result).what());
        //     } else if (std::get<ControlFlow>(result) == ControlFlow::Exit) {
        //         break;
        //     }
        // } else {
        //     tracing::warn("Unknown tag code: {}", tag_code);
        // }

        // Move to next tag
        // reader->seek(end_slice);
    }

    return true;
}

/// Utility method to construct a movie from a file on disk.
///
/// Parameters:
/// - path: Path to the SWF file
/// - loader_url: Optional loader URL
///
/// Returns:
/// - SwfMovie on success
/// - Error on failure
std::variant<SwfMovie, Error> movie_from_path(
    const std::string& path,
    const std::optional<std::string>& loader_url = std::nullopt
) {
    // Read file data
    // auto data = std::filesystem::read_file(path);
    // if (!data) return Error(ErrorType::IOError, data.error().message());

    // Get absolute path
    // auto abs_path = std::filesystem::canonical(path);

    // Convert to URL
    // auto url = url::Url::from_file_path(abs_path);
    // if (!url) return Error(ErrorType::InvalidSwfUrl);

    // Create movie from data
    // auto movie = SwfMovie::from_data(data.value(), url->to_string(), loader_url);
    // if (!movie) return Error(ErrorType::InvalidSwf, movie.error());

    // return movie.value();

    // Placeholder - actual implementation would go here
    return Error(ErrorType::IOError, "Not implemented");
}

} // namespace tag_utils
} // namespace common
} // namespace ruffle

#endif // RUFFLE_CORE_COMMON_TAG_UTILS_H
