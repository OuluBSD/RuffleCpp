// C++ translation of core/src/avm2/object/shared_object_object.rs
//! Object representation for SharedObjects

#ifndef RUFFLE_CORE_AVM2_OBJECT_SHARED_OBJECT_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_SHARED_OBJECT_OBJECT_H

#include <memory>
#include <string>
#include <optional>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Activation;
    template<typename T> class ScriptObjectData;
    template<typename T> class TObject;
    template<typename T> class Gc;
    template<typename T> class GcWeak;
    template<typename T> class Object;
    template<typename T> class Mutation;
    template<typename T> class UpdateContext;
    template<typename T> class AvmString;
    template<typename T> class Value;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Weak reference to SharedObjectObject data
template<typename GC>
class SharedObjectObjectWeak;

/// Data structure for SharedObjectObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class SharedObjectObjectData {
public:
    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    /// Get the SharedObject data
    Object<GC> data() const { return data_; }

    /// Set the SharedObject data
    void set_data(Object<GC> value) { data_ = value; }

    /// Get the name of this SharedObject
    const std::string& name() const { return name_; }

    /// Set the name of this SharedObject
    void set_name(const std::string& name) { name_ = name; }

private:
    /// The SharedObject data that this SharedObjectObject holds
    Object<GC> data_;

    /// The name of this SharedObject
    std::string name_;

    // Friend classes for accessing private members
    friend class SharedObjectObject<GC>;
    friend class SharedObjectObjectWeak<GC>;
};

/// SharedObjectObject - AVM2 SharedObject object representation
/// Represents a local shared object (LSO) in ActionScript
template<typename GC>
class SharedObjectObject : public TObject<GC> {
public:
    using DataType = SharedObjectObjectData<GC>;
    using WeakType = SharedObjectObjectWeak<GC>;

    SharedObjectObject() = default;
    explicit SharedObjectObject(Gc<GC, DataType> data) : data_(data) {}

    /// Create a new SharedObjectObject from data and name
    /// @param activation The current activation
    /// @param data The SharedObject data
    /// @param name The name of the SharedObject
    /// @return A new SharedObjectObject instance
    static SharedObjectObject from_data_and_name(
        Activation<GC>* activation,
        Object<GC> data,
        std::string name
    );

    /// Get the base script object data (for TObject trait)
    Gc<GC, ScriptObjectData<GC>> gc_base() const override {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base;
    }

    /// Get the SharedObject data
    Object<GC> data() const {
        return data_->data();
    }

    /// Reset the SharedObject data to an empty object
    /// @param context The update context
    void reset_data(UpdateContext<GC>* context);

    /// Get the name of this SharedObject
    const std::string& name() const {
        return data_->name();
    }

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to SharedObjectObject
template<typename GC>
class SharedObjectObjectWeak {
public:
    SharedObjectObjectWeak() = default;
    explicit SharedObjectObjectWeak(GcWeak<GC, SharedObjectObjectData<GC>> data)
        : data_(data) {}

private:
    GcWeak<GC, SharedObjectObjectData<GC>> data_;
};

// Template method implementations
template<typename GC>
SharedObjectObject<GC> SharedObjectObject<GC>::from_data_and_name(
    Activation<GC>* activation,
    Object<GC> data,
    std::string name
) {
    // Get the SharedObject class
    // auto class_obj = activation->avm2()->classes()->sharedobject;
    auto base = ScriptObjectData<GC>::create(/* class_obj */);
    
    auto shared_data = Gc<GC, DataType>::create(activation->gc(), DataType{base});
    shared_data->set_data(data);
    shared_data->set_name(std::move(name));
    
    return SharedObjectObject<GC>(shared_data);
}

template<typename GC>
void SharedObjectObject<GC>::reset_data(UpdateContext<GC>* context) {
    // Create empty object
    // auto empty_data = ScriptObject::new_object(context);
    
    // In real implementation, this would use gc_arena barrier unlocking
    // unlock!(Gc::write(context.gc(), self.0), SharedObjectObjectData, data).set(empty_data);
    data_->set_data(Object<GC>());
}

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_OBJECT_SHARED_OBJECT_OBJECT_H
