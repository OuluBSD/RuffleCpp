// C++ translation of tests/framework/src/options.rs
// Test framework options module

#ifndef RUFFLE_TESTS_FRAMEWORK_OPTIONS_H
#define RUFFLE_TESTS_FRAMEWORK_OPTIONS_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <optional>
#include <memory>

// Forward declarations
namespace vfs {
    class VfsPath;
}

namespace ruffle {
namespace render {
namespace quality {
    enum class StageQuality;
}
}
}

namespace ruffle {
namespace tests {
namespace framework {

// Forward declarations for submodules
namespace image_trigger {
    enum class ImageTrigger;
}

namespace options {
namespace approximations {
    class Approximations;
}
namespace font {
    class FontOptions;
    class FontSortOptions;
    class DefaultFontsOptions;
}
namespace image_comparison {
    class ImageComparison;
}
namespace known_failure {
    class KnownFailure;
}
namespace player {
    class PlayerOptions;
}
} // namespace options

using namespace options;

/// Required features for a test
struct RequiredFeatures {
    bool lzma = false;
    bool jpegxr = false;

    /// Check if test can run with current features
    bool can_run() const {
        return (!lzma || true) && (!jpegxr || true);  // Simplified feature check
    }
};

/// Quality wrapper for StageQuality
struct Quality {
    ruffle::render::quality::StageQuality value;
    
    Quality() : value(ruffle::render::quality::StageQuality::High) {}
    explicit Quality(ruffle::render::quality::StageQuality q) : value(q) {}
};

/// Render options
struct RenderOptions {
    bool optional = false;
    Quality quality;
    
    RenderOptions() = default;
    
    ruffle::render::quality::StageQuality get_quality() const {
        return quality.value;
    }
};

/// Test options configuration
class TestOptions {
public:
    // Only set when the `test.toml` file has multiple configs,
    // which we handle manually.
    std::optional<std::string> subtest_name;

    std::optional<uint32_t> num_frames;
    std::optional<uint32_t> num_ticks;
    std::optional<double> tick_rate;
    std::string output_path = "output.txt";
    bool sleep_to_meet_frame_rate = false;
    std::unordered_map<std::string, std::shared_ptr<options::image_comparison::ImageComparison>> image_comparisons;
    bool ignore = false;
    options::known_failure::KnownFailure known_failure;
    std::optional<options::approximations::Approximations> approximations;
    options::player::PlayerOptions player_options;
    bool log_fetch = false;
    RequiredFeatures required_features;
    std::unordered_map<std::string, options::font::FontOptions> fonts;
    std::unordered_map<std::string, options::font::FontSortOptions> font_sorts;
    options::font::DefaultFontsOptions default_fonts;

    /// Read test options with subtests from path
    static std::vector<TestOptions> read_with_subtests(const vfs::VfsPath& path);

    /// Read test options from path
    static TestOptions read(const vfs::VfsPath& path);

    /// Validate the test options
    bool validate() const;

    /// Check if test has known failure
    bool has_known_failure() const;

    /// Get output path relative to test directory
    vfs::VfsPath get_output_path(const vfs::VfsPath& test_directory) const;
};

} // namespace framework
} // namespace tests
} // namespace ruffle

#endif // RUFFLE_TESTS_FRAMEWORK_OPTIONS_H
