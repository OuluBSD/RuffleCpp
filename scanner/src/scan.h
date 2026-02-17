// C++ translation of scanner/src/scan.rs
// Main scanner process implementations

#ifndef RUFFLE_SCANNER_SCAN_H
#define RUFFLE_SCANNER_SCAN_H

#include <cstdint>
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>
#include <optional>

// Forward declarations
namespace ruffle {
namespace scanner {

struct ScanOpt;
struct FileResults;

namespace analyze {
    void analyze(const std::vector<FileResults>& results);
}

namespace cli_options {
    struct ScanOpt;
}

namespace file_results {
    struct FileResults;
}

namespace ser_bridge {
    template<typename T>
    class SerBridge;
}

} // namespace scanner
} // namespace ruffle

namespace ruffle {
namespace scanner {
namespace scan {

/// Find all SWF files in the given directory
///
/// \param root The root directory to search
/// \param ignore List of filenames to ignore
/// \return Vector of directory entries for found SWF files
std::vector<std::filesystem::directory_entry> find_files(
    const std::filesystem::path& root,
    const std::vector<std::string>& ignore);

/// Scan a single SWF file
///
/// \param exec_path Path to the scanner executable
/// \param file Directory entry for the file to scan
/// \param name Relative name/path of the file
/// \return Results of scanning the file
FileResults scan_file(
    const std::filesystem::path& exec_path,
    const std::filesystem::directory_entry& file,
    const std::string& name);

/// Main scanner entry point
///
/// Performs a parallel scan of all SWF files in the input directory,
/// writing results to a CSV file and performing analysis.
///
/// \param opt Scanner options including input/output paths
/// \return 0 on success, error code on failure
int scan_main(const ScanOpt& opt);

} // namespace scan

// Re-export types from submodules
using cli_options::ScanOpt;
using file_results::FileResults;

} // namespace scanner
} // namespace ruffle

#endif // RUFFLE_SCANNER_SCAN_H
