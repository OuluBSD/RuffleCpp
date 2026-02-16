// C++ translation of core/src/avm1/globals/button.rs
// Button prototype for AVM1

#ifndef RUFFLE_CORE_AVM1_GLOBALS_BUTTON_H
#define RUFFLE_CORE_AVM1_GLOBALS_BUTTON_H

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
namespace display_object {
    template<typename T> class Avm1Button;
    template<typename T> class InteractiveObject;
}
namespace swf {
    enum class BlendMode;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace button {

/// Property declarations for Button prototype
extern const StaticDeclarations PROTO_DECLS;

/// Create the Button class
///
/// \param context The declaration context for creating the class
/// \param super_proto The prototype object for the superclass
/// \return A SystemClass containing the created Button class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// Gets the blend mode of the button.
///
/// \param button The AVM1 button object
/// \param activation The current activation context
/// \return The blend mode as a string
template<typename GC>
Value<GC> blend_mode(
    display_object::Avm1Button<GC>* button,
    Activation<GC>* activation
);

/// Sets the blend mode of the button.
///
/// \param button The AVM1 button object
/// \param activation The current activation context
/// \param value The new blend mode value
/// \return Error if any
template<typename GC>
Value<GC> set_blend_mode(
    display_object::Avm1Button<GC>* button,
    Activation<GC>* activation,
    const Value<GC>& value
);

/// Gets the filters applied to the button.
///
/// \param button The AVM1 button object
/// \param activation The current activation context
/// \return Array of filter objects
template<typename GC>
Value<GC> filters(
    display_object::Avm1Button<GC>* button,
    Activation<GC>* activation
);

/// Sets the filters on the button.
///
/// \param button The AVM1 button object
/// \param activation The current activation context
/// \param value The array of filter objects
/// \return Error if any
template<typename GC>
Value<GC> set_filters(
    display_object::Avm1Button<GC>* button,
    Activation<GC>* activation,
    const Value<GC>& value
);

/// Gets whether the button is cached as a bitmap.
///
/// \param button The AVM1 button object
/// \param activation The current activation context
/// \return true if bitmap cached, false otherwise
template<typename GC>
Value<GC> cache_as_bitmap(
    display_object::Avm1Button<GC>* button,
    Activation<GC>* activation
);

/// Sets the bitmap cache preference for the button.
///
/// \param button The AVM1 button object
/// \param activation The current activation context
/// \param value The new cache preference value
/// \return Error if any
template<typename GC>
Value<GC> set_cache_as_bitmap(
    display_object::Avm1Button<GC>* button,
    Activation<GC>* activation,
    const Value<GC>& value
);

/// Gets the scale9Grid rectangle of the button.
///
/// \param button The AVM1 button object
/// \param activation The current activation context
/// \return The scaling grid rectangle, or Value::Undefined if not set
template<typename GC>
Value<GC> scale_9_grid(
    display_object::Avm1Button<GC>* button,
    Activation<GC>* activation
);

/// Sets the scale9Grid rectangle of the button.
///
/// \param button The AVM1 button object
/// \param activation The current activation context
/// \param value The rectangle object or Value::Undefined
/// \return Error if any
template<typename GC>
Value<GC> set_scale_9_grid(
    display_object::Avm1Button<GC>* button,
    Activation<GC>* activation,
    const Value<GC>& value
);

/// Gets the tab index of the button.
///
/// \param button The AVM1 button object
/// \param activation The current activation context
/// \return The tab index, or Value::Undefined if not set
template<typename GC>
Value<GC> tab_index(
    display_object::Avm1Button<GC>* button,
    Activation<GC>* activation
);

/// Sets the tab index of the button.
///
/// \param button The AVM1 button object
/// \param activation The current activation context
/// \param value The new tab index value
/// \return Error if any
template<typename GC>
Value<GC> set_tab_index(
    display_object::Avm1Button<GC>* button,
    Activation<GC>* activation,
    const Value<GC>& value
);

} // namespace button
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_BUTTON_H
