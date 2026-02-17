// C++ translation of tests/tests/regression_tests.rs
//! Tests running SWFs in a headless Ruffle instance.
//!
//! Trace output can be compared with correct output from the official Flash Player.

#ifndef RUFFLE_TESTS_REGRESSION_TESTS_H
#define RUFFLE_TESTS_REGRESSION_TESTS_H

#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <thread>
#include <filesystem>
#include <iostream>

// Forward declarations
namespace ruffle {
namespace tests {

namespace environment {
    class NativeEnvironment;
}

namespace regression_tests {

/// Test TOML filename constant
constexpr const char* TEST_TOML_NAME = "test.toml";

/// CLI options for running Ruffle tests
struct RuffleTestOpts {
    /// Ignore tests that are known to be failing
    bool ignore_known_failures = false;

    /// Parse command line arguments
    static RuffleTestOpts parse(int argc, char* argv[]) {
        RuffleTestOpts opts;
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "--ignore-known-failures") {
                opts.ignore_known_failures = true;
            }
        }
        return opts;
    }
};

/// Test status enum
enum class TestStatus {
    Continue,
    Sleep,
    Finished
};

/// Test runner interface
class TestRunner {
public:
    virtual ~TestRunner() = default;
    virtual TestStatus tick() = 0;
};

/// Load tests from a directory
/// @param test_dir Path to the test directory
/// @param name Test name
/// @return Vector of test descriptions
inline std::vector<std::string> load_test_dir(
    const std::filesystem::path& test_dir,
    const std::string& name
) {
    auto test_toml_path = test_dir / TEST_TOML_NAME;
    // Would parse test.toml and load test options here
    std::cout << "Loading tests from: " << test_toml_path.string() << std::endl;
    return std::vector<std::string>();
}

/// Create a trial for a test
/// @param opts Test options
/// @param test Test description
/// @param list_only Whether to only list tests
/// @return Trial description
struct Trial {
    std::string name;
    bool ignored = false;
    std::string kind;

    static Trial create(
        const RuffleTestOpts& opts,
        const std::string& test_name,
        bool list_only
    ) {
        Trial trial;
        trial.name = test_name;
        // Would determine if test should be ignored based on options
        return trial;
    }
};

/// Run a test
/// @param runner The test runner
/// @return Success or failure
inline bool run_test(TestRunner* runner) {
    while (true) {
        TestStatus status = runner->tick();
        if (status == TestStatus::Finished) {
            return true;
        }
        if (status == TestStatus::Sleep) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

/// Main entry point for regression tests
/// @param argc Argument count
/// @param argv Argument vector
/// @return Exit code
inline int regression_tests_main(int argc, char* argv[]) {
    // Initialize logging
    std::cout << "Initializing regression tests..." << std::endl;

    // Parse options
    auto opts = RuffleTestOpts::parse(argc, argv);

    // Check for RUFFLE_TEST_OPTS environment variable
    const char* env_opts = std::getenv("RUFFLE_TEST_OPTS");
    if (env_opts != nullptr && strlen(env_opts) > 0) {
        std::cout << "Using options from RUFFLE_TEST_OPTS: " << env_opts << std::endl;
    }

    // Create test runner
    // Would initialize FsTestsRunner here

    std::cout << "Regression tests initialized" << std::endl;
    std::cout << "  Ignore known failures: " << (opts.ignore_known_failures ? "yes" : "no") << std::endl;

    // Run tests
    // Would run all registered tests here

    return 0;
}

} // namespace regression_tests
} // namespace tests
} // namespace ruffle

#endif // RUFFLE_TESTS_REGRESSION_TESTS_H
