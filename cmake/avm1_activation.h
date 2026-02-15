/*
 * C++ header for AVM1 Activation functionality
 * This replaces the functionality of core/src/avm1/activation.rs
 */

#ifndef AVM1_ACTIVATION_H
#define AVM1_ACTIVATION_H

#include "avm1.h"
#include "avm1/value.h"
#include "avm1/object.h"
#include "avm1/scope.h"
#include "avm1/error.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

namespace ruffle {

// Enum for return types in AVM1
enum class ReturnType {
    IMPLICIT,
    EXPLICIT
};

// Frame control options
enum class FrameControl {
    CONTINUE,
    RETURN,
    JUMP
};

// Forward declarations
class Avm1Function;
class DisplayObject;
class MovieClip;
class SwfSlice;
class UpdateContext;

// Activation class for AVM1 execution context
class Activation {
private:
    // Context and state information
    std::shared_ptr<UpdateContext> context_;
    std::shared_ptr<Scope> scope_;
    std::shared_ptr<MovieClip> base_clip_;
    std::shared_ptr<MovieClip> target_clip_;
    std::shared_ptr<Object> this_object_;
    std::shared_ptr<Object> callee_object_;
    std::vector<std::shared_ptr<Value>> stack_;
    std::vector<std::shared_ptr<Value>> registers_;
    std::shared_ptr<SwfSlice> action_data_;
    std::shared_ptr<Avm1Function> function_;
    ActivationIdentifier id_;
    
    // Flags and state
    bool is_executing_;
    bool show_debug_output_;
    int recursion_depth_;

public:
    // Constructor
    Activation(std::shared_ptr<UpdateContext> context,
               std::shared_ptr<Scope> scope,
               std::shared_ptr<MovieClip> base_clip,
               std::shared_ptr<Object> this_object,
               std::shared_ptr<Avm1Function> function,
               ActivationIdentifier id)
        : context_(std::move(context))
        , scope_(std::move(scope))
        , base_clip_(std::move(base_clip))
        , target_clip_(base_clip_)
        , this_object_(std::move(this_object))
        , callee_object_(this_object_)
        , function_(std::move(function))
        , id_(id)
        , is_executing_(false)
        , show_debug_output_(false)
        , recursion_depth_(0) {
    }

    // Getters
    std::shared_ptr<UpdateContext> context() const { return context_; }
    std::shared_ptr<Scope> scope() const { return scope_; }
    std::shared_ptr<MovieClip> base_clip() const { return base_clip_; }
    std::shared_ptr<MovieClip> target_clip() const { return target_clip_; }
    std::shared_ptr<Object> this_object() const { return this_object_; }
    std::shared_ptr<Object> callee() const { return callee_object_; }
    std::shared_ptr<Avm1Function> function() const { return function_; }
    const ActivationIdentifier& id() const { return id_; }
    
    // Setters
    void set_scope(std::shared_ptr<Scope> scope) { scope_ = std::move(scope); }
    void set_target_clip(std::shared_ptr<MovieClip> clip) { target_clip_ = std::move(clip); }
    void set_this(std::shared_ptr<Object> obj) { this_object_ = std::move(obj); }
    void set_callee(std::shared_ptr<Object> obj) { callee_object_ = std::move(obj); }
    
    // Stack operations
    void push(std::shared_ptr<Value> value) { stack_.push_back(std::move(value)); }
    std::shared_ptr<Value> pop() {
        if (stack_.empty()) {
            return std::make_shared<Value>(Value::UNDEFINED);
        }
        auto value = stack_.back();
        stack_.pop_back();
        return value;
    }
    std::shared_ptr<Value> peek() const {
        if (stack_.empty()) {
            return std::make_shared<Value>(Value::UNDEFINED);
        }
        return stack_.back();
    }
    
    // Register operations
    void set_register(int index, std::shared_ptr<Value> value) {
        if (index >= static_cast<int>(registers_.size())) {
            registers_.resize(index + 1);
        }
        registers_[index] = std::move(value);
    }
    
    std::shared_ptr<Value> get_register(int index) const {
        if (index < 0 || index >= static_cast<int>(registers_.size())) {
            return std::make_shared<Value>(Value::UNDEFINED);
        }
        return registers_[index];
    }
    
    // Execution methods
    std::shared_ptr<Value> run_stack_frame_for_action(const std::string& action_name);
    std::shared_ptr<Value> run_with_data(std::shared_ptr<SwfSlice> data);
    
    // Target resolution
    std::optional<std::shared_ptr<Object>> resolve_target_path(
        std::shared_ptr<DisplayObject> root,
        std::shared_ptr<DisplayObject> start,
        const std::string& path,
        bool case_sensitive);
    
    // Set target for execution
    FrameControl set_target(const std::string& target);
    
    // Debugging
    bool show_debug_output() const { return show_debug_output_; }
    void set_show_debug_output(bool show) { show_debug_output_ = show; }
    
    // Recursion management
    int recursion_depth() const { return recursion_depth_; }
    void increment_recursion() { recursion_depth_++; }
    void decrement_recursion() { if (recursion_depth_ > 0) recursion_depth_--; }
    
    // Check if execution should continue
    bool should_continue_execution() const {
        return is_executing_ && recursion_depth_ < 256; // Prevent infinite recursion
    }
    
    // Get target clip or root if target is null
    std::shared_ptr<MovieClip> target_clip_or_root() const {
        return target_clip_ ? target_clip_ : base_clip_;
    }
    
    // Execute a function call
    std::shared_ptr<Value> execute_function(
        std::shared_ptr<Object> this_obj,
        const std::vector<std::shared_ptr<Value>>& args,
        ExecutionReason reason);
};

// Macro for AVM1 debugging (equivalent to avm_debug!)
#define AVM1_DEBUG(avm, ...) \
    do { \
        if (avm->show_debug_output()) { \
            printf("AVM1 Debug: " __VA_ARGS__); \
            printf("\n"); \
        } \
    } while(0)

} // namespace ruffle

#endif // AVM1_ACTIVATION_H