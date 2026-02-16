// C++ translation of core/src/avm1/globals/selection.rs
// Selection object for AVM1 text field selection

#ifndef RUFFLE_CORE_AVM1_GLOBALS_SELECTION_H
#define RUFFLE_CORE_AVM1_GLOBALS_SELECTION_H

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
}
namespace avm1 {
namespace globals {
namespace as_broadcaster {
    template<typename T> struct BroadcasterFunctions;
}
}
}
namespace display_object {
    template<typename T> class EditText;
    template<typename T> class InteractiveObject;
    struct TextSelection;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace selection {

/// Property declarations for Selection object
extern const StaticDeclarations OBJECT_DECLS;

/// Create the Selection object
///
/// Creates and initializes the Selection singleton object with broadcaster functionality.
///
/// \param context The declaration context for creating properties
/// \param broadcaster_fns Broadcaster functions for event handling
/// \param array_proto The array prototype for broadcaster
/// \return The created Selection object
template<typename GC>
Object<GC>* create(
    DeclContext<GC>* context,
    avm1::globals::as_broadcaster::BroadcasterFunctions<GC> broadcaster_fns,
    Object<GC>* array_proto
);

/// Implements Selection.getBeginIndex method
///
/// Gets the starting index of the current selection.
///
/// \param activation The current activation context
/// \param this_obj The Selection object (unused)
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return The start index, or -1 if no selection
template<typename GC>
Value<GC> get_begin_index(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Selection.getEndIndex method
///
/// Gets the ending index of the current selection.
///
/// \param activation The current activation context
/// \param this_obj The Selection object (unused)
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return The end index, or -1 if no selection
template<typename GC>
Value<GC> get_end_index(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Selection.getCaretIndex method
///
/// Gets the caret position of the current selection.
///
/// \param activation The current activation context
/// \param this_obj The Selection object (unused)
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return The caret index, or -1 if no selection
template<typename GC>
Value<GC> get_caret_index(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Selection.setFocus method
///
/// Sets focus to the specified display object.
///
/// \param activation The current activation context
/// \param this_obj The Selection object (unused)
/// \param args Arguments (display object path or null/undefined)
/// \param arg_count Number of arguments
/// \return true if focus was set successfully, false otherwise
template<typename GC>
Value<GC> set_focus(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Selection.getSelection method
///
/// Gets the path of the currently focused display object.
///
/// \param activation The current activation context
/// \param this_obj The Selection object (unused)
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return The path string of the focused object, or null if none
template<typename GC>
Value<GC> get_focus(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements Selection.setSelection method
///
/// Sets the selection range in the focused text field.
///
/// \param activation The current activation context
/// \param this_obj The Selection object (unused)
/// \param args Arguments (start index, end index)
/// \param arg_count Number of arguments
/// \return Value::Undefined
template<typename GC>
Value<GC> set_selection(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

} // namespace selection
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_SELECTION_H
