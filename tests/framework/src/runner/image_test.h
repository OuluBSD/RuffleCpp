// C++ translation of tests/framework/src/runner/image_test.rs
// Test framework image test runner

#ifndef RUFFLE_TESTS_FRAMEWORK_RUNNER_IMAGE_TEST_H
#define RUFFLE_TESTS_FRAMEWORK_RUNNER_IMAGE_TEST_H

#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <optional>

// Forward declarations
namespace vfs {
    class VfsPath;
}

namespace image {
    template<typename P, typename Container>
    class ImageBuffer;
    
    using RgbaImage = ImageBuffer</* Pixel */ int, std::vector<uint8_t>>;
    using RgbImage = ImageBuffer</* Pixel */ int, std::vector<uint8_t>>;
    using GrayImage = ImageBuffer</* Pixel */ int, std::vector<uint8_t>>;
    
    enum class ImageFormat;
}

namespace ruffle {
namespace core {
    class Player;
}
}

namespace ruffle {
namespace tests {
namespace framework {

// Forward declarations
namespace environment {
    class RenderInterface;
}

namespace options {
namespace image_comparison {
    class ImageComparison;
}
}

/// Image difference result
struct ImageDiff {
    uint32_t width;
    uint32_t height;
    std::vector<uint8_t> difference_data;
    size_t outliers;
    size_t max_outliers;
    uint8_t max_difference;
    bool is_alpha_different;

    /// Get color difference image
    std::optional<image::RgbImage> difference_color() const;

    /// Get alpha difference image
    std::optional<image::GrayImage> difference_alpha() const;
};

/// Capture and compare image against expected
void capture_and_compare_image(
    const vfs::VfsPath& base_path,
    const std::shared_ptr<std::mutex>& player_mutex,
    const std::shared_ptr<ruffle::core::Player>& player,
    const std::string& name,
    const options::image_comparison::ImageComparison& image_comparison,
    const environment::RenderInterface* render_interface
);

/// Test image comparison
std::optional<ImageDiff> test(
    const options::image_comparison::ImageComparison& comparison,
    const std::string& name,
    const image::RgbaImage& actual_image,
    image::RgbaImage expected_image
);

/// Calculate difference data between two images
std::vector<uint8_t> calculate_difference_data(
    const image::RgbaImage& actual_image,
    const image::RgbaImage& expected_image,
    bool& is_alpha_different
);

/// Calculate number of outliers in difference data
size_t calculate_outliers(const std::vector<uint8_t>& difference_data, uint8_t tolerance);

/// Calculate maximum difference in difference data
uint8_t calculate_max_difference(const std::vector<uint8_t>& difference_data);

/// Calculate per-pixel difference
uint8_t calc_difference(uint8_t lhs, uint8_t rhs);

/// Write image to path
void write_image(
    const vfs::VfsPath& path,
    const image::RgbaImage& image
);

} // namespace runner
} // namespace framework
} // namespace tests
} // namespace ruffle

#endif // RUFFLE_TESTS_FRAMEWORK_RUNNER_IMAGE_TEST_H
