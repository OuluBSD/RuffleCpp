/*
 * C++ header for SWF tag utilities functionality
 * This replaces the functionality of core/common/src/tag_utils.rs
 */

#ifndef SWF_TAG_UTILS_H
#define SWF_TAG_UTILS_H

#include "security_sandbox.h"
#include <vector>
#include <string>
#include <memory>
#include <optional>
#include <utility>
#include <cstdint>

// Forward declarations
struct HeaderExt;
class SwfMovie;
class SwfSlice;

// Type alias for SWF stream reader
using SwfStream = std::vector<uint8_t>;  // Simplified representation

// An open, fully parsed SWF movie ready to play back, either in a Player or a
// MovieClip.
class SwfMovie {
private:
    // The SWF header parsed from the data stream.
    HeaderExt header_;

    // Uncompressed SWF data.
    std::vector<uint8_t> data_;

    // The URL the SWF was downloaded from.
    std::string url_;

    // The URL that triggered the SWF load.
    std::optional<std::string> loader_url_;

    // Any parameters provided when loading this movie (also known as 'flashvars'),
    // as a list of key-value pairs.
    std::vector<std::pair<std::string, std::string>> parameters_;

    // The suggested encoding for this SWF.
    std::string encoding_;

    // The compressed length of the entire datastream
    size_t compressed_len_;

    // Whether this SwfMovie actually represents a loaded movie or fills in for
    // something else, like an loaded image, filler movie, or error state.
    bool is_movie_;

    // Security sandbox type enforced for this movie.
    //
    // It absolutely cannot be changed after constructing
    // the object in order to ensure proper sandboxing.
    SandboxType sandbox_type_;

public:
    // Constructor
    SwfMovie(HeaderExt header, 
             std::vector<uint8_t> data,
             std::string url,
             std::optional<std::string> loader_url,
             std::vector<std::pair<std::string, std::string>> parameters,
             std::string encoding,
             size_t compressed_len,
             bool is_movie,
             SandboxType sandbox_type)
        : header_(std::move(header))
        , data_(std::move(data))
        , url_(std::move(url))
        , loader_url_(std::move(loader_url))
        , parameters_(std::move(parameters))
        , encoding_(std::move(encoding))
        , compressed_len_(compressed_len)
        , is_movie_(is_movie)
        , sandbox_type_(sandbox_type) {}

    // Construct an empty movie.
    static SwfMovie empty(uint8_t swf_version, std::optional<std::string> loader_url = std::nullopt) {
        std::string url = "file:///";
        HeaderExt header = HeaderExt::with_swf_version(swf_version);

        // Determine sandbox type based on URL and header
        SandboxType sandbox_type = SecuritySandbox::infer(url, header);
        return SwfMovie(
            std::move(header),
            std::vector<uint8_t>(),
            std::move(url),
            std::move(loader_url),
            std::vector<std::pair<std::string, std::string>>(),
            "UTF-8",  // Simplified encoding representation
            0,
            false,
            sandbox_type
        );
    }

    // Construct an empty movie with a fake compressed length.
    static SwfMovie fake_with_compressed_len(
        uint8_t swf_version,
        std::optional<std::string> loader_url,
        size_t compressed_len) {
        
        std::string url = "file:///";
        HeaderExt header = HeaderExt::with_swf_version(swf_version);

        SandboxType sandbox_type = SecuritySandbox::infer(url, header);
        return SwfMovie(
            std::move(header),
            std::vector<uint8_t>(),
            std::move(url),
            std::move(loader_url),
            std::vector<std::pair<std::string, std::string>>(),
            "UTF-8",
            compressed_len,
            false,
            sandbox_type
        );
    }

    // Like fake_with_compressed_len, but uses actual data.
    static SwfMovie fake_with_compressed_data(
        uint8_t swf_version,
        std::optional<std::string> loader_url,
        std::vector<uint8_t> compressed_data) {
        
        std::string url = "file:///";
        HeaderExt header = HeaderExt::with_swf_version(swf_version);

        SandboxType sandbox_type = SecuritySandbox::infer(url, header);
        return SwfMovie(
            std::move(header),
            std::move(compressed_data),
            std::move(url),
            std::move(loader_url),
            std::vector<std::pair<std::string, std::string>>(),
            "UTF-8",
            compressed_data.size(),
            false,
            sandbox_type
        );
    }

