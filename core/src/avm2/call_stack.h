// C++ translation of core/src/avm2/call_stack.rs
//! AVM2 Call Stack for stack trace display

#ifndef RUFFLE_CORE_AVM2_CALL_STACK_H
#define RUFFLE_CORE_AVM2_CALL_STACK_H

#include <vector>
#include <string>

// Forward declarations
namespace ruffle {
namespace core {
    class WString;

    namespace avm2 {
        template<typename GC>
        class Method;
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// CallStack - Tracks the call stack for AVM2 execution
/// 
/// Used for generating stack traces when errors occur.
/// Collects methods as they are called and can display them
/// in a human-readable format.
/// 
/// \tparam GC The garbage collector type
template<typename GC>
class CallStack {
private:
    std::vector<Method<GC>> stack_;

public:
    /// Create a new empty call stack
    CallStack() = default;

    /// Push a method onto the call stack
    /// 
    /// \param method The method being called
    void push(Method<GC> method);

    /// Pop the most recent method from the call stack
    void pop();

    /// Display the call stack to a string
    /// 
    /// Formats the stack trace with method names and special handling
    /// for script initializers.
    /// 
    /// \param output The string to append the formatted stack trace to
    void display(WString* output) const;

    /// Check if the call stack is empty
    /// 
    /// \return true if no methods are on the stack
    bool is_empty() const {
        return stack_.empty();
    }
};

/// Display trait implementation for CallStack
/// 
/// Formats the call stack for output streams.
template<typename GC>
std::string to_string(const CallStack<GC>& stack);

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_CALL_STACK_H
