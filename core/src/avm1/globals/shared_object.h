// C++ translation of core/src/avm1/globals/shared_object.rs
// SharedObject class for AVM1

#ifndef RUFFLE_CORE_AVM1_GLOBALS_SHARED_OBJECT_H
#define RUFFLE_CORE_AVM1_GLOBALS_SHARED_OBJECT_H

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
    struct SystemClass;
    enum class Attribute;
}
namespace string {
    template<typename T> class AvmString;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace shared_object {

/// Property declarations for SharedObject prototype
extern const StaticDeclarations PROTO_DECLS;

/// Property declarations for SharedObject constructor
extern const StaticDeclarations OBJECT_DECLS;

/// Create the SharedObject class
///
/// @param context The declaration context for creating the class
/// @param super_proto The prototype for the class
/// @return The created SystemClass
template<typename GC>
SystemClass<GC> create_class(DeclContext<GC>* context, Object<GC>* super_proto);

/// Implements SharedObject.deleteAll
///
/// Deletes all shared objects.
///
/// @param activation The current activation context
/// @param this_obj The SharedObject constructor
/// @param args Arguments (unused)
/// @return Value::Undefined (stub)
template<typename GC>
Value<GC> delete_all(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements SharedObject.getDiskUsage
///
/// Returns disk usage for shared objects.
///
/// @param activation The current activation context
/// @param this_obj The SharedObject constructor
/// @param args Arguments (unused)
/// @return Value::Undefined (stub)
template<typename GC>
Value<GC> get_disk_usage(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements SharedObject.getLocal
///
/// Gets or creates a local shared object.
///
/// @param activation The current activation context
/// @param this_obj The SharedObject constructor
/// @param args Arguments (name at index 0, optional localPath at index 1, optional secure at index 2)
/// @return SharedObject or Value::Null on error
template<typename GC>
Value<GC> get_local(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements SharedObject.getRemote
///
/// Gets a remote shared object.
///
/// @param activation The current activation context
/// @param this_obj The SharedObject constructor
/// @param args Arguments (unused)
/// @return Value::Undefined (stub)
template<typename GC>
Value<GC> get_remote(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements SharedObject.clear
///
/// Clears all data from the shared object.
///
/// @param activation The current activation context
/// @param this_obj The SharedObject instance
/// @param args Arguments (unused)
/// @return Value::Undefined
template<typename GC>
Value<GC> clear(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements SharedObject.close
///
/// Closes the shared object.
///
/// @param activation The current activation context
/// @param this_obj The SharedObject instance
/// @param args Arguments (unused)
/// @return Value::Undefined (stub)
template<typename GC>
Value<GC> close(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements SharedObject.connect
///
/// Connects to a remote shared object.
///
/// @param activation The current activation context
/// @param this_obj The SharedObject instance
/// @param args Arguments (unused)
/// @return Value::Undefined (stub)
template<typename GC>
Value<GC> connect(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements SharedObject.flush
///
/// Flushes the shared object to disk.
///
/// @param activation The current activation context
/// @param this_obj The SharedObject instance
/// @param args Arguments (unused)
/// @return Boolean indicating success
template<typename GC>
Value<GC> flush(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements SharedObject.getSize
///
/// Returns the size of the shared object in bytes.
///
/// @param activation The current activation context
/// @param this_obj The SharedObject instance
/// @param args Arguments (unused)
/// @return Size in bytes as integer
template<typename GC>
Value<GC> get_size(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements SharedObject.send
///
/// Sends data to a remote shared object.
///
/// @param activation The current activation context
/// @param this_obj The SharedObject instance
/// @param args Arguments (unused)
/// @return Value::Undefined (stub)
template<typename GC>
Value<GC> send(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements SharedObject.setFps
///
/// Sets the FPS for shared object synchronization.
///
/// @param activation The current activation context
/// @param this_obj The SharedObject instance
/// @param args Arguments (unused)
/// @return Value::Undefined (stub)
template<typename GC>
Value<GC> set_fps(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements SharedObject.onStatus
///
/// Event handler for status messages.
///
/// @param activation The current activation context
/// @param this_obj The SharedObject instance
/// @param args Arguments (unused)
/// @return Value::Undefined (stub)
template<typename GC>
Value<GC> on_status(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Implements SharedObject.onSync
///
/// Event handler for sync messages.
///
/// @param activation The current activation context
/// @param this_obj The SharedObject instance
/// @param args Arguments (unused)
/// @return Value::Undefined (stub)
template<typename GC>
Value<GC> on_sync(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// SharedObject constructor
///
/// @param activation The current activation context
/// @param this_obj The SharedObject object being constructed
/// @param args Arguments (unused)
/// @return Value::Undefined
template<typename GC>
Value<GC> constructor(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args,
    size_t arg_count
);

/// Serialize an AVM1 value to AMF format for storage
///
/// @param activation The current activation context
/// @param value The value to serialize
/// @return Serialized AMF value
template<typename GC>
struct AmfValue serialize(Activation<GC>* activation, Value<GC> value);

} // namespace shared_object
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_SHARED_OBJECT_H
