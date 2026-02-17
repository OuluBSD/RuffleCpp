// C++ translation of scanner/src/analyze.rs
//! Post-scan analysis

#ifndef RUFFLE_SCANNER_ANALYZE_H
#define RUFFLE_SCANNER_ANALYZE_H

#include <cstdint>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>

// Forward declarations
namespace ruffle {
namespace scanner {

struct FileResults;

namespace analyze {

/// Generate and print statistics related to a scan's results
/// @param results_begin Iterator to beginning of results
/// @param results_end Iterator to end of results
template<typename Iterator>
void analyze(Iterator results_begin, Iterator results_end) {
    uint64_t total = 0;
    uint64_t start = 0;
    uint64_t read = 0;
    uint64_t decompress = 0;
    uint64_t parse = 0;
    uint64_t execute = 0;
    uint64_t complete = 0;

    for (auto it = results_begin; it != results_end; ++it) {
        total += 1;

        // Match on the progress step
        switch (it->progress) {
            case Step::Start:
                start += 1;
                break;
            case Step::Read:
                read += 1;
                break;
            case Step::Decompress:
                decompress += 1;
                break;
            case Step::Parse:
                parse += 1;
                break;
            case Step::Execute:
                execute += 1;
                break;
            case Step::Complete:
                complete += 1;
                break;
        }
    }

    std::cout << "Scanned " << total << " swf files." << std::endl;

    // Calculate digit width for aligned output
    uint64_t counts[] = {start, read, decompress, parse, execute, complete};
    size_t digits = 4; // minimum width
    for (uint64_t count : counts) {
        if (count > 0) {
            size_t width = static_cast<size_t>(std::ceil(std::log10(static_cast<double>(count)))) + 1;
            digits = std::max(digits, width);
        }
    }
    digits += 3; // extra padding

    std::cout << std::endl;

    if (start > 0) {
        std::cout << std::setw(digits) << start << " movies panicked or crashed the scanner" << std::endl;
    }

    std::cout << std::setw(digits) << read << " movies failed when reading" << std::endl;
    std::cout << std::setw(digits) << decompress << " movies failed to decompress" << std::endl;
    std::cout << std::setw(digits) << parse << " movies failed to parse" << std::endl;
    std::cout << std::setw(digits) << execute << " movies failed to execute" << std::endl;
    std::cout << std::setw(digits) << complete << " movies completed without errors" << std::endl;
    std::cout << std::endl;
}

/// Main entry point for analyze command
/// @param input_path Path to the CSV file with scan results
/// @return 0 on success, error code on failure
inline int analyze_main(const std::string& input_path) {
    // This would read the CSV file and call analyze()
    // Implementation would depend on CSV parsing library
    std::cout << "Analyzing results from: " << input_path << std::endl;
    // Placeholder - actual implementation would read CSV and process results
    return 0;
}

} // namespace analyze
} // namespace scanner
} // namespace ruffle

#endif // RUFFLE_SCANNER_ANALYZE_H
