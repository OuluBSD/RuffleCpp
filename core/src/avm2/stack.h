// C++ translation of core/src/avm2/stack.rs
//! Internal representation of the AVM2 value stack

#ifndef RUFFLE_CORE_AVM2_STACK_H
#define RUFFLE_CORE_AVM2_STACK_H

#include <vector>
#include <cell>
#include <cstddef>
#include <cassert>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Value;
    template<typename T> class Method;
    template<typename T> class Gc;
    template<typename T> class Mutation;
}
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Preallocated stack size for AVM2 execution
constexpr size_t PREALLOCATED_STACK_SIZE = 200000;

/// Cell wrapper for interior mutability
template<typename T>
class Cell {
public:
    Cell() : value_(T()) {}
    explicit Cell(const T& value) : value_(value) {}

    T get() const { return value_; }
    void set(const T& value) { value_ = value; }

private:
    T value_;
};

/// Data structure for Stack - holds the actual stack storage
template<typename GC>
class StackData {
public:
    std::vector<Cell<Value<GC>>> stack;
    Cell<size_t> stack_pointer;

    StackData()
        : stack(PREALLOCATED_STACK_SIZE, Cell<Value<GC>>(Value<GC>::undefined()))
        , stack_pointer(0)
    {}
};

/// StackFrame - A stack frame for a particular method
/// Despite its name, this stores both method locals and method stack
template<typename GC>
class StackFrame {
public:
    StackFrame()
        : data_(nullptr)
        , size_(0)
        , stack_pointer_(0)
    {}

    /// Create an empty stack frame
    static StackFrame empty() {
        return StackFrame();
    }

    /// Push a value onto the operand stack
    void push(const Value<GC>& value) {
        data_[stack_pointer_++].set(value);
    }

    /// Retrieve the top-most value on the operand stack
    Value<GC> pop() {
        return data_[--stack_pointer_].get();
    }

    /// Peek the n-th value from the end of the operand stack
    Value<GC> peek(size_t index) const {
        return data_[stack_pointer_ - index - 1].get();
    }

    /// Get the top of the stack
    Cell<Value<GC>>& stack_top() {
        return data_[stack_pointer_ - 1];
    }

    /// Get value at a specific index
    Value<GC> value_at(size_t index) const {
        return data_[index].get();
    }

    /// Set value at a specific index
    void set_value_at(size_t index, const Value<GC>& value) {
        data_[index].set(value);
    }

    /// Pop multiple arguments from the stack
    Cell<Value<GC>>* pop_args(uint32_t arg_count) {
        size_t base = stack_pointer_ - arg_count;
        stack_pointer_ = base;
        return &data_[base];
    }

    /// Set the stack pointer
    void set_stack_pointer(size_t size) {
        stack_pointer_ = size;
    }

    /// Get the number of entries currently on the stack
    size_t len() const {
        return stack_pointer_;
    }

    /// Take ownership of this frame, leaving it empty
    StackFrame take() {
        StackFrame new_frame;
        new_frame.data_ = data_;
        new_frame.size_ = size_;
        new_frame.stack_pointer_ = stack_pointer_;

        data_ = nullptr;
        size_ = 0;
        stack_pointer_ = 0;

        return new_frame;
    }

    /// Get the underlying data slice
    Cell<Value<GC>>* data() { return data_; }
    size_t size() const { return size_; }

private:
    Cell<Value<GC>>* data_;
    size_t size_;
    size_t stack_pointer_;

    friend class Stack<GC>;

    static StackFrame for_data(Cell<Value<GC>>* data, size_t size) {
        StackFrame frame;
        frame.data_ = data;
        frame.size_ = size;
        frame.stack_pointer_ = 0;
        return frame;
    }
};

/// The global, preallocated value stack
/// This has little use directly due to the way AVM2 works; to do anything with it,
/// a StackFrame must be obtained first through the get_stack_frame method.
///
/// We use this instead of a std::vector<Value> to allow for obtaining "mutable"
/// (via interior mutability) references to the stack without requiring a mutable
/// borrow on the stack or the Avm2.
template<typename GC>
class Stack {
public:
    Stack() = default;

    explicit Stack(Gc<GC, StackData<GC>> data) : data_(data) {}

    /// Create a new stack
    static Stack create(Mutation<GC>* mc) {
        auto stack_data = std::make_unique<StackData<GC>>();
        return Stack(Gc<GC, StackData<GC>>::create(mc, std::move(stack_data)));
    }

    /// Returns a slice of stack data for the specified method, starting at the
    /// current stack pointer. Stack frames obtained from this method must be
    /// properly disposed of by using the dispose_stack_frame method.
    StackFrame<GC> get_stack_frame(const Method<GC>& method) {
        // First calculate the frame size
        auto body = method.body();
        assert(body.has_value() && "Cannot execute non-native method without body");

        size_t frame_size = body->max_stack + body->num_locals;

        // Then actually create the stack frame
        auto stack_data = data_->stack.data();
        auto stack_pointer = data_->stack_pointer.get();

        auto subslice = &stack_data[stack_pointer];

        data_->stack_pointer.set(stack_pointer + frame_size);

        // Ensure the StackFrame returned to the caller does not have any
        // old values on it, as these may contain Gc pointers that have already
        // been collected.
        for (size_t i = 0; i < frame_size; ++i) {
            subslice[i].set(Value<GC>::undefined());
        }

        return StackFrame<GC>::for_data(subslice, frame_size);
    }

    /// Dispose of a stack frame, returning its space to the stack
    void dispose_stack_frame(const StackFrame<GC>& stack_frame) {
        data_->stack_pointer.set(
            data_->stack_pointer.get() - stack_frame.len()
        );
    }

private:
    Gc<GC, StackData<GC>> data_;
};

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_STACK_H
