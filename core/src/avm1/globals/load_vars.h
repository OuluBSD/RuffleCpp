// C++ translation of core/src/avm1/globals/load_vars.rs
// LoadVars object for AVM1 HTTP requests

#ifndef RUFFLE_CORE_AVM1_GLOBALS_LOAD_VARS_H
#define RUFFLE_CORE_AVM1_GLOBALS_LOAD_VARS_H

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
namespace backend {
namespace navigator {
    enum class NavigationMethod;
    struct Request;
}
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace load_vars {

/// Property declarations for LoadVars prototype
extern const StaticDeclarations PROTO_DECLS;

/// Create the LoadVars class
///
/// \param context The declaration context for creating the class
/// \param super_proto The prototype object for the superclass
/// \return A SystemClass containing the created LoadVars class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto
);

/// Implements LoadVars.addRequestHeader method
///
/// Adds a request header (stub - not implemented).
///
/// \param activation The current activation context
/// \param this_obj The LoadVars object
/// \param args Arguments (header name and value)
/// \param arg_count Number of arguments
/// \return Value::Undefined
template<typename GC>
Value<GC> add_request_header(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements LoadVars.decode method
///
/// Decodes a query string and sets properties on this object.
///
/// \param activation The current activation context
/// \param this_obj The LoadVars object
/// \param args Arguments (query string to decode)
/// \param arg_count Number of arguments
/// \return Value::Undefined
template<typename GC>
Value<GC> decode(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements LoadVars.getBytesLoaded method
///
/// Gets the number of bytes loaded.
///
/// \param activation The current activation context
/// \param this_obj The LoadVars object
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return Number of bytes loaded
template<typename GC>
Value<GC> get_bytes_loaded(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements LoadVars.getBytesTotal method
///
/// Gets the total number of bytes to load.
///
/// \param activation The current activation context
/// \param this_obj The LoadVars object
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return Total number of bytes
template<typename GC>
Value<GC> get_bytes_total(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements LoadVars.load method
///
/// Loads variables from a URL.
///
/// \param activation The current activation context
/// \param this_obj The LoadVars object
/// \param args Arguments (URL to load from)
/// \param arg_count Number of arguments
/// \return true if load started successfully
template<typename GC>
Value<GC> load(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements LoadVars.onData method
///
/// Default handler for data load completion.
///
/// \param activation The current activation context
/// \param this_obj The LoadVars object
/// \param args Arguments (loaded data or false on failure)
/// \param arg_count Number of arguments
/// \return Value::Undefined
template<typename GC>
Value<GC> on_data(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements LoadVars.onLoad method
///
/// Default handler for load completion.
///
/// \param activation The current activation context
/// \param this_obj The LoadVars object
/// \param args Arguments (success flag)
/// \param arg_count Number of arguments
/// \return Value::Undefined
template<typename GC>
Value<GC> on_load(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements LoadVars.send method
///
/// Sends variables to a URL and navigates to the response.
///
/// \param activation The current activation context
/// \param this_obj The LoadVars object
/// \param args Arguments (URL, window, method)
/// \param arg_count Number of arguments
/// \return true if send started successfully
template<typename GC>
Value<GC> send(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements LoadVars.sendAndLoad method
///
/// Sends variables to a URL and loads the response into a target object.
///
/// \param activation The current activation context
/// \param this_obj The LoadVars object
/// \param args Arguments (URL, target object, method)
/// \param arg_count Number of arguments
/// \return true if send started successfully
template<typename GC>
Value<GC> send_and_load(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements LoadVars.toString method
///
/// Converts the LoadVars object to a query string.
///
/// \param activation The current activation context
/// \param this_obj The LoadVars object
/// \param args Arguments (unused)
/// \param arg_count Number of arguments
/// \return Query string representation
template<typename GC>
Value<GC> to_string(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Spawn a future to fetch form data into a LoadVars object
///
/// \param activation The current activation context
/// \param loader_object The LoadVars object to load data into
/// \param url The URL to fetch from
/// \param send_object Optional object to send data from, with method
/// \return Value::Undefined
template<typename GC>
Value<GC> spawn_load_var_fetch(
    Activation<GC>* activation,
    Object<GC>* loader_object,
    const char* url,
    const std::pair<Object<GC>*, backend::navigator::NavigationMethod>* send_object
);

} // namespace load_vars
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_LOAD_VARS_H
