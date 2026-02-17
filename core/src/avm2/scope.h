// C++ translation of core/src/avm2/scope.rs
//! Represents AVM2 scope chain resolution

#ifndef RUFFLE_CORE_AVM2_SCOPE_H
#define RUFFLE_CORE_AVM2_SCOPE_H

#include <vector>
#include <optional>
#include <memory>
#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC>
    class Activation;
    template<typename GC>
    class Value;
    template<typename GC>
    class Error;
    template<typename GC>
    class Multiname;
    template<typename GC>
    class Namespace;
    template<typename GC>
    class Class;
    template<typename GC>
    class Domain;
    template<typename GC>
    class PropertyMap;
    template<typename GC>
    class TObject;

    namespace gc_arena {
        template<typename T>
        class Gc;
        template<typename T>
        class GcWeak;
        template<typename T>
        class RefLock;
        class Mutation;
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {

using gc_arena::Gc;
using gc_arena::GcWeak;
using gc_arena::RefLock;
using gc_arena::Mutation;

/// Represents a Scope that can be on either a ScopeChain or local ScopeStack.
template<typename GC>
class Scope {
private:
    /// The underlying object of this Scope
    Value<GC> values_;

    /// Indicates whether or not this is a `with` scope.
    ///
    /// A `with` scope allows searching the dynamic properties of
    /// this scope.
    bool with_;

public:
    /// Creates a new regular Scope.
    ///
    /// It is the caller's responsibility to ensure that the `values` passed
    /// to this method is not Value::Null or Value::Undefined.
    explicit Scope(Value<GC> values)
        : values_(values)
        , with_(false)
    {}

    /// Creates a new `with` Scope.
    ///
    /// It is the caller's responsibility to ensure that the `values` passed
    /// to this method is not Value::Null or Value::Undefined.
    static Scope new_with(Value<GC> values) {
        return Scope(values, true);
    }

    bool with() const { return with_; }
    Value<GC> values() const { return values_; }

private:
    Scope(Value<GC> values, bool with)
        : values_(values)
        , with_(with)
    {}
};

/// Internal container that a ScopeChain uses
template<typename GC>
struct ScopeContainer {
    /// The scopes of this ScopeChain
    std::vector<Scope<GC>> scopes;

    /// The cache of this ScopeChain. A value of None indicates that caching is disabled
    /// for this ScopeChain.
    std::optional<RefLock<PropertyMap<GC, Value<GC>>>> cache;

    ScopeContainer() = default;

    explicit ScopeContainer(std::vector<Scope<GC>> scopes)
        : scopes(std::move(scopes))
    {
        // Cache is enabled only if no scope is a `with` scope
        bool has_with = false;
        for (const auto& scope : scopes) {
            if (scope.with()) {
                has_with = true;
                break;
            }
        }
        if (!has_with) {
            cache = RefLock<PropertyMap<GC, Value<GC>>>();
        }
    }

    std::optional<Scope<GC>> get(size_t index) const {
        if (index < scopes.size()) {
            return scopes[index];
        }
        return std::nullopt;
    }

    /// Like `get`, but panics if the scope index is out of bounds.
    Scope<GC> get_unchecked(size_t index) const {
        return scopes[index];
    }

    bool is_empty() const {
        return scopes.empty();
    }
};

/// A ScopeChain "chains" scopes together.
///
/// A ScopeChain is used for "remembering" what a scope looked like. A ScopeChain also
/// contains an associated Domain that should be the domain that was in use during it's
/// initial creation.
///
/// A ScopeChain is either created by chaining new scopes on top of an already existing
/// ScopeChain, or if we haven't created one yet (like during script initialization), you can
/// create an empty ScopeChain with only a Domain. A ScopeChain should **always** have a Domain.
///
/// ScopeChain's are copy-on-write, meaning when we chain new scopes on top of a ScopeChain, we
/// actually create a completely brand new ScopeChain. The Domain of the ScopeChain we are chaining
/// on top of will be used for the new ScopeChain.
template<typename GC>
class ScopeChain {
private:
    std::optional<Gc<GC, ScopeContainer<GC>>> container_;
    Domain<GC> domain_;

public:
    /// Creates a brand new ScopeChain with a domain. The domain should be the current domain in use.
    explicit ScopeChain(Domain<GC> domain)
        : container_(std::nullopt)
        , domain_(domain)
    {}

    /// Creates a new ScopeChain by chaining new scopes on top of this ScopeChain
    ScopeChain chain(Mutation<GC>* mc, const std::vector<Scope<GC>>& new_scopes) const {
        if (new_scopes.empty()) {
            // If we are not actually adding any new scopes, we don't need to do anything.
            return *this;
        }

        if (container_.has_value()) {
            // The new ScopeChain is created by cloning the scopes of this ScopeChain,
            // and pushing the new scopes on top of that.
            auto cloned = container_->read().scopes;
            cloned.insert(cloned.end(), new_scopes.begin(), new_scopes.end());
            return ScopeChain(
                Gc<GC, ScopeContainer<GC>>::new(mc, ScopeContainer<GC>(cloned)),
                domain_
            );
        } else {
            // We are chaining on top of an empty ScopeChain, so we don't actually
            // need to chain anything.
            return ScopeChain(
                Gc<GC, ScopeContainer<GC>>::new(mc, ScopeContainer<GC>(new_scopes)),
                domain_
            );
        }
    }

    std::optional<Scope<GC>> get(size_t index) const {
        if (container_.has_value()) {
            return container_->read().get(index);
        }
        return std::nullopt;
    }

    /// Like `get`, but panics if the container doesn't exist or
    /// the scope index is out of bounds.
    Scope<GC> get_unchecked(size_t index) const {
        return container_->read().get_unchecked(index);
    }

    bool is_empty() const {
        if (container_.has_value()) {
            return container_->read().is_empty();
        }
        return true;
    }

    /// Returns the domain associated with this ScopeChain.
    Domain<GC> domain() const { return domain_; }

    /// Find a value in the scope chain
    std::optional<Value<GC>> find(
        const Multiname<GC>* multiname,
        Activation<GC>* activation
    ) const;

    /// Resolve a property in the scope chain
    std::optional<Value<GC>> resolve(
        const Multiname<GC>* multiname,
        Activation<GC>* activation
    ) const;

private:
    ScopeChain(Gc<GC, ScopeContainer<GC>> container, Domain<GC> domain)
        : container_(container)
        , domain_(domain)
    {}

    /// Internal find implementation
    std::optional<std::pair<std::optional<Namespace<GC>>, Value<GC>>> find_internal(
        const Multiname<GC>* multiname,
        Activation<GC>* activation
    ) const;
};

/// Searches for a scope in the scope stack by a multiname.
///
/// The `global` parameter indicates whether we are on global$init (script initializer).
/// When the `global` parameter is true, the scope at depth 0 is considered the global scope, and is skipped.
template<typename GC>
std::optional<Value<GC>> search_scope_stack(
    Activation<GC>* activation,
    const Multiname<GC>* multiname,
    bool global
);

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_SCOPE_H
