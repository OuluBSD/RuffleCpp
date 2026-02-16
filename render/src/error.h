// C++ translation of render/src/error.rs
// Rendering error types

#ifndef RUFFLE_RENDER_ERROR_H
#define RUFFLE_RENDER_ERROR_H

#include <string>
#include <variant>
#include <system_error>

// Forward declarations
namespace ruffle {
namespace render {
    struct BitmapHandle;
}
namespace swf {
    struct BitmapFormat;
}
}

#ifdef __EMSCRIPTEN__
#include <emscripten/val.h>
#endif

namespace ruffle {
namespace render {

/// Rendering error types
///
/// Mirrors the Rust Error enum from render/src/error.rs
class Error {
public:
    /// Error variant types
    enum class Kind {
        TooLarge,                    // Bitmap texture is larger than the rendering device supports
        UnknownType,                 // Unknown bitmap format
        InvalidZlibCompression,      // Invalid ZLIB compression
        InvalidJpeg,                 // Invalid JPEG
        InvalidPng,                  // Invalid PNG
        InvalidGif,                  // Invalid GIF
        EmptyGif,                    // Empty GIF
        UnsupportedLosslessFormat,   // Unsupported DefineBitsLossless format
        UnknownHandle,               // Unknown bitmap handle
        Unimplemented,               // Not yet implemented
#ifdef __EMSCRIPTEN__
        JavascriptError,             // JavaScript error (web only)
#endif
    };

private:
    Kind kind_;
    std::string message_;

    // Additional data for specific error types
    std::variant<
        std::monostate,              // For simple errors
        std::string,                 // For InvalidZlibCompression, Unimplemented
        int,                         // For jpeg_decoder error code
        int,                         // For png error code
        int,                         // For gif error code
        std::pair<uint8_t, int>,     // For UnsupportedLosslessFormat (version, format)
        BitmapHandle                 // For UnknownHandle
    > data_;

public:
    /// Create a simple error
    explicit Error(Kind kind);

    /// Create an error with a message
    Error(Kind kind, const std::string& message);

    /// Create ZLIB compression error
    static Error invalid_zlib_compression(const std::string& io_error);

    /// Create JPEG error
    static Error invalid_jpeg(int error_code);

    /// Create PNG error
    static Error invalid_png(int error_code);

    /// Create GIF error
    static Error invalid_gif(int error_code);

    /// Create empty GIF error
    static Error empty_gif();

    /// Create unsupported lossless format error
    static Error unsupported_lossless_format(uint8_t version, int format);

    /// Create unknown handle error
    static Error unknown_handle(const BitmapHandle& handle);

    /// Create unimplemented error
    static Error unimplemented(const std::string& feature);

#ifdef __EMSCRIPTEN__
    /// Create JavaScript error (web only)
    static Error javascript_error(emscripten::val js_value);
#endif

    /// Get the error kind
    Kind kind() const { return kind_; }

    /// Get the error message
    const std::string& message() const { return message_; }

    /// Get full error description
    std::string to_string() const;
};

/// Stream output operator for Error
std::ostream& operator<<(std::ostream& os, const Error& error);

} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_ERROR_H
