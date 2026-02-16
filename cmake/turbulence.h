/*
 * C++ header for turbulence/noise functionality
 * This replaces the functionality of core/src/bitmap/turbulence.rs
 */

#ifndef TURBULENCE_H
#define TURBULENCE_H

#include <array>
#include <vector>
#include <cmath>
#include <cstdint>
#include <cstddef>

namespace ruffle {

// Structure for stitch information used in turbulence calculations
struct StitchInfo {
    // How much width to subtract to wrap for stitching
    int32_t width;
    // How much height to subtract to wrap for stitching
    int32_t height;
    // Minimum value of x to wrap
    int32_t wrap_x;
    // Minimum value of y to wrap
    int32_t wrap_y;
    
    StitchInfo(int32_t w, int32_t h, int32_t x, int32_t y) 
        : width(w), height(h), wrap_x(x), wrap_y(y) {}
};

// Constants for Perlin noise algorithm
constexpr size_t B_SIZE = 0x100;  // 256
constexpr int32_t BM = 0xff;     // 255
constexpr int32_t PERLIN_N = 0x1000;  // 4096

// Constants for random number generation
constexpr int64_t RAND_M = 2147483647;  // 2^31 - 1
constexpr int64_t RAND_A = 16807;     // 7^5; primitive root of m
constexpr int64_t RAND_Q = 127773;    // m / a
constexpr int64_t RAND_R = 2836;      // m % a

// Helper function to set up the seed for random number generation
inline int64_t setup_seed(int64_t seed) {
    if (seed <= 0) {
        seed = -(seed % (RAND_M - 1)) + 1;
    }
    if (seed > RAND_M - 1) {
        seed = RAND_M - 1;
    }
    return seed;
}

// Random number generator function
inline int64_t random(int64_t seed) {
    int64_t result = RAND_A * (seed % RAND_Q) - RAND_R * (seed / RAND_Q);
    if (result <= 0) {
        result += RAND_M;
    }
    return result;
}

// S-curve function for smooth interpolation
inline double s_curve(double t) {
    return t * t * (3.0 - 2.0 * t);
}

// Linear interpolation function
inline double lerp(double t, double a, double b) {
    return a + t * (b - a);
}

// Turbulence class implementing Perlin noise algorithm
class Turbulence {
private:
    // Lattice selector array for noise generation
    std::array<int32_t, B_SIZE + B_SIZE + 2> lattice_selector_;
    
    // Gradient arrays for each color channel
    std::array<std::array<std::array<double, 2>, B_SIZE + B_SIZE + 2>, 4> gradient_;

public:
    // Constructor from seed
    explicit Turbulence(int64_t seed) {
        double s;
        auto& lattice_selector = lattice_selector_;
        auto& gradient = gradient_;

        seed = setup_seed(seed);
        for (size_t k = 0; k < 4; ++k) {
            for (size_t i = 0; i < B_SIZE; ++i) {
                lattice_selector[i] = static_cast<int32_t>(i);
                for (size_t j = 0; j < 2; ++j) {
                    seed = random(seed);
                    gradient[k][i][j] = 
                        (static_cast<double>((seed % (B_SIZE + B_SIZE)) - B_SIZE)) / B_SIZE;
                }
                s = std::sqrt(
                    gradient[k][i][0] * gradient[k][i][0] + 
                    gradient[k][i][1] * gradient[k][i][1]
                );
                gradient[k][i][0] /= s;
                gradient[k][i][1] /= s;
            }
        }
        
        // Shuffle the lattice selector array
        for (size_t i = B_SIZE - 1; i > 0; --i) {
            int32_t k = lattice_selector[i];
            seed = random(seed);
            size_t j = static_cast<size_t>(seed % B_SIZE);
            lattice_selector[i] = lattice_selector[j];
            lattice_selector[j] = k;
        }
        
        // Copy values to extend the array
        for (size_t i = 0; i < B_SIZE + 2; ++i) {
            lattice_selector[B_SIZE + i] = lattice_selector[i];
            for (size_t k = 0; k < 4; ++k) {
                for (size_t j = 0; j < 2; ++j) {
                    gradient[k][B_SIZE + i][j] = gradient[k][i][j];
                }
            }
        }
    }

    // Static factory method to create turbulence from seed
    static std::shared_ptr<Turbulence> from_seed(int64_t seed) {
        return std::make_shared<Turbulence>(seed);
    }

