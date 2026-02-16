// C++ translation of desktop/src/log.rs
// Log file filename pattern handling

#ifndef RUFFLE_DESKTOP_LOG_H
#define RUFFLE_DESKTOP_LOG_H

#include <string>
#include <string_view>
#include <optional>
#include <filesystem>

namespace ruffle {
namespace desktop {
namespace log {

/// Filename pattern for log files
enum class FilenamePattern : uint8_t {
    SingleFile,
    WithTimestamp,
};

/// Default value for FilenamePattern
inline constexpr FilenamePattern default_filename_pattern() {
    return FilenamePattern::SingleFile;
}

/// Parse a FilenamePattern from a string
/// Returns std::nullopt if the string is not a valid pattern
[[nodiscard]] inline std::optional<FilenamePattern> from_str(std::string_view s) {
    if (s == "single_file") return FilenamePattern::SingleFile;
    if (s == "with_timestamp") return FilenamePattern::WithTimestamp;
    return std::nullopt;
}

/// Create a log file path based on the pattern
///
/// \param pattern The filename pattern to use
/// \param directory The directory to create the path in
/// \return The full path to the log file
[[nodiscard]] inline std::filesystem::path create_path(
    FilenamePattern pattern,
    const std::filesystem::path& directory
) {
    switch (pattern) {
        case FilenamePattern::SingleFile:
            return directory / "ruffle.log";
        case FilenamePattern::WithTimestamp: {
            // Note: In a real implementation, this would use a proper timestamp
            // For now, we use a placeholder format string
            // The actual timestamp would be generated at runtime
            return directory / "ruffle_%F_%H-%M-%S.log";
        }
        default:
            return directory / "ruffle.log";
    }
}

/// Get the pattern as a string
[[nodiscard]] inline const char* as_str(FilenamePattern pattern) {
    switch (pattern) {
        case FilenamePattern::SingleFile:
            return "single_file";
        case FilenamePattern::WithTimestamp:
            return "with_timestamp";
        default:
            return "unknown";
    }
}

} // namespace log
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_LOG_H
