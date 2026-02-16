// C++ translation of core/src/avm2/object/net_connection_object.rs
//! Object representation for NetConnection

#ifndef RUFFLE_CORE_AVM2_OBJECT_NET_CONNECTION_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_NET_CONNECTION_OBJECT_H

#include <memory>
#include <optional>
#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class ScriptObjectData;
    template<typename T> class TObject;
    template<typename T> class Gc;
    template<typename T> class GcWeak;
    template<typename T> class ClassObject;
    template<typename T> class Object;
    template<typename T> class Error;
}
namespace net_connection {
    class NetConnectionHandle;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Weak reference to NetConnectionObject data
template<typename GC>
class NetConnectionObjectWeak;

/// Data structure for NetConnectionObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class NetConnectionObjectData {
public:
    NetConnectionObjectData() = default;

    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    /// NetConnection handle
    std::optional<net_connection::NetConnectionHandle> handle;

    // Friend classes for accessing private members
    friend class NetConnectionObject<GC>;
    friend class NetConnectionObjectWeak<GC>;
};

/// NetConnectionObject - AVM2 NetConnection object representation
/// Represents a NetConnection instance in ActionScript
template<typename GC>
class NetConnectionObject {
public:
    using DataType = NetConnectionObjectData<GC>;
    using WeakType = NetConnectionObjectWeak<GC>;

    NetConnectionObject() = default;
    explicit NetConnectionObject(Gc<GC, DataType> data) : data_(data) {}

    /// Get the NetConnection handle
    std::optional<net_connection::NetConnectionHandle> handle() const {
        return data_->handle;
    }

    /// Set the NetConnection handle
    /// @param handle The new handle value
    /// @return The previous handle value
    std::optional<net_connection::NetConnectionHandle> set_handle(
        std::optional<net_connection::NetConnectionHandle> handle) {
        auto old = data_->handle;
        data_->handle = handle;
        return old;
    }

    /// Get the base script object data (for TObject trait)
    Gc<GC, ScriptObjectData<GC>> gc_base() const {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base;
    }

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to NetConnectionObject
template<typename GC>
class NetConnectionObjectWeak {
public:
    NetConnectionObjectWeak() = default;
    explicit NetConnectionObjectWeak(GcWeak<GC, NetConnectionObjectData<GC>> data)
        : data_(data) {}

private:
    GcWeak<GC, NetConnectionObjectData<GC>> data_;
};

/// NetConnection object allocator function
/// @param class_obj The class object for NetConnection
/// @param activation The current activation context
/// @return Result containing the new NetConnectionObject or error
template<typename GC>
Object<GC> net_connection_allocator(
    ClassObject<GC> class_obj,
    Activation<GC>* activation);

// Template method implementations
template<typename GC>
Object<GC> net_connection_allocator(
    ClassObject<GC> class_obj,
    Activation<GC>* activation) {
    auto base = ScriptObjectData<GC>::create(class_obj);
    auto data = Gc<GC, DataType>::create(activation->gc(), DataType{base});
    return Object<GC>(NetConnectionObject<GC>(data));
}

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_OBJECT_NET_CONNECTION_OBJECT_H
