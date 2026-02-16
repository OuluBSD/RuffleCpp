// C++ translation of tests/framework/src/options/image_comparison.rs
// Image comparison options for tests

#ifndef RUFFLE_TESTS_FRAMEWORK_OPTIONS_IMAGE_COMPARISON_H
#define RUFFLE_TESTS_FRAMEWORK_OPTIONS_IMAGE_COMPARISON_H

#include <vector>
#include <optional>
#include <memory>
#include <expected>
#include <stdexcept>

#include "expression.h"
#include "image_trigger.h"

namespace ruffle {
namespace tests {
namespace framework {
namespace options {

/// Image comparison check configuration
struct ImageComparisonCheck {
    uint8_t tolerance = 0;
    size_t max_outliers = 0;
    std::optional<TestExpression> filter;

    /// Default constructor
    ImageComparisonCheck() = default;

    /// Constructor with parameters
    ImageComparisonCheck(uint8_t tol, size_t max, std::optional<TestExpression> f)
        : tolerance(tol), max_outliers(max), filter(std::move(f)) {}
};

/// Image comparison configuration for tests
class ImageComparison {
private:
    std::optional<uint8_t> tolerance;
    std::optional<size_t> max_outliers;
    std::vector<ImageComparisonCheck> checks;

public:
    ImageTrigger trigger;
    bool known_failure = false;

    /// Default constructor
    ImageComparison() = default;

    /// Get the list of checks
    /// 
    /// If simple checks (tolerance/max_outliers) are defined along with
    /// advanced checks, returns an error.
    /// If no advanced checks are defined, returns a single check with
    /// the simple settings.
    /// 
    /// \return Vector of image comparison checks
    [[nodiscard]] std::expected<std::vector<ImageComparisonCheck>, std::runtime_error> get_checks() const {
        bool has_simple_check = tolerance.has_value() || max_outliers.has_value();
        
        if (has_simple_check && !checks.empty()) {
            return std::unexpected(
                std::runtime_error(
                    "Both simple and advanced checks are defined. "
                    "Either remove 'tolerance' & 'max_outliers', or move it to 'checks'."
                )
            );
        }

        if (!checks.empty()) {
            return checks;
        }

        // Return a single check with simple settings
        std::vector<ImageComparisonCheck> result;
        result.push_back(ImageComparisonCheck{
            tolerance.value_or(0),
            max_outliers.value_or(0),
            std::nullopt
        });
        return result;
    }

    /// Set tolerance
    void set_tolerance(uint8_t tol) { tolerance = tol; }

    /// Set max outliers
    void set_max_outliers(size_t max) { max_outliers = max; }

    /// Add a check
    void add_check(const ImageComparisonCheck& check) { checks.push_back(check); }

    /// Get tolerance
    [[nodiscard]] std::optional<uint8_t> get_tolerance() const { return tolerance; }

    /// Get max outliers
    [[nodiscard]] std::optional<size_t> get_max_outliers() const { return max_outliers; }

    /// Get checks (direct access)
    [[nodiscard]] const std::vector<ImageComparisonCheck>& get_checks_direct() const { return checks; }
};

} // namespace options
} // namespace framework
} // namespace tests
} // namespace ruffle

#endif // RUFFLE_TESTS_FRAMEWORK_OPTIONS_IMAGE_COMPARISON_H
