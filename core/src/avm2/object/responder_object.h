// C++ translation of core/src/avm2/object/responder_object.rs
//! Responder object for network callbacks

#ifndef RUFFLE_CORE_AVM2_OBJECT_RESPONDER_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_RESPONDER_OBJECT_H

#include <optional>
#include <functional>

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
    template<typename T> class FunctionObject;
    template<typename T> class Object;
    template<typename T> class Mutation;
    template<typename T> class Error;
    template<typename T> class Value;
}
namespace net_connection {
    enum class ResponderCallback;
}
}
}

// For AMF value
namespace flash_lso {
namespace types {
    class Value;
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Weak reference to ResponderObject data
template<typename GC>
class ResponderObjectWeak;

/// Data structure for ResponderObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class ResponderObjectData {
public:
    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    /// Method to call with any result
    std::optional<FunctionObject<GC>> result;

    /// Method to call with status info (likely errors)
    std::optional<FunctionObject<GC>> status;

    // Friend classes for accessing private members
    friend class ResponderObject<GC>;
    friend class ResponderObjectWeak<GC>;
};

/// ResponderObject - AVM2 Responder object representation
/// Used for handling network operation callbacks (result/status)
template<typename GC>
class ResponderObject : public TObject<GC> {
public:
    using DataType = ResponderObjectData<GC>;
    using WeakType = ResponderObjectWeak<GC>;

    ResponderObject() = default;
    explicit ResponderObject(Gc<GC, DataType> data) : data_(data) {}

    /// Get the base script object data (for TObject trait)
    Gc<GC, ScriptObjectData<GC>> gc_base() const override {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base;
    }

    /// Get the result callback function
    std::optional<FunctionObject<GC>> result() const {
        return data_->result;
    }

    /// Get the status callback function
    std::optional<FunctionObject<GC>> status() const {
        return data_->status;
    }

    /// Set the callback functions
    void set_callbacks(
        Mutation<GC>* mc,
        std::optional<FunctionObject<GC>> result,
        std::optional<FunctionObject<GC>> status
    ) {
        auto write = data_->write(mc);
        write->result = std::move(result);
        write->status = std::move(status);
    }

    /// Send a callback to the appropriate handler
    void send_callback(
        Activation<GC>* activation,
        net_connection::ResponderCallback callback,
        const flash_lso::types::Value& message
    ) {
        auto function = (callback == net_connection::ResponderCallback::Result)
            ? data_->result
            : data_->status;

        if (function.has_value()) {
            // Note: This would need proper AMF deserialization
            // auto value = amf::deserialize_value(activation, message);
            // function.value().call(activation, *this, {value});
        }
    }

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to ResponderObject
template<typename GC>
class ResponderObjectWeak {
public:
    ResponderObjectWeak() = default;
    explicit ResponderObjectWeak(GcWeak<GC, ResponderObjectData<GC>> data)
        : data_(data) {}

private:
    GcWeak<GC, ResponderObjectData<GC>> data_;
};

/// Allocator function for ResponderObject
/// Creates a new ResponderObject instance
template<typename GC>
Object<GC> responder_allocator(
    ClassObject<GC> class_obj,
    Activation<GC>* activation
) {
    auto base = ScriptObjectData<GC>::create(class_obj);
    auto data = Gc<GC, DataType>::create(activation->gc(), DataType{base});
    return Object<GC>(ResponderObject<GC>(data));
}

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_OBJECT_RESPONDER_OBJECT_H
