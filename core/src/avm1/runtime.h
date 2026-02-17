// C++ translation of core/src/avm1/runtime.rs
// AVM1 Runtime Environment

#ifndef RUFFLE_CORE_AVM1_RUNTIME_H
#define RUFFLE_CORE_AVM1_RUNTIME_H

#include <vector>
#include <array>
#include <optional>
#include <memory>
#include <cstdint>
#include <string>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC>
    class UpdateContext;
    template<typename GC>
    class StringContext;

    namespace avm1 {
        template<typename GC>
        class Activation;
        template<typename GC>
        class Value;
        template<typename GC>
        class Object;
        template<typename GC>
        class Scope;
        template<typename GC>
        class PropertyMap;

        namespace function {
            enum class ExecutionReason;
            template<typename GC>
            struct BroadcasterFunctions;
        }

        namespace globals {
            template<typename GC>
            struct SystemPrototypes;
        }

        namespace object {
            namespace stage_object {
                template<typename GC>
                class DisplayPropertyMap;
            }
        }

        namespace scope {
            enum class ScopeClass;
        }

        struct ActivationIdentifier;
        template<typename GC>
        class Error;
        template<typename GC>
        class AvmString;

        // Skip actions helper
        void skip_actions(void* reader, uint8_t num_actions_to_skip);
    }

    namespace display_object {
        template<typename GC>
        class DisplayObject;
        template<typename GC>
        class MovieClip;
        template<typename GC>
        class TDisplayObject;
        template<typename GC>
        class TDisplayObjectContainer;
    }

    namespace tag_utils {
        template<typename GC>
        class SwfSlice;
    }

    namespace frame_lifecycle {
        enum class FramePhase;
    }

    namespace gc_arena {
        template<typename T>
        class Gc;
        class Mutation;
        class Collect;
    }
}
}

namespace swf {
namespace avm1 {
namespace read {
    class Reader;
}
}
}

namespace tracing {
    void error(const std::string& message);
    void warn(const std::string& message);
}

namespace ruffle {
namespace core {

using gc_arena::Gc;
using gc_arena::Mutation;

namespace avm1 {

using display_object::DisplayObject;
using display_object::MovieClip;
using display_object::TDisplayObject;
using display_object::TDisplayObjectContainer;
using tag_utils::SwfSlice;
using frame_lifecycle::FramePhase;
using scope::ScopeClass;
using function::ExecutionReason;
using function::BroadcasterFunctions;
using globals::SystemPrototypes;
using object::stage_object::DisplayPropertyMap;

/// Debug macro for AVM1
#ifdef AVM_DEBUG
    #define avm_debug(avm, ...) tracing_debug(__VA_ARGS__)
#else
    #define avm_debug(avm, ...)
#endif

/// The global environment.
///
/// Because SWFs v6 and v7+ use different case-sensitivity rules, Flash
/// keeps two environments, one case-sensitive, the other not.
template<typename GC>
class GlobalEnv {
private:
    /// The global scope (pre-allocated so that it can be reused by fresh Activations).
    Gc<GC, Scope<GC>> global_scope_;

    /// System built-ins that we use internally to construct new objects.
    SystemPrototypes<GC> prototypes_;

    /// Cached functions for the AsBroadcaster.
    BroadcasterFunctions<GC> broadcaster_functions_;

    /// The mappings between symbol names and constructors registered
    /// with Object.registerClass(). This is either case-sensitive or case-insensitive.
    PropertyMap<GC, Object<GC>> constructor_registry_;

public:
    GlobalEnv() = default;

    /// Create a new global environment
    static GlobalEnv<GC> create(StringContext<GC>* context);

    Gc<GC, Scope<GC>> global_scope() const { return global_scope_; }
    const SystemPrototypes<GC>& prototypes() const { return prototypes_; }
    BroadcasterFunctions<GC> broadcaster_functions() const { return broadcaster_functions_; }
    PropertyMap<GC, Object<GC>>& constructor_registry() { return constructor_registry_; }
    const PropertyMap<GC, Object<GC>>& constructor_registry() const { return constructor_registry_; }
};

/// The AVM1 runtime state.
template<typename GC>
class Avm1 {
private:
    /// The Flash Player version we're emulating.
    uint8_t player_version_;

    /// The constant pool to use for new activations from code sources that
    /// don't close over the constant pool they were defined with.
    Gc<GC, std::vector<Value<GC>>> constant_pool_;

    /// The global environment, dependent on the ambient SWF version.
    GlobalEnv<GC> env_case_sensitive_;
    GlobalEnv<GC> env_case_insensitive_;

    /// DisplayObject property map.
    DisplayPropertyMap<GC> display_properties_;

    /// The operand stack (shared across functions).
    std::vector<Value<GC>> stack_;

    /// The register slots (also shared across functions).
    std::array<Value<GC>, 4> registers_;

    /// If a serious error has occurred, or a user has requested it, the AVM may be halted.
    bool halted_;

    /// The maximum amount of functions that can be called before a recursion limit error.
    uint16_t max_recursion_depth_;

    /// Whether a Mouse listener has been registered.
    bool has_mouse_listener_;

    /// The list of all movie clips in execution order.
    MovieClip<GC> clip_exec_list_;

