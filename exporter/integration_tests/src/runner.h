// C++ translation of exporter/integration_tests/src/runner.rs
// Integration Tests Runner for Exporter

#ifndef RUFFLE_EXPORTER_INTEGRATION_TESTS_RUNNER_H
#define RUFFLE_EXPORTER_INTEGRATION_TESTS_RUNNER_H

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <functional>
#include <filesystem>

// Forward declarations
namespace vfs {
    class VfsPath;
}

namespace ruffle {
namespace exporter {
namespace cli {
    struct Opt;
}
}
}

namespace ruffle {
namespace fs_tests_runner {
    class FsTestsRunner;
    struct TestLoaderParams;
}
}

namespace ruffle {
namespace exporter {
namespace integration_tests {

/// Test options loaded from test.toml
struct TestOptions {
    std::vector<std::string> args;
    std::string swf;
    bool ignore;
    std::optional<std::string> input_dir;
    std::optional<std::string> output_dir;
    std::optional<int32_t> expected_status;
    std::optional<std::string> expected_stdout;
    std::optional<std::string> expected_stderr;

    TestOptions()
        : swf("test.swf"), ignore(false) {}

    /// Read test options from TOML file
    static TestOptions read(const vfs::VfsPath& path);

    /// Get input directory
    vfs::VfsPath get_input_directory(const vfs::VfsPath& test_dir) const;

    /// Get output directory
    vfs::VfsPath get_output_directory(const vfs::VfsPath& test_dir) const;

    /// Get actual directory
    vfs::VfsPath get_actual_directory(const vfs::VfsPath& test_dir) const;
};

/// Integration test runner
class IntegrationTestRunner {
private:
    static constexpr const char* TEST_TOML_NAME = "test.toml";
    static constexpr const char* CHILD_PROCESS_ENV_NAME = "__RUFFLE_EXPORTER_TEST_CHILD_PROCESS__";

public:
    /// Main entry point
    /// If CHILD_PROCESS_ENV_NAME is set, runs as child process
    /// Otherwise, runs the test suite
    static int main(const std::vector<std::string>& args);

    /// Load a test from the given parameters
    static void load_test(const ruffle::fs_tests_runner::TestLoaderParams& params,
                         std::function<void(const std::string& name, bool ignore)> register_trial);
};

/// Verify that actual directory matches expected directory
void verify_dirs(const vfs::VfsPath& actual_dir,
                const vfs::VfsPath& expected_dir,
                const vfs::VfsPath& input_dir);

/// Rebase a path from one base to another
vfs::VfsPath rebase_path(const vfs::VfsPath& path,
                        const vfs::VfsPath& base,
                        const vfs::VfsPath& new_base);

/// Read file contents as bytes
std::vector<uint8_t> read_bytes(const vfs::VfsPath& file);

/// Compare two images for equality
bool images_equal(const std::vector<uint8_t>& expected,
                 const std::vector<uint8_t>& actual);

} // namespace integration_tests
} // namespace exporter
} // namespace ruffle

#endif // RUFFLE_EXPORTER_INTEGRATION_TESTS_RUNNER_H
