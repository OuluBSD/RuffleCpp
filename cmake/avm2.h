/*
 * C++ header for AVM2 functionality
 * This replaces the functionality of core/src/avm2.rs
 */

#ifndef AVM2_H
#define AVM2_H

#include "avm1.h"
#include "avm2/value.h"
#include "avm2/object.h"
#include "avm2/class.h"
#include "avm2/function.h"
#include "avm2/activation.h"
#include "avm2/error.h"
#include "avm2/scope.h"
#include "avm2/script.h"
#include "avm2/stack.h"
#include "avm2/call_stack.h"
#include "avm2/domain.h"
#include "avm2/e4x.h"
#include "avm2/bytearray.h"
#include "avm2/api_version.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

namespace ruffle {

// Forward declarations
class PlayerRuntime;
class UpdateContext;
class SwfMovie;
class MovieClip;
class EventObject;
class QName;
class Namespace;
class Multiname;
class TranslationUnit;
class CommonNamespaces;

// Broadcast whitelist for events
constexpr const char* BROADCAST_WHITELIST[] = {
    "enterFrame", "exitFrame", "frameConstructed", "render"
};

// The state of an AVM2 interpreter.
class Avm2 {
private:
    // The Flash Player version we're emulating.
    uint8_t player_version_;

    // The player runtime we're emulating
    PlayerRuntime player_runtime_;

    // Values currently present on the operand stack.
    Stack stack_;

    // Scopes currently present on the scope stack.
    std::vector<std::shared_ptr<Scope>> scope_stack_;

    // The current call stack of the player.
    CallStack call_stack_;

    // This domain is used exclusively for classes from playerglobals
    std::shared_ptr<Domain> playerglobals_domain_;

    // The domain associated with 'stage.loaderInfo.applicationDomain'.
    // Note that this is a parent of the root movie clip's domain
    // (which can be observed from ActionScript)
    std::shared_ptr<Domain> stage_domain_;

    // System classes.
    std::optional<SystemClasses> system_classes_;

    // System class definitions.
    std::optional<SystemClassDefs> system_class_defs_;

    // Top-level global object. It contains most top-level types (Object, Class) and functions.
    // However, it's not strictly defined which items end up there.
    std::shared_ptr<Object> toplevel_global_object_;

    // Pre-created known namespaces.
    std::shared_ptr<CommonNamespaces> namespaces_;

    // Native method tables
    std::vector<std::optional<NativeMethodImpl>> native_method_table_;
    std::vector<std::optional<AllocatorFn>> native_instance_allocator_table_;
    std::vector<std::optional<NativeMethodImpl>> native_call_handler_table_;
    std::vector<std::optional<CustomConstructorFn>> native_custom_constructor_table_;
    std::vector<size_t> native_fast_call_list_;

    // A list of objects which are capable of receiving broadcasts.
    // Certain types of events are "broadcast events" that are emitted on all
    // constructed objects in order of their creation, whether or not they are
    // currently present on the display list. This list keeps track of that.
    std::unordered_map<std::string, std::vector<std::weak_ptr<Object>>> broadcast_list_;

    std::unordered_map<std::string, std::shared_ptr<ClassObject>> alias_to_class_map_;
    std::unordered_map<std::shared_ptr<Class>, std::string> class_to_alias_map_;

    XmlSettings xml_settings_;
    ObjectEncoding default_bytearray_encoding_;

    // The api version of our root movie clip. Note - this is used as the
    // api version for swfs loaded via `Loader`, overriding the api version
    // specified in the loaded SWF. This is only used for API versioning (hiding
    // definitions from playerglobals) - version-specific behavior in things like
    // `gotoAndPlay` uses the current movie clip's SWF version.
    ApiVersion root_api_version_;

    bool debug_output_;
    bool optimizer_enabled_;

public:
    // Constructor
    Avm2(uint8_t player_version, PlayerRuntime player_runtime) 
        : player_version_(player_version), player_runtime_(player_runtime),
          stack_(), call_stack_(), debug_output_(false), optimizer_enabled_(true),
          xml_settings_(XmlSettings::new_default()), 
          default_bytearray_encoding_(ObjectEncoding::AMF3),
          root_api_version_(ApiVersion::ALL_VERSIONS) {
        
        // Initialize domains
        playerglobals_domain_ = std::make_shared<Domain>(/* uninitialized */);
        stage_domain_ = std::make_shared<Domain>(playerglobals_domain_);
        
        // Initialize namespaces
        namespaces_ = std::make_shared<CommonNamespaces>();
    }

