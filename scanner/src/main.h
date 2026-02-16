// C++ translation of scanner/src/main.rs
// Scanner main entry point

#ifndef RUFFLE_SCANNER_MAIN_H
#define RUFFLE_SCANNER_MAIN_H

#include "cli_options.h"
#include "analyze.h"
#include "execute.h"
#include "scan.h"

namespace ruffle {
namespace scanner {

/// Main entry point for the scanner
/// 
/// \param opt The command-line options
/// \return Result of the scan operation
[[nodiscard]] inline int main_func(const cli_options::Opt& opt) {
    switch (opt.mode) {
        case cli_options::Mode::Scan:
            return scan::scan_main(opt.scan_opt);
        case cli_options::Mode::Analyze:
            return analyze::analyze_main(opt.analyze_opt);
        case cli_options::Mode::ExecuteReport:
            if (execute::execute_report_main(opt.execute_report_opt).is_err()) {
                // Do nothing.
            }
            // Do NOT report errors in this function so it doesn't pollute the
            // CSV output.
            return 0;
    }
}

} // namespace scanner
} // namespace ruffle

#endif // RUFFLE_SCANNER_MAIN_H
