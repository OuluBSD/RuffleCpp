// C++ translation of core/src/avm2/object/domain_object.rs
//! Application Domain objects for scripts

#ifndef RUFFLE_CORE_AVM2_OBJECT_DOMAIN_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_DOMAIN_OBJECT_H

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
        template<typename T> class Object;
        template<typename T> class Error;
        template<typename T> class Domain;
        template<typename T> class Mutation;
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Weak reference to DomainObject data
template<typename GC>
class DomainObjectWeak;

/// Data structure for DomainObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class DomainObjectData {
public:
    DomainObjectData() = default;

    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    /// The domain this object holds
    Domain<GC> domain;

    // Friend classes for accessing private members
    friend class DomainObject<GC>;
    friend class DomainObjectWeak<GC>;
};

/// DomainObject - AVM2 ApplicationDomain object representation
/// Represents an ApplicationDomain instance in ActionScript 3
template<typename GC>
class DomainObject {
public:
    using DataType = DomainObjectData<GC>;
    using WeakType = DomainObjectWeak<GC>;

    DomainObject() = default;
    explicit DomainObject(Gc<GC, DataType> data) : data_(data) {}

    /// Create a new DomainObject for a given domain
    ///
    /// This function will call instance initializers. You do not need to do so
    /// yourself.
    ///
    /// @param activation The current activation context
    /// @param domain The domain to associate with this object
    /// @return New DomainObject
    static DomainObject from_domain(Activation<GC>* activation, Domain<GC> domain);

    /// Get the domain
    /// @return The domain
    Domain<GC> domain() const {
        return data_->domain;
    }

    /// Initialize the domain
    ///
    /// @param mc The mutation context
    /// @param domain The domain to set
    void init_domain(Mutation<GC>* mc, Domain<GC> domain);

    /// Get the base script object data (for TObject trait)
    Gc<GC, ScriptObjectData<GC>> gc_base() const {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base;
    }

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to DomainObject
template<typename GC>
class DomainObjectWeak {
public:
    DomainObjectWeak() = default;
    explicit DomainObjectWeak(GcWeak<GC, DomainObjectData<GC>> data)
        : data_(data) {}

private:
    GcWeak<GC, DomainObjectData<GC>> data_;
};

// Template method implementations

template<typename GC>
DomainObject<GC> DomainObject<GC>::from_domain(Activation<GC>* activation, Domain<GC> domain) {
    auto class_obj = activation->avm2()->classes()->application_domain;
    auto base = ScriptObjectData<GC>::create(class_obj);
    auto data = Gc<GC, DataType>::create(
        activation->gc(),
        DataType{base, domain}
    );
    return DomainObject<GC>(data);
}

template<typename GC>
void DomainObject<GC>::init_domain(Mutation<GC>* mc, Domain<GC> domain) {
    data_->domain = domain;
}

} // namespace avm2
} // namespace core
} // namespace ruffle

// Debug formatter specialization
template<typename GC>
struct std::formatter<ruffle::core::avm2::DomainObject<GC>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const ruffle::core::avm2::DomainObject<GC>& obj, FormatContext& ctx) {
        return std::format_to(ctx.out(), "DomainObject(ptr={})",
            static_cast<const void*>(obj.data_.get()));
    }
};

#endif // RUFFLE_CORE_AVM2_OBJECT_DOMAIN_OBJECT_H
