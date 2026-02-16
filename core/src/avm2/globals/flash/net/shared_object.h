// C++ translation of core/src/avm2/globals/flash/net/shared_object.rs
// flash.net.SharedObject builtin/prototype

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_SHARED_OBJECT_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_SHARED_OBJECT_H

#include <vector>
#include <string>
#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class Value;
    template<typename T> class Error;
    template<typename T> class TObject;
    template<typename T> class AvmString;
    namespace object {
        template<typename T> class ScriptObject;
        template<typename T> class SharedObjectObject;
    }
}
namespace string {
    template<typename T> class AvmString;
}
}
}

// Forward declare flash_lso types
namespace flash_lso {
namespace types {
    enum class AMFVersion;
    class Lso;
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash {
namespace net {
namespace shared_object {

/// Helper function to create a new LSO from AVM2 object data
/// Serializes the data object to AMF3 format
template<typename GC>
flash_lso::types::Lso new_lso(
    Activation<GC>* activation,
    const std::string& name,
    typename avm2::object::ScriptObject<GC>* data
);

/// Implements SharedObject.getLocal
/// Retrieves or creates a local shared object
/// @param name The name of the shared object
/// @param localPath Optional path to restrict access
/// @param secure Whether the shared object requires HTTPS
/// @returns The shared object or null if invalid
template<typename GC>
Value<GC> get_local(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements SharedObject.data getter
/// Returns the data object for this shared object
template<typename GC>
Value<GC> get_data(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements SharedObject.flush
/// Flushes the shared object data to disk
/// @param minDiskSpace Minimum required disk space in bytes
/// @returns "flushed" on success, throws Error #2130 on failure
template<typename GC>
Value<GC> flush(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implementation helper for flush
/// Writes the LSO data to storage
template<typename GC>
typename avm2::AvmString<GC> flush_impl(
    Activation<GC>* activation,
    typename avm2::object::SharedObjectObject<GC>* shared_object,
    int32_t min_disk_space
);

/// Implements SharedObject.getSize
/// Returns the size of the shared object data in bytes
/// Returns 0 for empty shared objects
template<typename GC>
Value<GC> get_size(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements SharedObject.close
/// Closes the shared object connection
/// Currently a stub implementation
template<typename GC>
Value<GC> close(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements SharedObject.clear
/// Clears the shared object data and removes it from storage
template<typename GC>
Value<GC> clear(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements SharedObject.objectEncoding getter
/// Returns the object encoding version
/// Currently a stub returning 0
template<typename GC>
Value<GC> get_object_encoding(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

/// Implements SharedObject.objectEncoding setter
/// Sets the object encoding version
/// Currently a stub implementation
template<typename GC>
Value<GC> set_object_encoding(
    Activation<GC>* activation,
    Value<GC> this_value,
    const std::vector<Value<GC>>& args
);

} // namespace shared_object
} // namespace net
} // namespace flash
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_NET_SHARED_OBJECT_H
