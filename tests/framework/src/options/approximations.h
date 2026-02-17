// C++ translation of tests/framework/src/options/approximations.rs
// Test approximations for floating point comparisons

#ifndef RUFFLE_TESTS_FRAMEWORK_OPTIONS_APPROXIMATIONS_H
#define RUFFLE_TESTS_FRAMEWORK_OPTIONS_APPROXIMATIONS_H

#include <string>
#include <vector>
#include <optional>
#include <regex>
#include <stdexcept>
#include <cmath>

// Forward declare anyhow error type
namespace anyhow {
    class Error;
    template<typename T>
    using Result = std::expected<T, Error>;
}

namespace ruffle {
namespace tests {
namespace framework {
namespace options {

/// Helper function for serde default
inline bool default_for_bare_numbers() {
    return true;
}

/// Approximations configuration for floating point comparisons
class Approximations {
private:
    /// Whether to allow bare number comparisons
    bool bare_numbers_;

    /// Regex patterns for matching numbers in strings
    std::vector<std::string> number_patterns_;

    /// Absolute epsilon for comparisons
    std::optional<double> epsilon_;

    /// Maximum relative difference for comparisons
    std::optional<double> max_relative_;

public:
    /// Default constructor
    Approximations()
        : bare_numbers_(default_for_bare_numbers())
        , number_patterns_()
        , epsilon_(std::nullopt)
        , max_relative_(std::nullopt)
    {}

    /// Constructor with all parameters
    Approximations(
        bool bare_numbers,
        std::vector<std::string> number_patterns,
        std::optional<double> epsilon,
        std::optional<double> max_relative
    )
        : bare_numbers_(bare_numbers)
        , number_patterns_(std::move(number_patterns))
        , epsilon_(epsilon)
        , max_relative_(max_relative)
    {}

    // Getters
    bool bare_numbers() const { return bare_numbers_; }
    const std::vector<std::string>& number_patterns() const { return number_patterns_; }
    std::optional<double> epsilon() const { return epsilon_; }
    std::optional<double> max_relative() const { return max_relative_; }

    // Setters
    void set_bare_numbers(bool value) { bare_numbers_ = value; }
    void set_number_patterns(std::vector<std::string> patterns) { number_patterns_ = std::move(patterns); }
    void set_epsilon(std::optional<double> value) { epsilon_ = value; }
    void set_max_relative(std::optional<double> value) { max_relative_ = value; }

    /// Compare two floating point values with configured approximations
    /// @param actual The actual value
    /// @param expected The expected value
    /// @return Result indicating success or error message
    anyhow::Result<void> compare(double actual, double expected) const {
        // In Rust: uses approx::relative_eq! macro
        bool result = false;

        if (epsilon_.has_value() && max_relative_.has_value()) {
            // Both epsilon and max_relative specified
            result = relative_eq(actual, expected, epsilon_.value(), max_relative_.value());
        } else if (epsilon_.has_value()) {
            // Only epsilon specified
            result = relative_eq(actual, expected, epsilon_.value(), 0.0);
        } else if (max_relative_.has_value()) {
            // Only max_relative specified
            result = relative_eq(actual, expected, 0.0, max_relative_.value());
        } else {
            // Use defaults
            result = relative_eq(actual, expected, 0.0, 0.0);
        }

        if (result) {
            return {};  // Success
        } else {
            // Return error with details
            std::string msg = "Approximation failed: expected " + std::to_string(expected) +
                             ", found " + std::to_string(actual) +
                             ". Epsilon = " + (epsilon_.has_value() ? std::to_string(epsilon_.value()) : "none") +
                             ", Max Relative = " + (max_relative_.has_value() ? std::to_string(max_relative_.value()) : "none");
            return anyhow::Error(msg);
        }
    }

    /// Validate the approximations configuration
    /// @return Result indicating success or error message
    anyhow::Result<void> validate() const {
        if (!bare_numbers_ && number_patterns_.empty()) {
            return anyhow::Error(
                "approximations with `bare_numbers = false` should have at least one explicit number pattern"
            );
        }
        // TODO: consider checking number_pattern regexes too
        return {};  // Success
    }

    /// Get compiled regex patterns
    /// @return Vector of compiled regex patterns
    std::vector<std::regex> compiled_number_patterns() const {
        std::vector<std::regex> patterns;
        patterns.reserve(number_patterns_.size());
        for (const auto& pattern : number_patterns_) {
            patterns.emplace_back(pattern);
        }
        return patterns;
    }

private:
    /// Helper function for relative equality check
    static bool relative_eq(double a, double b, double epsilon, double max_relative) {
        // Simplified implementation of approx::relative_eq
        double diff = std::fabs(a - b);
        
        if (diff <= epsilon) {
            return true;
        }
        
        double largest = std::max(std::fabs(a), std::fabs(b));
        if (max_relative > 0.0) {
            return diff <= largest * max_relative;
        }
        
        // Default relative epsilon
        const double default_relative = 1e-9;
        return diff <= largest * default_relative;
    }
};

} // namespace options
} // namespace framework
} // namespace tests
} // namespace ruffle

#endif // RUFFLE_TESTS_FRAMEWORK_OPTIONS_APPROXIMATIONS_H