    /// If getBounds/getRect is called on a MovieClip with invalid bounds and the
    /// target space is identical to the origin space, but the target is not the
    /// MovieClip itself, the call can return either the default invalid rectangle
    /// or a special invalid bounds rectangle.
    bool use_new_invalid_bounds_value_;

#ifdef AVM_DEBUG
public:
    bool debug_output_;
#endif

public:
    /// Construct a new AVM1 interpreter.
    Avm1(StringContext<GC>* context, uint8_t player_version);

    /// Add a stack frame that executes code in timeline scope.
    ///
    /// This creates a new frame stack.
    static void run_stack_frame_for_action(
        DisplayObject<GC> active_clip,
        const std::string& name,
        SwfSlice code,
        UpdateContext<GC>* context
    );

    /// Add a stack frame that executes code in initializer scope.
    ///
    /// This creates a new frame stack.
    template<typename F, typename R>
    static R run_with_stack_frame_for_display_object(
        DisplayObject<GC> active_clip,
        UpdateContext<GC>* action_context,
        F function
    );

    /// Add a stack frame that executes code in initializer scope.
    ///
    /// This creates a new frame stack.
    static void run_stack_frame_for_init_action(
        DisplayObject<GC> active_clip,
        SwfSlice code,
        UpdateContext<GC>* context
    );

    /// Add a stack frame that executes code in timeline scope for an object
    /// method, such as an event handler.
    ///
    /// This creates a new frame stack.
    static void run_stack_frame_for_method(
        DisplayObject<GC> active_clip,
        Object<GC> obj,
        AvmString<GC> name,
        const std::vector<Value<GC>>& args,
        UpdateContext<GC>* context
    );

    /// Notify system listeners of an event.
    static void notify_system_listeners(
        DisplayObject<GC> active_clip,
        AvmString<GC> broadcaster_name,
        AvmString<GC> method,
        const std::vector<Value<GC>>& args,
        UpdateContext<GC>* context
    );

    /// Returns true if the Mouse object has a listener registered.
    bool has_mouse_listener() const { return has_mouse_listener_; }

    /// Halts the AVM, preventing execution of any further actions.
    void halt();

    /// Get the stack length.
    size_t stack_len() const { return stack_.size(); }

    /// Resets the operand stack and the global registers.
    void clear();

    /// Push a value onto the stack.
    void push(Value<GC> value);

    /// Pop a value from the stack.
    Value<GC> pop();

    /// Check if case sensitivity is enabled for the given SWF version.
    static constexpr bool is_case_sensitive(uint8_t swf_version) {
        return swf_version >= 7;
    }

    /// Obtain a reference to the global scope.
    Gc<GC, Scope<GC>> global_scope(uint8_t swf_version) const;

    /// Obtain system built-in prototypes for this instance.
    const SystemPrototypes<GC>& prototypes(uint8_t swf_version) const;

    /// Obtains the constant pool to use for new activations.
    Gc<GC, std::vector<Value<GC>>> constant_pool() const { return constant_pool_; }

    /// Sets the constant pool to use for new activations.
    void set_constant_pool(Gc<GC, std::vector<Value<GC>>> constant_pool);

    /// DisplayObject property map.
    const DisplayPropertyMap<GC>& display_properties() const { return display_properties_; }

    /// Get the maximum recursion depth.
    uint16_t max_recursion_depth() const { return max_recursion_depth_; }

    /// Set the maximum recursion depth.
    void set_max_recursion_depth(uint16_t max_recursion_depth);

    /// Get broadcaster functions for the given SWF version.
    BroadcasterFunctions<GC> broadcaster_functions(uint8_t swf_version) const;

    /// The Flash Player version we're emulating.
    uint8_t player_version() const { return player_version_; }

    /// Get a register by ID.
    const Value<GC>* get_register(size_t id) const;

    /// Get a mutable reference to a register by ID.
    Value<GC>* get_register_mut(size_t id);

    /// Run a single frame.
    static void run_frame(UpdateContext<GC>* context);

    /// Adds a movie clip to the execution list.
    void add_to_exec_list(Mutation<GC>* gc_context, MovieClip<GC> clip);

    /// Get a registered constructor.
    std::optional<Object<GC>> get_registered_constructor(
        uint8_t swf_version,
        AvmString<GC> symbol
    ) const;

    /// Register a constructor.
    void register_constructor(
        uint8_t swf_version,
        AvmString<GC> symbol,
        std::optional<Object<GC>> constructor
    );

    /// Returns use_new_invalid_bounds_value.
    bool get_use_new_invalid_bounds_value() const { return use_new_invalid_bounds_value_; }

    /// Sets use_new_invalid_bounds_value to true.
    void activate_use_new_invalid_bounds_value() { use_new_invalid_bounds_value_ = true; }

#ifdef AVM_DEBUG
    bool show_debug_output() const { return debug_output_; }
    void set_show_debug_output(bool visible) { debug_output_ = visible; }
#else
    constexpr bool show_debug_output() const { return false; }
    constexpr void set_show_debug_output(bool) {}
#endif

private:
    /// Find all display objects with negative depth recursively.
    static void find_display_objects_pending_removal(
        DisplayObject<GC> obj,
        std::vector<DisplayObject<GC>>* out
    );

    /// Remove all display objects pending removal.
    static void remove_pending(UpdateContext<GC>* context);
};

/// Root error handler for AVM1.
template<typename GC>
void root_error_handler(
    Activation<GC>* activation,
    Error<GC> error
);

} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_RUNTIME_H
