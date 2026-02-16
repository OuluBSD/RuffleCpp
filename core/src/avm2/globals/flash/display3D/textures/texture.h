// C++ translation of core/src/avm2/globals/flash/display3D/textures/texture.rs
// flash.display3D.textures.Texture implementation

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY3D_TEXTURES_TEXTURE_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY3D_TEXTURES_TEXTURE_H

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
namespace bitmap {
namespace bitmap_data {
    class BitmapData;
    class Color;
}
}
namespace render {
namespace backend {
    enum class Context3DTextureFormat;
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
namespace texture {

/// Internal helper function to copy texture data from ByteArray
/// @param activation The current AVM2 activation
/// @param data The ByteArray object containing texture data
/// @param texture The target texture object
/// @param byte_array_offset Offset in the ByteArray to read from
/// @param side The cube map side (0-5 for cube maps, 0 otherwise)
/// @param mip_level The mip level to upload to
template<typename GC>
void do_copy(
    Activation<GC>* activation,
    Object<GC> data,
    TextureObject<GC> texture,
    uint32_t byte_array_offset,
    uint32_t side,
    uint32_t mip_level
);

/// Internal helper for uploadCompressedTextureFromByteArray
/// @param activation The current AVM2 activation
/// @param this_value The Texture object
/// @param args Method arguments (data, byte_array_offset)
template<typename GC>
Value<GC> upload_compressed_texture_from_byte_array_internal(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display3D.textures.Texture.uploadFromByteArray
/// Uploads texture data from a ByteArray
/// @param data The ByteArray containing texture data in BGRA format
/// @param byte_array_offset Offset in the ByteArray to start reading from
/// @param mip_level The mip level to upload to (only 0 is fully supported)
template<typename GC>
Value<GC> upload_from_byte_array(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display3D.textures.Texture.uploadFromBitmapData
/// Uploads texture data from a BitmapData object
/// @param source The BitmapData to upload from
/// @param mip_level The mip level to upload to (only 0 is fully supported)
template<typename GC>
Value<GC> upload_from_bitmap_data(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace texture
} // namespace textures
} // namespace display3D
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY3D_TEXTURES_TEXTURE_H
