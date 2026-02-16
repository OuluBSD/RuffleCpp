// C++ translation of video/external/src/backend.rs
// External video backend with H.264 hardware decoding support

#ifndef RUFFLE_VIDEO_EXTERNAL_BACKEND_H
#define RUFFLE_VIDEO_EXTERNAL_BACKEND_H

#include <cstdint>
#include <memory>
#include <vector>
#include <optional>
#include <variant>

// Forward declarations for ruffle types
namespace ruffle {
namespace render {
    class RenderBackend;
    struct BitmapHandle;
    struct BitmapInfo;
    struct PixelRegion;
}
namespace video {
    struct VideoStreamHandle;
    struct EncodedFrame;
    enum class FrameDependency;

    namespace error {
        class Error;
    }

    namespace software {
        namespace backend {
            class SoftwareVideoBackend;
        }
    }
}
}

// Forward declarations for swf types
namespace swf {
    enum class VideoCodec : uint8_t;
    enum class VideoDeblocking;
}

// Forward declarations for external decoder types
namespace ruffle {
namespace video {
namespace external {

// Forward declaration for decoder types
namespace decoder {
    class VideoDecoder;
}

#ifdef RUFFLE_OPENH264
namespace decoder {
namespace openh264 {
    class OpenH264Codec;
}
}
#endif

#ifdef RUFFLE_WEBCODECS
// Log subscriber type for webcodecs
using LogSubscriberArc = std::shared_ptr<void>; // Placeholder for tracing subscriber
#endif

/// A single preloaded video stream.
///
/// Contains the decoder and bitmap handle for an owned video stream.
class VideoStream {
private:
    std::optional<ruffle::render::BitmapHandle> bitmap_;
    std::unique_ptr<decoder::VideoDecoder> decoder_;

public:
    /// Create a new video stream.
    /// @param decoder The video decoder to use
    explicit VideoStream(std::unique_ptr<decoder::VideoDecoder> decoder)
        : bitmap_(std::nullopt)
        , decoder_(std::move(decoder)) {}

    /// Get the bitmap handle.
    /// @return Optional bitmap handle
    const std::optional<ruffle::render::BitmapHandle>& bitmap() const {
        return bitmap_;
    }

    /// Set the bitmap handle.
    /// @param handle The new bitmap handle
    void set_bitmap(const ruffle::render::BitmapHandle& handle) {
        bitmap_ = handle;
    }

    /// Get the decoder.
    /// @return Reference to the decoder
    decoder::VideoDecoder& decoder() {
        return *decoder_;
    }

    /// Get the decoder (const version).
    /// @return Const reference to the decoder
    const decoder::VideoDecoder& decoder() const {
        return *decoder_;
    }
};

/// Proxy or stream variant.
///
/// Represents either a proxied stream (passed to software backend)
/// or an owned stream (handled directly by this backend).
class ProxyOrStream {
public:
    /// Proxied stream - passed through to software backend.
    struct Proxied {
        VideoStreamHandle inner_handle;  ///< Internal handle for software backend
    };

    /// Owned stream - handled directly by this backend.
    struct Owned {
        VideoStream stream;
    };

private:
    std::variant<Proxied, Owned> data_;

public:
    /// Create a proxied stream.
    /// @param inner_handle The internal handle for the software backend
    explicit ProxyOrStream(Proxied proxied)
        : data_(std::move(proxied)) {}

    /// Create an owned stream.
    /// @param stream The video stream
    explicit ProxyOrStream(Owned owned)
        : data_(std::move(owned)) {}

    /// Check if this is a proxied stream.
    /// @return true if proxied, false if owned
    bool is_proxied() const {
        return std::holds_alternative<Proxied>(data_);
    }

    /// Check if this is an owned stream.
    /// @return true if owned, false if proxied
    bool is_owned() const {
        return std::holds_alternative<Owned>(data_);
    }

    /// Get the proxied data.
    /// @return Pointer to proxied data, or nullptr if owned
    const Proxied* as_proxied() const {
        return std::get_if<Proxied>(&data_);
    }

    /// Get the owned data.
    /// @return Pointer to owned data, or nullptr if proxied
    const Owned* as_owned() const {
        return std::get_if<Owned>(&data_);
    }

    /// Get mutable proxied data.
    /// @return Pointer to proxied data, or nullptr if owned
    Proxied* as_proxied() {
        return std::get_if<Proxied>(&data_);
    }

    /// Get mutable owned data.
    /// @return Pointer to owned data, or nullptr if proxied
    Owned* as_owned() {
        return std::get_if<Owned>(&data_);
    }
};

/// A video backend that falls back to the software backend for most codecs,
/// except for H.264, for which it uses an external decoder.
///
/// This backend provides hardware-accelerated H.264 decoding when available,
/// while falling back to software decoding for other codecs.
class ExternalVideoBackend {
private:
    // Using std::vector as a simple slotmap alternative
    // In production, would use a proper slotmap implementation
    std::vector<std::optional<ProxyOrStream>> streams_;
    std::vector<size_t> free_indices_;

#ifdef RUFFLE_OPENH264
    std::optional<decoder::openh264::OpenH264Codec> openh264_codec_;
#endif

#ifdef RUFFLE_WEBCODECS
    std::optional<LogSubscriberArc> log_subscriber_;
#endif

