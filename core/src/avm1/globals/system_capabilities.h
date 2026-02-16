// C++ translation of core/src/avm1/globals/system_capabilities.rs
// System.capabilities object for AVM1

#ifndef RUFFLE_CORE_AVM1_GLOBALS_SYSTEM_CAPABILITIES_H
#define RUFFLE_CORE_AVM1_GLOBALS_SYSTEM_CAPABILITIES_H

#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename T> class AvmString;
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Error;
    template<typename T> class Object;
    template<typename T> class Value;
    struct DeclContext;
    struct StaticDeclarations;
}
namespace system_properties {
    enum class SystemCapabilities;
}
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace system_capabilities {

/// Property declarations for System.capabilities object
extern const StaticDeclarations OBJECT_DECLS;

/// Create the System.capabilities object
///
/// \param context The declaration context for creating the object
/// \return The created capabilities object
template<typename GC>
Object<GC> create(DeclContext<GC>* context);

/// System.capabilities.hasAccessibility property getter
template<typename GC>
Value<GC> get_has_accessibility(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.capabilities.pixelAspectRatio property getter
template<typename GC>
Value<GC> get_pixel_aspect_ratio(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.capabilities.screenColor property getter
template<typename GC>
Value<GC> get_screen_color(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.capabilities.screenDPI property getter
template<typename GC>
Value<GC> get_screen_dpi(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.capabilities.screenResolutionY property getter
template<typename GC>
Value<GC> get_screen_resolution_y(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.capabilities.screenResolutionX property getter
template<typename GC>
Value<GC> get_screen_resolution_x(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.capabilities.hasTLS property getter
template<typename GC>
Value<GC> get_has_tls(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.capabilities.hasVideoEncoder property getter
template<typename GC>
Value<GC> get_has_video_encoder(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.capabilities.hasAudioEncoder property getter
template<typename GC>
Value<GC> get_has_audio_encoder(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.capabilities.hasMP3 property getter
template<typename GC>
Value<GC> get_has_mp3(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.capabilities.hasAudio property getter
template<typename GC>
Value<GC> get_has_audio(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.capabilities.serverString property getter
template<typename GC>
Value<GC> get_server_string(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.capabilities.version property getter
template<typename GC>
Value<GC> get_version(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.capabilities.hasStreamingAudio property getter
template<typename GC>
Value<GC> get_has_streaming_audio(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.capabilities.hasStreamingVideo property getter
template<typename GC>
Value<GC> get_has_streaming_video(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.capabilities.hasEmbeddedVideo property getter
template<typename GC>
Value<GC> get_has_embedded_video(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.capabilities.hasPrinting property getter
template<typename GC>
Value<GC> get_has_printing(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.capabilities.hasScreenPlayback property getter
template<typename GC>
Value<GC> get_has_screen_playback(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.capabilities.hasScreenBroadcast property getter
template<typename GC>
Value<GC> get_has_screen_broadcast(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.capabilities.isDebugger property getter
template<typename GC>
Value<GC> get_is_debugger(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.capabilities.playerType property getter
template<typename GC>
Value<GC> get_player_type(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.capabilities.avHardwareDisable property getter
template<typename GC>
Value<GC> get_is_av_hardware_disabled(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.capabilities.localFileReadDisable property getter
template<typename GC>
Value<GC> get_is_local_file_read_disabled(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.capabilities.windowlessDisable property getter
template<typename GC>
Value<GC> get_is_windowless_disabled(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.capabilities.maxLevelIDC property getter
template<typename GC>
Value<GC> get_max_idc_level(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.capabilities.isEmbeddedInAcrobat property getter
template<typename GC>
Value<GC> get_is_acrobat_embedded(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.capabilities.manufacturer property getter
template<typename GC>
Value<GC> get_manufacturer(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.capabilities.os property getter
template<typename GC>
Value<GC> get_os_name(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.capabilities.cpuArchitecture property getter
template<typename GC>
Value<GC> get_cpu_architecture(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.capabilities.language property getter
template<typename GC>
Value<GC> get_language(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.capabilities.hasIME property getter
template<typename GC>
Value<GC> get_has_ime(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.capabilities.supports32BitProcesses property getter
template<typename GC>
Value<GC> get_has_32_bit_support(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// System.capabilities.supports64BitProcesses property getter
template<typename GC>
Value<GC> get_has_64_bit_support(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

} // namespace system_capabilities
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_SYSTEM_CAPABILITIES_H
