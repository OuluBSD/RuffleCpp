// C++ translation of scanner/src/cli_options.rs
// CLI Options for scanner

#ifndef RUFFLE_SCANNER_CLI_OPTIONS_H
#define RUFFLE_SCANNER_CLI_OPTIONS_H

#include <string>
#include <vector>
#include <variant>
#include <filesystem>

namespace ruffle {
namespace scanner {

namespace fs = std::filesystem;

/// Scan mode options
struct ScanOpt {
    /// The directory (containing SWF files) to scan
    fs::path input_path;

    /// The file to store results in CSV format
    fs::path output_path;

    /// Filenames to ignore
    std::vector<std::string> ignore;
};

/// Analyze mode options
struct AnalyzeOpt {
    /// The CSV file to reanalyze
    fs::path input_path;
};

/// Execute report mode options
struct ExecuteReportOpt {
    /// The single SWF file to parse and run
    fs::path input_path;
};

/// Mode enum - represents the selected CLI subcommand
enum class ModeType {
    Scan,
    Analyze,
    ExecuteReport
};

/// Mode - holds one of the mode options
struct Mode {
    ModeType type;
    std::variant<ScanOpt, AnalyzeOpt, ExecuteReportOpt> data;

    Mode(ScanOpt opt) : type(ModeType::Scan), data(std::move(opt)) {}
    Mode(AnalyzeOpt opt) : type(ModeType::Analyze), data(std::move(opt)) {}
    Mode(ExecuteReportOpt opt) : type(ModeType::ExecuteReport), data(std::move(opt)) {}
};

/// Main CLI options structure
struct Opt {
    Mode mode;

    Opt(Mode m) : mode(std::move(m)) {}
};

} // namespace scanner
} // namespace ruffle

#endif // RUFFLE_SCANNER_CLI_OPTIONS_H
