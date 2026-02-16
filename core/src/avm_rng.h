// C++ translation of core/src/avm_rng.rs
// AVM Random Number Generator

#ifndef RUFFLE_CORE_AVM_RNG_H
#define RUFFLE_CORE_AVM_RNG_H

#include <cstdint>
#include <chrono>

namespace ruffle {
namespace core {

// https://github.com/adobe/avmplus/blob/858d034a3bd3a54d9b70909386435cf4aec81d21/core/MathUtils.cpp#L1546
inline constexpr int32_t C1 = 1376312589;
inline constexpr int32_t C2 = 789221;
inline constexpr int32_t C3 = 15731;
inline constexpr int32_t K_RANDOM_PURE_MAX = 0x7FFFFFFF;

inline constexpr uint32_t U_XOR_MASK = 0x48000000;

/// AVM Random Number Generator
/// This class should not be cloned or copied.
class AvmRng {
private:
    uint32_t u_value;

    void init_with_seed(uint32_t seed) {
        u_value = seed;
    }

    int32_t random_fast_next() {
        if ((u_value & 1) != 0) {
            u_value = (u_value >> 1) ^ U_XOR_MASK;
        } else {
            u_value >>= 1;
        }
        return static_cast<int32_t>(u_value);
    }

    int32_t random_pure_hasher(int32_t i_seed) const {
        i_seed = ((i_seed << 13) ^ i_seed) - (i_seed >> 21);

        int32_t i_result = i_seed * i_seed;
        i_result = i_result * C3;
        i_result = i_result + C2;
        i_result = i_result * i_seed;
        i_result = i_result + C1;
        i_result &= K_RANDOM_PURE_MAX;

        i_result = i_result + i_seed;
        i_result = ((i_result << 13) ^ i_result) - (i_result >> 21);

        return i_result;
    }

public:
    AvmRng() : u_value(0) {}

    /// Generate a random number
    int32_t generate_random_number() {
        // In avmplus, RNG is initialized on first use.
        if (u_value == 0) {
            uint32_t seed = get_seed();
            init_with_seed(seed);
        }

        int32_t a_num = random_fast_next();
        a_num = random_pure_hasher(a_num * 71);

        return a_num & K_RANDOM_PURE_MAX;
    }

private:
    /// Get seed from current time
    // https://github.com/adobe-flash/avmplus/blob/65a05927767f3735db37823eebf7d743531f5d37/VMPI/PosixSpecificUtils.cpp#L18
    static uint32_t get_seed() {
        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        auto micros = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
        return static_cast<uint32_t>(micros);
    }
};

} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM_RNG_H
