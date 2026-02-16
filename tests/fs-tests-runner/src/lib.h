// C++ translation of tests/fs-tests-runner/src/lib.rs
//! Makes it easy to discover, filter, and execute filesystem-based tests.
//!
//! Filesystem-based tests are tests that are defined in directories with
//! specific descriptor files (e.g. test.toml).
//!
//! libtest_mimic is used so that the runner is compatible with `cargo test`.

#ifndef RUFFLE_TESTS_FS_TESTS_RUNNER_LIB_H
#define RUFFLE_TESTS_FS_TESTS_RUNNER_LIB_H

#include <string>
#include <vector>
#include <filesystem>
#include <memory>
#include <functional>
#include <regex>

// Forward declarations
namespace libtest_mimic {
    class Arguments;
    class Trial;
}

namespace vfs {
    class VfsPath;
    class PhysicalFS;
}

namespace ruffle {
namespace tests {
namespace fs_tests_runner {

namespace fs = std::filesystem;

/// Convert the filter (e.g. from the CLI) to a test name.
///
/// These two values may differ due to how
/// libtest_mimic handles test kind annotations:
/// a test may be named `test` or `[kind] test` when a kind is present.
/// This function removes the "kind" prefix from
/// the name to match tests similarly to libtest_mimic.
///
/// @param filter The filter string
/// @return The test name without kind prefix
std::string filter_to_test_name(const std::string& filter);

/// Check if a test should be included based on arguments
///
/// @param args The test arguments/filters
/// @param test_name The name of the test
/// @return true if the test should be included
bool is_candidate(const libtest_mimic::Arguments* args, const std::string& test_name);

/// Parameters for test loading
struct TestLoaderParams {
    const libtest_mimic::Arguments* args;
    std::shared_ptr<vfs::VfsPath> test_dir;
    fs::path test_dir_real;
    std::string test_name;
};

/// Test loader function type
using TestLoader = std::function<void(const TestLoaderParams&, std::function<void(libtest_mimic::Trial)>)>;

/// Filesystem-based test runner
class FsTestsRunner {
public:
    /// Default constructor
    FsTestsRunner();

    /// Set the root directory for tests
    /// @param root_dir The root directory path
    /// @return Reference to this runner
    FsTestsRunner& with_root_dir(const fs::path& root_dir);

    /// Set the descriptor file name
    /// @param descriptor_name The descriptor file name (e.g., "test.toml")
    /// @return Reference to this runner
    FsTestsRunner& with_descriptor_name(std::string descriptor_name);

    /// Add an additional test
    /// @param test The test to add
    /// @return Reference to this runner
    FsTestsRunner& with_additional_test(libtest_mimic::Trial test);

    /// Set the test loader function
    /// @param test_loader The test loader function
    /// @return Reference to this runner
    FsTestsRunner& with_test_loader(TestLoader test_loader);

    /// Set whether to canonicalize paths
    /// @param canonicalize_paths Whether to canonicalize paths
    /// @return Reference to this runner
    FsTestsRunner& with_canonicalize_paths(bool canonicalize_paths);

    /// Run the tests
    /// This will discover, load, and execute all matching tests
    /// @return Exit code (never returns, calls exit())
    [[noreturn]] int run();

private:
    /// Ensure the root directory exists
    void ensure_root_dir_exists() const;

    /// Look up a specific test by name
    /// @param args The test arguments
    /// @param out Output vector for discovered tests
    /// @return Result of the lookup
    int look_up_test(const libtest_mimic::Arguments* args, std::vector<libtest_mimic::Trial>& out);

    /// Load a test from a file
    /// @param args The test arguments
    /// @param file The test file path
    /// @param name The test name
    /// @param out Output vector for discovered tests
    void load_test(const libtest_mimic::Arguments* args, const fs::path& file,
                   const std::string& name, std::vector<libtest_mimic::Trial>& out);

    fs::path root_dir_;
    std::string descriptor_name_;
    std::vector<libtest_mimic::Trial> additional_tests_;
    std::optional<TestLoader> test_loader_;
    bool canonicalize_paths_;
};

} // namespace fs_tests_runner
} // namespace tests
} // namespace ruffle

#endif // RUFFLE_TESTS_FS_TESTS_RUNNER_LIB_H
