// C++ translation of core/src/bitmap/turbulence.rs
//! This file is a Rust port of the C reference implementation of the
//! feTurbulence element in the SVG specification. It's the usual Perlin noise.
//!
//! See: <https://www.w3.org/TR/SVG11/filters.html#feTurbulenceElement>.
//! The `octave_offsets` parameter of `turbulence` was added after porting.

// Copyright © 2015 W3C® (MIT, ERCIM, Keio, Beihang).
// This software or document includes material copied from or derived
// from https://www.w3.org/TR/SVG11/filters.html#feTurbulenceElement.

#ifndef RUFFLE_CORE_BITMAP_TURBULENCE_H
#define RUFFLE_CORE_BITMAP_TURBULENCE_H

#include <cstdint>
#include <cmath>
#include <array>
#include <vector>
#include <optional>

namespace ruffle {
namespace core {
namespace bitmap {
namespace turbulence {

/* Produces results in the range [1, 2**31 - 2].
Algorithm is: r = (a * r) mod m
where a = 16807 and m = 2**31 - 1 = 2147483647
See [Park & Miller], CACM vol. 31 no. 10 p. 1195, Oct. 1988
To test: the algorithm should produce the result 1043618065
as the 10,000th generated number if the original seed is 1.
*/
inline constexpr int64_t RAND_M = 2147483647; // 2**31 - 1
inline constexpr int64_t RAND_A = 16807; // 7**5; primitive root of m
inline constexpr int64_t RAND_Q = 127773; // m / a
inline constexpr int64_t RAND_R = 2836; // m % a

/// Setup seed for random number generator
inline int64_t setup_seed(int64_t seed) {
    if (seed <= 0) {
        seed = -(seed % (RAND_M - 1)) + 1;
    }
    if (seed > RAND_M - 1) {
        seed = RAND_M - 1;
    }
    return seed;
}

/// Generate next random number
inline int64_t random(int64_t seed) {
    int64_t result = RAND_A * (seed % RAND_Q) - RAND_R * (seed / RAND_Q);
    if (result <= 0) {
        result += RAND_M;
    }
    return result;
}

/// Stitch info for wrapping during turbulence calculation
struct StitchInfo {
    /// How much width to subtract to wrap for stitching.
    int32_t width;
    /// How much height to subtract to wrap for stitching.
    int32_t height;
    /// Minimum value of x to wrap.
    int32_t wrap_x;
    /// Minimum value of y to wrap.
    int32_t wrap_y;
};

/// S-curve for smooth interpolation
inline double s_curve(double t) {
    return t * t * (3.0 - 2.0 * t);
}

/// Linear interpolation
inline double lerp(double t, double a, double b) {
    return a + t * (b - a);
}

inline constexpr size_t B_SIZE = 0x100;
inline constexpr int32_t BM = 0xff;
inline constexpr int32_t PERLIN_N = 0x1000;
// inline constexpr int32_t NP = 12; // 2^PerlinN
// inline constexpr int32_t NM = 0xfff;

/// Turbulence generator using Perlin noise
class Turbulence {
public:
    Turbulence() = default;

    /// Create turbulence from seed
    static Turbulence from_seed(int64_t seed) {
        Turbulence result;
        double s;
        std::array<int32_t, B_SIZE + B_SIZE + 2> lattice_selector{};
        std::array<std::array<std::array<double, 2>, B_SIZE + B_SIZE + 2>, 4> gradient{};

        seed = setup_seed(seed);
        for (size_t k = 0; k < 4; ++k) {
            for (size_t i = 0; i < B_SIZE; ++i) {
                lattice_selector[i] = static_cast<int32_t>(i);
                for (size_t j = 0; j < 2; ++j) {
                    seed = random(seed);
                    gradient[k][i][j] =
                        (static_cast<double>((seed % static_cast<int64_t>(B_SIZE + B_SIZE)) - 
                         static_cast<int64_t>(B_SIZE))) / static_cast<double>(B_SIZE);
                }
                s = std::sqrt(
                    gradient[k][i][0] * gradient[k][i][0] + 
                    gradient[k][i][1] * gradient[k][i][1]
                );
                gradient[k][i][0] /= s;
                gradient[k][i][1] /= s;
            }
        }
        for (int32_t i = static_cast<int32_t>(B_SIZE) - 1; i > 0; --i) {
            int32_t k = lattice_selector[i];
            seed = random(seed);
            size_t j = static_cast<size_t>(seed % static_cast<int64_t>(B_SIZE));
            lattice_selector[i] = lattice_selector[j];
            lattice_selector[j] = k;
        }
        for (size_t i = 0; i < B_SIZE + 2; ++i) {
            lattice_selector[B_SIZE + i] = lattice_selector[i];
            for (size_t k = 0; k < 4; ++k) {
                for (size_t j = 0; j < 2; ++j) {
                    gradient[k][B_SIZE + i][j] = gradient[k][i][j];
                }
            }
        }

        result.lattice_selector_ = lattice_selector;
        result.gradient_ = gradient;
        return result;
    }

