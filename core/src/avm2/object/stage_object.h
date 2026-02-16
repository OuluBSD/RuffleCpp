// C++ translation of core/src/avm2/object/stage_object.rs
// AVM2 object impl for the display hierarchy.

#ifndef RUFFLE_CORE_AVM2_OBJECT_STAGE_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_STAGE_OBJECT_H

#include <memory>
#include <vector>
#include <optional>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename GC> class Activation;
    template<typename GC> class Value;
    template<typename GC> class Error;
    template<typename GC> class Object;
    template<typename GC> class ClassObject;
    template<typename GC> class ScriptObjectData;
    template<typename GC> class TObject;
    template<typename GC> class FunctionArgs;

    namespace globals {
        namespace flash {
            namespace display {
                template<typename GC> class Graphics;
            }
        }
    }
}
}

namespace display_object {
    template<typename GC> class DisplayObject;
}

namespace gc_arena {
    template<typename T> class Gc;
    template<typename T> class GcWeak;
    class Mutation;
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace object {

/// StageObject - AVM2 side of a display object
template<typename GC>
class StageObject;

/// Weak reference to StageObject
template<typename GC>
class StageObjectWeak;

/// Data structure for StageObject
template<typename GC>
class StageObjectData {
private:
    /// The base data common to all AVM2 objects.
    std::shared_ptr<ScriptObjectData<GC>> base_;

    /// The associated display object.
    display_object::DisplayObject<GC> display_object_;

public:
    StageObjectData() = default;

    StageObjectData(std::shared_ptr<ScriptObjectData<GC>> base,
                    display_object::DisplayObject<GC> display_object)
        : base_(std::move(base)), display_object_(std::move(display_object)) {}

    ScriptObjectData<GC>* base() { return base_.get(); }
    const ScriptObjectData<GC>* base() const { return base_.get(); }

    display_object::DisplayObject<GC>& display_object() { return display_object_; }
    const display_object::DisplayObject<GC>& display_object() const { return display_object_; }
};

/// Strong reference to StageObject
template<typename GC>
class StageObject {
private:
    std::shared_ptr<StageObjectData<GC>> data_;

public:
    StageObject() = default;
    explicit StageObject(std::shared_ptr<StageObjectData<GC>> data)
        : data_(std::move(data)) {}

    StageObjectData<GC>* data() { return data_.get(); }
    const StageObjectData<GC>* data() const { return data_.get(); }

    ScriptObjectData<GC>* base() { return data_ ? data_->base() : nullptr; }
    const ScriptObjectData<GC>* base() const { return data_ ? data_->base() : nullptr; }

    explicit operator bool() const { return data_ != nullptr; }

    /// Allocate the AVM2 side of a display object intended to be of a given
    /// class's type.
    ///
    /// This function makes no attempt to construct the returned object. You
    /// are responsible for calling the native initializer of the given
    /// class at a later time.
    static StageObject for_display_object(
        gc_arena::Mutation* mc,
        display_object::DisplayObject<GC> display_object,
        ClassObject<GC> class_
    );

    /// Allocate and construct the AVM2 side of a display object intended to be
    /// of a given class's type.
    ///
    /// This function is intended for display objects that do not have children
    /// and thus do not need to be allocated and initialized in separate phases.
    static std::optional<StageObject> for_display_object_childless(
        Activation<GC>* activation,
        display_object::DisplayObject<GC> display_object,
        ClassObject<GC> class_
    );

    /// Create a graphics object for a given display object.
    static StageObject graphics(
        Activation<GC>* activation,
        display_object::DisplayObject<GC> display_object
    );

    /// Get the associated display object
    display_object::DisplayObject<GC> display_object() const {
        return data_ ? data_->display_object() : display_object::DisplayObject<GC>();
    }
};

/// Weak reference to StageObject
template<typename GC>
class StageObjectWeak {
private:
    std::weak_ptr<StageObjectData<GC>> data_;

public:
    StageObjectWeak() = default;
    explicit StageObjectWeak(std::weak_ptr<StageObjectData<GC>> data)
        : data_(std::move(data)) {}

    StageObject<GC> upgrade() const {
        auto locked = data_.lock();
        if (locked) {
            return StageObject<GC>(locked);
        }
        return StageObject<GC>();
    }

    bool expired() const { return data_.expired(); }
};

} // namespace object
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_OBJECT_STAGE_OBJECT_H
