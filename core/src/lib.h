// C++ translation of core/src/lib.rs
// Core library module declarations and re-exports

#ifndef RUFFLE_CORE_LIB_H
#define RUFFLE_CORE_LIB_H

// Standard library includes
#include <cstdint>
#include <memory>

// Forward declarations for core modules
namespace ruffle {
namespace core {

// Display object module
namespace display_object {
    enum class StageAlign;
    enum class StageDisplayState;
    enum class StageScaleMode;
}

// Re-export display object types
using StageAlign = display_object::StageAlign;
using StageDisplayState = display_object::StageDisplayState;
using StageScaleMode = display_object::StageScaleMode;

// AVM1 module
namespace avm1 {
    // AVM1 functionality
}

// AVM2 module
namespace avm2 {
    // AVM2 functionality
}

// AVM RNG module
class AvmRng;

// Binary data module
class BinaryData;

// Bitmap module
namespace bitmap {
    // Bitmap functionality
}

// Character module
template<typename GC>
class Character;

// Context module
template<typename GC>
class UpdateContext;
template<typename GC>
class StringContext;

// Context menu module
namespace context_menu {
    template<typename GC>
    class ContextMenuItem;
}

// Drawing module
namespace drawing {
    // Drawing functionality
}

// Events module
namespace events {
    struct PlayerEvent;
}

// Focus tracker module
template<typename GC>
class FocusTracker;

// Font module
template<typename GC>
class Font;

// Frame lifecycle module
namespace frame_lifecycle {
    // Frame lifecycle functionality
}

// HTML module
namespace html {
    // HTML functionality
}

// Input module
namespace input {
    // Input functionality
}

// Library module
template<typename GC>
class Library;

// Limits module
namespace limits {
    // Limits functionality
}

// Loader module
namespace loader {
    enum class LoadBehavior;
    template<typename GC>
    class Loader;
}

// Local connection module
namespace local_connection {
    // Local connection functionality
}

// Locale module
namespace locale {
    // Locale functionality
}

// Net connection module
template<typename GC>
class NetConnection;

// Orphan manager module
namespace orphan_manager {
    // Orphan manager functionality
}

// Pixel bender module
namespace pixel_bender {
    // Pixel bender functionality
}

// Player module
enum class PlayerMode;
template<typename GC>
class Player;
template<typename GC>
class PlayerRuntime;
class StaticCallstack;

// Prelude module - common type aliases
namespace prelude {
    // Common type aliases
}

// Socket module
template<typename GC>
class Socket;

// Streams module
namespace streams {
    // Streams functionality
}

// String module
template<typename GC>
class AvmString;

// System properties module
namespace system_properties {
    // System properties functionality
}

// Tag utils module
namespace tag_utils {
    template<typename GC>
    class SwfMovie;
}

// Timer module
template<typename GC>
class Timer;

// Types module
namespace types {
    // Type definitions
}

// VM interface module
namespace vminterface {
    // VM interface functionality
}

// Backend module
namespace backend {
    // Backend functionality
}

// Compatibility rules module
class CompatibilityRules;

// Config module
namespace config {
    // Configuration functionality
}

// Debug UI module (optional, feature-gated)
#ifdef FEATURE_EGUI
namespace debug_ui {
    // Debug UI functionality
}
#endif

// External module
namespace external {
    // External interface functionality
}

// I18N module
namespace i18n {
    // Internationalization functionality
}

// Stub module
namespace stub {
    // Stub functionality
}

// Re-exports from other modules
using ContextMenuItem = context_menu::ContextMenuItem;
using PlayerEvent = events::PlayerEvent;
using LoadBehavior = loader::LoadBehavior;
using Player = core::Player;
using PlayerBuilder = core::Player;  // Simplified - PlayerBuilder pattern
using PlayerMode = core::PlayerMode;
using PlayerRuntime = core::PlayerRuntime;
using StaticCallstack = core::StaticCallstack;

} // namespace core
} // namespace ruffle

// Re-exports from render module
namespace ruffle {
namespace render {
namespace backend {
    struct ViewportDimensions;
}
}
}

namespace ruffle {
namespace core {
using ViewportDimensions = ruffle::render::backend::ViewportDimensions;
}
}

// Re-exports from swf module
namespace swf {
    struct Color;
    using CharacterId = uint16_t;
}

namespace ruffle {
namespace core {
using Color = swf::Color;
}
}

// External library re-exports
namespace indexmap {
    // indexmap functionality - would use std::unordered_map or similar
}

namespace ttf_parser {
    // ttf_parser functionality
}

namespace ruffle {
namespace core {

/// The newest Flash Player version known to Ruffle.
inline constexpr uint8_t NEWEST_PLAYER_VERSION = 51;

/// The default Flash Player version that Ruffle will emulate.
inline constexpr uint8_t DEFAULT_PLAYER_VERSION = 32;

} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_LIB_H
