/*
 * C++ header for AVM1 scope functionality
 * This replaces the functionality of core/src/avm1/scope.rs
 */

#ifndef AVM1_SCOPE_H
#define AVM1_SCOPE_H

#include "avm1.h"
#include "avm1/value.h"
#include "avm1/object.h"
#include "avm1/activation.h"
#include "avm1/error.h"
#include "avm1/callable_value.h"
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace ruffle {

// Enum for scope class types
enum class ScopeClass {
    // Scope represents global scope.
    GLOBAL,
    // Target represents timeline scope. All timeline actions execute with
    // the current clip object in lieu of a local scope, and the timeline scope
    // can be changed via `tellTarget`.
    TARGET,
    // Scope represents local scope and is inherited when a closure is defined.
    LOCAL,
    // Scope represents an object added to the scope chain with `with`.
    // It is not inherited when closures are defined.
    WITH
};

// Represents a scope chain for an AVM1 activation.
class Scope {
private:
    std::shared_ptr<Scope> parent_;
    ScopeClass class_;
    std::shared_ptr<Object> values_;

public:
    // Constructor
    Scope(std::shared_ptr<Scope> parent, ScopeClass scope_class, std::shared_ptr<Object> values)
        : parent_(std::move(parent)), class_(scope_class), values_(std::move(values)) {}

    // Construct a global scope (one without a parent).
    static Scope from_global_object(std::shared_ptr<Object> globals) {
        return Scope(nullptr, ScopeClass::GLOBAL, std::move(globals));
    }

    // Construct a child scope of another scope.
    static Scope new_local_scope(std::shared_ptr<Scope> parent) {
        auto new_obj = std::make_shared<Object>(nullptr); // Create object without prototype
        return Scope(std::move(parent), ScopeClass::LOCAL, std::move(new_obj));
    }

    // Construct a scope for use with `tellTarget` code where the timeline
    // scope has been replaced with another given object.
    static std::shared_ptr<Scope> new_target_scope(
        std::shared_ptr<Scope> parent,
        std::shared_ptr<Object> clip) {
        
        auto scope = std::make_shared<Scope>(*parent);
        
        if (scope->class_ == ScopeClass::TARGET) {
            scope->values_ = std::move(clip);
        } else {
            if (scope->parent_) {
                scope->parent_ = new_target_scope(scope->parent_, clip);
            }
        }

        return scope;
    }

    // Construct a with scope to be used as the scope during a with block.
    //
    // A with block adds an object to the top of the scope chain, so unqualified
    // references will try to resolve on that object first.
    static Scope new_with_scope(std::shared_ptr<Scope> parent_scope, 
                               std::shared_ptr<Object> with_object) {
        return Scope(std::move(parent_scope), ScopeClass::WITH, std::move(with_object));
    }

    // Construct an arbitrary scope.
    static Scope new_scope(std::shared_ptr<Scope> parent, 
                          ScopeClass scope_class, 
                          std::shared_ptr<Object> with_object) {
        return Scope(std::move(parent), scope_class, std::move(with_object));
    }

    // Returns a reference to the current local scope object.
    std::shared_ptr<Object> locals() const { return values_; }

    // Returns a reference to the parent scope object.
    std::shared_ptr<Scope> parent() const { return parent_; }

    // Returns the class.
    ScopeClass class_type() const { return class_; }

    // Resolve a particular value in the scope chain and the object which this value would expect as its `this` parameter if called.
    CallableValue resolve(const std::string& name, 
                         std::shared_ptr<Activation> activation) const {
        return resolve_recursive(name, activation, true);
    }

    // Recursively resolve a value on the scope chain
    // See `Scope::resolve` for details
    CallableValue resolve_recursive(const std::string& name, 
                                   std::shared_ptr<Activation> activation, 
                                   bool top_level) const {
        if (values_->has_property(name, activation)) {
            auto value = values_->get(name, activation);
            return CallableValue(values_, value);
        }
        
        if (parent_) {
            auto res = parent_->resolve(name, activation);

            // If we failed to find the value in the scope chain, but it *would* resolve on `self.locals()` if it wasn't
            // a removed clip, then try resolving on root instead
            if (top_level && 
                res.get_value()->is_undefined() && 
                values_->has_own_property(activation, name)) {
                
                auto root = activation->base_clip()->avm1_root();
                auto root_obj = root->object1_or_bare(activation->gc());
                auto root_value = root_obj->get(name, activation);
                return CallableValue(values_, root_value);
            }

            return res;
        }
        
        return CallableValue(std::make_shared<Value>(Value::UNDEFINED));
    }

    // Update a particular value in the scope chain.
    //
    // Traverses the scope chain in search of a value. If it's found, it's overwritten.
    // The traversal stops at Target scopes, which represents the movie clip timeline
    // the code is executing in.
    // If the value is not found, it is defined on this Target scope.
    void set(const std::string& name, 
             std::shared_ptr<Value> value, 
             std::shared_ptr<Activation> activation) const {
        
        bool removed = values_->as_display_object() ? 
                      values_->as_display_object()->avm1_removed() : false;

        if (!removed && 
            (class_ == ScopeClass::TARGET || values_->has_property(name, activation))) {
            // Value found on this object, so overwrite it.
            // Or we've hit the executing movie clip, so create it here.
            values_->set(name, value, activation);
        } else if (parent_) {
            // Traverse the scope chain in search of the value.
            parent_->set(name, value, activation);
        } else {
            // This probably shouldn't happen -- all AVM1 code runs in reference to some MovieClip,
            // so we should always have a MovieClip scope.
            // Define on the top-level scope.
            values_->set(name, value, activation);
        }
    }

    // Define a named local variable on the scope.
    //
    // If the property does not already exist on the local scope, it will be created.
    // Otherwise, the existing property will be set to `value`. This does not crawl the scope
    // chain. Any properties with the same name deeper in the scope chain will be shadowed.
    void define_local(const std::string& name, 
                     std::shared_ptr<Value> value, 
                     std::shared_ptr<Activation> activation) const {
        
        // When defining a local in a with scope, we first need to check if that local already exists on the with target
        // If it does, then the property of the target itself should be modified
        // If it doesn't, then the property should be defined in the first non-with parent scope
        if (class_ == ScopeClass::WITH && parent_) {
            // Does this property already exist on the target?
            if (values_->has_own_property(activation, name)) {
                values_->set(name, value, activation);
            } else {
                // Otherwise, carry up the scope chain
                parent_->define_local(name, value, activation);
            }
        } else {
            values_->set(name, value, activation);
        }
    }

    // Create a local property on the activation.
    //
    // This inserts a value as a stored property on the local scope. If the property already
    // exists, it will be forcefully overwritten. Used internally to initialize objects.
    void force_define_local(const std::string& name, std::shared_ptr<Value> value) const {
        values_->define_value(nullptr, name, value, 0); // 0 for empty attributes
    }

    // Delete a value from scope.
    bool delete_value(std::shared_ptr<Activation> activation, const std::string& name) const {
        if (values_->has_property(name, activation)) {
            return values_->delete_property(activation, name);
        }

        if (parent_) {
            return parent_->delete_value(activation, name);
        }

        return false;
    }

    // Iterator for traversing scope ancestors
    class AncestorIterator {
    private:
        std::vector<std::shared_ptr<Scope>> scopes_;
        size_t current_;

    public:
        AncestorIterator(std::shared_ptr<Scope> scope) : current_(0) {
            // Collect all ancestors
            std::shared_ptr<Scope> current = scope;
            while (current) {
                scopes_.push_back(current);
                current = current->parent_;
            }
        }

        std::shared_ptr<Scope> next() {
            if (current_ < scopes_.size()) {
                return scopes_[current_++];
            }
            return nullptr;
        }

        bool has_next() const {
            return current_ < scopes_.size();
        }
    };

    // Produce an iterator for the scope and its ancestors
    static AncestorIterator ancestors(std::shared_ptr<Scope> scope) {
        return AncestorIterator(scope);
    }
};

} // namespace ruffle

#endif // AVM1_SCOPE_H