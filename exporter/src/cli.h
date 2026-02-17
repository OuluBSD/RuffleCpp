// C++ translation of exporter/src/cli.rs
// CLI options for Ruffle Exporter

#ifndef RUFFLE_EXPORTER_CLI_H
#define RUFFLE_EXPORTER_CLI_H

#include <string>
#include <optional>
#include <cstdint>
#include <filesystem>
#include <mutex>
#include <memory>

// Forward declarations
namespace ruffle {
namespace core {
    class Player;
}
}

namespace ruffle {
namespace exporter {

/// Size options for export
///
/// Controls the output size and scaling of exported frames.
struct SizeOpt {
    /// The amount to scale the page size with
    double scale = 1.0;

    /// Optionally override the output width
    std::optional<uint32_t> width;

    /// Optionally override the output height
    std::optional<uint32_t> height;
};

/// Frame selection mode
///
/// Determines which frames to export from the SWF.
enum class FrameSelection {
    All,        /// Export all frames
    Count       /// Export a specific number of frames
};

/// Frame selection with count
struct FrameSelectionCount {
    uint32_t count;
    
    bool is_single_frame() const {
        return count == 1;
    }
};

/// Frame selection wrapper
class FrameSelectionWrapper {
private:
    FrameSelection mode_;
    std::optional<uint32_t> count_;

public:
    /// Create a frame selection for all frames
    static FrameSelectionWrapper all() {
        FrameSelectionWrapper fs;
        fs.mode_ = FrameSelection::All;
        return fs;
    }

    /// Create a frame selection for a specific count
    ///
    /// \param count The number of frames to export
    /// \return A FrameSelectionWrapper, or nullopt if count is 0
    static std::optional<FrameSelectionWrapper> count(uint32_t count) {
        if (count == 0) {
            return std::nullopt;
        }
        FrameSelectionWrapper fs;
        fs.mode_ = FrameSelection::Count;
        fs.count_ = count;
        return fs;
    }

    /// Parse frame selection from string
    ///
    /// \param s The string to parse ("all" or a number)
    /// \return The parsed frame selection, or error message
    static std::variant<FrameSelectionWrapper, std::string> from_string(const std::string& s) {
        std::string s_lower = s;
        std::transform(s_lower.begin(), s_lower.end(), s_lower.begin(), ::tolower);
        
        if (s_lower == "all") {
            return all();
        }
        
        try {
            uint32_t n = std::stoul(s);
            if (n == 0) {
                return std::string("Frame count must be greater than 0");
            }
            return count(n);
        } catch (...) {
            return std::string("Invalid value for --frames: " + s);
        }
    }

    /// Check if this is a single frame selection
    bool is_single_frame() const {
        if (mode_ == FrameSelection::All) {
            return false;
        }
        return count_.value_or(1) == 1;
    }

    /// Get the total number of frames to export
    ///
    /// \param player The player to get frame count from
    /// \param skipframes Number of frames to skip
    /// \return The total number of frames
    uint32_t total_frames(const std::shared_ptr<std::mutex>& player, uint32_t skipframes) const {
        if (mode_ == FrameSelection::All) {
            // TODO: Getting frame count from the header won't always work.
            // This would need to call player->header_frames()
            return 0; // Placeholder
        }
        return count_.value_or(1) + skipframes;
    }
};

/// Graphics backend selection
enum class GraphicsBackend {
    Default,
    Gl,
    Vulkan,
    Dx12,
    Metal,
    BrowserWebGPU
};

/// Power preference for graphics device
enum class PowerPreference {
    Low,
    High,
    None
};

/// Command-line options for Ruffle Exporter
///
/// This class holds all the command-line arguments for the exporter tool.
struct Opt {
    /// The file or directory of files to export frames from
    std::filesystem::path swf;

    /// The file or directory to store the capture in.
    /// The default value will either be:
    /// - If given one swf and one frame, the name of the swf + ".png"
    /// - If given one swf and multiple frames, the name of the swf as a directory
    /// - If given multiple swfs, this field is required.
    std::optional<std::filesystem::path> output_path;

    /// Number of frames to capture per file. Use 'all' to capture all frames.
    FrameSelectionWrapper frames = FrameSelectionWrapper::count(1).value_or(FrameSelectionWrapper::all());

    /// Number of frames to skip
    uint32_t skipframes = 0;

    /// Don't show a progress bar
    bool silent = false;

    /// Size options
    SizeOpt size;

    /// Force the main timeline to play, bypassing "Click to Play" buttons
    bool force_play = false;

    /// Type of graphics backend to use
    GraphicsBackend graphics = GraphicsBackend::Default;

    /// Power preference for the graphics device used
    PowerPreference power = PowerPreference::High;

    /// TODO Unused, remove after some time
    bool skip_unsupported = false;
};

} // namespace exporter
} // namespace ruffle

#endif // RUFFLE_EXPORTER_CLI_H
