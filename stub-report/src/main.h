// C++ translation of stub-report/src/main.rs
// Ruffle Stub Report Generator

#ifndef RUFFLE_STUB_REPORT_MAIN_H
#define RUFFLE_STUB_REPORT_MAIN_H

#include <string>
#include <filesystem>

// Forward declarations
namespace ruffle {
namespace core {
    class PlayerBuilder;
}
}

namespace ruffle {
namespace stub_report {

/// Command-line options for the stub report generator
struct Opt {
    /// The file to store the stub report output
    std::filesystem::path output_path;
};

/// Main entry point for the stub report generator
///
/// @param output_path The file path to store the stub report output
/// @return 0 on success, non-zero on error
int main(const std::filesystem::path& output_path);

} // namespace stub_report
} // namespace ruffle

#endif // RUFFLE_STUB_REPORT_MAIN_H
