// C++ translation of core/src/avm2/globals/flash/display/shader_job.rs
// flash.display.ShaderJob implementation

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_SHADER_JOB_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_SHADER_JOB_H

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename GC> class Activation;
    template<typename GC> class Value;
    template<typename GC> class Error;
    template<typename GC> class Object;
    namespace bytearray {
        enum class Endian;
    }
}
namespace pixel_bender {
    class PixelBenderShaderHandle;
    struct PixelBenderType;
    enum class PixelBenderTypeOpcode;
    struct PixelBenderMetadata;
}
namespace pixel_bender_support {
    struct PixelBenderShaderArgument;
    struct FloatPixelData;
    enum class ImageInputTexture;
}
namespace render {
namespace backend {
    enum class PixelBenderOutput;
    struct PixelBenderTarget;
}
namespace bitmap {
    struct PixelRegion;
}
}
}
}

// Standard library
#include <vector>
#include <string>

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace slots {
    // Slot indices for shader-related objects
    extern const int flash_display_shader;
    extern const int flash_display_shader_input;
    extern const int flash_display_shader_job;
    extern const int flash_display_shader_parameter;
}
}

namespace globals {
namespace flash {
namespace display {
namespace shader_job {

/// Get the default value for a shader parameter from its metadata.
/// If no default is found, returns an empty value of the appropriate type.
pixel_bender::PixelBenderType get_default_shader_param_value(
    const std::vector<pixel_bender::PixelBenderMetadata>& metadata,
    pixel_bender::PixelBenderTypeOpcode param_type);

/// Get shader arguments from a shader object
/// @param shader_obj The shader object
/// @param activation The current AVM2 activation
/// @return Pair of shader handle and arguments
template<typename GC>
Result<
    std::pair<
        pixel_bender::PixelBenderShaderHandle,
        std::vector<pixel_bender_support::PixelBenderShaderArgument>
    >,
    Error<GC>
> get_shader_args(
    Object<GC> shader_obj,
    Activation<GC>* activation);

/// Trait for pixel data sources
template<typename T>
concept PixelSource = requires(T t, size_t num_pixels) {
    { t.template collect<N>(num_pixels) } -> std::optional<std::vector<std::array<float, N>>>;
};

/// Create a float texture from pixel data
/// @param activation The current AVM2 activation
/// @param shader_name Name of the shader for error messages
/// @param source Source pixel data
/// @param width Texture width
/// @param height Texture height
/// @param input_channels Number of input channels
/// @return Image input texture
template<typename GC, PixelSource S>
Result<pixel_bender_support::ImageInputTexture, Error<GC>> make_float_texture(
    Activation<GC>* activation,
    const char* shader_name,
    S source,
    uint32_t width,
    uint32_t height,
    uint32_t input_channels);

/// Implements `ShaderJob.start`
/// @param activation The current AVM2 activation
/// @param this_value The ShaderJob object
/// @param args Function arguments
/// @return Result value or error
template<typename GC>
Value<GC> start(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args);

} // namespace shader_job
} // namespace display
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_SHADER_JOB_H
