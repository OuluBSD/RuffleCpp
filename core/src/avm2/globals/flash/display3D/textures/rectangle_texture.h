// C++ translation of core/src/avm2/globals/flash/display3D/textures/rectangle_texture.rs
// flash.display3D.textures.RectangleTexture

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY3D_TEXTURES_RECTANGLE_TEXTURE_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY3D_TEXTURES_RECTANGLE_TEXTURE_H

#include <vector>
#include <cstdint>

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
namespace textures {
namespace rectangle_texture {

/// Implements uploadFromByteArray
/// 
/// Uploads pixel data from a ByteArray to the texture.
/// 
/// \param activation The current activation context
/// \param this_value The RectangleTexture instance
/// \param args Function arguments (data: ByteArray, byte_array_offset: uint)
/// \return undefined
template<typename GC>
Value<GC> upload_from_byte_array(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements uploadFromBitmapData
/// 
/// Uploads pixel data from a BitmapData to the texture.
/// 
/// \param activation The current activation context
/// \param this_value The RectangleTexture instance
/// \param args Function arguments (source: BitmapData)
/// \return undefined
template<typename GC>
Value<GC> upload_from_bitmap_data(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace rectangle_texture
} // namespace textures
} // namespace display3D
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY3D_TEXTURES_RECTANGLE_TEXTURE_H
