// C++ translation of core/src/avm2/globals/flash/display3D/program_3d.rs
// flash.display3D.Program3D implementation

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY3D_PROGRAM_3D_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY3D_PROGRAM_3D_H

#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace display3D {
namespace program_3d {

/// Implements flash.display3D.Program3D.upload
/// Uploads vertex and fragment AGAL shaders to the Program3D
template<typename GC>
Value<GC> upload(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace program_3d
} // namespace display3D
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY3D_PROGRAM_3D_H
