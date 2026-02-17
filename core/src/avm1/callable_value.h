// C++ translation of core/src/avm1/callable_value.rs
// Callable value handling for AVM1

#ifndef RUFFLE_CORE_AVM1_CALLABLE_VALUE_H
#define RUFFLE_CORE_AVM1_CALLABLE_VALUE_H

#include <cstdint>
#include <vector>
#include <variant>
#include <memory>

// Forward declarations
namespace ruffle {
namespace core {
namespace string {
    template<typename T> class AvmString;
}
namespace avm1 {
    template<typename T> class Activation;
    template<typename T> class Object;
    template<typename T> class Value;
    namespace error {
        template<typename T> class Error;
    }
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace callable_value {

/// CallableValue represents a value that may or may not be callable
///
/// This enum distinguishes between:
/// - UnCallable: A value that is not directly callable
/// - Callable: A value with an associated 'this' context for calling
template<typename GC>
class CallableValue {
public:
    /// Variant types
    enum class Kind {
        /// Not callable - just a regular value
        UnCallable,

        /// Callable with an associated 'this' object
        Callable
    };

private:
    Kind kind_;

    // For Callable variant: the 'this' object
    Object<GC>* this_object_;

    // The underlying value (used for both variants)
    Value<GC> value_;

public:
    /// Create an UnCallable variant
    ///
    /// \param value The non-callable value
    explicit CallableValue(Value<GC> value)
        : kind_(Kind::UnCallable), this_object_(nullptr), value_(value) {}

    /// Create a Callable variant
    ///
    /// \param this_obj The 'this' object for the call
    /// \param value The callable value (typically an Object)
    CallableValue(Object<GC>* this_obj, Value<GC> value)
        : kind_(Kind::Callable), this_object_(this_obj), value_(value) {}

    /// Get the kind of callable value
    Kind kind() const { return kind_; }

    /// Get the 'this' object for Callable variants
    Object<GC>* this_object() const { return this_object_; }

    /// Get the underlying value
    const Value<GC>& value() const { return value_; }

    /// Convert to Value
    ///
    /// Extracts the underlying value, discarding the 'this' context.
    Value<GC> to_value() const {
        return value_;
    }

    /// Call the callable value with a default 'this'
    ///
    /// If this is a Callable variant with an Object, calls the object
    /// with its stored 'this' context.
    /// If this is an UnCallable Object, uses the provided default 'this'.
    /// Otherwise returns Undefined.
    ///
    /// \param default_this The default 'this' to use if not already set
    /// \param name The name of the function being called
    /// \param activation The current activation context
    /// \param args The arguments to pass to the function
    /// \return The result of the call, or Undefined if not callable
    Value<GC> call_with_default_this(
        Value<GC> default_this,
        string::AvmString<GC> name,
        Activation<GC>* activation,
        const std::vector<Value<GC>>& args
    ) {
        switch (kind_) {
            case Kind::Callable: {
                // Callable(this, Value::Object(val))
                if (value_.is_object()) {
                    Object<GC>* obj = value_.as_object().value();
                    return obj->call(name, activation, Value<GC>::object(this_object_), args);
                }
                break;
            }
            case Kind::UnCallable: {
                // UnCallable(Value::Object(val))
                if (value_.is_object()) {
                    Object<GC>* obj = value_.as_object().value();
                    return obj->call(name, activation, default_this, args);
                }
                break;
            }
        }

        // Not callable or not an object
        return Value<GC>::undefined();
    }
};

} // namespace callable_value
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_CALLABLE_VALUE_H
