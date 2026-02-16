// C++ translation of core/src/avm1/globals/context_menu.rs
// ContextMenu object for AVM1

#ifndef RUFFLE_CORE_AVM1_GLOBALS_CONTEXT_MENU_H
#define RUFFLE_CORE_AVM1_GLOBALS_CONTEXT_MENU_H

#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Error;
    template<typename T> class DisplayObject;
    template<typename T> class Object;
    template<typename T> class Value;
    struct DeclContext;
    struct StaticDeclarations;
    template<typename T> class SystemClass;
}
namespace context_menu {
    template<typename T> class ContextMenuState;
    struct BuiltInItemFlags;
    struct ContextMenuItem;
    template<typename T> class ContextMenuCallback;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace context_menu {

/// Property declarations for ContextMenu prototype
extern const StaticDeclarations PROTO_DECLS;

/// Create the ContextMenu class
///
/// \param context The declaration context for creating the class
/// \param super_proto The prototype object for the superclass
/// \return A SystemClass containing the created ContextMenu class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// ContextMenu constructor
///
/// Initializes the ContextMenu object with an onSelect callback.
///
/// \param activation The current activation context
/// \param this_obj The ContextMenu object being constructed
/// \param args Arguments for the constructor (onSelect callback)
/// \param arg_count Number of arguments
/// \return Value::Undefined
template<typename GC>
Value<GC> constructor(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ContextMenu.copy method
///
/// Creates a copy of this ContextMenu object.
///
/// \param activation The current activation context
/// \param this_obj The ContextMenu object
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return A new ContextMenu object
template<typename GC>
Value<GC> copy(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ContextMenu.hideBuiltInItems method
///
/// Hides all built-in context menu items.
///
/// \param activation The current activation context
/// \param this_obj The ContextMenu object
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return Value::Undefined
template<typename GC>
Value<GC> hide_builtin_items(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Create a context menu state from a ContextMenu object
///
/// Converts an AVM1 ContextMenu object to the internal ContextMenuState.
///
/// \param menu The ContextMenu object, or nullptr for default menu
/// \param object The display object the menu is associated with
/// \param activation The current activation context
/// \return The created ContextMenuState
template<typename GC>
context_menu::ContextMenuState<GC> make_context_menu_state(
    Object<GC>* menu,
    DisplayObject<GC>* object,
    Activation<GC>* activation
);

} // namespace context_menu
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_CONTEXT_MENU_H
