/*
 * C++ header for AVM1 runtime functionality
 * This replaces the functionality of core/src/avm1/runtime.rs
 */

#ifndef AVM1_RUNTIME_H
#define AVM1_RUNTIME_H

#include "avm1.h"
#include "avm1/value.h"
#include "avm1/object.h"
#include "avm1/activation.h"
#include "avm1/error.h"
#include "avm1/scope.h"
#include "avm1/property_map.h"
#include "avm1/globals.h"
#include "avm1/globals/as_broadcaster.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace ruffle {

// Forward declarations
class Avm1;
class UpdateContext;
class DisplayObject;
class MovieClip;
class SwfSlice;

// The global environment.
//
// Because SWFs v6 and v7+ use different case-sensitivity rules, Flash
// keeps two environments, one case-sensitive, the other not (for an
// example, see the `global_swf6_7_8` test).
class GlobalEnv {
private:
    std::shared_ptr<Scope> global_scope_;
    SystemPrototypes prototypes_;
    BroadcasterFunctions broadcaster_functions_;
    PropertyMap<std::shared_ptr<Object>> constructor_registry_;

public:
    explicit GlobalEnv(std::shared_ptr<Scope> global_scope,
                      SystemPrototypes prototypes,
                      BroadcasterFunctions broadcaster_functions)
        : global_scope_(std::move(global_scope)),
          prototypes_(std::move(prototypes)),
          broadcaster_functions_(std::move(broadcaster_functions)) {}

    // Create a new GlobalEnv
    static GlobalEnv create(std::shared_ptr<StringContext> context) {
        auto [prototypes, globals, broadcaster_functions] = create_globals(context);
        auto global_scope = std::make_shared<Scope>(Scope::from_global_object(globals));
        
        return GlobalEnv(std::move(global_scope), 
                        std::move(prototypes), 
                        std::move(broadcaster_functions));
    }

    // Getters
    std::shared_ptr<Scope> global_scope() const { return global_scope_; }
    const SystemPrototypes& prototypes() const { return prototypes_; }
    const BroadcasterFunctions& broadcaster_functions() const { return broadcaster_functions_; }
    PropertyMap<std::shared_ptr<Object>>& constructor_registry() { return constructor_registry_; }
    const PropertyMap<std::shared_ptr<Object>>& constructor_registry() const { return constructor_registry_; }
};

// AVM1 runtime class
class Avm1 {
private:
    GlobalEnv global_env_;
    GlobalEnv global_env_swf6_;  // Separate environment for SWF6 (case-insensitive)
    bool halted_;
    bool show_debug_output_;
    std::vector<std::shared_ptr<Activation>> active_activations_;
    int max_recursion_depth_;
    int max_execution_units_;
    bool debug_output_;

public:
    Avm1(std::shared_ptr<StringContext> context)
        : global_env_(GlobalEnv::create(context)),
          global_env_swf6_(GlobalEnv::create(context)),
          halted_(false),
          show_debug_output_(false),
          max_recursion_depth_(256),
          max_execution_units_(1000000),
          debug_output_(false) {}

    // Get the global scope for the current SWF version
    std::shared_ptr<Scope> global_scope(int swf_version) const {
        if (swf_version <= 6) {
            return global_env_swf6_.global_scope();
        } else {
            return global_env_.global_scope();
        }
    }

    // Get the system prototypes
    const SystemPrototypes& prototypes() const {
        return global_env_.prototypes();
    }

    // Get the broadcaster functions
    const BroadcasterFunctions& broadcaster_functions() const {
        return global_env_.broadcaster_functions();
    }

    // Check if the AVM is halted
    bool is_halted() const { return halted_; }

    // Halt the AVM
    void halt() { halted_ = true; }

    // Resume execution
    void resume() { halted_ = false; }

    // Check if debug output should be shown
    bool show_debug_output() const { return show_debug_output_; }

    // Set whether debug output should be shown
    void set_show_debug_output(bool show) { show_debug_output_ = show; }

    // Register a constructor for a symbol name
    void register_constructor(const std::string& name, 
                             std::shared_ptr<Object> constructor, 
                             bool case_sensitive, 
                             int swf_version) {
        if (swf_version <= 6) {
            global_env_swf6_.constructor_registry().insert(name, constructor, case_sensitive);
        } else {
            global_env_.constructor_registry().insert(name, constructor, case_sensitive);
        }
    }