    // Static factory method
    static std::shared_ptr<Avm2> create(uint8_t player_version, PlayerRuntime player_runtime) {
        return std::make_shared<Avm2>(player_version, player_runtime);
    }

    // Load player globals
    static void load_player_globals(std::shared_ptr<UpdateContext> context) {
        auto globals = context->avm2->playerglobals_domain();
        // In a real implementation, this would load playerglobal
        // globals::load_playerglobal(context, globals);
    }

    // Getters
    std::shared_ptr<Domain> playerglobals_domain() const { return playerglobals_domain_; }
    std::shared_ptr<Domain> stage_domain() const { return stage_domain_; }
    
    // Return the current set of system classes.
    // This function panics if the interpreter has not yet been initialized.
    const SystemClasses& classes() const {
        if (!system_classes_.has_value()) {
            throw std::runtime_error("System classes not initialized");
        }
        return system_classes_.value();
    }

    // Return the current set of system class definitions.
    // This function panics if the interpreter has not yet been initialized.
    const SystemClassDefs& class_defs() const {
        if (!system_class_defs_.has_value()) {
            throw std::runtime_error("System class definitions not initialized");
        }
        return system_class_defs_.value();
    }

    std::shared_ptr<Object> toplevel_global_object() const { return toplevel_global_object_; }
    const XmlSettings& xml_settings() const { return xml_settings_; }
    ObjectEncoding default_bytearray_encoding() const { return default_bytearray_encoding_; }
    ApiVersion root_api_version() const { return root_api_version_; }
    bool show_debug_output() const { return debug_output_; }
    bool optimizer_enabled() const { return optimizer_enabled_; }

    // Register class alias
    void register_class_alias(const std::string& name, std::shared_ptr<ClassObject> class_object) {
        alias_to_class_map_[name] = class_object;
        class_to_alias_map_[class_object->inner_class_definition()] = name;
    }

    // Get class by alias
    std::shared_ptr<ClassObject> get_class_by_alias(const std::string& name) const {
        auto it = alias_to_class_map_.find(name);
        if (it != alias_to_class_map_.end()) {
            return it->second;
        }
        return nullptr;
    }

    // Get alias by class
    std::string get_alias_by_class(std::shared_ptr<Class> cls) const {
        auto it = class_to_alias_map_.find(cls);
        if (it != class_to_alias_map_.end()) {
            return it->second;
        }
        return "";
    }

    // Run a script's initializer method
    static bool run_script_initializer(std::shared_ptr<Script> script, 
                                      std::shared_ptr<UpdateContext> context) {
        auto [method, global_object, domain] = script->init();

        auto scope = ScopeChain::new_scope(domain);
        // Script `global` classes extend Object
        auto bound_superclass = context->avm2->classes().object;

        // Provide a callee object if necessary
        std::shared_ptr<FunctionObject> callee = nullptr;
        if (method->needs_arguments_object()) {
            callee = FunctionObject::from_method(context, method, scope, 
                                               std::make_shared<Value>(global_object), 
                                               bound_superclass);
        }

        // Create activation
        auto activation = Activation::from_nothing(context);

        return exec(method, scope, std::make_shared<Value>(global_object), 
                   bound_superclass, FunctionArgs::empty(), activation, callee);
    }

    // Dispatch an event on an object
    static bool dispatch_event(std::shared_ptr<UpdateContext> context,
                              std::shared_ptr<EventObject> event,
                              std::shared_ptr<Object> target) {
        return dispatch_event_internal(context, event, target, false);
    }

    // Simulate dispatching an event
    static bool simulate_event_dispatch(std::shared_ptr<UpdateContext> context,
                                       std::shared_ptr<EventObject> event,
                                       std::shared_ptr<Object> target) {
        return dispatch_event_internal(context, event, target, true);
    }

private:
    static bool dispatch_event_internal(std::shared_ptr<UpdateContext> context,
                                       std::shared_ptr<EventObject> event,
                                       std::shared_ptr<Object> target,
                                       bool simulate_dispatch) {
        auto activation = Activation::from_nothing(context);
        return events::dispatch_event(activation, target, event, simulate_dispatch);
    }

public:
    // Add an object to the broadcast list
    void register_broadcast_listener(std::shared_ptr<UpdateContext> context,
                                   std::shared_ptr<Object> object,
                                   const std::string& event_name) {
        // Check if event is in the broadcast whitelist
        bool is_broadcast_event = false;
        for (const char* whitelist_event : BROADCAST_WHITELIST) {
            if (event_name == whitelist_event) {
                is_broadcast_event = true;
                break;
            }
        }
        
        if (!is_broadcast_event) {
            return;
        }

        auto& bucket = broadcast_list_[event_name];

        // Check if object is already registered
        for (const auto& entry : bucket) {
            auto locked_obj = entry.lock();
            if (locked_obj && locked_obj == object) {
                return; // Already registered
            }
        }

        bucket.push_back(object); // Add weak reference
    }

