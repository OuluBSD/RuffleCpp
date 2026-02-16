// C++ translation of core/src/avm2/object/netstream_object.rs
//! Object representation for NetStreams

#ifndef RUFFLE_CORE_AVM2_OBJECT_NETSTREAM_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_NETSTREAM_OBJECT_H

#include <memory>
#include <cstdint>
#include <format>

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
    template<typename T> class Value;
}
namespace streams {
    template<typename GC> class NetStream;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Weak reference to NetStreamObject data
template<typename GC>
class NetStreamObjectWeak;

/// Data structure for NetStreamObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class NetStreamObjectData {
public:
    NetStreamObjectData() = default;

    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    /// NetStream handle
    streams::NetStream<GC> ns;

    // Friend classes for accessing private members
    friend class NetStreamObject<GC>;
    friend class NetStreamObjectWeak<GC>;
};

/// NetStreamObject - AVM2 NetStream object representation
/// Represents a NetStream instance in ActionScript
template<typename GC>
class NetStreamObject {
public:
    using DataType = NetStreamObjectData<GC>;
    using WeakType = NetStreamObjectWeak<GC>;

    NetStreamObject() = default;
    explicit NetStreamObject(Gc<GC, DataType> data) : data_(data) {}

    /// Get the NetStream
    streams::NetStream<GC> netstream() const {
        return data_->ns;
    }

    /// Get the base script object data (for TObject trait)
    Gc<GC, ScriptObjectData<GC>> gc_base() const {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base;
    }

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to NetStreamObject
template<typename GC>
class NetStreamObjectWeak {
public:
    NetStreamObjectWeak() = default;
    explicit NetStreamObjectWeak(GcWeak<GC, NetStreamObjectData<GC>> data)
        : data_(data) {}

private:
    GcWeak<GC, NetStreamObjectData<GC>> data_;
};

/// NetStream object allocator function
///
/// Creates a new NetStream object and initializes it
///
/// @param class_obj The class object for NetStream
/// @param activation The current activation context
/// @return Result containing the new NetStreamObject or error
template<typename GC>
Object<GC> netstream_allocator(
    ClassObject<GC> class_obj,
    Activation<GC>* activation);

// Template method implementations

template<typename GC>
Object<GC> netstream_allocator(
    ClassObject<GC> class_obj,
    Activation<GC>* activation) {
    auto base = ScriptObjectData<GC>::create(class_obj);

    auto ns = streams::NetStream<GC>::new_avm2(activation->gc());
    auto data = Gc<GC, DataType>::create(
        activation->gc(),
        DataType{base, ns}
    );
    auto this_obj = NetStreamObject<GC>(data);

    ns.set_avm2_object(activation->gc(), this_obj);
    ns.set_client(activation->gc(), Value<GC>::from_object(this_obj));

    return Object<GC>(this_obj);
}

} // namespace avm2
} // namespace core
} // namespace ruffle

// Debug formatter specialization
template<typename GC>
struct std::formatter<ruffle::core::avm2::NetStreamObject<GC>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const ruffle::core::avm2::NetStreamObject<GC>& obj, FormatContext& ctx) {
        return std::format_to(ctx.out(), "NetStreamObject(ptr={})",
            static_cast<const void*>(obj.data_.get()));
    }
};

#endif // RUFFLE_CORE_AVM2_OBJECT_NETSTREAM_OBJECT_H
