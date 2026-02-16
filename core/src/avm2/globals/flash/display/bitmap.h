// C++ translation of core/src/avm2/globals/flash/display/bitmap.rs
// flash.display.Bitmap builtin/prototype

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_BITMAP_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_BITMAP_H

#include <vector>
#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class ClassObject;
    template<typename T> class TObject;
    namespace object {
        template<typename T> class BitmapDataObject;
        template<typename T> class Object;
    }
}
namespace bitmap {
    namespace bitmap_data {
        template<typename T> class BitmapData;
    }
}
namespace display_object {
    template<typename T> class Bitmap;
}
namespace character {
    enum class Character;
}
}
namespace render {
namespace bitmap {
    enum class PixelSnapping;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace display {

// Forward declare functions from bitmap_data module
namespace bitmap_data {
    template<typename GC>
    typename bitmap::bitmap_data::BitmapData<GC>* fill_bitmap_data_from_symbol(
        Activation<GC>* activation,
        const character::Character* bitmap
    );
}

namespace bitmap {

/// Allocator function for flash.display.Bitmap
/// Creates a new Bitmap display object
/// If called on a Bitmap subclass with a symbol registered, creates bitmap from that symbol
template<typename GC>
typename avm2::object::Object<GC>* bitmap_allocator(
    ClassObject<GC>* class_obj,
    Activation<GC>* activation
);

/// Implements flash.display.Bitmap.init method
/// Called from the constructor to initialize the bitmap
/// @param bitmapData The BitmapData to display
/// @param pixelSnapping "always", "auto", or "never"
/// @param smoothing Whether to smooth the bitmap
template<typename GC>
Value<GC> init(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Bitmap.bitmapData getter
/// Returns the BitmapData object displayed by this bitmap
template<typename GC>
Value<GC> get_bitmap_data(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Bitmap.bitmapData setter
/// Sets the BitmapData object displayed by this bitmap
/// Passing null results in a dummy BitmapData being set
template<typename GC>
Value<GC> set_bitmap_data(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Bitmap.pixelSnapping getter
/// Returns the pixel snapping mode: "always", "auto", or "never"
template<typename GC>
Value<GC> get_pixel_snapping(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Bitmap.pixelSnapping setter
/// Sets the pixel snapping mode
/// @param value "always", "auto", or "never"
/// @throws Error #2008 if invalid pixelSnapping value
template<typename GC>
Value<GC> set_pixel_snapping(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Bitmap.smoothing getter
/// Returns whether smoothing is enabled
template<typename GC>
Value<GC> get_smoothing(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements Bitmap.smoothing setter
/// Sets whether smoothing is enabled
template<typename GC>
Value<GC> set_smoothing(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace bitmap
} // namespace display
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_BITMAP_H