    // Broadcast an event on all objects in the current execution list
    void broadcast_event(std::shared_ptr<UpdateContext> context,
                        std::shared_ptr<EventObject> event,
                        std::shared_ptr<ClassObject> on_type) {
        auto event_name = event->event_type();

        // Check if event is in the broadcast whitelist
        bool is_broadcast_event = false;
        for (const char* whitelist_event : BROADCAST_WHITELIST) {
            if (event_name == whitelist_event) {
                is_broadcast_event = true;
                break;
            }
        }
        
        if (!is_broadcast_event) {
            return;
        }

        auto& bucket = broadcast_list_[event_name];
        size_t el_length = bucket.size();

        for (size_t i = 0; i < el_length; ++i) {
            if (auto object = bucket[i].lock()) {
                if (object->is_of_type(on_type->inner_class_definition())) {
                    auto activation = Activation::from_nothing(context);
                    events::broadcast_event(activation, object, event);
                }
            }
        }
        
        // Remove dead weak references
        bucket.erase(
            std::remove_if(bucket.begin(), bucket.end(),
                          [](const std::weak_ptr<Object>& obj) { return obj.expired(); }),
            bucket.end());
    }

    // Lookup class for character
    static std::shared_ptr<ClassObject> lookup_class_for_character(
        std::shared_ptr<Activation> activation,
        std::shared_ptr<MovieClip> movie_clip,
        std::shared_ptr<Domain> domain,
        const std::string& name,
        uint16_t id) {
        auto movie = movie_clip->movie();

        auto class_object = domain->get_defined_value_handling_vector(activation, name)
                               .as_object()
                               .as_class_object();
        
        if (!class_object) {
            // In a real implementation, this would throw an error
            // make_error_1014(activation, Error1014Type::ReferenceError, name);
            return nullptr;
        }

        auto class_def = class_object->inner_class_definition();

        // In a real implementation, this would check the library for the character
        // and validate class inheritance

        return class_object;
    }

    // Load an ABC file embedded in a DoAbc or DoAbc2 tag
    static std::optional<std::shared_ptr<Script>> do_abc(
        std::shared_ptr<UpdateContext> context,
        const std::vector<uint8_t>& data,
        std::optional<std::string> name,
        uint32_t flags,  // DoAbc2Flag equivalent
        std::shared_ptr<Domain> domain,
        std::shared_ptr<SwfMovie> movie) {
        
        // In a real implementation, this would parse the ABC data
        // For now, return nullopt
        return std::nullopt;
    }

    // Load the playerglobal ABC file
    static void load_builtin_abc(
        std::shared_ptr<UpdateContext> context,
        const std::vector<uint8_t>& data,
        std::shared_ptr<Domain> domain,
        std::shared_ptr<SwfMovie> movie) {
        
        // In a real implementation, this would load the builtin ABC file
        // This involves parsing the ABC, loading classes, and initializing the runtime
    }

    // Pushes an executable on the call stack
    void push_call(std::shared_ptr<Method> method) {
        call_stack_.push(method);
    }

    // Pops an executable off the call stack
    void pop_call() {
        call_stack_.pop();
    }

    const CallStack& call_stack() const { return call_stack_; }
    CallStack capture_call_stack() const { return call_stack_; }

    // Scope stack operations
    void push_scope(std::shared_ptr<Scope> scope) {
        scope_stack_.push_back(scope);
    }

    void pop_scope() {
        if (!scope_stack_.empty()) {
            scope_stack_.pop_back();
        }
    }

    void set_show_debug_output(bool visible) { debug_output_ = visible; }
    void set_optimizer_enabled(bool value) { optimizer_enabled_ = value; }

    // Gets the public namespace, versioned based on the current root SWF
    Namespace find_public_namespace() const {
        return namespaces_->public_for(root_api_version_);
    }

    // Report an uncaught AVM2 error
    static void uncaught_error(std::shared_ptr<Activation> activation,
                              std::optional<std::shared_ptr<Object>> display_object,
                              Avm2Error error,
                              const std::string& info) {
        printf("Uncaught AVM2 error: %s - %s\n", info.c_str(), error.message().c_str());
        // In a real implementation, this would push the error onto loaderInfo.uncaughtErrorEvents
    }
};

} // namespace ruffle

#endif // AVM2_H