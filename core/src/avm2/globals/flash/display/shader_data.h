// C++ translation of core/src/avm2/globals/flash/display/shader_data.rs
// ShaderData implementation

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_SHADER_DATA_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_SHADER_DATA_H

#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class Object;
    template<typename T> class AvmString;

    namespace object {
        template<typename GC>
        using shader_data_allocator = /* would come from object module */ void;
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
namespace shader_data {

/// Re-export shader_data_allocator from object module
template<typename GC>
using shader_data_allocator = ruffle::core::avm2::object::shader_data_allocator<GC>;

/// Implements ShaderData._setByteCode, which is called from the constructor
template<typename GC>
Value<GC> _set_byte_code(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace shader_data
} // namespace display
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_SHADER_DATA_H
