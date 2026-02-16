// C++ translation of exporter/src/main.rs
// Exporter main entry point

#ifndef RUFFLE_EXPORTER_MAIN_H
#define RUFFLE_EXPORTER_MAIN_H

#include <expected>
#include <string>

namespace ruffle {
namespace exporter {
namespace cli {
    /// Command-line options for the exporter
    struct Opt {
        // Command-line options would be defined here
        // Using clap::Parser in Rust, equivalent to manual parsing or CLI11 in C++
    };
}

/// Run the exporter main logic
/// @param opt Command-line options
/// @return Result indicating success or error message
std::expected<void, std::string> run_main(const cli::Opt& opt);

} // namespace exporter
} // namespace ruffle

#endif // RUFFLE_EXPORTER_MAIN_H
