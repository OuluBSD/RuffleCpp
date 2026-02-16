// C++ translation of core/src/avm1/globals/text_snapshot.rs
// TextSnapshot object

#ifndef RUFFLE_CORE_AVM1_GLOBALS_TEXT_SNAPSHOT_H
#define RUFFLE_CORE_AVM1_GLOBALS_TEXT_SNAPSHOT_H

#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Error;
    template<typename T> class NativeObject;
    template<typename T> class Object;
    template<typename T> class Value;
    struct DeclContext;
    struct StaticDeclarations;
    template<typename T> class SystemClass;
}
namespace display_object {
    template<typename T> class MovieClip;
    template<typename T> class TextSnapshot;
}
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace text_snapshot {

/// TextSnapshotObject wrapper class
///
/// Wraps a TextSnapshot display object for use in AVM1.
template<typename GC>
class TextSnapshotObject {
public:
    /// Create a new TextSnapshotObject from a MovieClip
    ///
    /// \param context The update context
    /// \param target The target MovieClip
    static TextSnapshotObject<GC> create(
        void* context,
        display_object::MovieClip<GC> target
    );

    /// Get the underlying TextSnapshot
    display_object::TextSnapshot<GC> text_snapshot() const;
};

/// Property declarations for TextSnapshot prototype
extern const StaticDeclarations PROTO_DECLS;

/// Create the TextSnapshot class
///
/// \param context The declaration context for creating the class
/// \param super_proto The prototype object for the superclass
/// \return A SystemClass containing the created TextSnapshot class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// TextSnapshot constructor
///
/// Creates a TextSnapshot object for the given MovieClip.
///
/// \param activation The current activation context
/// \param this_obj The object being constructed
/// \param args Arguments (first arg should be a MovieClip)
/// \return Value::Undefined
template<typename GC>
Value<GC> constructor(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements TextSnapshot.getCount
///
/// Returns the number of text items in the snapshot.
///
/// \param activation The current activation context
/// \param this_obj The TextSnapshot object
/// \param args Arguments (should be empty)
/// \return The count of text items
template<typename GC>
Value<GC> get_count(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements TextSnapshot.setSelected (stub)
template<typename GC>
Value<GC> set_selected(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements TextSnapshot.getSelected (stub)
template<typename GC>
Value<GC> get_selected(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements TextSnapshot.getText
///
/// Returns text from the snapshot between the specified indices.
///
/// \param activation The current activation context
/// \param this_obj The TextSnapshot object
/// \param args Arguments:
///   - args[0]: from index
///   - args[1]: to index
///   - args[2]: includeNewlines (optional boolean)
/// \return The text string
template<typename GC>
Value<GC> get_text(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements TextSnapshot.getSelectedText (stub)
template<typename GC>
Value<GC> get_selected_text(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements TextSnapshot.hitTestTextNearPos (stub)
template<typename GC>
Value<GC> hit_test_text_near_pos(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements TextSnapshot.setSelectColor (stub)
template<typename GC>
Value<GC> set_select_color(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements TextSnapshot.findText
///
/// Finds text in the snapshot starting from the specified index.
///
/// \param activation The current activation context
/// \param this_obj The TextSnapshot object
/// \param args Arguments:
///   - args[0]: start index
///   - args[1]: text to find
///   - args[2]: caseSensitive (boolean)
/// \return The index where the text was found, or -1 if not found
template<typename GC>
Value<GC> find_text(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements TextSnapshot.getTextRunInfo (stub)
template<typename GC>
Value<GC> get_text_run_info(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

} // namespace text_snapshot
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_TEXT_SNAPSHOT_H
