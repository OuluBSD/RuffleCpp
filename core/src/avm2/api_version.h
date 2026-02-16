// C++ translation of core/src/avm2/api_version.rs
// AVM2 API version enumeration and mapping

#ifndef RUFFLE_CORE_AVM2_API_VERSION_H
#define RUFFLE_CORE_AVM2_API_VERSION_H

#include <cstdint>
#include <array>
#include <utility>

namespace ruffle {
namespace core {
namespace avm2 {

/// Player runtime type (Flash Player or AIR)
enum class PlayerRuntime {
    AIR,
    FlashPlayer
};

/// API version enumeration based on Adobe AVMPlus api-versions.h
///
/// These versions map to specific Flash Player and AIR releases.
/// See: https://github.com/adobe/avmplus/blob/master/core/api-versions.h
enum class ApiVersion : uint8_t {
    AllVersions = 0,
    AIR_1_0 = 1,
    FP_10_0 = 2,
    AIR_1_5 = 3,
    AIR_1_5_1 = 4,
    FP_10_0_32 = 5,
    AIR_1_5_2 = 6,
    FP_10_1 = 7,
    AIR_2_0 = 8,
    AIR_2_5 = 9,
    FP_10_2 = 10,
    AIR_2_6 = 11,
    SWF_12 = 12,
    AIR_2_7 = 13,
    SWF_13 = 14,
    AIR_3_0 = 15,
    SWF_14 = 16,
    AIR_3_1 = 17,
    SWF_15 = 18,
    AIR_3_2 = 19,
    SWF_16 = 20,
    AIR_3_3 = 21,
    SWF_17 = 22,
    AIR_3_4 = 23,
    SWF_18 = 24,
    AIR_3_5 = 25,
    SWF_19 = 26,
    AIR_3_6 = 27,
    SWF_20 = 28,
    AIR_3_7 = 29,
    SWF_21 = 30,
    AIR_3_8 = 31,
    SWF_22 = 32,
    AIR_3_9 = 33,
    SWF_23 = 34,
    AIR_4_0 = 35,
    SWF_24 = 36,
    AIR_13_0 = 37,
    SWF_25 = 38,
    AIR_14_0 = 39,
    SWF_26 = 40,
    AIR_15_0 = 41,
    SWF_27 = 42,
    AIR_16_0 = 43,
    SWF_28 = 44,
    AIR_17_0 = 45,
    SWF_29 = 46,
    AIR_18_0 = 47,
    SWF_30 = 48,
    AIR_19_0 = 49,
    SWF_31 = 50,
    AIR_20_0 = 51,
    VM_INTERNAL = 52
};

/// Transfer table mapping API versions to valid playerglobals versions
///
/// This maps an ApiVersion from playerglobals SWF to the closest valid version
/// based on the active runtime.
///
/// If runtime is AIR: leave AIR_* unchanged, map FP_* to closest AIR version
/// If runtime is FlashPlayer: leave FP_* unchanged, map AIR_* to VM_INTERNAL
///
/// See: https://github.com/adobe/avmplus/blob/858d034a3bd3a54d9b70909386435cf4aec81d21/core/api-versions.cpp#L63
inline const std::array<std::pair<ApiVersion, ApiVersion>, 53>& get_transfer_table() {
    static const std::array<std::pair<ApiVersion, ApiVersion>, 53> TRANSFER_TABLE = {{
        {ApiVersion::AllVersions, ApiVersion::AllVersions},
        {ApiVersion::AIR_1_0, ApiVersion::VM_INTERNAL},
        {ApiVersion::FP_10_0, ApiVersion::AIR_1_5},
        {ApiVersion::AIR_1_5, ApiVersion::VM_INTERNAL},
        {ApiVersion::AIR_1_5_1, ApiVersion::VM_INTERNAL},
        {ApiVersion::FP_10_0_32, ApiVersion::AIR_1_5_2},
        {ApiVersion::AIR_1_5_2, ApiVersion::VM_INTERNAL},
        {ApiVersion::FP_10_1, ApiVersion::AIR_2_0},
        {ApiVersion::AIR_2_0, ApiVersion::VM_INTERNAL},
        {ApiVersion::AIR_2_5, ApiVersion::VM_INTERNAL},
        {ApiVersion::FP_10_2, ApiVersion::AIR_2_6},
        {ApiVersion::AIR_2_6, ApiVersion::VM_INTERNAL},
        {ApiVersion::SWF_12, ApiVersion::SWF_12},
        {ApiVersion::AIR_2_7, ApiVersion::VM_INTERNAL},
        {ApiVersion::SWF_13, ApiVersion::AIR_3_0},
        {ApiVersion::AIR_3_0, ApiVersion::VM_INTERNAL},
        {ApiVersion::SWF_14, ApiVersion::AIR_3_1},
        {ApiVersion::AIR_3_1, ApiVersion::VM_INTERNAL},
        {ApiVersion::SWF_15, ApiVersion::AIR_3_2},
        {ApiVersion::AIR_3_2, ApiVersion::VM_INTERNAL},
        {ApiVersion::SWF_16, ApiVersion::AIR_3_3},
        {ApiVersion::AIR_3_3, ApiVersion::VM_INTERNAL},
        {ApiVersion::SWF_17, ApiVersion::AIR_3_4},
        {ApiVersion::AIR_3_4, ApiVersion::VM_INTERNAL},
        {ApiVersion::SWF_18, ApiVersion::AIR_3_5},
        {ApiVersion::AIR_3_5, ApiVersion::VM_INTERNAL},
        {ApiVersion::SWF_19, ApiVersion::AIR_3_6},
        {ApiVersion::AIR_3_6, ApiVersion::VM_INTERNAL},
        {ApiVersion::SWF_20, ApiVersion::AIR_3_7},
        {ApiVersion::AIR_3_7, ApiVersion::VM_INTERNAL},
        {ApiVersion::SWF_21, ApiVersion::AIR_3_8},
        {ApiVersion::AIR_3_8, ApiVersion::VM_INTERNAL},
        {ApiVersion::SWF_22, ApiVersion::AIR_3_9},
        {ApiVersion::AIR_3_9, ApiVersion::VM_INTERNAL},
        {ApiVersion::SWF_23, ApiVersion::AIR_4_0},
        {ApiVersion::AIR_4_0, ApiVersion::VM_INTERNAL},
        {ApiVersion::SWF_24, ApiVersion::AIR_13_0},
        {ApiVersion::AIR_13_0, ApiVersion::VM_INTERNAL},
        {ApiVersion::SWF_25, ApiVersion::AIR_14_0},
        {ApiVersion::AIR_14_0, ApiVersion::VM_INTERNAL},
        {ApiVersion::SWF_26, ApiVersion::AIR_15_0},
        {ApiVersion::AIR_15_0, ApiVersion::VM_INTERNAL},
        {ApiVersion::SWF_27, ApiVersion::AIR_16_0},
        {ApiVersion::AIR_16_0, ApiVersion::VM_INTERNAL},
        {ApiVersion::SWF_28, ApiVersion::AIR_17_0},
        {ApiVersion::AIR_17_0, ApiVersion::VM_INTERNAL},
        {ApiVersion::SWF_29, ApiVersion::AIR_18_0},
        {ApiVersion::AIR_18_0, ApiVersion::VM_INTERNAL},
        {ApiVersion::SWF_30, ApiVersion::AIR_19_0},
        {ApiVersion::AIR_19_0, ApiVersion::VM_INTERNAL},
        {ApiVersion::SWF_31, ApiVersion::AIR_20_0},
        {ApiVersion::AIR_20_0, ApiVersion::VM_INTERNAL},
        {ApiVersion::VM_INTERNAL, ApiVersion::VM_INTERNAL}
    }};
    return TRANSFER_TABLE;
}

/// Convert an API version to a valid playerglobals version based on runtime.
///
/// @param version The API version to convert
/// @param runtime The player runtime (AIR or FlashPlayer)
/// @return The valid API version for the given runtime
inline ApiVersion to_valid_playerglobals_version(ApiVersion version, PlayerRuntime runtime) {
    const auto& table = get_transfer_table();
    const auto& active_series = table[static_cast<size_t>(version)];

    switch (runtime) {
        case PlayerRuntime::AIR:
            return active_series.first;
        case PlayerRuntime::FlashPlayer:
            return active_series.second;
    }
}

/// Convert a SWF version to an API version based on runtime.
///
/// Based on the SWF version chart:
/// https://github.com/ruffle-rs/ruffle/wiki/SWF-version-chart
///
/// @param swf_version The SWF version (9-31+)
/// @param runtime The player runtime (AIR or FlashPlayer)
/// @return The corresponding API version
inline ApiVersion from_swf_version(uint8_t swf_version, PlayerRuntime runtime) {
    // There's no specific entry for SWF 9 in avmplus,
    // so map it to the lowest entry.
    // NOTE: FP10 and above will refuse to run AVM2 SWFs with a SWF
    // version that is 8 or below. We want to keep these SWFs working,
    // so be lenient here and allow SWFs with a version <=9 to run.
    if (swf_version <= 9) {
        return ApiVersion::AllVersions;
    }

    switch (swf_version) {
        case 10:
            return (runtime == PlayerRuntime::FlashPlayer) ? ApiVersion::FP_10_1 : ApiVersion::AIR_2_0;
        case 11:
            return (runtime == PlayerRuntime::FlashPlayer) ? ApiVersion::FP_10_2 : ApiVersion::AIR_2_6;
        case 12:
            return (runtime == PlayerRuntime::FlashPlayer) ? ApiVersion::SWF_12 : ApiVersion::AIR_2_7;
        case 13:
            return (runtime == PlayerRuntime::FlashPlayer) ? ApiVersion::SWF_13 : ApiVersion::AIR_3_0;
        case 14:
            return (runtime == PlayerRuntime::FlashPlayer) ? ApiVersion::SWF_14 : ApiVersion::AIR_3_1;
        case 15:
            return (runtime == PlayerRuntime::FlashPlayer) ? ApiVersion::SWF_15 : ApiVersion::AIR_3_2;
        case 16:
            return (runtime == PlayerRuntime::FlashPlayer) ? ApiVersion::SWF_16 : ApiVersion::AIR_3_3;
        case 17:
            return (runtime == PlayerRuntime::FlashPlayer) ? ApiVersion::SWF_17 : ApiVersion::AIR_3_4;
        case 18:
            return (runtime == PlayerRuntime::FlashPlayer) ? ApiVersion::SWF_18 : ApiVersion::AIR_3_5;
        case 19:
            return (runtime == PlayerRuntime::FlashPlayer) ? ApiVersion::SWF_19 : ApiVersion::AIR_3_6;
        case 20:
            return (runtime == PlayerRuntime::FlashPlayer) ? ApiVersion::SWF_20 : ApiVersion::AIR_3_7;
        case 21:
            return (runtime == PlayerRuntime::FlashPlayer) ? ApiVersion::SWF_21 : ApiVersion::AIR_3_8;
        case 22:
            return (runtime == PlayerRuntime::FlashPlayer) ? ApiVersion::SWF_22 : ApiVersion::AIR_3_9;
        case 23:
            return (runtime == PlayerRuntime::FlashPlayer) ? ApiVersion::SWF_23 : ApiVersion::AIR_4_0;
        case 24:
            return (runtime == PlayerRuntime::FlashPlayer) ? ApiVersion::SWF_24 : ApiVersion::AIR_13_0;
        case 25:
            return (runtime == PlayerRuntime::FlashPlayer) ? ApiVersion::SWF_25 : ApiVersion::AIR_14_0;
        case 26:
            return (runtime == PlayerRuntime::FlashPlayer) ? ApiVersion::SWF_26 : ApiVersion::AIR_15_0;
        case 27:
            return (runtime == PlayerRuntime::FlashPlayer) ? ApiVersion::SWF_27 : ApiVersion::AIR_16_0;
        case 28:
            return (runtime == PlayerRuntime::FlashPlayer) ? ApiVersion::SWF_28 : ApiVersion::AIR_17_0;
        case 29:
            return (runtime == PlayerRuntime::FlashPlayer) ? ApiVersion::SWF_29 : ApiVersion::AIR_18_0;
        case 30:
            return (runtime == PlayerRuntime::FlashPlayer) ? ApiVersion::SWF_30 : ApiVersion::AIR_19_0;
        case 31:
            return (runtime == PlayerRuntime::FlashPlayer) ? ApiVersion::SWF_31 : ApiVersion::AIR_20_0;
        default:
            // We haven't yet created entries from higher versions - just map them
            // to the highest non-VM_INTERNAL version.
            return (runtime == PlayerRuntime::FlashPlayer) ? ApiVersion::SWF_31 : ApiVersion::AIR_20_0;
    }
}

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_API_VERSION_H
