// C++ translation of core/src/avm2/globals/flash/display3D/textures/cube_texture.rs
// flash.display3D.textures.CubeTexture implementation

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY3D_TEXTURES_CUBE_TEXTURE_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY3D_TEXTURES_CUBE_TEXTURE_H

#include <vector>
#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class Object;
    template<typename T> class TextureObject;
    namespace parameters {
        class ParametersExt;
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
namespace cube_texture {

/// Implements flash.display3D.textures.CubeTexture.uploadFromByteArray
/// Uploads texture data from a ByteArray to a specific cube map side
/// @param activation The current AVM2 activation
/// @param this_value The CubeTexture object
/// @param args Method arguments: data (ByteArray), byte_array_offset (uint), side (uint), mip_level (uint)
template<typename GC>
Value<GC> upload_from_byte_array(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display3D.textures.CubeTexture.uploadCompressedTextureFromByteArray
/// Uploads compressed texture data from a ByteArray
/// @param activation The current AVM2 activation
/// @param this_value The CubeTexture object
/// @param args Method arguments: data (ByteArray), byte_array_offset (uint), async (bool)
template<typename GC>
Value<GC> upload_compressed_texture_from_byte_array(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display3D.textures.CubeTexture.uploadFromBitmapData
/// Uploads texture data from a BitmapData object to a specific cube map side
/// @param activation The current AVM2 activation
/// @param this_value The CubeTexture object
/// @param args Method arguments: source (BitmapData), side (uint), mip_level (uint)
template<typename GC>
Value<GC> upload_from_bitmap_data(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace cube_texture
} // namespace textures
} // namespace display3D
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY3D_TEXTURES_CUBE_TEXTURE_H
