// C++ translation of core/src/avm2/object/error_object.rs
//! Object representation for Error objects

#ifndef RUFFLE_CORE_AVM2_OBJECT_ERROR_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_ERROR_OBJECT_H

#include <memory>
#include <cstdint>
#include <string>

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
    template<typename T> class Value;
    template<typename T> class AvmString;
    class CallStack;
    namespace function {
        template<typename T> class FunctionArgs;
    }
}
namespace string {
    template<typename T> class WString;
    class WStr;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Weak reference to ErrorObject data
template<typename GC>
class ErrorObjectWeak;

/// Data structure for ErrorObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class ErrorObjectData {
public:
    ErrorObjectData() = default;

    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    /// The call stack for debugging purposes
    CallStack call_stack;

    // Friend classes for accessing private members
    friend class ErrorObject<GC>;
    friend class ErrorObjectWeak<GC>;
};

/// ErrorObject - AVM2 Error object representation
/// Represents an Error instance in ActionScript
template<typename GC>
class ErrorObject {
public:
    using DataType = ErrorObjectData<GC>;
    using WeakType = ErrorObjectWeak<GC>;

    ErrorObject() = default;
    explicit ErrorObject(Gc<GC, DataType> data) : data_(data) {}

    /// Allocates a new ErrorObject for the given class, without running the
    /// class initializer.
    /// @param activation The current activation context
    /// @param class_obj The class object for Error
    /// @return The new ErrorObject
    static ErrorObject create(Activation<GC>* activation, ClassObject<GC> class_obj);

    /// Allocates and constructs a new ErrorObject for the given class, using
    /// the provided error message and id.
    /// @param activation The current activation context
    /// @param class_obj The class object for Error
    /// @param message The error message
    /// @param error_id The error ID
    /// @return The new ErrorObject
    static ErrorObject from_info(
        Activation<GC>* activation,
        ClassObject<GC> class_obj,
        AvmString<GC> message,
        uint32_t error_id);

    /// Get the display string for this error
    /// @return The error display string
    string::WString display() const;

    /// Get the full display string including stack trace
    /// @return The full error display string
    string::WString display_full() const;

    /// Get the call stack
    /// @return Reference to the call stack
    const CallStack& call_stack() const {
        return data_->call_stack;
    }

    /// Get the base script object data (for TObject trait)
    Gc<GC, ScriptObjectData<GC>> gc_base() const {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base;
    }

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to ErrorObject
template<typename GC>
class ErrorObjectWeak {
public:
    ErrorObjectWeak() = default;
    explicit ErrorObjectWeak(GcWeak<GC, ErrorObjectData<GC>> data)
        : data_(data) {}

private:
    GcWeak<GC, ErrorObjectData<GC>> data_;
};

/// Error object allocator function
/// @param class_obj The class object for Error
/// @param activation The current activation context
/// @return Result containing the new ErrorObject or error
template<typename GC>
Object<GC> error_allocator(
    ClassObject<GC> class_obj,
    Activation<GC>* activation);

// Template method implementations
template<typename GC>
ErrorObject<GC> ErrorObject<GC>::create(Activation<GC>* activation, ClassObject<GC> class_obj) {
    auto base = ScriptObjectData<GC>::create(class_obj);
    
    // Stack trace is always collected for debugging purposes.
    auto call_stack = activation->avm2()->capture_call_stack();
    
    auto data = Gc<GC, DataType>::create(activation->gc(), DataType{base});
    data->call_stack = call_stack;
    return ErrorObject<GC>(data);
}

template<typename GC>
ErrorObject<GC> ErrorObject<GC>::from_info(
    Activation<GC>* activation,
    ClassObject<GC> class_obj,
    AvmString<GC> message,
    uint32_t error_id) {
    auto allocated_object = create(activation, class_obj);
    
    auto arguments = std::array<Value<GC>, 2>{message.into(), Value<GC>::from(error_id)};
    
    class_obj.call_init(
        Object<GC>(allocated_object),
        function::FunctionArgs<GC>::from_slice(arguments),
        activation
    );
    
    return allocated_object;
}

template<typename GC>
string::WString ErrorObject<GC>::display() const {
    // Note: This would need access to error_slots for NAME and MESSAGE
    // Simplified implementation for now
    return string::WString::from_utf8("Error");
}

template<typename GC>
string::WString ErrorObject<GC>::display_full() const {
    auto output = display();
    call_stack().display(&output);
    return output;
}

template<typename GC>
Object<GC> error_allocator(
    ClassObject<GC> class_obj,
    Activation<GC>* activation) {
    auto error_object = ErrorObject<GC>::create(activation, class_obj);
    return Object<GC>(error_object);
}

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_OBJECT_ERROR_OBJECT_H
