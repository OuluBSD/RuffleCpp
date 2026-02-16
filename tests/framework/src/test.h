// C++ translation of tests/framework/src/test.rs
//! Test structure and methods for test framework

#ifndef RUFFLE_TESTS_FRAMEWORK_TEST_H
#define RUFFLE_TESTS_FRAMEWORK_TEST_H

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <optional>

// Forward declarations
namespace ruffle {
namespace tests {
namespace framework {

class Environment;
class TestOptions;
class TestRunner;

namespace options {
    namespace known_failure {
        class KnownFailure;
    }
}

} // namespace framework
} // namespace tests

// External dependencies
namespace vfs {
    class VfsPath;
}

namespace ruffle_core {
namespace font {
    class FontQuery;
    enum class FontType;
}
namespace tag_utils {
    class SwfMovie;
}
}

namespace ruffle_input_format {
    class InputInjector;
}

namespace ruffle_socket_format {
    class SocketEvent;
}

} // namespace ruffle

namespace ruffle {
namespace tests {
namespace framework {

/// Font structure for test fonts
struct Font {
    std::vector<uint8_t> bytes;
    std::string family;
    bool bold;
    bool italic;
};

/// Test structure representing a single test case
class Test {
public:
    Test() = default;
    
    Test(
        TestOptions options,
        vfs::VfsPath swf_path,
        vfs::VfsPath input_path,
        vfs::VfsPath socket_path,
        vfs::VfsPath output_path,
        vfs::VfsPath root_path,
        std::string name);

    /// Create a test from options
    /// @param options The test options
    /// @param test_dir The test directory
    /// @param name The test name
    /// @return The created test or error
    static std::optional<Test> from_options(
        TestOptions options,
        vfs::VfsPath test_dir,
        std::string name);

    /// Create a test runner for this test
    /// @param environment The test environment
    /// @return The test runner or error
    std::optional<TestRunner> create_test_runner(const Environment* environment) const;

    /// Load the SWF movie for this test
    /// @return The SWF movie or error
    std::optional<ruffle_core::tag_utils::SwfMovie> movie() const;

    /// Get socket events for this test
    /// @return Optional vector of socket events or error
    std::optional<std::vector<ruffle_socket_format::SocketEvent>> socket_events() const;

    /// Get input injector for this test
    /// @return The input injector or error
    ruffle_input_format::InputInjector input_injector() const;

    /// Get fonts for this test
    /// @return Map of font queries to fonts or error
    std::optional<std::unordered_map<ruffle_core::font::FontQuery, Font>> fonts() const;

    /// Get font sorts for this test
    /// @return Map of font queries to sorted font queries
    std::unordered_map<ruffle_core::font::FontQuery, std::vector<ruffle_core::font::FontQuery>> font_sorts() const;

    /// Check if this test should run
    /// @param ignore_known_failures Whether to ignore known failures
    /// @param check_renderer Whether to check renderer compatibility
    /// @param environment The test environment
    /// @return true if test should run, false otherwise
    bool should_run(
        bool ignore_known_failures,
        bool check_renderer,
        const Environment* environment) const;

    // Getters
    const TestOptions& options() const { return options_; }
    const vfs::VfsPath& swf_path() const { return swf_path_; }
    const vfs::VfsPath& input_path() const { return input_path_; }
    const vfs::VfsPath& socket_path() const { return socket_path_; }
    const vfs::VfsPath& output_path() const { return output_path_; }
    const vfs::VfsPath& root_path() const { return root_path_; }
    const std::string& name() const { return name_; }

private:
    TestOptions options_;
    vfs::VfsPath swf_path_;
    vfs::VfsPath input_path_;
    vfs::VfsPath socket_path_;
    vfs::VfsPath output_path_;
    vfs::VfsPath root_path_;
    std::string name_;
};

} // namespace framework
} // namespace tests
} // namespace ruffle

#endif // RUFFLE_TESTS_FRAMEWORK_TEST_H
