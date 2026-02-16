// C++ translation of core/src/avm2/object/message_channel_object.rs
//! Object representation for MessageChannel objects

#ifndef RUFFLE_CORE_AVM2_OBJECT_MESSAGE_CHANNEL_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_MESSAGE_CHANNEL_OBJECT_H

#include <memory>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class ScriptObjectData;
    template<typename T> class TObject;
    template<typename T> class Gc;
    template<typename T> class GcWeak;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Weak reference to MessageChannelObject data
template<typename GC>
class MessageChannelObjectWeak;

/// Data structure for MessageChannelObject
template<typename GC>
class MessageChannelObjectData {
    // Base script object - using prefix field pattern
    ScriptObjectData<GC> base_;
    
    // Friend classes for accessing private members
    friend class MessageChannelObject<GC>;
    friend class MessageChannelObjectWeak<GC>;
};

/// MessageChannelObject - AVM2 MessageChannel object representation
template<typename GC>
class MessageChannelObject {
public:
    using DataType = MessageChannelObjectData<GC>;
    using WeakType = MessageChannelObjectWeak<GC>;

    MessageChannelObject() = default;
    explicit MessageChannelObject(Gc<GC, DataType> data) : data_(data) {}

    /// Create a new MessageChannelObject
    static MessageChannelObject create(Activation<GC>* activation);

    /// Get the base script object data
    Gc<GC, ScriptObjectData<GC>> gc_base() const {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base_;
    }

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to MessageChannelObject
template<typename GC>
class MessageChannelObjectWeak {
public:
    MessageChannelObjectWeak() = default;
    explicit MessageChannelObjectWeak(GcWeak<GC, MessageChannelObjectData<GC>> data) 
        : data_(data) {}

private:
    GcWeak<GC, MessageChannelObjectData<GC>> data_;
};

// Template method implementations
template<typename GC>
MessageChannelObject<GC> MessageChannelObject<GC>::create(Activation<GC>* activation) {
    // Get the MessageChannel class from activation
    // In real implementation: auto class_obj = activation->avm2()->classes()->messagechannel;
    auto base = ScriptObjectData<GC>::create(/* class_obj */);
    auto data = Gc<GC, DataType>::create(activation->gc(), DataType{base});
    return MessageChannelObject<GC>(data);
}

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_OBJECT_MESSAGE_CHANNEL_OBJECT_H
