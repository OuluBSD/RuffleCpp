// C++ translation of core/src/avm2/object/worker_object.rs
// AVM2 Worker object

#ifndef RUFFLE_CORE_AVM2_OBJECT_WORKER_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_WORKER_OBJECT_H

#include <cstdint>
#include <memory>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename GC>
    class Activation;
    template<typename GC>
    class TObject;
    template<typename GC>
    class ScriptObjectData;
    template<typename GC>
    class Gc;
    template<typename GC>
    class GcWeak;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace object {

/// Worker object data structure
template<typename GC>
class WorkerObjectData {
public:
    /// Base script object
    ScriptObjectData<GC> base;

    /// Default constructor
    WorkerObjectData() = default;
};

/// Weak reference to Worker object
template<typename GC>
class WorkerObjectWeak {
public:
    GcWeak<GC, WorkerObjectData<GC>> ptr;

    /// Constructor
    explicit WorkerObjectWeak(GcWeak<GC, WorkerObjectData<GC>> p) : ptr(p) {}
};

/// Worker object for AVM2
template<typename GC>
class WorkerObject : public TObject<GC> {
public:
    Gc<GC, WorkerObjectData<GC>> ptr;

    /// Constructor
    explicit WorkerObject(Gc<GC, WorkerObjectData<GC>> p) : ptr(p) {}

    /// Get the base script object
    Gc<GC, ScriptObjectData<GC>> gc_base() override {
        return ptr->base;
    }

    /// Create a new Worker object
    /// 
    /// \param activation The current activation
    /// \return New Worker object
    static WorkerObject<GC> new_object(Activation<GC>* activation) {
        auto class_obj = activation->avm2()->classes()->worker;
        auto base = ScriptObjectData<GC>::new_object(class_obj);
        auto data = std::make_shared<WorkerObjectData<GC>>();
        data->base = base;
        return WorkerObject(Gc::new_gc(activation->gc(), data));
    }
};

} // namespace object
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_OBJECT_WORKER_OBJECT_H
