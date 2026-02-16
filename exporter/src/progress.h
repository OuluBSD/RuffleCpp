// C++ translation of exporter/src/progress.rs
// Exporter progress bar functionality

#ifndef RUFFLE_EXPORTER_PROGRESS_H
#define RUFFLE_EXPORTER_PROGRESS_H

#include <string>
#include <optional>
#include <cstdint>

// Forward declarations - would use a progress bar library like indicatif
namespace indicatif {
    class ProgressBar;
    class ProgressStyle;
}

namespace ruffle {
namespace exporter {

// Forward declarations
namespace cli {
    struct Opt;
}

/// Exporter progress bar wrapper
class ExporterProgress {
public:
    /// Create a new progress bar
    /// @param opt CLI options (used to check silent mode)
    /// @param files_count Total number of files to process
    ExporterProgress(const cli::Opt& opt, uint64_t files_count);

    /// Set the progress message
    /// @param msg The message to display
    void set_message(const std::string& msg);

    /// Increment the progress counter
    /// @param delta Amount to increment by
    void inc(uint64_t delta);

    /// Finish the progress bar with a message
    /// @param msg The final message to display
    void finish_with_message(const std::string& msg);

private:
    std::optional<indicatif::ProgressBar> progress_;
};

} // namespace exporter
} // namespace ruffle

#endif // RUFFLE_EXPORTER_PROGRESS_H