    ruffle::video::software::backend::SoftwareVideoBackend software_;

    /// Create a decoder for H.264 streams.
    /// @return Result containing the decoder or Error
    std::variant<std::unique_ptr<decoder::VideoDecoder>, ruffle::video::error::Error> make_decoder() {
#ifdef RUFFLE_OPENH264
        if (openh264_codec_.has_value()) {
            // Create OpenH264 decoder
            // return std::make_unique<decoder::openh264::H264Decoder>(&openh264_codec_.value());
        }
#endif

#ifdef RUFFLE_WEBCODECS
        if (log_subscriber_.has_value()) {
            // Create WebCodecs decoder
            // return decoder::webcodecs::H264Decoder::new(log_subscriber_.value());
        }
#endif

        // No H.264 decoder available
        return ruffle::video::error::Error::decoder_error("No H.264 decoder available");
    }

public:
    /// Create a new external video backend.
    ///
    /// Neither the OpenH264 nor the WebCodecs backend will be available.
    ExternalVideoBackend()
        : streams_()
        , free_indices_()
#ifdef RUFFLE_OPENH264
        , openh264_codec_(std::nullopt)
#endif
#ifdef RUFFLE_WEBCODECS
        , log_subscriber_(std::nullopt)
#endif
        , software_() {}

#ifdef RUFFLE_OPENH264
    /// Create a new external video backend with OpenH264 support.
    /// @param openh264_codec The OpenH264 codec instance
    explicit ExternalVideoBackend(decoder::openh264::OpenH264Codec openh264_codec)
        : streams_()
        , free_indices_()
        , openh264_codec_(std::move(openh264_codec))
#ifdef RUFFLE_WEBCODECS
        , log_subscriber_(std::nullopt)
#endif
        , software_() {}
#endif

#ifdef RUFFLE_WEBCODECS
    /// Create a new external video backend with WebCodecs support.
    /// @param log_subscriber The log subscriber for WebCodecs
    explicit ExternalVideoBackend(LogSubscriberArc log_subscriber)
        : streams_()
        , free_indices_()
#ifdef RUFFLE_OPENH264
        , openh264_codec_(std::nullopt)
#endif
        , log_subscriber_(std::move(log_subscriber))
        , software_() {}
#endif

    /// Register a new video stream.
    ///
    /// For H.264 codec, creates an owned stream with external decoder.
    /// For other codecs, proxies to the software backend.
    ///
    /// @param num_frames Number of frames in the video
    /// @param width Video width
    /// @param height Video height
    /// @param codec Video codec used
    /// @param filter Deblocking filter setting
    /// @return VideoStreamHandle on success, Error on failure
    std::variant<VideoStreamHandle, ruffle::video::error::Error> register_video_stream(
        uint32_t num_frames,
        uint16_t width,
        uint16_t height,
        swf::VideoCodec codec,
        swf::VideoDeblocking filter
    );

    /// Configure the decoder of a given video stream.
    ///
    /// The configuration_data contains codec-specific parameters.
    ///
    /// @param stream The video stream handle to configure
    /// @param configuration_data Codec-specific configuration data
    /// @param configuration_data_size Size of configuration data
    /// @return Result indicating success or error
    std::variant<std::monostate, ruffle::video::error::Error> configure_video_stream_decoder(
        VideoStreamHandle stream,
        const uint8_t* configuration_data,
        size_t configuration_data_size
    );

    /// Preload a frame of a given video stream.
    ///
    /// No decoding is intended to happen at this point in time.
    ///
    /// @param stream The video stream handle
    /// @param encoded_frame The encoded frame to preload
    /// @return FrameDependency indicating frame dependencies, or Error
    std::variant<FrameDependency, ruffle::video::error::Error> preload_video_stream_frame(
        VideoStreamHandle stream,
        const EncodedFrame& encoded_frame
    );

    /// Decode a frame of a given video stream.
    ///
    /// @param stream The video stream handle
    /// @param encoded_frame The encoded frame to decode
    /// @param renderer The render backend to produce bitmaps for
    /// @return BitmapInfo for the decoded frame, or Error on failure
    std::variant<ruffle::render::BitmapInfo, ruffle::video::error::Error> decode_video_stream_frame(
        VideoStreamHandle stream,
        const EncodedFrame& encoded_frame,
        ruffle::render::RenderBackend* renderer
    );
};

} // namespace external
} // namespace video
} // namespace ruffle

#endif // RUFFLE_VIDEO_EXTERNAL_BACKEND_H