    // Get a registered constructor
    std::optional<std::shared_ptr<Object>> get_registered_constructor(
        const std::string& name, 
        bool case_sensitive, 
        int swf_version) const {
        if (swf_version <= 6) {
            return global_env_swf6_.constructor_registry().get(name, case_sensitive);
        } else {
            return global_env_.constructor_registry().get(name, case_sensitive);
        }
    }

    // Execute an action script
    std::shared_ptr<Value> execute_action(
        std::shared_ptr<SwfSlice> action_data,
        std::shared_ptr<MovieClip> target_clip,
        std::shared_ptr<UpdateContext> context) {
        
        if (halted_) {
            return nullptr;
        }

        // Create an activation for execution
        auto activation = std::make_shared<Activation>(
            context, 
            global_scope(context->swf_version()), 
            target_clip, 
            target_clip->object1(), 
            nullptr,  // No function for top-level execution
            ActivationIdentifier(0, "top_level_action"));

        // Add to active activations
        active_activations_.push_back(activation);

        try {
            // Execute the action script
            // This would involve parsing and executing the SWF bytecode
            // For now, we'll return undefined
            return std::make_shared<Value>(Value::UNDEFINED);
        } catch (const Avm1Exception& e) {
            // Handle the error with the root error handler
            root_error_handler(activation, e.get_error());
            return std::make_shared<Value>(Value::UNDEFINED);
        } finally {
            // Remove from active activations
            active_activations_.erase(
                std::remove(active_activations_.begin(), active_activations_.end(), activation),
                active_activations_.end());
        }
    }

    // Get the maximum recursion depth
    int max_recursion_depth() const { return max_recursion_depth_; }

    // Set the maximum recursion depth
    void set_max_recursion_depth(int depth) { max_recursion_depth_ = depth; }

    // Get the maximum execution units
    int max_execution_units() const { return max_execution_units_; }

    // Set the maximum execution units
    void set_max_execution_units(int units) { max_execution_units_ = units; }

    // Wait for frame action (used by AVM1 opcodes)
    void action_wait_for_frame(std::shared_ptr<Activation> activation, 
                              std::shared_ptr<Reader> reader, 
                              int num_actions_to_skip) {
        // Skip the specified number of actions
        skip_actions(reader, static_cast<uint8_t>(num_actions_to_skip));
    }

    // Wait for frame action (alternative version)
    void action_wait_for_frame_2(std::shared_ptr<Activation> activation, 
                                std::shared_ptr<Reader> reader, 
                                int num_actions_to_skip) {
        // Skip the specified number of actions
        skip_actions(reader, static_cast<uint8_t>(num_actions_to_skip));
    }
};

// Utility function used by Avm1::action_wait_for_frame and Avm1::action_wait_for_frame_2
inline void skip_actions(std::shared_ptr<Reader> reader, uint8_t num_actions_to_skip) {
    for (int i = 0; i < num_actions_to_skip; ++i) {
        try {
            reader->read_action();
        } catch (const std::exception& e) {
            // Log warning about skipping action
            printf("Warning: Couldn't skip action: %s\n", e.what());
        }
    }
}

// Root error handler for uncaught exceptions
inline void root_error_handler(std::shared_ptr<Activation> activation, Avm1Error error) {
    switch (error.type()) {
        case Avm1ErrorType::THROWN_VALUE: {
            printf("Uncaught AVM1 error: %s\n", error.message().c_str());

            // Convert the value to string for display
            std::string string_repr = "[type Object]"; // Default for objects
            if (auto value = error.thrown_value()) {
                string_repr = value->to_string(activation);
            }

            activation->context()->avm_warning("Uncaught exception: " + string_repr);
            
            // Continue execution without halting
            return;
        }
        case Avm1ErrorType::INVALID_SWF: {
            printf("SWF Error: %s\n", error.swf_error_message().c_str());
            break;
        }
        default: {
            printf("Error: %s\n", error.message().c_str());
            break;
        }
    }
    
    // Halt the AVM on critical errors
    activation->context()->avm1.halt();
}

} // namespace ruffle

#endif // AVM1_RUNTIME_H