    // 2D noise function
    double noise2(size_t color_channel, 
                 std::pair<double, double> vec, 
                 std::optional<StitchInfo> stitch_info = std::nullopt) const {
        
        double t = vec.first + PERLIN_N;
        int32_t bx0 = static_cast<int32_t>(t);
        int32_t bx1 = bx0 + 1;
        double rx0 = t - static_cast<double>(bx0);
        double rx1 = rx0 - 1.0;

        t = vec.second + PERLIN_N;
        int32_t by0 = static_cast<int32_t>(t);
        int32_t by1 = by0 + 1;
        double ry0 = t - static_cast<double>(by0);
        double ry1 = ry0 - 1.0;

        // If stitching, adjust lattice points accordingly
        if (stitch_info.has_value()) {
            auto& info = stitch_info.value();
            if (bx0 >= info.wrap_x) bx0 -= info.width;
            if (bx1 >= info.wrap_x) bx1 -= info.width;
            if (by0 >= info.wrap_y) by0 -= info.height;
            if (by1 >= info.wrap_y) by1 -= info.height;
        }

        bx0 &= BM;
        bx1 &= BM;
        by0 &= BM;
        by1 &= BM;

        int32_t i = lattice_selector_[bx0];
        int32_t j = lattice_selector_[bx1];
        int32_t b00 = lattice_selector_[(i + by0) & BM];
        int32_t b10 = lattice_selector_[(j + by0) & BM];
        int32_t b01 = lattice_selector_[(i + by1) & BM];
        int32_t b11 = lattice_selector_[(j + by1) & BM];

        double sx = s_curve(rx0);
        double sy = s_curve(ry0);

        auto q = gradient_[color_channel][b00];
        double u = rx0 * q[0] + ry0 * q[1];
        q = gradient_[color_channel][b10];
        double v = rx1 * q[0] + ry0 * q[1];
        double a = lerp(sx, u, v);

        q = gradient_[color_channel][b01];
        u = rx0 * q[0] + ry1 * q[1];
        q = gradient_[color_channel][b11];
        v = rx1 * q[0] + ry1 * q[1];
        double b = lerp(sx, u, v);

        return lerp(sy, a, b);
    }

    // Main turbulence function
    double turbulence(
        size_t color_channel,
        std::pair<double, double> point,
        std::pair<double, double> base_freq,
        size_t num_octaves,
        bool fractal_sum,
        bool do_stitching,
        std::pair<double, double> tile_pos,
        std::pair<double, double> tile_size,
        const std::vector<std::pair<double, double>>& octave_offsets) const {
        
        std::optional<StitchInfo> stitch_info = std::nullopt;
        
        // Adjust the base frequencies if necessary for stitching
        auto adjusted_base_freq = base_freq;
        if (do_stitching) {
            // When stitching tiled turbulence, the frequencies must be adjusted
            // so that the tile borders will be continuous
            if (base_freq.first != 0.0) {
                double lo_freq = std::floor(tile_size.first * base_freq.first) / tile_size.first;
                double hi_freq = std::ceil(tile_size.first * base_freq.first) / tile_size.first;
                adjusted_base_freq.first = (base_freq.first / lo_freq < hi_freq / base_freq.first) ? 
                                          lo_freq : hi_freq;
            }
            if (base_freq.second != 0.0) {
                double lo_freq = std::floor(tile_size.second * base_freq.second) / tile_size.second;
                double hi_freq = std::ceil(tile_size.second * base_freq.second) / tile_size.second;
                adjusted_base_freq.second = (base_freq.second / lo_freq < hi_freq / base_freq.second) ? 
                                           lo_freq : hi_freq;
            }
            
            // Set up initial stitch values
            int32_t w = static_cast<int32_t>(tile_size.first * adjusted_base_freq.first + 0.5);
            int32_t h = static_cast<int32_t>(tile_size.second * adjusted_base_freq.second + 0.5);
            stitch_info = StitchInfo{
                w, h,
                static_cast<int32_t>(tile_pos.first * adjusted_base_freq.first) + PERLIN_N + w,
                static_cast<int32_t>(tile_pos.second * adjusted_base_freq.second) + PERLIN_N + h
            };
        }
        
        double sum = 0.0;
        double ratio = 1.0;
        
        for (size_t octave = 0; octave < num_octaves; ++octave) {
            auto offset = octave < octave_offsets.size() ? octave_offsets[octave] : std::make_pair(0.0, 0.0);
            auto vec = std::make_pair(
                (point.first + offset.first) * adjusted_base_freq.first * ratio,
                (point.second + offset.second) * adjusted_base_freq.second * ratio
            );
            
            double noise = this->noise2(color_channel, vec, stitch_info);
            sum += (fractal_sum ? noise : std::abs(noise)) / ratio;
            ratio *= 2.0;
            
            if (stitch_info.has_value()) {
                // Update stitch values. Subtracting PerlinN before the multiplication and
                // adding it afterward simplifies to subtracting it once.
                auto& info = stitch_info.value();
                info.width *= 2;
                info.wrap_x = 2 * info.wrap_x - PERLIN_N;
                info.height *= 2;
                info.wrap_y = 2 * info.wrap_y - PERLIN_N;
            }
        }
        
        return sum;
    }
};

} // namespace ruffle

#endif // TURBULENCE_H