    // Constructs the error state movie stub in which some attributes have certain
    // error values to signal that no valid file could be loaded.
    static SwfMovie error_movie(std::string movie_url) {
        HeaderExt header = HeaderExt::default_error_header();

        SandboxType sandbox_type = SecuritySandbox::infer(movie_url, header);
        return SwfMovie(
            std::move(header),
            std::vector<uint8_t>(),
            std::move(movie_url),
            std::nullopt,
            std::vector<std::pair<std::string, std::string>>(),
            "UTF-8",
            0,
            false,
            sandbox_type
        );
    }

    // Construct a movie based on the contents of the SWF datastream.
    static std::optional<SwfMovie> from_data(
        const std::vector<uint8_t>& swf_data,
        std::string url,
        std::optional<std::string> loader_url) {
        
        try {
            size_t compressed_len = swf_data.size();
            // In a real implementation, this would decompress the SWF data
            std::vector<uint8_t> decompressed_data = swf_data;  // Simplified
            
            HeaderExt header = HeaderExt::with_swf_version(9);  // Simplified
            SandboxType sandbox_type = SecuritySandbox::infer(url, header);
            
            SwfMovie movie(
                std::move(header),
                std::move(decompressed_data),
                std::move(url),
                std::move(loader_url),
                std::vector<std::pair<std::string, std::string>>(),
                "UTF-8",
                compressed_len,
                true,
                sandbox_type
            );
            
            movie.append_parameters_from_url();
            return movie;
        } catch (...) {
            return std::nullopt;
        }
    }

    // Construct a movie based on a loaded image (JPEG, GIF or PNG).
    static SwfMovie from_loaded_image(std::string url, size_t length) {
        HeaderExt header = HeaderExt::with_uncompressed_len(static_cast<int32_t>(length));
        SandboxType sandbox_type = SecuritySandbox::infer(url, header);
        
        SwfMovie movie(
            std::move(header),
            std::vector<uint8_t>(),
            std::move(url),
            std::nullopt,
            std::vector<std::pair<std::string, std::string>>(),
            "UTF-8",
            length,
            false,
            sandbox_type
        );
        
        movie.append_parameters_from_url();
        return movie;
    }

    // Getters
    const HeaderExt& header() const { return header_; }
    
    // Get the version of the SWF.
    uint8_t version() const { return header_.version(); }
    
    const std::vector<uint8_t>& data() const { return data_; }
    
    // Returns the suggested string encoding for the given SWF version.
    const std::string& encoding() const { return encoding_; }
    
    // The width of the movie in twips.
    int32_t width() const { return header_.stage_size().width; }
    
    // The height of the movie in twips.
    int32_t height() const { return header_.stage_size().height; }
    
    // Get the URL this SWF was fetched from.
    const std::string& url() const { return url_; }
    
    // Get the URL that triggered the fetch of this SWF.
    const std::optional<std::string>& loader_url() const { return loader_url_; }
    
    const std::vector<std::pair<std::string, std::string>>& parameters() const { return parameters_; }
    
    size_t compressed_len() const { return compressed_len_; }
    
    int32_t uncompressed_len() const { return header_.uncompressed_len(); }
    
    bool is_action_script_3() const { return header_.is_action_script_3(); }
    
    const auto& stage_size() const { return header_.stage_size(); }
    
    uint16_t num_frames() const { return header_.num_frames(); }
    
    float frame_rate() const { return header_.frame_rate(); }
    
    bool is_movie() const { return is_movie_; }
    
    SandboxType sandbox_type() const { return sandbox_type_; }

    // Setters
    void set_url(std::string url) { url_ = std::move(url); }
    
    void append_parameters(const std::vector<std::pair<std::string, std::string>>& params) {
        parameters_.insert(parameters_.end(), params.begin(), params.end());
    }

private:
    void append_parameters_from_url() {
        // In a real implementation, this would parse the URL and extract query parameters
        // For now, we'll just leave it as a placeholder
    }
};

// A shared-ownership reference to some portion of an SWF datastream.
class SwfSlice {
private:
    std::shared_ptr<SwfMovie> movie_;
    size_t start_;
    size_t end_;

public:
    SwfSlice(std::shared_ptr<SwfMovie> movie, size_t start, size_t end)
        : movie_(std::move(movie)), start_(start), end_(end) {}

