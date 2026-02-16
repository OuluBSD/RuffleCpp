// C++ translation of core/src/avm2/object/stage3d_object.rs
//! Object representation for Stage3D objects

#ifndef RUFFLE_CORE_AVM2_OBJECT_STAGE3D_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_STAGE3D_OBJECT_H

#include <memory>
#include <optional>
#include <atomic>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Object;
    template<typename T> class ScriptObjectData;
    template<typename T> class TObject;
    template<typename T> class Gc;
    template<typename T> class GcWeak;
    template<typename T> class Mutation;
    template<typename T> class UpdateContext;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Weak reference to Stage3DObject data
template<typename GC>
class Stage3DObjectWeak;

/// Data structure for Stage3DObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class Stage3DObjectData {
public:
    Stage3DObjectData() : context3d_(std::nullopt), visible_(true) {}
    
    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    /// The context3D object associated with this Stage3D object,
    /// if it's been created with requestContext3D
    std::optional<Object<GC>> context3d() const { return context3d_; }
    void set_context3d(const std::optional<Object<GC>>& value) { context3d_ = value; }

    /// Visibility state of the Stage3D object
    bool visible() const { return visible_; }
    void set_visible(bool value) { visible_ = value; }

private:
    std::optional<Object<GC>> context3d_;
    std::atomic<bool> visible_;
    
    // Friend classes for accessing private members
    friend class Stage3DObject<GC>;
    friend class Stage3DObjectWeak<GC>;
};

/// Stage3DObject - AVM2 Stage3D object representation
/// Represents a Stage3D context for 3D rendering in Flash
template<typename GC>
class Stage3DObject {
public:
    using DataType = Stage3DObjectData<GC>;
    using WeakType = Stage3DObjectWeak<GC>;

    Stage3DObject() = default;
    explicit Stage3DObject(Gc<GC, DataType> data) : data_(data) {}

    /// Create a new Stage3DObject
    static Stage3DObject create(UpdateContext<GC>* context);

    /// Get the context3D object associated with this Stage3D
    std::optional<Object<GC>> context3d() const {
        return data_->context3d();
    }

    /// Set the context3D object
    void set_context3d(const std::optional<Object<GC>>& context3d, Mutation<GC>* mc);

    /// Get the visibility state
    bool visible() const {
        return data_->visible();
    }

    /// Set the visibility state
    void set_visible(bool visible);

    /// Get the base script object data (for TObject trait)
    Gc<GC, ScriptObjectData<GC>> gc_base() const {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base;
    }

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to Stage3DObject
template<typename GC>
class Stage3DObjectWeak {
public:
    Stage3DObjectWeak() = default;
    explicit Stage3DObjectWeak(GcWeak<GC, Stage3DObjectData<GC>> data) 
        : data_(data) {}

private:
    GcWeak<GC, Stage3DObjectData<GC>> data_;
};

// Template method implementations
template<typename GC>
Stage3DObject<GC> Stage3DObject<GC>::create(UpdateContext<GC>* context) {
    // Get the Stage3D class from context
    // In real implementation: auto class_obj = context->avm2.classes().stage3d;
    auto base = ScriptObjectData<GC>::create(/* class_obj */);
    
    auto data = Gc<GC, DataType>::create(context->gc(), DataType{});
    data->base = base;
    
    return Stage3DObject<GC>(data);
}

template<typename GC>
void Stage3DObject<GC>::set_context3d(const std::optional<Object<GC>>& context3d, Mutation<GC>* mc) {
    // In real implementation, this would use gc_arena barrier unlocking
    data_->set_context3d(context3d);
}

template<typename GC>
void Stage3DObject<GC>::set_visible(bool visible) {
    data_->set_visible(visible);
}

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_OBJECT_STAGE3D_OBJECT_H
