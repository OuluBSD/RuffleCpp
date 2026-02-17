// C++ translation of exporter/src/exporter.rs
// Exporter implementation for Ruffle

#ifndef RUFFLE_EXPORTER_EXPORTER_H
#define RUFFLE_EXPORTER_EXPORTER_H

#include <memory>
#include <string>
#include <filesystem>
#include <mutex>
#include <optional>
#include <variant>

// Forward declarations
namespace ruffle {
namespace core {
    class Player;
    class PlayerBuilder;
    namespace limits {
        class ExecutionLimit;
    }
    namespace tag_utils {
        // Function to load movie from path
        // std::shared_ptr<SwfMovie> movie_from_path(const std::filesystem::path& path, ...);
    }
}
namespace render_wgpu {
namespace backend {
    class WgpuRenderBackend;
    // Functions: create_wgpu_instance, request_adapter_and_device
}
namespace descriptors {
    class Descriptors;
}
namespace target {
    class TextureTarget;
}
// WebGPU types would be forward declared here
}
}

// Image types
// RgbaImage would come from an image library

// Error handling
namespace anyhow {
    class Error;
    template<typename T>
    using Result = std::variant<T, Error>;
    
    inline Error error(const std::string& msg) {
        return Error(); // Placeholder
    }
}

#include "cli.h"

namespace ruffle {
namespace exporter {

/// Exporter class for exporting SWF frames
///
/// This class manages the WebGPU context and provides methods to export
/// frames from SWF files.
class Exporter {
private:
    std::shared_ptr<ruffle::render_wgpu::descriptors::Descriptors> descriptors_;
    SizeOpt size_;
    uint32_t skipframes_;
    FrameSelectionWrapper frames_;
    bool force_play_;

public:
    /// Create a new exporter with the given options
    ///
    /// \param opt The command-line options
    /// \return Result containing the exporter or an error
    static anyhow::Result<std::unique_ptr<Exporter>> new_exporter(const Opt& opt);

    /// Start exporting a movie from the given path
    ///
    /// \param swf_path The path to the SWF file
    /// \return Result containing the movie export or an error
    anyhow::Result<std::unique_ptr<class MovieExport>> start_exporting_movie(
        const std::filesystem::path& swf_path);

    /// Get the descriptors
    const std::shared_ptr<ruffle::render_wgpu::descriptors::Descriptors>& descriptors() const {
        return descriptors_;
    }

    /// Get the size options
    const SizeOpt& size() const {
        return size_;
    }

    /// Get the number of frames to skip
    uint32_t skipframes() const {
        return skipframes_;
    }

    /// Get the frame selection
    const FrameSelectionWrapper& frames() const {
        return frames_;
    }

    /// Check if force play is enabled
    bool force_play() const {
        return force_play_;
    }
};

/// Movie export class for exporting frames from a specific movie
///
/// This class manages the player instance and provides methods to run
/// frames and capture images.
class MovieExport {
private:
    std::shared_ptr<std::mutex> player_;  // Arc<Mutex<Player>>
    uint32_t skipframes_;
    FrameSelectionWrapper frames_;
    bool force_play_;

public:
    /// Get the total number of frames to export
    ///
    /// \return The total number of frames
    uint32_t total_frames() const {
        return frames_.total_frames(player_, skipframes_);
    }

    /// Run a single frame
    ///
    /// This advances the player by one frame, handling preloading
    /// and force play if enabled.
    void run_frame();

    /// Capture the current frame as an image
    ///
    /// \return Result containing the captured image or an error
    anyhow::Result<void*> capture_frame();  // Returns RgbaImage* in actual implementation

    /// Get the player
    const std::shared_ptr<std::mutex>& player() const {
        return player_;
    }

    /// Get the number of frames to skip
    uint32_t skipframes() const {
        return skipframes_;
    }

    /// Get the frame selection
    const FrameSelectionWrapper& frames() const {
        return frames_;
    }

    /// Check if force play is enabled
    bool force_play() const {
        return force_play_;
    }
};

} // namespace exporter
} // namespace ruffle

#endif // RUFFLE_EXPORTER_EXPORTER_H
