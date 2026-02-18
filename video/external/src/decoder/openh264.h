// C++ translation of video/external/src/decoder/openh264.rs
// OpenH264 H.264 Video Decoder

#ifndef RUFFLE_VIDEO_EXTERNAL_DECODER_OPENH264_H
#define RUFFLE_VIDEO_EXTERNAL_DECODER_OPENH264_H

#include <string>
#include <vector>
#include <memory>
#include <cstdint>
#include <array>
#include <filesystem>

// Forward declarations
namespace ruffle {
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
namespace render {
namespace bitmap {
    enum class BitmapFormat;
}
}
}

namespace ruffle {
namespace video {
namespace external {
namespace decoder {

// Forward declaration for OpenH264 library wrapper
class OpenH264;
struct ISVCDecoder;

/// OpenH264 version representation
class OpenH264Version {
private:
    uint32_t major;
    uint32_t minor;
    uint32_t revision;

public:
    OpenH264Version(uint32_t major, uint32_t minor, uint32_t revision)
        : major(major), minor(minor), revision(revision) {}

    uint32_t get_major() const { return major; }
    uint32_t get_minor() const { return minor; }
    uint32_t get_revision() const { return revision; }

    bool operator==(const OpenH264Version& other) const {
        return major == other.major && minor == other.minor && revision == other.revision;
    }

    bool operator!=(const OpenH264Version& other) const {
        return !(*this == other);
    }

    std::string to_string() const;
};

/// OpenH264 error types
enum class OpenH264ErrorType {
    LibraryLoadingError,
    VersionMismatchError
};

/// OpenH264 error representation
class OpenH264Error {
private:
    OpenH264ErrorType type;
    std::string message;
    std::optional<OpenH264Version> expected_version;
    std::optional<OpenH264Version> actual_version;

public:
    static OpenH264Error library_loading_error(const std::string& message);
    static OpenH264Error version_mismatch_error(const OpenH264Version& expected,
                                                const OpenH264Version& actual);

    OpenH264ErrorType get_type() const { return type; }
    const std::string& get_message() const { return message; }
};

/// OpenH264 codec representation
class OpenH264Codec {
private:
    static const OpenH264Version VERSION;  // Version 2.4.1

    std::shared_ptr<OpenH264> openh264;

    /// OpenH264 library data for current platform
    struct OpenH264Data {
        std::vector<std::string> local_filenames;
        std::string download_filename;
        std::string download_sha256;
    };

    /// Get library data for current platform
    static OpenH264Data get_data();

    /// Download and verify OpenH264 library
    static std::filesystem::path fetch_and_verify(const OpenH264Data& data,
                                                  const std::filesystem::path& directory);

    /// Load existing OpenH264 library
    static std::shared_ptr<OpenH264Codec> load_existing(const std::string& filename);

public:
    /// Load OpenH264 library
    /// First tries system library, then falls back to local file, downloading if necessary
    static std::shared_ptr<OpenH264Codec> load(const std::filesystem::path& directory);

    std::shared_ptr<OpenH264> get_library() const { return openh264; }
};

/// H.264 video decoder using OpenH264
class H264Decoder {
private:
    uint8_t length_size;  // Bytes used to store NALU length (1, 2, 3, or 4)
    std::shared_ptr<OpenH264> openh264;
    ISVCDecoder* decoder;

public:
    /// Create a new H264 decoder
    /// @param codec OpenH264 codec instance
    explicit H264Decoder(const OpenH264Codec& codec);

    ~H264Decoder();

    /// Configure decoder with AVCC format data
    /// @param configuration_data Decoder configuration including PPS and SPS
    void configure_decoder(const std::vector<uint8_t>& configuration_data);

    /// Preload a frame to determine dependency
    /// @param encoded_frame The encoded frame
    /// @return Frame dependency type
    ruffle::video::frame::FrameDependency preload_frame(
        const ruffle::video::frame::EncodedFrame& encoded_frame);

    /// Decode a frame
    /// @param encoded_frame The encoded frame
    /// @return Decoded frame
    ruffle::video::frame::DecodedFrame decode_frame(
        const ruffle::video::frame::EncodedFrame& encoded_frame);
};

} // namespace decoder
} // namespace external
} // namespace video
} // namespace ruffle

#endif // RUFFLE_VIDEO_EXTERNAL_DECODER_OPENH264_H
