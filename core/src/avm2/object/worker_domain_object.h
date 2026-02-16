// C++ translation of core/src/avm2/object/worker_domain_object.rs
//! Object representation for WorkerDomain objects

#ifndef RUFFLE_CORE_AVM2_OBJECT_WORKER_DOMAIN_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_WORKER_DOMAIN_OBJECT_H

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
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Weak reference to WorkerDomainObject data
template<typename GC>
class WorkerDomainObjectWeak;

/// Data structure for WorkerDomainObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class WorkerDomainObjectData {
public:
    WorkerDomainObjectData() = default;

    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    // Friend classes for accessing private members
    friend class WorkerDomainObject<GC>;
    friend class WorkerDomainObjectWeak<GC>;
};

/// WorkerDomainObject - AVM2 WorkerDomain object representation
/// Represents a WorkerDomain instance in ActionScript
template<typename GC>
class WorkerDomainObject {
public:
    using DataType = WorkerDomainObjectData<GC>;
    using WeakType = WorkerDomainObjectWeak<GC>;

    WorkerDomainObject() = default;
    explicit WorkerDomainObject(Gc<GC, DataType> data) : data_(data) {}

    /// Create a new WorkerDomain object
    /// @param activation The current activation context
    /// @return New WorkerDomainObject
    static WorkerDomainObject new_object(Activation<GC>* activation) {
        auto class_obj = activation->avm2()->classes()->workerdomain;
        auto base = ScriptObjectData<GC>::create(class_obj);
        auto data = Gc<GC, DataType>::create(activation->gc(), DataType{base});
        return WorkerDomainObject(data);
    }

    /// Get the base script object data (for TObject trait)
    Gc<GC, ScriptObjectData<GC>> gc_base() const {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base;
    }

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to WorkerDomainObject
template<typename GC>
class WorkerDomainObjectWeak {
public:
    WorkerDomainObjectWeak() = default;
    explicit WorkerDomainObjectWeak(GcWeak<GC, WorkerDomainObjectData<GC>> data)
        : data_(data) {}

private:
    GcWeak<GC, WorkerDomainObjectData<GC>> data_;
};

} // namespace avm2
} // namespace core
} // namespace ruffle

// Debug formatter specialization
template<typename GC>
struct std::formatter<ruffle::core::avm2::WorkerDomainObject<GC>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const ruffle::core::avm2::WorkerDomainObject<GC>& obj, FormatContext& ctx) {
        return std::format_to(ctx.out(), "WorkerDomainObject(ptr={})", 
            static_cast<const void*>(obj.data_.get()));
    }
};

#endif // RUFFLE_CORE_AVM2_OBJECT_WORKER_DOMAIN_OBJECT_H
