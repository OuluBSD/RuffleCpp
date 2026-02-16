// C++ translation of core/src/avm1/object_reference.rs
// MovieClipReference for AVM1 paths

#ifndef RUFFLE_CORE_AVM1_OBJECT_REFERENCE_H
#define RUFFLE_CORE_AVM1_OBJECT_REFERENCE_H

#include <cstdint>
#include <vector>
#include <optional>
#include <memory>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename T> class AvmString;
    template<typename T> class WString;
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Object;
    template<typename T> class ObjectWeak;
    template<typename T> class Value;
namespace object {
    template<typename T> class ObjectWeak;
}
}
namespace display_object {
    template<typename T> class DisplayObject;
    template<typename T> class TDisplayObject;
    template<typename T> class TDisplayObjectContainer;
    template<typename T> class MovieClip;
}
namespace gc_arena {
    template<typename T> class Gc;
    template<typename T> class Lock;
    class Mutation;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace object_reference {

/// MovieClipPath represents a parsed path to a movie clip
///
/// Stores the level and path segments for efficient path resolution.
template<typename GC>
class MovieClipPath {
public:
    /// Create a MovieClipPath from a string path
    ///
    /// \param mc The mutation context for GC allocations
    /// \param path The path string to parse
    /// \return A new MovieClipPath instance
    static MovieClipPath new_from_path(
        gc_arena::Mutation* mc,
        const WString<GC>& path
    );

    /// Get the level this path starts from
    ///
    /// \return The level number
    int32_t level() const { return level_; }

    /// Get the path segments
    ///
    /// \return Vector of path segment strings
    const std::vector<AvmString<GC>>& path_segments() const { return path_segments_; }

    /// Get the full original path string
    ///
    /// \return The full path as a string
    const AvmString<GC>& full_path() const { return full_path_; }

private:
    MovieClipPath() = default;

    int32_t level_;
    std::vector<AvmString<GC>> path_segments_;
    AvmString<GC> full_path_;
};

/// MovieClipReferenceData holds the data for a movie clip reference
template<typename GC>
struct MovieClipReferenceData {
    /// The path to the target clip
    MovieClipPath<GC> path;

    /// A weak reference to the cached target object
    /// Used for fast-path resolution when possible
    std::optional<object::ObjectWeak<GC>> cached_object;
};

/// MovieClipReference represents a reference to a movie clip in AVM1
///
/// This consists of a string path which will be resolved to a target value when used.
/// It also handles caching to maintain performance.
template<typename GC>
class MovieClipReference {
public:
    /// Try to create a MovieClipReference from a stage object
    ///
    /// \param activation The current activation context
    /// \param object The object to create a reference from
    /// \return Optional MovieClipReference if successful
    static std::optional<MovieClipReference> try_from_stage_object(
        Activation<GC>* activation,
        Object<GC>* object
    );

    /// Resolve this reference to an object
    ///
    /// First checks the cache, then falls back to manual path walking.
    ///
    /// \param activation The current activation context
    /// \return Tuple of (is_cached, object, display_object) if found
    std::optional<std::tuple<bool, Object<GC>*, display_object::DisplayObject<GC>*>>
    resolve_reference(Activation<GC>* activation);

    /// Convert this reference to an Object
    ///
    /// \param activation The current activation context
    /// \return The resolved object, or nullopt if not found
    std::optional<Object<GC>*> coerce_to_object(Activation<GC>* activation);

    /// Convert this reference to a String
    ///
    /// \param activation The current activation context
    /// \return The path string of the resolved object
    AvmString<GC> coerce_to_string(Activation<GC>* activation);

    /// Get the path used for this reference
    ///
    /// \return The original path string
    AvmString<GC> path() const;

private:
    MovieClipReference(gc_arena::Gc<MovieClipReferenceData<GC>> data)
        : data_(data) {}

    /// Handle the logic of SWFv5 DisplayObjects
    ///
    /// \param activation The current activation context
    /// \param display_object The display object to process
    /// \return Processed display object
    static std::optional<display_object::DisplayObject<GC>*> process_swf5_references(
        Activation<GC>* activation,
        display_object::DisplayObject<GC>* display_object
    );

    gc_arena::Gc<MovieClipReferenceData<GC>> data_;
};

} // namespace object_reference
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_OBJECT_REFERENCE_H
