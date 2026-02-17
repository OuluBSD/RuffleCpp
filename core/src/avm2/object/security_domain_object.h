// C++ translation of core/src/avm2/object/security_domain_object.rs
//! SecurityDomain object implementation for AVM2

#ifndef RUFFLE_CORE_AVM2_OBJECT_SECURITY_DOMAIN_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_SECURITY_DOMAIN_OBJECT_H

#include <memory>
#include <cstdint>
#include <format>

// Forward declarations
namespace ruffle {
namespace core {
    class WString;
    template<typename GC> class UpdateContext;

    namespace avm2 {
        template<typename T> class Activation;
        template<typename T> class ScriptObjectData;
        template<typename T> class TObject;
        template<typename T> class Gc;
        template<typename T> class GcWeak;
        template<typename T> class ClassObject;
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Weak reference to SecurityDomainObject data
template<typename GC>
class SecurityDomainObjectWeak;

/// Data structure for SecurityDomainObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class SecurityDomainObjectData {
public:
    SecurityDomainObjectData() = default;

    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    // Friend classes for accessing private members
    friend class SecurityDomainObject<GC>;
    friend class SecurityDomainObjectWeak<GC>;
};

/// SecurityDomainObject - AVM2 SecurityDomain object representation
/// Represents a SecurityDomain instance in ActionScript 3
template<typename GC>
class SecurityDomainObject {
public:
    using DataType = SecurityDomainObjectData<GC>;
    using WeakType = SecurityDomainObjectWeak<GC>;

    SecurityDomainObject() = default;
    explicit SecurityDomainObject(Gc<GC, DataType> data) : data_(data) {}

    /// Create a new SecurityDomainObject
    ///
    /// @param activation The current activation context
    /// @return New SecurityDomainObject
    static SecurityDomainObject new_(Activation<GC>* activation);

    /// Get the base script object data (for TObject trait)
    Gc<GC, ScriptObjectData<GC>> gc_base() const {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base;
    }

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to SecurityDomainObject
template<typename GC>
class SecurityDomainObjectWeak {
public:
    SecurityDomainObjectWeak() = default;
    explicit SecurityDomainObjectWeak(GcWeak<GC, SecurityDomainObjectData<GC>> data)
        : data_(data) {}

private:
    GcWeak<GC, SecurityDomainObjectData<GC>> data_;
};

// Template method implementations

template<typename GC>
SecurityDomainObject<GC> SecurityDomainObject<GC>::new_(Activation<GC>* activation) {
    auto class_obj = activation->avm2()->classes()->securitydomain;
    auto base = ScriptObjectData<GC>::create(class_obj);
    auto data = Gc<GC, DataType>::create(
        activation->gc(),
        DataType{base}
    );
    return SecurityDomainObject<GC>(data);
}

} // namespace avm2
} // namespace core
} // namespace ruffle

// Debug formatter specialization
template<typename GC>
struct std::formatter<ruffle::core::avm2::SecurityDomainObject<GC>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const ruffle::core::avm2::SecurityDomainObject<GC>& obj, FormatContext& ctx) {
        return std::format_to(ctx.out(), "SecurityDomainObject(ptr={})",
            static_cast<const void*>(obj.data_.get()));
    }
};

#endif // RUFFLE_CORE_AVM2_OBJECT_SECURITY_DOMAIN_OBJECT_H
