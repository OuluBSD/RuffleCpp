// C++ translation of core/src/avm1/globals/movie_clip_loader.rs
// MovieClipLoader impl

#ifndef RUFFLE_CORE_AVM1_GLOBALS_MOVIE_CLIP_LOADER_H
#define RUFFLE_CORE_AVM1_GLOBALS_MOVIE_CLIP_LOADER_H

#include <cstdint>
#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
namespace string {
    template<typename T> class AvmString;
}
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Error;
    template<typename T> class Object;
    template<typename T> class Value;
    struct DeclContext;
    struct StaticDeclarations;
    template<typename T> class SystemClass;
    namespace globals {
        namespace as_broadcaster {
            template<typename T> struct BroadcasterFunctions;
        }
    }
}
namespace backend {
    namespace navigator {
        class Request;
    }
}
namespace display_object {
    template<typename T> class TDisplayObject;
}
namespace loader {
    class MovieLoaderVMData;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {
namespace movie_clip_loader {

/// Property declarations for MovieClipLoader prototype
extern const StaticDeclarations PROTO_DECLS;

/// Create the MovieClipLoader class
///
/// \param context The declaration context for creating the class
/// \param super_proto The superclass prototype
/// \param broadcaster_fns The broadcaster functions for AsBroadcaster integration
/// \param array_proto The array prototype for the _listeners array
/// \return A SystemClass containing the created MovieClipLoader class
template<typename GC>
SystemClass<GC> create_class(
    DeclContext<GC>* context,
    Object<GC>* super_proto,
    globals::as_broadcaster::BroadcasterFunctions<GC> broadcaster_fns,
    Object<GC>* array_proto
);

/// MovieClipLoader constructor
///
/// Initializes the MovieClipLoader object with _listeners array.
///
/// \param activation The current activation context
/// \param this_obj The MovieClipLoader object being constructed
/// \param args Arguments for the constructor (unused)
/// \return Value::Undefined
template<typename GC>
Value<GC> constructor(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args
);

/// Implements MovieClipLoader.loadClip method
///
/// Loads a movie into a specified target clip or level.
///
/// \param activation The current activation context
/// \param this_obj The MovieClipLoader object
/// \param args Arguments (url, target)
/// \return true if load started, false otherwise
template<typename GC>
Value<GC> load_clip(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args
);

/// Implements MovieClipLoader.unloadClip method
///
/// Unloads a movie from a specified target clip or level.
///
/// \param activation The current activation context
/// \param this_obj The MovieClipLoader object (unused)
/// \param args Arguments (target)
/// \return true if unload started, false otherwise
template<typename GC>
Value<GC> unload_clip(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args
);

/// Implements MovieClipLoader.getProgress method
///
/// Gets the loading progress of a target clip or level.
///
/// \param activation The current activation context
/// \param this_obj The MovieClipLoader object (unused)
/// \param args Arguments (target)
/// \return An object with bytesLoaded and bytesTotal properties
template<typename GC>
Value<GC> get_progress(
    Activation<GC>* activation,
    Object<GC>* this_obj,
    const Value<GC>* args
);

} // namespace movie_clip_loader
} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_MOVIE_CLIP_LOADER_H
