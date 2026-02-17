// C++ translation of core/src/avm2/globals/flash/display3D/index_buffer_3d.rs
// flash.display3D.IndexBuffer3D native methods

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY3D_INDEX_BUFFER_3D_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY3D_INDEX_BUFFER_3D_H

#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class Object;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace display3D {
namespace index_buffer_3d {

/// Implements flash.display3D.IndexBuffer3D.uploadFromByteArray
/// Uploads index data from a ByteArray
///
/// @param activation The current activation context
/// @param this_value The IndexBuffer3D object
/// @param args Function arguments (byteArray, byteOffset, startOffset, count)
/// @return Value::Undefined
template<typename GC>
Value<GC> upload_from_byte_array(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements flash.display3D.IndexBuffer3D.uploadFromVector
/// Uploads index data from a Vector.<uint>
///
/// @param activation The current activation context
/// @param this_value The IndexBuffer3D object
/// @param args Function arguments (data, startOffset, count)
/// @return Value::Undefined
template<typename GC>
Value<GC> upload_from_vector(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace index_buffer_3d
} // namespace display3D
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY3D_INDEX_BUFFER_3D_H
