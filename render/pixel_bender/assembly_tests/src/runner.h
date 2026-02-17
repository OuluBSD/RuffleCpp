// C++ translation of render/pixel_bender/assembly_tests/src/runner.rs
//! Pixel Bender assembly tests runner.

#ifndef RUFFLE_RENDER_PIXEL_BENDER_ASSEMBLY_TESTS_RUNNER_H
#define RUFFLE_RENDER_PIXEL_BENDER_ASSEMBLY_TESTS_RUNNER_H

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <functional>
#include <filesystem>

namespace ruffle {
namespace render {
namespace pixel_bender {
namespace assembly_tests {

/// Test type enumeration
enum class TestType {
    Roundtrip,
    Assemble,
    Disassemble
};

/// Test options structure
struct TestOptions {
    TestType type = TestType::Roundtrip;
    bool ignore = false;
    std::string pbj_path = "test.pbj";
    std::string asm_path = "test.pbasm";

    /// Check if test performs assembly
    bool performs_assembly() const {
        return type == TestType::Assemble || type == TestType::Roundtrip;
    }

    /// Check if test performs disassembly
    bool performs_disassembly() const {
        return type == TestType::Disassemble || type == TestType::Roundtrip;
    }
};

/// Test loader parameters
struct TestLoaderParams {
    std::string test_dir;      // Virtual path
    std::filesystem::path test_dir_real;  // Real filesystem path
    std::string test_name;
};

/// Test trial result
struct TrialResult {
    bool success;
    std::string message;
    bool ignored;

    TrialResult() : success(false), message(""), ignored(false) {}
    TrialResult(bool s, const std::string& m, bool i = false)
        : success(s), message(m), ignored(i) {}
};

/// Test trial
class Trial {
private:
    std::string name_;
    std::function<TrialResult()> test_fn_;
    bool ignored_ = false;

public:
    Trial(const std::string& name, std::function<TrialResult()> test_fn)
        : name_(name), test_fn_(test_fn) {}

    /// Mark trial as ignored
    Trial with_ignored_flag(bool ignored) {
        ignored_ = ignored;
        return *this;
    }

    /// Run the test
    TrialResult run() {
        if (ignored_) {
            return TrialResult(true, "IGNORED", true);
        }
        return test_fn_();
    }

    const std::string& name() const { return name_; }
    bool is_ignored() const { return ignored_; }
};

/// File system tests runner
class FsTestsRunner {
private:
    std::string descriptor_name_ = "test.toml";
    std::function<Trial(const TestLoaderParams&)> test_loader_;
    std::vector<Trial> trials_;

public:
    FsTestsRunner() = default;

    /// Set the descriptor file name
    FsTestsRunner& with_descriptor_name(const std::string& name) {
        descriptor_name_ = name;
        return *this;
    }

    /// Set the test loader function
    FsTestsRunner& with_test_loader(
        std::function<Trial(const TestLoaderParams&, std::function<Trial(const TestLoaderParams&)>)> loader
    ) {
        test_loader_ = loader;
        return *this;
    }

    /// Register a trial
    void register_trial(const Trial& trial) {
        trials_.push_back(trial);
    }

    /// Run all tests
    void run() {
        size_t passed = 0;
        size_t failed = 0;
        size_t ignored = 0;

        for (auto& trial : trials_) {
            auto result = trial.run();
            
            if (result.ignored) {
                ++ignored;
                std::cout << "[IGNORED] " << trial.name() << std::endl;
            } else if (result.success) {
                ++passed;
                std::cout << "[PASS] " << trial.name() << std::endl;
            } else {
                ++failed;
                std::cout << "[FAIL] " << trial.name() << ": " << result.message << std::endl;
            }
        }

        std::cout << "\nResults: " << passed << " passed, " 
                  << failed << " failed, " << ignored << " ignored" << std::endl;
    }
};

/// Load a test from parameters
Trial load_test(const TestLoaderParams& params);

/// Convert virtual path to real path
std::filesystem::path to_real_path(
    const std::filesystem::path& real_dir,
    const std::string& file_path
);

/// Run a test comparing expected and actual output
template<typename F>
std::optional<std::string> run_test(
    const std::filesystem::path& expected_path,
    const std::filesystem::path& actual_path,
    F&& proc
) {
    // Execute the procedure
    auto result = proc();
    if (!result) {
        return "Failed to execute (dis)assembly: " + result.error();
    }

    // Read actual output
    std::vector<uint8_t> actual;
    try {
        std::ifstream actual_file(actual_path, std::ios::binary);
        actual = std::vector<uint8_t>(
            std::istreambuf_iterator<char>(actual_file),
            std::istreambuf_iterator<char>()
        );
    } catch (const std::exception& e) {
        return "Error reading actual file: " + std::string(e.what());
    }

    // Read expected output
    std::vector<uint8_t> expected;
    try {
        std::ifstream expected_file(expected_path, std::ios::binary);
        expected = std::vector<uint8_t>(
            std::istreambuf_iterator<char>(expected_file),
            std::istreambuf_iterator<char>()
        );
    } catch (const std::exception& e) {
        return "Error reading test file " + expected_path.string() + ": " + std::string(e.what());
    }

    // Compare
    if (actual != expected) {
        return "Test failed: Output doesn't match: " + actual_path.string();
    }

    // Clean up actual file
    std::filesystem::remove(actual_path);

    return std::nullopt; // Success
}

} // namespace assembly_tests
} // namespace pixel_bender
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_PIXEL_BENDER_ASSEMBLY_TESTS_RUNNER_H
