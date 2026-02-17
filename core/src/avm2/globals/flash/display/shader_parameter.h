// C++ translation of core/src/avm2/globals/flash/display/shader_parameter.rs
// flash.display.ShaderParameter helper

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_SHADER_PARAMETER_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_SHADER_PARAMETER_H

#include <cstddef>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class Object;
    namespace classes {
        template<typename T> class AVM2Classes;
    }
}
namespace pixel_bender {
    class PixelBenderParam;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace slots {
    // Forward declare slot constants
    namespace flash_display_shader_parameter {
        extern const int _INDEX;
        extern const int _TYPE;
        extern const int _VALUE;
    }
    namespace flash_display_shader_input {
        extern const int _CHANNELS;
        extern const int _INDEX;
    }
}
}
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace display {
namespace shader_parameter {

/// Creates a ShaderParameter or ShaderInput from a PixelBenderParam
///
/// Processes the PixelBenderParam and creates the appropriate AVM2 object:
/// - Normal parameters become ShaderParameter objects with type, value, and metadata
/// - Texture parameters become ShaderInput objects with channels and index
///
/// @param activation The current activation context
/// @param param The PixelBenderParam to convert
/// @param index The parameter index
/// @return Value containing the ShaderParameter or ShaderInput object
template<typename GC>
Value<GC> make_shader_parameter(
    Activation<GC>* activation,
    const pixel_bender::PixelBenderParam* param,
    size_t index
);

} // namespace shader_parameter
} // namespace display
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_SHADER_PARAMETER_H