    /// Generate 2D noise at given position
    double noise2(
        size_t color_channel,
        std::pair<double, double> vec,
        std::optional<StitchInfo> stitch_info
    ) const {
        double t = vec.first + static_cast<double>(PERLIN_N);
        int32_t bx0 = static_cast<int32_t>(t);
        int32_t bx1 = bx0 + 1;
        double rx0 = t - static_cast<double>(bx0);
        double rx1 = rx0 - 1.0;

        t = vec.second + static_cast<double>(PERLIN_N);
        int32_t by0 = static_cast<int32_t>(t);
        int32_t by1 = by0 + 1;
        double ry0 = t - static_cast<double>(by0);
        double ry1 = ry0 - 1.0;

        // If stitching, adjust lattice points accordingly.
        if (stitch_info.has_value()) {
            const auto& info = stitch_info.value();
            if (bx0 >= info.wrap_x) {
                bx0 -= info.width;
            }
            if (bx1 >= info.wrap_x) {
                bx1 -= info.width;
            }
            if (by0 >= info.wrap_y) {
                by0 -= info.height;
            }
            if (by1 >= info.wrap_y) {
                by1 -= info.height;
            }
        }

        bx0 &= BM;
        bx1 &= BM;
        by0 &= BM;
        by1 &= BM;

        int32_t i = lattice_selector_[static_cast<size_t>(bx0)];
        int32_t j = lattice_selector_[static_cast<size_t>(bx1)];
        int32_t b00 = lattice_selector_[static_cast<size_t>(i + by0)];
        int32_t b10 = lattice_selector_[static_cast<size_t>(j + by0)];
        int32_t b01 = lattice_selector_[static_cast<size_t>(i + by1)];
        int32_t b11 = lattice_selector_[static_cast<size_t>(j + by1)];

        double sx = s_curve(rx0);
        double sy = s_curve(ry0);

        const auto& q = gradient_[color_channel][static_cast<size_t>(b00)];
        double u = rx0 * q[0] + ry0 * q[1];
        const auto& q2 = gradient_[color_channel][static_cast<size_t>(b10)];
        double v = rx1 * q2[0] + ry0 * q2[1];
        double a = lerp(sx, u, v);

        const auto& q3 = gradient_[color_channel][static_cast<size_t>(b01)];
        u = rx0 * q3[0] + ry1 * q3[1];
        const auto& q4 = gradient_[color_channel][static_cast<size_t>(b11)];
        v = rx1 * q4[0] + ry1 * q4[1];
        double b = lerp(sx, u, v);

        return lerp(sy, a, b);
    }

    /// Generate turbulence noise
    double turbulence(
        size_t color_channel,
        std::pair<double, double> point,
        std::pair<double, double> base_freq,
        size_t num_octaves,
        bool fractal_sum,
        bool do_stitching,
        std::pair<double, double> tile_pos,
        std::pair<double, double> tile_size,
        const std::vector<std::pair<double, double>>& octave_offsets
    ) const {
        std::optional<StitchInfo> stitch_info; // Not stitching when None.
        std::pair<double, double> adjusted_freq = base_freq;

        // Adjust the base frequencies if necessary for stitching.
        if (do_stitching) {
            // When stitching tiled turbulence, the frequencies must be adjusted
            // so that the tile borders will be continuous.
            if (adjusted_freq.first != 0.0) {
                double lo_freq = std::floor(tile_size.first * adjusted_freq.first) / tile_size.first;
                double hi_freq = std::ceil(tile_size.first * adjusted_freq.first) / tile_size.first;
                adjusted_freq.first = (adjusted_freq.first / lo_freq < hi_freq / adjusted_freq.first)
                    ? lo_freq : hi_freq;
            }
            if (adjusted_freq.second != 0.0) {
                double lo_freq = std::floor(tile_size.second * adjusted_freq.first) / tile_size.second;
                double hi_freq = std::ceil(tile_size.second * adjusted_freq.second) / tile_size.second;
                adjusted_freq.second = (adjusted_freq.second / lo_freq < hi_freq / adjusted_freq.second)
                    ? lo_freq : hi_freq;
            }
            // Set up initial stitch values.
            int32_t w = static_cast<int32_t>(std::floor(tile_size.first * adjusted_freq.first + 0.5));
            int32_t h = static_cast<int32_t>(std::floor(tile_size.second * adjusted_freq.second + 0.5));
            stitch_info = StitchInfo{
                w, h,
                static_cast<int32_t>(tile_pos.first * adjusted_freq.first) + PERLIN_N + w,
                static_cast<int32_t>(tile_pos.second * adjusted_freq.second) + PERLIN_N + h
            };
        }

        double sum = 0.0;
        double ratio = 1.0;
        for (size_t octave = 0; octave < num_octaves; ++octave) {
            const auto& offset = (octave < octave_offsets.size()) 
                ? octave_offsets[octave] 
                : std::make_pair(0.0, 0.0);
            std::pair<double, double> vec = std::make_pair(
                (point.first + offset.first) * adjusted_freq.first * ratio,
                (point.second + offset.second) * adjusted_freq.second * ratio
            );
            double noise = noise2(color_channel, vec, stitch_info);
            sum += fractal_sum ? noise : std::abs(noise);
            sum /= ratio;
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

private:
    std::array<int32_t, B_SIZE + B_SIZE + 2> lattice_selector_;
    std::array<std::array<std::array<double, 2>, B_SIZE + B_SIZE + 2>, 4> gradient_;
};

} // namespace turbulence
} // namespace bitmap
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_BITMAP_TURBULENCE_H
