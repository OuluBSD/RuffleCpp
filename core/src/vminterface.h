// C++ translation of core/src/vminterface.rs
// Any-AVM object references

#ifndef RUFFLE_CORE_VMINTERFACE_H
#define RUFFLE_CORE_VMINTERFACE_H

#include <variant>
#include <optional>

// Forward declarations
namespace ruffle {
namespace avm1 {
    template<typename GC>
    class Object;
}
namespace avm2 {
    template<typename GC>
    class StageObject;
}
}

namespace ruffle {
namespace core {

/// The VM (or lack thereof) that created a given object.
///
/// This primary purpose of this type is to communicate if a movie clip is
/// being created on an AVM stack, and if so, which type. If it is on-stack,
/// then it needs to be constructed immediately before user code can continue
/// running. Otherwise, its constructor should be queued to run later.
///
/// A secondary purpose of this type is to flag which VM is creating an object,
/// which can be used to ensure the object is instantiated as tied to the
/// correct VM.
enum class Instantiator {
    /// This object was instantiated by a tag in a given SWF movie, or by a VM
    /// action which does not implicitly instantiate a given object.
    Movie,

    /// This object was instantiated by AVM1 code constructing the object.
    Avm1,

    /// This object was instantiated by AVM2 code constructing the object.
    Avm2,
};

inline bool instantiator_is_avm(Instantiator inst) {
    /// Returns true if the instantiation happened on an AVM stack (either kind).
    ///
    /// If that is the case, then any constructor calls necessary to finish the
    /// object must happen on-stack.
    return inst == Instantiator::Avm1 || inst == Instantiator::Avm2;
}

/// A reference to either an AVM1 or AVM2 object.
///
/// Used by non-AVM code to retain VM objects that may have been customized or
/// altered by user code. Non-AVM structures may be held by either VM, and thus
/// those structures must also hold the VM side of themselves as well.
///
/// This structure is specifically designed to only store one VM's
/// representation of the object. Objects cannot be shared across multiple VMs
/// and attempting to do so will generate a runtime error. Dual-representation
/// objects are prohibited.
template<typename GC>
class AvmObject {
private:
    using Avm1Object = ruffle::avm1::Object<GC>;
    using Avm2StageObject = ruffle::avm2::StageObject<GC>;
    
    std::variant<Avm1Object, Avm2StageObject> inner;

public:
    /// Create from AVM1 object
    explicit AvmObject(Avm1Object obj) : inner(obj) {}

    /// Create from AVM2 object
    explicit AvmObject(Avm2StageObject obj) : inner(obj) {}

    /// Attempt to access the AVM1 claim to this object, returning `std::nullopt` if
    /// the object cannot be accessed by the VM.
    std::optional<Avm1Object> as_avm1_object() const {
        if (auto* obj = std::get_if<Avm1Object>(&inner)) {
            return *obj;
        }
        return std::nullopt;
    }

    /// Attempt to access the AVM2 claim to this object, returning `std::nullopt` if
    /// the object cannot be accessed by the VM.
    std::optional<Avm2StageObject> as_avm2_object() const {
        if (auto* obj = std::get_if<Avm2StageObject>(&inner)) {
            return *obj;
        }
        return std::nullopt;
    }
};

} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_VMINTERFACE_H
