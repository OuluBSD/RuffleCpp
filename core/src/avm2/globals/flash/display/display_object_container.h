// C++ translation of core/src/avm2/globals/flash/display/display_object_container.rs
// flash.display.DisplayObjectContainer builtin/prototype

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_DISPLAY_OBJECT_CONTAINER_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_DISPLAY_OBJECT_CONTAINER_H

#include <cstdint>
#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class Object;
    class ArrayObject;
    template<typename T> class ArrayStorage;
    namespace parameters {
        enum class ParametersExt;
    }
}
namespace display_object {
    template<typename T> class DisplayObject;
    template<typename T> class TDisplayObject;
    template<typename T> class TDisplayObjectContainer;
    enum class HitTestOptions;
}
namespace swf {
    template<typename T> class Point;
    template<typename T> class Twips;
}
namespace context {
    template<typename T> class UpdateContext;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace slots {
    // Forward declare slot constants
    namespace flash_geom_point {
        extern const int X;
        extern const int Y;
    }
}
}
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace display {
namespace display_object_container {

/// Validate if we can add a child to a parent at a given index.
///
/// There are several conditions which should cause an add operation to fail:
///  - The index is off the end of the child list of the proposed parent.
///  - The child is already a transitive child of the proposed parent.
///
/// @param activation The current activation context
/// @param new_parent The proposed parent display object
/// @param proposed_child The proposed child display object
/// @param proposed_index The index at which to add the child
/// @return Result indicating success or an error
template<typename GC>
Result<void, Error<GC>> validate_add_operation(
    Activation<GC>* activation,
    display_object::DisplayObject<GC> new_parent,
    display_object::DisplayObject<GC> proposed_child,
    size_t proposed_index
);

/// Validate if we can remove a child from a given parent.
///
/// There are several conditions which should cause a remove operation to fail:
///  - The child is not a child of the parent
///
/// @param activation The current activation context
/// @param old_parent The parent display object
/// @param proposed_child The child display object to remove
/// @return Result indicating success or an error
template<typename GC>
Result<void, Error<GC>> validate_remove_operation(
    Activation<GC>* activation,
    display_object::DisplayObject<GC> old_parent,
    display_object::DisplayObject<GC> proposed_child
);

/// Remove an element from its parent display list.
///
/// @param context The update context
/// @param child The child display object to remove
template<typename GC>
void remove_child_from_displaylist(
    context::UpdateContext<GC>* context,
    display_object::DisplayObject<GC> child
);

/// Add the child to parent's display list.
///
/// @param context The update context
/// @param parent The parent display object
/// @param child The child display object to add
/// @param index The index at which to add the child
template<typename GC>
void add_child_to_displaylist(
    context::UpdateContext<GC>* context,
    display_object::DisplayObject<GC> parent,
    display_object::DisplayObject<GC> child,
    size_t index
);

/// Implements DisplayObjectContainer.getChildAt
///
/// Returns the child display object at the specified index.
///
/// @param activation The current activation context
/// @param this_value The DisplayObjectContainer object
/// @param args Arguments (index)
/// @return The child display object or throws error if index is out of range
template<typename GC>
Value<GC> get_child_at(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements DisplayObjectContainer.getChildByName
///
/// Returns the child display object with the specified name.
///
/// @param activation The current activation context
/// @param this_value The DisplayObjectContainer object
/// @param args Arguments (name)
/// @return The child display object or null if not found
template<typename GC>
Value<GC> get_child_by_name(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements DisplayObjectContainer.addChild
///
/// Adds a child to the end of the display list.
///
/// @param activation The current activation context
/// @param this_value The DisplayObjectContainer object
/// @param args Arguments (child)
/// @return The added child display object
template<typename GC>
Value<GC> add_child(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements DisplayObjectContainer.addChildAt
///
/// Adds a child to the display list at the specified index.
///
/// @param activation The current activation context
/// @param this_value The DisplayObjectContainer object
/// @param args Arguments (child, index)
/// @return The added child display object
template<typename GC>
Value<GC> add_child_at(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements DisplayObjectContainer.removeChild
///
/// Removes a child from the display list.
///
/// @param activation The current activation context
/// @param this_value The DisplayObjectContainer object
/// @param args Arguments (child)
/// @return The removed child display object
template<typename GC>
Value<GC> remove_child(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements DisplayObjectContainer.numChildren property getter
///
/// @param activation The current activation context
/// @param this_value The DisplayObjectContainer object
/// @param args Arguments (unused)
/// @return The number of children
template<typename GC>
Value<GC> get_num_children(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements DisplayObjectContainer.contains
///
/// Checks if the specified display object is a child of this container.
///
/// @param activation The current activation context
/// @param this_value The DisplayObjectContainer object
/// @param args Arguments (child)
/// @return true if the child is contained
template<typename GC>
Value<GC> contains(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements DisplayObjectContainer.getChildIndex
///
/// Returns the index of the specified child.
///
/// @param activation The current activation context
/// @param this_value The DisplayObjectContainer object
/// @param args Arguments (child)
/// @return The index of the child
template<typename GC>
Value<GC> get_child_index(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements DisplayObjectContainer.removeChildAt
///
/// Removes a child from the display list at the specified index.
///
/// @param activation The current activation context
/// @param this_value The DisplayObjectContainer object
/// @param args Arguments (index)
/// @return The removed child display object
template<typename GC>
Value<GC> remove_child_at(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements DisplayObjectContainer.removeChildren
///
/// Removes a range of children from the display list.
///
/// @param activation The current activation context
/// @param this_value The DisplayObjectContainer object
/// @param args Arguments (beginIndex, endIndex)
/// @return Value::Undefined
template<typename GC>
Value<GC> remove_children(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements DisplayObjectContainer.setChildIndex
///
/// Changes the position of an existing child in the display list.
///
/// @param activation The current activation context
/// @param this_value The DisplayObjectContainer object
/// @param args Arguments (child, index)
/// @return Value::Undefined
template<typename GC>
Value<GC> set_child_index(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements DisplayObjectContainer.swapChildrenAt
///
/// Swaps the positions of two children at specified indices.
///
/// @param activation The current activation context
/// @param this_value The DisplayObjectContainer object
/// @param args Arguments (index1, index2)
/// @return Value::Undefined
template<typename GC>
Value<GC> swap_children_at(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements DisplayObjectContainer.swapChildren
///
/// Swaps the positions of two specified children.
///
/// @param activation The current activation context
/// @param this_value The DisplayObjectContainer object
/// @param args Arguments (child1, child2)
/// @return Value::Undefined
template<typename GC>
Value<GC> swap_children(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements DisplayObjectContainer.stopAllMovieClips
///
/// Stops all MovieClip children recursively.
///
/// @param activation The current activation context
/// @param this_value The DisplayObjectContainer object
/// @param args Arguments (unused)
/// @return Value::Undefined
template<typename GC>
Value<GC> stop_all_movie_clips(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements DisplayObjectContainer.getObjectsUnderPoint
///
/// Returns an array of display objects under the specified point.
///
/// @param activation The current activation context
/// @param this_value The DisplayObjectContainer object
/// @param args Arguments (point)
/// @return Array of display objects under the point
template<typename GC>
Value<GC> get_objects_under_point(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements DisplayObjectContainer.areInaccessibleObjectsUnderPoint
///
/// Checks if there are inaccessible objects under the specified point.
///
/// @param activation The current activation context
/// @param this_value The DisplayObjectContainer object (unused)
/// @param args Arguments (point, unused)
/// @return false (stub implementation)
template<typename GC>
Value<GC> are_inaccessible_objects_under_point(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements DisplayObjectContainer.mouseChildren property getter
///
/// @param activation The current activation context
/// @param this_value The DisplayObjectContainer object
/// @param args Arguments (unused)
/// @return The mouse children count
template<typename GC>
Value<GC> get_mouse_children(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements DisplayObjectContainer.mouseChildren property setter
///
/// @param activation The current activation context
/// @param this_value The DisplayObjectContainer object
/// @param args Arguments (value)
/// @return Value::Undefined
template<typename GC>
Value<GC> set_mouse_children(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements DisplayObjectContainer.tabChildren property getter
///
/// @param activation The current activation context
/// @param this_value The DisplayObjectContainer object
/// @param args Arguments (unused)
/// @return The tab children setting
template<typename GC>
Value<GC> get_tab_children(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements DisplayObjectContainer.tabChildren property setter
///
/// @param activation The current activation context
/// @param this_value The DisplayObjectContainer object
/// @param args Arguments (value)
/// @return Value::Undefined
template<typename GC>
Value<GC> set_tab_children(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace display_object_container
} // namespace display
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY_DISPLAY_OBJECT_CONTAINER_H
