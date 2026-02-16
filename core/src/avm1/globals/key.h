// C++ translation of core/src/avm1/globals/key.rs
// Key object

#ifndef RUFFLE_CORE_AVM1_GLOBALS_KEY_H
#define RUFFLE_CORE_AVM1_GLOBALS_KEY_H

#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Error;
    template<typename T> class Object;
    template<typename T> class Value;
    struct DeclContext;
    struct StaticDeclarations;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace key {

// Key code constants
namespace key_code {
    constexpr int32_t CAPSLOCK = 64;      // KeyCode::CAPS_LOCK.value()
    constexpr int32_t BACKSPACE = 8;      // KeyCode::BACKSPACE.value()
    constexpr int32_t DELETEKEY = 46;     // KeyCode::DELETE.value()
    constexpr int32_t INSERT = 45;        // KeyCode::INSERT.value()
    constexpr int32_t ESCAPE = 27;        // KeyCode::ESCAPE.value()
    constexpr int32_t SHIFT = 16;         // KeyCode::SHIFT.value()
    constexpr int32_t CONTROL = 17;       // KeyCode::CONTROL.value()
    constexpr int32_t TAB = 9;            // KeyCode::TAB.value()
    constexpr int32_t END = 35;           // KeyCode::END.value()
    constexpr int32_t HOME = 36;          // KeyCode::HOME.value()
    constexpr int32_t PGDN = 34;          // KeyCode::PAGE_DOWN.value()
    constexpr int32_t PGUP = 33;          // KeyCode::PAGE_UP.value()
    constexpr int32_t RIGHT = 39;         // KeyCode::RIGHT.value()
    constexpr int32_t LEFT = 37;          // KeyCode::LEFT.value()
    constexpr int32_t DOWN = 40;          // KeyCode::DOWN.value()
    constexpr int32_t UP = 38;            // KeyCode::UP.value()
    constexpr int32_t SPACE = 32;         // KeyCode::SPACE.value()
    constexpr int32_t ENTER = 13;         // KeyCode::ENTER.value()
    constexpr int32_t ALT = 18;           // KeyCode::ALT.value()
}

/// Property declarations for Key object
extern const StaticDeclarations OBJECT_DECLS;

/// Create the Key object
///
/// \param context The declaration context for creating the object
/// \param broadcaster_functions Broadcaster functions for event handling
/// \param array_proto The Array prototype object
/// \return The created Key object
template<typename GC>
Object<GC> create(
    DeclContext<GC>* context,
    // BroadcasterFunctions<GC> broadcaster_functions,
    Object<GC>* array_proto
);

/// Implements Key.isDown method
///
/// \param activation The current activation context
/// \param this_obj The Key object
/// \param args Arguments passed to the method (key code)
/// \param arg_count Number of arguments
/// \return true if the key is currently down
template<typename GC>
Value<GC> is_down(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Key.isToggled method
///
/// Checks if a toggle key (Caps Lock, Num Lock, Scroll Lock) is toggled.
///
/// \param activation The current activation context
/// \param this_obj The Key object
/// \param args Arguments passed to the method (key code)
/// \param arg_count Number of arguments
/// \return true if the key is toggled
template<typename GC>
Value<GC> is_toggled(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Key.getAscii method
///
/// \param activation The current activation context
/// \param this_obj The Key object
/// \param args Arguments passed to the method
/// \param arg_count Number of arguments
/// \return ASCII code of the last key pressed
template<typename GC>
Value<GC> get_ascii(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Key.getCode method
///
/// \param activation The current activation context
/// \param this_obj The Key object
/// \param args Arguments passed to the method
/// \param arg_count Number of arguments
/// \return Virtual key code of the last key pressed
template<typename GC>
Value<GC> get_code(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

} // namespace key
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_KEY_H
