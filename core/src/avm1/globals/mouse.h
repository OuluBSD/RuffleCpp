// C++ translation of core/src/avm1/globals/mouse.rs
// Mouse object for AVM1

#ifndef RUFFLE_CORE_AVM1_GLOBALS_MOUSE_H
#define RUFFLE_CORE_AVM1_GLOBALS_MOUSE_H

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
namespace globals {
namespace as_broadcaster {
    template<typename T> struct BroadcasterFunctions;
}
}
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace mouse {

// Property declarations for Mouse object
extern const StaticDeclarations OBJECT_DECLS;

/// Create the Mouse object
///
/// \param context The declaration context for creating the object
/// \param broadcaster_functions Functions for broadcaster initialization
/// \param array_proto The array prototype object
/// \return The created Mouse object
template<typename GC>
Object<GC>* create(
    DeclContext<GC>* context,
    globals::as_broadcaster::BroadcasterFunctions<GC> broadcaster_functions,
    Object<GC>* array_proto
);

/// Mouse.show method - Shows the mouse cursor
///
/// \param activation The current activation context
/// \param this_obj The Mouse object
/// \param args Arguments passed to the method (none expected)
/// \return 0 if mouse was already visible, 1 otherwise
template<typename GC>
Value<GC> show_mouse(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Mouse.hide method - Hides the mouse cursor
///
/// \param activation The current activation context
/// \param this_obj The Mouse object
/// \param args Arguments passed to the method (none expected)
/// \return 0 if mouse was already hidden, 1 otherwise
template<typename GC>
Value<GC> hide_mouse(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

} // namespace mouse
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_MOUSE_H
