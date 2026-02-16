// C++ translation of core/src/avm2/globals/flash/system/capabilities.rs
// flash.system.Capabilities native methods

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_SYSTEM_CAPABILITIES_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_SYSTEM_CAPABILITIES_H

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
namespace system {
namespace capabilities {

/// Implements flash.system.Capabilities.os
///
/// Returns the operating system string.
/// For FlashPlayer runtime, returns "Windows 8" for broad compatibility.
/// For AIR runtime, returns the actual OS.
template<typename GC>
Value<GC> get_os(
    Activation<GC>* activation,
    Value<GC> this_value,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements flash.system.Capabilities.version
///
/// Returns the Flash Player version string in format "OS major,0,0,0".
template<typename GC>
Value<GC> get_version(
    Activation<GC>* activation,
    Value<GC> this_value,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements flash.system.Capabilities.playerType
///
/// Returns the player type: "PlugIn" for WASM, "StandAlone" for FlashPlayer,
/// or "Desktop" for AIR.
template<typename GC>
Value<GC> get_player_type(
    Activation<GC>* activation,
    Value<GC> this_value,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements flash.system.Capabilities.screenResolutionX
///
/// Returns the screen resolution width, adjusted for HiDPI.
template<typename GC>
Value<GC> get_screen_resolution_x(
    Activation<GC>* activation,
    Value<GC> this_value,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements flash.system.Capabilities.screenResolutionY
///
/// Returns the screen resolution height, adjusted for HiDPI.
template<typename GC>
Value<GC> get_screen_resolution_y(
    Activation<GC>* activation,
    Value<GC> this_value,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements flash.system.Capabilities.pixelAspectRatio
///
/// Returns the pixel aspect ratio (always 1 for modern displays).
template<typename GC>
Value<GC> get_pixel_aspect_ratio(
    Activation<GC>* activation,
    Value<GC> this_value,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements flash.system.Capabilities.screenDPI
///
/// Returns the screen DPI (always 72 as per Flash Player behavior).
template<typename GC>
Value<GC> get_screen_dpi(
    Activation<GC>* activation,
    Value<GC> this_value,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements flash.system.Capabilities.language
///
/// Returns the system language code.
template<typename GC>
Value<GC> get_language(
    Activation<GC>* activation,
    Value<GC> this_value,
    const Value<GC>* args,
    size_t arg_count
);

} // namespace capabilities
} // namespace system
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_SYSTEM_CAPABILITIES_H
