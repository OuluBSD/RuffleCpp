// C++ translation of core/src/pixel_bender.rs
// Pixel Bender type conversions for AVM2

#ifndef RUFFLE_CORE_PIXEL_BENDER_H
#define RUFFLE_CORE_PIXEL_BENDER_H

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

// Forward declarations - would be defined in respective headers
namespace ruffle {
namespace render {
namespace pixel_bender {
    enum class PixelBenderTypeOpcode;
    
    // Pixel Bender type variants
    struct PixelBenderType {
        enum class Type {
            TFloat,
            TFloat2,
            TFloat3,
            TFloat4,
            TFloat2x2,
            TFloat3x3,
            TFloat4x4,
            TInt,
            TInt2,
            TInt3,
            TInt4,
            TString,
            TBool,
            TBool2,
            TBool3,
            TBool4,
        };
        
        Type type;
        std::optional<float> float_val;
        std::optional<std::array<float, 2>> float2_val;
        std::optional<std::array<float, 3>> float3_val;
        std::optional<std::array<float, 4>> float4_val;
        std::optional<std::array<float, 4>> float2x2_val; // flattened
        std::optional<std::array<float, 9>> float3x3_val; // flattened
        std::optional<std::array<float, 16>> float4x4_val; // flattened
        std::optional<int16_t> int_val;
        std::optional<std::array<int16_t, 2>> int2_val;
        std::optional<std::array<int16_t, 3>> int3_val;
        std::optional<std::array<int16_t, 4>> int4_val;
        std::optional<std::string> string_val;
        std::optional<int16_t> bool_val;
        std::optional<std::array<int16_t, 2>> bool2_val;
        std::optional<std::array<int16_t, 3>> bool3_val;
        std::optional<std::array<int16_t, 4>> bool4_val;
    };
}
}
}

namespace ruffle {
namespace core {

// Forward declarations for AVM2 types
template<typename GC>
class Value;
template<typename GC>
class Activation;
template<typename GC>
class Error;
template<typename GC>
class AvmString;
template<typename GC>
class UpdateContext;

/// Pixel Bender Type Extension trait
/// Provides conversion between AVM2 values and Pixel Bender types
template<typename GC>
class PixelBenderTypeExt {
public:
    /// Convert an AVM2 value to a Pixel Bender type
    static render::pixel_bender::PixelBenderType from_avm2_value(
        Activation<GC>* activation,
        Value<GC> value,
        const render::pixel_bender::PixelBenderTypeOpcode* kind
    );
    
    /// Convert a Pixel Bender type to an AVM2 value
    static Value<GC> as_avm2_value(
        const render::pixel_bender::PixelBenderType& self,
        UpdateContext<GC>* context,
        bool tint_as_int
    );
};

// Implementation would go here (typically in a .cpp file for templates)
// The implementation follows the Rust version:
// - from_avm2_value: Parses array values into appropriate Pixel Bender type
// - as_avm2_value: Converts Pixel Bender type back to AVM2 array/string value
// - Uses f64_to_wrapping_i32 for float-to-int conversion when fractional part is 0

} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_PIXEL_BENDER_H
