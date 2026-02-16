// C++ translation of core/src/avm2/globals/flash/display/stage_3d.rs
// flash.display.Stage3D native methods

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_STAGE_3D_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_STAGE_3D_H

#include <vector>
#include <utility>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Object;
    template<typename T> class Value;
    template<typename T> class Error;
}
}
}

namespace ruffle {
namespace render {
namespace backend {
    enum class Context3DProfile;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace display {
namespace stage_3d {

/// Profile constants - high to low priority
extern const std::vector<std::pair<const char*, ruffle::render::backend::Context3DProfile>> PROFILES_HIGH_TO_LOW;

/// Implements Stage3D.requestContext3DInternal
template<typename GC>
Value<GC> request_context3d_internal(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Stage3D.context3D getter
template<typename GC>
Value<GC> get_context_3d(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Stage3D.visible getter
template<typename GC>
Value<GC> get_visible(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Stage3D.visible setter
template<typename GC>
Value<GC> set_visible(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace stage_3d
} // namespace display
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_STAGE_3D_H
