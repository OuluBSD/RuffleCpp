// C++ translation of core/src/avm1/globals/context_menu_item.rs
// ContextMenuItem class implementation

#ifndef RUFFLE_CORE_AVM1_GLOBALS_CONTEXT_MENU_ITEM_H
#define RUFFLE_CORE_AVM1_GLOBALS_CONTEXT_MENU_ITEM_H

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
    template<typename T> class SystemClass;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace context_menu_item {

/// Property declarations for ContextMenuItem prototype
extern const StaticDeclarations PROTO_DECLS;

/// Create the ContextMenuItem class
///
/// \param context The declaration context for creating the class
/// \param super_proto The prototype object for the superclass
/// \return A SystemClass containing the created ContextMenuItem class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// ContextMenuItem constructor
///
/// Creates a new ContextMenuItem with the specified properties.
///
/// \param activation The current activation context
/// \param this_obj The object being constructed
/// \param args Arguments:
///   - args[0]: caption (string)
///   - args[1]: callback (optional function)
///   - args[2]: separatorBefore (optional boolean)
///   - args[3]: enabled (optional boolean)
///   - args[4]: visible (optional boolean)
/// \return Value::Undefined
template<typename GC>
Value<GC> constructor(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements ContextMenuItem.copy
///
/// Creates a copy of this ContextMenuItem.
///
/// \param activation The current activation context
/// \param this_obj The ContextMenuItem object
/// \param args Arguments (should be empty)
/// \return A new ContextMenuItem with the same properties
template<typename GC>
Value<GC> copy(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

} // namespace context_menu_item
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_CONTEXT_MENU_ITEM_H
