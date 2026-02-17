// C++ translation of scanner/src/execute.rs
//! Child/executor process impls

#ifndef RUFFLE_SCANNER_EXECUTE_H
#define RUFFLE_SCANNER_EXECUTE_H

#include <string>
#include <vector>
#include <chrono>
#include <filesystem>

// Forward declarations
namespace ruffle {
namespace scanner {
namespace cli_options {
    struct ExecuteReportOpt;
}
namespace file_results {
    enum class AvmType;
    struct FileResults;
    enum class Step;
}
namespace logging {
    class ScanLogBackend;
    class ThreadLocalScanLogger;
}
}
namespace core {
    class PlayerBuilder;
    namespace backend {
        namespace navigator {
            class NullExecutor;
            class NullNavigatorBackend;
        }
    }
    namespace limits {
        class ExecutionLimit;
    }
    namespace swf {
        std::vector<uint8_t> decompress_swf(const std::vector<uint8_t>& data);
        // Swf parse_swf(const std::vector<uint8_t>& data);
    }
    namespace tag_utils {
        // Movie movie_from_path(const std::filesystem::path& path, ...);
    }
}
}

namespace ruffle {
namespace scanner {
namespace execute {

/// Execute a SWF file in Ruffle player
/// Runs one frame of the movie for testing purposes
void execute_swf(const std::filesystem::path& file);

/// Write checkpoint data to CSV writer
/// Updates file_result with elapsed time and serializes to CSV
/// Returns error if file_result has an error
template<typename W>
bool checkpoint(
    file_results::FileResults& file_result,
    const std::chrono::steady_clock::time_point& start,
    W& writer
);

/// Main entry point for execute report
/// Processes a SWF file through all scanning steps:
/// - Read: Load file data
/// - Decompress: Decompress SWF if needed
/// - Parse: Parse SWF structure
/// - Execute: Run one frame in Ruffle player
/// Outputs results as CSV to stdout
int execute_report_main(const cli_options::ExecuteReportOpt& execute_report_opt);

} // namespace execute
} // namespace scanner
} // namespace ruffle

#endif // RUFFLE_SCANNER_EXECUTE_H
