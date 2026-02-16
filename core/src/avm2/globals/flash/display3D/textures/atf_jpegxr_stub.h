// C++ translation of core/src/avm2/globals/flash/display3D/textures/atf_jpegxr_stub.rs
// Stub implementation for compressed texture upload (ATF JPEG XR)

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY3D_TEXTURES_ATF_JPEGXR_STUB_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY3D_TEXTURES_ATF_JPEGXR_STUB_H

#include <cstddef>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Error;
    namespace object {
        template<typename T> class TextureObject;
        template<typename T> class Object;
    }
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace display3D {
namespace textures {
namespace atf_jpegxr_stub {

/// Implements compressed texture upload stub
/// Returns an error indicating compressed textures are not supported
template<typename GC>
Error<GC> do_compressed_upload(
    Activation<GC>* activation,
    ruffle::core::avm2::object::TextureObject<GC> texture,
    ruffle::core::avm2::object::Object<GC> source,
    std::size_t offset,
    bool async
);

} // namespace atf_jpegxr_stub
} // namespace textures
} // namespace display3D
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY3D_TEXTURES_ATF_JPEGXR_STUB_H
