// C++ translation of core/src/debug_ui/handle.rs
// Handle types for debug UI - weak references to GC-managed objects

#ifndef RUFFLE_CORE_DEBUG_UI_HANDLE_H
#define RUFFLE_CORE_DEBUG_UI_HANDLE_H

#include <cstdint>
#include <cstddef>
#include <functional>

// Forward declarations for GC arena
namespace gc_arena {
    template<typename T>
    class DynamicRoot;

    template<typename T>
    class DynamicRootSet;

    template<typename T>
    class Gc;

    template<typename T>
    struct Rootable;
}

namespace ruffle {
namespace core {

// Forward declarations
template<typename GC>
class UpdateContext;

namespace display_object {
    class DisplayObjectPtr;
    template<typename GC>
    class DisplayObject;
}

namespace avm1 {
    class ObjectPtr;
    template<typename GC>
    class Object;
}

namespace avm2 {
    class DomainPtr;
    template<typename GC>
    class Domain;
    namespace object {
        class ObjectPtr;
        template<typename GC>
        class TObject;
        template<typename GC>
        using Object = TObject<GC>;
    }
}

} // namespace core
} // namespace ruffle

namespace ruffle {
namespace core {
namespace debug_ui {

/// Handle to a DisplayObject for debug UI
/// Uses a weak reference pattern to avoid preventing garbage collection
class DisplayObjectHandle {
private:
    gc_arena::DynamicRoot<gc_arena::Rootable<display_object::DisplayObject</*'gc*/void>*>> root_;
    const display_object::DisplayObjectPtr* ptr_;

public:
    /// Create a new handle to a display object
    /// @param context The update context for GC access
    /// @param object The display object to reference
    template<typename GC>
    static DisplayObjectHandle create(
        UpdateContext<GC>* context,
        display_object::DisplayObject<GC> object
    );

    /// Fetch the display object from the handle
    /// @param dynamic_root_set The root set for GC access
    /// @return The display object
    template<typename GC>
    display_object::DisplayObject<GC> fetch(gc_arena::DynamicRootSet<GC> dynamic_root_set) const;

    /// Get the raw pointer
    const display_object::DisplayObjectPtr* as_ptr() const { return ptr_; }

    /// Equality comparison - compares pointers
    bool operator==(const DisplayObjectHandle& other) const {
        return ptr_ == other.ptr_;
    }

    bool operator!=(const DisplayObjectHandle& other) const {
        return !(*this == other);
    }

    /// Hash function for use in hash maps
    size_t hash() const {
        return std::hash<const void*>{}(ptr_);
    }
};

/// Handle to an AVM1 Object for debug UI
/// Uses a weak reference pattern to avoid preventing garbage collection
class AVM1ObjectHandle {
private:
    gc_arena::DynamicRoot<gc_arena::Rootable<avm1::Object</*'gc*/void>*>> root_;
    const avm1::ObjectPtr* ptr_;

public:
    /// Create a new handle to an AVM1 object
    /// @param context The update context for GC access
    /// @param object The AVM1 object to reference
    template<typename GC>
    static AVM1ObjectHandle create(
        UpdateContext<GC>* context,
        avm1::Object<GC> object
    );

    /// Fetch the AVM1 object from the handle
    /// @param dynamic_root_set The root set for GC access
    /// @return The AVM1 object
    template<typename GC>
    avm1::Object<GC> fetch(gc_arena::DynamicRootSet<GC> dynamic_root_set) const;

    /// Get the raw pointer
    const avm1::ObjectPtr* as_ptr() const { return ptr_; }

    /// Equality comparison - compares pointers
    bool operator==(const AVM1ObjectHandle& other) const {
        return ptr_ == other.ptr_;
    }

    bool operator!=(const AVM1ObjectHandle& other) const {
        return !(*this == other);
    }

    /// Hash function for use in hash maps
    size_t hash() const {
        return std::hash<const void*>{}(ptr_);
    }
};

/// Handle to an AVM2 Object for debug UI
/// Uses a weak reference pattern to avoid preventing garbage collection
class AVM2ObjectHandle {
private:
    gc_arena::DynamicRoot<gc_arena::Rootable<avm2::object::Object</*'gc*/void>*>> root_;
    const avm2::object::ObjectPtr* ptr_;

public:
    /// Create a new handle to an AVM2 object
    /// @param context The update context for GC access
    /// @param object The AVM2 object to reference
    template<typename GC>
    static AVM2ObjectHandle create(
        UpdateContext<GC>* context,
        avm2::object::Object<GC> object
    );

    /// Fetch the AVM2 object from the handle
    /// @param dynamic_root_set The root set for GC access
    /// @return The AVM2 object
    template<typename GC>
    avm2::object::Object<GC> fetch(gc_arena::DynamicRootSet<GC> dynamic_root_set) const;

    /// Get the raw pointer
    const avm2::object::ObjectPtr* as_ptr() const { return ptr_; }

    /// Equality comparison - compares pointers
    bool operator==(const AVM2ObjectHandle& other) const {
        return ptr_ == other.ptr_;
    }

    bool operator!=(const AVM2ObjectHandle& other) const {
        return !(*this == other);
    }

    /// Hash function for use in hash maps
    size_t hash() const {
        return std::hash<const void*>{}(ptr_);
    }
};

/// Handle to an AVM2 Domain for debug UI
/// Uses a weak reference pattern to avoid preventing garbage collection
class DomainHandle {
private:
    gc_arena::DynamicRoot<gc_arena::Rootable<avm2::Domain</*'gc*/void>*>> root_;
    const avm2::DomainPtr* ptr_;

public:
    /// Create a new handle to a domain
    /// @param context The update context for GC access
    /// @param domain The domain to reference
    template<typename GC>
    static DomainHandle create(
        UpdateContext<GC>* context,
        avm2::Domain<GC> domain
    );

    /// Fetch the domain from the handle
    /// @param dynamic_root_set The root set for GC access
    /// @return The domain
    template<typename GC>
    avm2::Domain<GC> fetch(gc_arena::DynamicRootSet<GC> dynamic_root_set) const;

    /// Get the raw pointer
    const avm2::DomainPtr* as_ptr() const { return ptr_; }

    /// Equality comparison - compares pointers
    bool operator==(const DomainHandle& other) const {
        return ptr_ == other.ptr_;
    }

    bool operator!=(const DomainHandle& other) const {
        return !(*this == other);
    }

    /// Hash function for use in hash maps
    size_t hash() const {
        return std::hash<const void*>{}(ptr_);
    }
};

} // namespace debug_ui
} // namespace core
} // namespace ruffle

// Hash function specializations for use in unordered_map/unordered_set
namespace std {

template<>
struct hash<ruffle::core::debug_ui::DisplayObjectHandle> {
    size_t operator()(const ruffle::core::debug_ui::DisplayObjectHandle& h) const {
        return h.hash();
    }
};

template<>
struct hash<ruffle::core::debug_ui::AVM1ObjectHandle> {
    size_t operator()(const ruffle::core::debug_ui::AVM1ObjectHandle& h) const {
        return h.hash();
    }
};

template<>
struct hash<ruffle::core::debug_ui::AVM2ObjectHandle> {
    size_t operator()(const ruffle::core::debug_ui::AVM2ObjectHandle& h) const {
        return h.hash();
    }
};

template<>
struct hash<ruffle::core::debug_ui::DomainHandle> {
    size_t operator()(const ruffle::core::debug_ui::DomainHandle& h) const {
        return h.hash();
    }
};

} // namespace std

#endif // RUFFLE_CORE_DEBUG_UI_HANDLE_H
