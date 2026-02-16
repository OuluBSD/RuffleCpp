// C++ translation of core/src/avm1/globals/system.rs
// System object for AVM1

#ifndef RUFFLE_CORE_AVM1_GLOBALS_SYSTEM_H
#define RUFFLE_CORE_AVM1_GLOBALS_SYSTEM_H

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
namespace system {

/// Property declarations for System object
extern const StaticDeclarations OBJECT_DECLS;

/// Create the System object
///
/// \param context The declaration context for creating the object
/// \return The created System object
template<typename GC>
Object<GC> create(DeclContext<GC>* context);

/// SettingsPanel enum for showSettings method
enum class SettingsPanel : uint8_t {
    Privacy = 0,
    LocalStorage = 1,
    Microphone = 2,
    Camera = 3
};

/// Implements System.setClipboard method
///
/// Sets the system clipboard content.
///
/// \param activation The current activation context
/// \param this_obj The System object
/// \param args Arguments (clipboard content string)
/// \return Value::Undefined
template<typename GC>
Value<GC> set_clipboard(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements System.showSettings method
///
/// Shows the Flash Player settings dialog.
///
/// \param activation The current activation context
/// \param this_obj The System object
/// \param args Arguments (optional panel index)
/// \return Value::Undefined
template<typename GC>
Value<GC> show_settings(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements System.useCodepage property setter
///
/// \param activation The current activation context
/// \param this_obj The System object
/// \param args Arguments (boolean value)
/// \return Value::Undefined
template<typename GC>
Value<GC> set_use_code_page(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements System.useCodepage property getter
///
/// \param activation The current activation context
/// \param this_obj The System object
/// \param args Arguments (unused)
/// \return Boolean value indicating useCodepage setting
template<typename GC>
Value<GC> get_use_code_page(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements System.exactSettings property setter
///
/// \param activation The current activation context
/// \param this_obj The System object
/// \param args Arguments (boolean value)
/// \return Value::Undefined
template<typename GC>
Value<GC> set_exact_settings(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements System.exactSettings property getter
///
/// \param activation The current activation context
/// \param this_obj The System object
/// \param args Arguments (unused)
/// \return Boolean value indicating exactSettings setting
template<typename GC>
Value<GC> get_exact_settings(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements System.onStatus method
///
/// Event handler for status messages.
///
/// \param activation The current activation context
/// \param this_obj The System object
/// \param args Arguments (unused)
/// \return Value::Undefined
template<typename GC>
Value<GC> on_status(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

} // namespace system
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_SYSTEM_H