    // Creates an empty SwfSlice.
    static SwfSlice empty(std::shared_ptr<SwfMovie> movie) {
        return SwfSlice(std::move(movie), 0, 0);
    }

    // Creates an empty SwfSlice of the same movie.
    SwfSlice copy_empty() const {
        return SwfSlice::empty(movie_);
    }

    // Construct a new SwfSlice from a movie subslice.
    SwfSlice to_subslice(const std::vector<uint8_t>& slice) const {
        const uint8_t* self_pval = movie_->data().data();
        const uint8_t* slice_pval = slice.data();

        if ((self_pval + start_) <= slice_pval && slice_pval < (self_pval + end_)) {
            size_t slice_start = slice_pval - self_pval;
            size_t slice_end = slice_start + slice.size();
            return SwfSlice(movie_, slice_start, slice_end);
        } else {
            return copy_empty();
        }
    }

    // Construct a new SwfSlice from a movie subslice (unbounded).
    SwfSlice to_unbounded_subslice(const std::vector<uint8_t>& slice) const {
        const uint8_t* self_pval = movie_->data().data();
        size_t self_len = movie_->data().size();
        const uint8_t* slice_pval = slice.data();

        if (self_pval <= slice_pval && slice_pval < (self_pval + self_len)) {
            size_t slice_start = slice_pval - self_pval;
            size_t slice_end = slice_start + slice.size();
            return SwfSlice(movie_, slice_start, slice_end);
        } else {
            return copy_empty();
        }
    }

    // Construct a new SwfSlice from a start and an end.
    SwfSlice to_start_and_end(size_t start, size_t end) const {
        size_t new_start = this->start_ + start;
        size_t new_end = this->start_ + end;

        if (new_start <= new_end && new_end <= movie_->data().size()) {
            // Create a subslice of the data
            std::vector<uint8_t> sub_data(
                movie_->data().begin() + new_start,
                movie_->data().begin() + new_end
            );
            return to_subslice(sub_data);
        } else {
            return copy_empty();
        }
    }

    // Convert the SwfSlice into a standard data slice.
    std::vector<uint8_t> data() const {
        if (start_ <= movie_->data().size() && end_ <= movie_->data().size() && start_ <= end_) {
            return std::vector<uint8_t>(
                movie_->data().begin() + start_,
                movie_->data().begin() + end_
            );
        } else {
            return std::vector<uint8_t>();
        }
    }

    // Get the version of the SWF this data comes from.
    uint8_t version() const {
        return movie_->version();
    }

    // Checks if this slice is empty
    bool is_empty() const {
        return end_ == start_;
    }

    // Get the length of the SwfSlice.
    size_t len() const {
        return end_ - start_;
    }

    // Getters
    const std::shared_ptr<SwfMovie>& movie() const { return movie_; }
    size_t start() const { return start_; }
    size_t end() const { return end_; }
};

// Structure representing header extension information
struct HeaderExt {
    uint8_t version_{0};
    int32_t uncompressed_len_{0};
    bool is_action_script_3_{false};
    float frame_rate_{12.0f};  // Default frame rate
    uint16_t num_frames_{1};
    struct StageSize {
        int32_t width{0};
        int32_t height{0};
    } stage_size_{0, 0};

    HeaderExt() = default;
    
    static HeaderExt with_swf_version(uint8_t version) {
        HeaderExt header;
        header.version_ = version;
        return header;
    }
    
    static HeaderExt default_error_header() {
        HeaderExt header;
        header.version_ = 0;  // Error state
        return header;
    }
    
    static HeaderExt with_uncompressed_len(int32_t len) {
        HeaderExt header;
        header.uncompressed_len_ = len;
        return header;
    }
    
    uint8_t version() const { return version_; }
    int32_t uncompressed_len() const { return uncompressed_len_; }
    bool is_action_script_3() const { return is_action_script_3_; }
    float frame_rate() const { return frame_rate_; }
    uint16_t num_frames() const { return num_frames_; }
    const StageSize& stage_size() const { return stage_size_; }
};

#endif // SWF_TAG_UTILS_H