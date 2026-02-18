// C++ translation of tests/framework/src/runner/trace.rs
// Trace output comparison for test framework

#ifndef RUFFLE_TESTS_FRAMEWORK_RUNNER_TRACE_H
#define RUFFLE_TESTS_FRAMEWORK_RUNNER_TRACE_H

#include <string>
#include <vector>
#include <regex>
#include <optional>
#include <memory>
#include <sstream>

// Forward declarations
namespace vfs {
    class VfsPath;
}

namespace anyhow {
    class Error;
    template<typename T>
    using Result = std::expected<T, Error>;
}

namespace ruffle {
namespace tests {
namespace framework {

namespace backends {
    class TestLogBackend;
}

namespace options {
namespace approximations {
    class Approximations;
}
namespace known_failure {
    class KnownFailure;
}
}

namespace runner {
namespace trace {

/// Wrapper around string slice that makes debug output print string same way as regular output
/// Used for pretty assertion failures
class PrettyString {
private:
    std::string str_;

public:
    explicit PrettyString(const std::string& str) : str_(str) {}

    const std::string& str() const { return str_; }
};

/// Compare trace output against expected output
/// @param log The test log backend containing actual trace output
/// @param expected_path Path to the expected trace file
/// @param approx Optional approximations for floating point comparisons
/// @param known_failure Known failure configuration
/// @return Result indicating success or error message
anyhow::Result<void> compare_trace_output(
    const backends::TestLogBackend& log,
    const vfs::VfsPath& expected_path,
    const options::approximations::Approximations* approx,
    const options::known_failure::KnownFailure& known_failure
);

/// Internal test function for comparing outputs
/// @param expected_name Name of the expected output for error messages
/// @param approx Optional approximations for floating point comparisons
/// @param expected_output The expected output string
/// @param actual_output The actual output string
/// @return Result indicating success or error message
anyhow::Result<void> test(
    const std::string& expected_name,
    const options::approximations::Approximations* approx,
    const std::string& expected_output,
    const std::string& actual_output
);

/// Assert that two text strings match
/// @param ruffle The actual output
/// @param expected The expected output
/// @param expected_name Name of the expected output for error messages
/// @return Result indicating success or error message
anyhow::Result<void> assert_text_matches(
    const std::string& ruffle,
    const std::string& expected,
    const std::string& expected_name
);

/// Add a suffix to a VfsPath filename
/// @param path The original path
/// @param suffix The suffix to add
/// @return Result containing the new path with suffix
anyhow::Result<vfs::VfsPath> path_with_suffix(
    const vfs::VfsPath& path,
    const std::string& suffix
);

} // namespace trace
} // namespace runner
} // namespace framework
} // namespace tests
} // namespace ruffle

#endif // RUFFLE_TESTS_FRAMEWORK_RUNNER_TRACE_H
