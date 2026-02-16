// C++ translation of core/src/avm1/globals/as_broadcaster.rs
// ActionScript Broadcaster (AsBroadcaster)

#ifndef RUFFLE_CORE_AVM1_GLOBALS_AS_BROADCASTER_H
#define RUFFLE_CORE_AVM1_GLOBALS_AS_BROADCASTER_H

#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename T> class AvmString;
    class StringContext;
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Error;
    template<typename T> class Object;
    template<typename T> class Value;
    struct DeclContext;
    struct StaticDeclarations;
    template<typename T> class SystemClass;
    enum class ExecutionReason;
namespace parameters {
    template<typename T> struct ParametersExt;
    enum class UndefinedAs;
}
namespace property {
    enum class Attribute;
}
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace as_broadcaster {

/// Property declarations for AsBroadcaster object
extern const StaticDeclarations OBJECT_DECLS;

/// BroadcasterFunctions holds the broadcaster method objects
template<typename GC>
struct BroadcasterFunctions {
    Object<GC>* add_listener;
    Object<GC>* remove_listener;
    Object<GC>* broadcast_message;
};

/// Create the AsBroadcaster class
///
/// \param context The declaration context for creating the class
/// \param super_proto The superclass prototype
/// \return Pair of (BroadcasterFunctions, SystemClass)
template<typename GC>
std::pair<BroadcasterFunctions<GC>, SystemClass<GC>> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// Initialize a broadcaster object
///
/// Adds _listeners array and broadcaster methods to the object.
///
/// \param context The string context for GC allocations
/// \param broadcaster The object to initialize as a broadcaster
/// \param functions The broadcaster functions to add
/// \param array_proto The array prototype for the _listeners array
template<typename GC>
void initialize_internal(
    StringContext* context,
    Object<GC>* broadcaster,
    BroadcasterFunctions<GC> functions,
    Object<GC>* array_proto
);

/// Implements AsBroadcaster.addListener function
///
/// Adds a listener to the broadcaster's listener list.
///
/// \param activation The current activation context
/// \param this_obj The broadcaster object
/// \param args Arguments (listener object to add)
/// \param arg_count Number of arguments
/// \return true if listener was added
template<typename GC>
Value<GC> add_listener(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements AsBroadcaster.removeListener function
///
/// Removes a listener from the broadcaster's listener list.
///
/// \param activation The current activation context
/// \param this_obj The broadcaster object
/// \param args Arguments (listener object to remove)
/// \param arg_count Number of arguments
/// \return true if listener was removed
template<typename GC>
Value<GC> remove_listener(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements AsBroadcaster.broadcastMessage function
///
/// Broadcasts a message to all listeners.
///
/// \param activation The current activation context
/// \param this_obj The broadcaster object
/// \param args Arguments (event name, followed by event arguments)
/// \param arg_count Number of arguments
/// \return true if message was broadcast
template<typename GC>
Value<GC> broadcast_message(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Internal broadcast function
///
/// Broadcasts a method call to all listeners.
///
/// \param this_obj The broadcaster object
/// \param call_args Arguments to pass to the method
/// \param method_name The method name to call
/// \param activation The current activation context
/// \return true if any listeners were called
template<typename GC>
bool broadcast_internal(
    Object<GC>* this_obj,
    const Value<GC>* call_args,
    size_t call_arg_count,
    AvmString<GC> method_name,
    Activation<GC>* activation
);

/// Implements AsBroadcaster.initialize method
///
/// Initializes an object as a broadcaster.
///
/// \param activation The current activation context
/// \param this_obj The AsBroadcaster object
/// \param args Arguments (object to initialize)
/// \param arg_count Number of arguments
/// \return Value::Undefined
template<typename GC>
Value<GC> initialize(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

} // namespace as_broadcaster
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_AS_BROADCASTER_H
