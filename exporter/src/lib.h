// C++ translation of exporter/src/lib.rs
//! SWF exporter library

#ifndef RUFFLE_EXPORTER_LIB_H
#define RUFFLE_EXPORTER_LIB_H

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <filesystem>
#include <cstdint>

// Forward declarations
namespace ruffle {
namespace exporter {

// Forward declare from cli module
namespace cli {
    struct Opt;
    enum class FrameSelection;
}

// Forward declare from exporter module
class Exporter;

// Forward declare from progress module
class ExporterProgress;

/// Capture a screenshot from an SWF file.
/// The resulting image uses straight alpha.
///
/// @param exporter The exporter instance
/// @param swf_path Path to the SWF file
/// @param frames Frame selection (single, count, or all)
/// @param skipframes Number of frames to skip at the beginning
/// @param progress Progress indicator
/// @return Vector of captured frames (RGBA images)
std::vector<std::vector<uint8_t>> take_screenshot(
    const Exporter* exporter,
    const std::filesystem::path& swf_path,
    cli::FrameSelection frames,
    uint32_t skipframes,
    const ExporterProgress* progress
);

/// Find SWF files in a directory tree.
///
/// @param root Root directory to search
/// @param with_progress Show progress spinner while searching
/// @return Vector of directory entries for found SWF files
std::vector<std::filesystem::path> find_files(
    const std::filesystem::path& root,
    bool with_progress
);

/// Capture a single SWF file.
///
/// @param exporter The exporter instance
/// @param opt Export options
/// @return Result indicating success or error
std::optional<std::string> capture_single_swf(
    const Exporter* exporter,
    const cli::Opt* opt
);

/// Capture multiple SWF files in parallel.
///
/// @param exporter The exporter instance
/// @param opt Export options
/// @return Result indicating success or error
std::optional<std::string> capture_multiple_swfs(
    const Exporter* exporter,
    const cli::Opt* opt
);

/// Run the main export process.
///
/// @param opt Export options
/// @return Result indicating success or error
std::optional<std::string> run_main(const cli::Opt& opt);

} // namespace exporter
} // namespace ruffle

#endif // RUFFLE_EXPORTER_LIB_H
