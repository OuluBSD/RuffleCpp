// C++ translation of core/src/avm1/scope.rs
// Represents AVM1 scope chain resolution.

#ifndef RUFFLE_CORE_AVM1_SCOPE_H
#define RUFFLE_CORE_AVM1_SCOPE_H

#include <cstdint>
#include <memory>
#include <optional>

// Forward declarations
namespace ruffle {
namespace core {
namespace string {
    template<typename T> class AvmString;
}
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Error;
    template<typename T> class Object;
    template<typename T> class Value;
    namespace callable_value {
        template<typename T> class CallableValue;
    }
}
namespace gc_arena {
    template<typename T> class Gc;
    class Mutation;
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace scope {

/// Indicates what kind of scope a scope is.
enum class ScopeClass {
    /// Scope represents global scope.
    Global,

    /// Target represents timeline scope. All timeline actions execute with
    /// the current clip object in lieu of a local scope, and the timeline scope
    /// can be changed via `tellTarget`.
    Target,

    /// Scope represents local scope and is inherited when a closure is defined.
    Local,

    /// Scope represents an object added to the scope chain with `with`.
    /// It is not inherited when closures are defined.
    With
};

/// Represents a scope chain for an AVM1 activation.
///
/// Scopes form a chain where each scope has an optional parent.
/// Variable resolution traverses the chain from child to parent.
template<typename GC>
class Scope {
private:
    std::shared_ptr<Scope<GC>> parent_;
    ScopeClass class_;
    Object<GC> values_;

public:
    /// Construct a global scope (one without a parent).
    ///
    /// \param globals The global object
    explicit Scope(Object<GC> globals)
        : parent_(nullptr)
        , class_(ScopeClass::Global)
        , values_(globals)
    {
    }

    /// Construct a child scope of another scope.
    ///
    /// Creates a new local scope with the given parent.
    ///
    /// \param parent The parent scope
    /// \param mc The mutation context for GC allocations
    /// \return A new Scope instance
    static Scope new_local_scope(
        const std::shared_ptr<Scope<GC>>& parent,
        gc_arena::Mutation* mc
    ) {
        return Scope(parent, ScopeClass::Local, Object<GC>::new_without_proto(mc));
    }

    /// Construct a scope for use with `tellTarget` code.
    ///
    /// The timeline scope is replaced with another given object.
    ///
    /// \param parent The parent scope
    /// \param clip The clip object to use as timeline scope
    /// \param mc The mutation context for GC allocations
    /// \return A new Scope instance
    static std::shared_ptr<Scope<GC>> new_target_scope(
        const std::shared_ptr<Scope<GC>>& parent,
        Object<GC> clip,
        gc_arena::Mutation* mc
    );

    /// Construct a with scope to be used as the scope during a with block.
    ///
    /// A with block adds an object to the top of the scope chain, so unqualified
    /// references will try to resolve on that object first.
    ///
    /// \param parent_scope The parent scope
    /// \param with_object The object to add to the scope chain
    /// \return A new Scope instance
    static Scope new_with_scope(
        const std::shared_ptr<Scope<GC>>& parent_scope,
        Object<GC> with_object
    ) {
        return Scope(parent_scope, ScopeClass::With, with_object);
    }

    /// Construct an arbitrary scope.
    ///
    /// \param parent The parent scope
    /// \param class_ The scope class
    /// \param with_object The object for this scope
    /// \return A new Scope instance
    static Scope new(
        const std::shared_ptr<Scope<GC>>& parent,
        ScopeClass class_,
        Object<GC> with_object
    ) {
        return Scope(parent, class_, with_object);
    }

    /// Returns a reference to the current local scope object.
    const Object<GC>& locals() const { return values_; }

    /// Returns a reference to the current local scope object (copy).
    Object<GC> locals_cell() const { return values_; }

    /// Returns a reference to the parent scope.
    const std::shared_ptr<Scope<GC>>& parent() const { return parent_; }

    /// Returns the class.
    ScopeClass class_() const { return class_; }

    /// Resolve a particular value in the scope chain.
    ///
    /// Returns the value and the object which this value would expect as its
    /// `this` parameter if called.
    ///
    /// Because scopes are object chains, the same rules for `Object::get`
    /// still apply here. This function is allowed to yield `None` to indicate
    /// that the result will be calculated on the AVM stack.
    ///
    /// \param name The property name to resolve
    /// \param activation The current activation context
    /// \return The callable value or Undefined if not found
    callable_value::CallableValue<GC> resolve(
        string::AvmString<GC> name,
        Activation<GC>* activation
    );

    /// Update a particular value in the scope chain.
    ///
    /// Traverses the scope chain in search of a value. If it's found, it's overwritten.
    /// The traversal stops at Target scopes, which represents the movie clip timeline
    /// the code is executing in.
    /// If the value is not found, it is defined on this Target scope.
    ///
    /// \param name The property name to set
    /// \param value The value to set
    /// \param activation The current activation context
    /// \return Ok(()) if successful, Error otherwise
    void set(
        string::AvmString<GC> name,
        Value<GC> value,
        Activation<GC>* activation
    );

    /// Define a named local variable on the scope.
    ///
    /// If the property does not already exist on the local scope, it will be created.
    /// Otherwise, the existing property will be set to `value`. This does not crawl the scope
    /// chain. Any properties with the same name deeper in the scope chain will be shadowed.
    ///
    /// \param name The property name
    /// \param value The value to define
    /// \param activation The current activation context
    /// \return Ok(()) if successful, Error otherwise
    void define_local(
        string::AvmString<GC> name,
        Value<GC> value,
        Activation<GC>* activation
    );

    /// Create a local property on the activation.
    ///
    /// This inserts a value as a stored property on the local scope. If the property already
    /// exists, it will be forcefully overwritten. Used internally to initialize objects.
    ///
    /// \param name The property name
    /// \param value The value to define
    /// \param mc The mutation context for GC allocations
    void force_define_local(
        string::AvmString<GC> name,
        Value<GC> value,
        gc_arena::Mutation* mc
    );

    /// Delete a value from scope.
    ///
    /// \param activation The current activation context
    /// \param name The property name to delete
    /// \return true if the property was deleted
    bool delete_(
        Activation<GC>* activation,
        string::AvmString<GC> name
    );

private:
    /// Private constructor
    Scope(
        const std::shared_ptr<Scope<GC>>& parent,
        ScopeClass class_,
        Object<GC> values
    )
        : parent_(parent)
        , class_(class_)
        , values_(values)
    {
    }

    /// Recursively resolve a value on the scope chain.
    ///
    /// See `resolve` for details.
    ///
    /// \param name The property name to resolve
    /// \param activation The current activation context
    /// \param top_level Whether this is the top-level scope
    /// \return The callable value
    callable_value::CallableValue<GC> resolve_recursive(
        string::AvmString<GC> name,
        Activation<GC>* activation,
        bool top_level
    );
};

} // namespace scope
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_SCOPE_H
