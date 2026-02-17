// C++ translation of core/src/avm2.rs
//! ActionScript Virtual Machine 2 (AS3) support

#ifndef RUFFLE_CORE_AVM2_H
#define RUFFLE_CORE_AVM2_H

#include <vector>
#include <unordered_map>
#include <optional>
#include <memory>
#include <cstdint>
#include <array>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC>
    class Player;
    template<typename GC>
    class PlayerRuntime;
    template<typename GC>
    class AvmString;
    template<typename GC>
    class UpdateContext;
    template<typename GC>
    class StringContext;
    template<typename GC>
    class Character;

    namespace avm2 {
        template<typename GC>
        class Activation;
        template<typename GC>
        class Avm2;
        template<typename GC>
        class Value;
        template<typename GC>
        class Error;
        template<typename GC>
        class Class;
        template<typename GC>
        class ClassObject;
        template<typename GC>
        class Domain;
        template<typename GC>
        class ScopeChain;
        template<typename GC>
        class Script;
        template<typename GC>
        class TranslationUnit;
        template<typename GC>
        class Stack;
        template<typename GC>
        class Scope;
        template<typename GC>
        class Multiname;
        template<typename GC>
        class Namespace;
        template<typename GC>
        class QName;
        template<typename GC>
        class Object;
        template<typename GC>
        class FunctionObject;
        template<typename GC>
        class EventObject;
        template<typename GC>
        class ArrayObject;
        template<typename GC>
        class BitmapDataObject;
        template<typename GC>
        class LoaderInfoObject;
        template<typename GC>
        class SharedObjectObject;
        template<typename GC>
        class SoundChannelObject;
        template<typename GC>
        class StageObject;
        template<typename GC>
        class TObject;
        template<typename GC>
        class ArrayStorage;
        template<typename GC>
        class CallStack;
        template<typename GC>
        class Method;
        template<typename GC>
        class WeakObject;

        namespace bytearray {
            enum class ObjectEncoding;
        }

        namespace class_ {
            template<typename GC>
            using AllocatorFn = void(*)();
            template<typename GC>
            using CustomConstructorFn = void(*)();
        }

        namespace e4x {
            struct XmlSettings;
        }

        namespace function {
            template<typename GC>
            class FunctionArgs;
        }

        namespace globals {
            template<typename GC>
            struct SystemClasses;
            template<typename GC>
            struct SystemClassDefs;

            namespace flash {
                namespace ui {
                    namespace context_menu {
                        // Forward declaration
                    }
                }
            }
        }

        namespace api_version {
            enum class ApiVersion;
        }

        using globals::SystemClasses;
        using globals::SystemClassDefs;
        using api_version::ApiVersion;
        using bytearray::ObjectEncoding;
        using class_::AllocatorFn;
        using class_::CustomConstructorFn;
    }

    namespace display_object {
        template<typename GC>
        class DisplayObject;
        template<typename GC>
        class MovieClip;
        template<typename GC>
        class TDisplayObject;
    }

    namespace tag_utils {
        template<typename GC>
        class SwfMovie;
    }

    namespace gc_arena {
        template<typename T>
        class Gc;
        template<typename T>
        class GcRefLock;
        class Mutation;
    }
}
}

namespace swf {
enum class DoAbc2Flag : uint8_t;

namespace avm2 {
namespace read {
    class Reader;
}
}
}

namespace fnv {
    template<typename K, typename V>
    class FnvHashMap : public std::unordered_map<K, V> {};
}

namespace ruffle {
namespace core {

using gc_arena::Gc;
using gc_arena::GcRefLock;
using gc_arena::Mutation;

/// Debug macro for AVM2
#ifdef AVM_DEBUG
    #define avm_debug(avm, ...) tracing_debug(__VA_ARGS__)
#else
    #define avm_debug(avm, ...)
#endif

/// Broadcast event whitelist
constexpr const char* BROADCAST_WHITELIST[] = {
    "enterFrame",
    "exitFrame",
    "frameConstructed",
    "render"
};

/// The state of an AVM2 interpreter.
template<typename GC>
class Avm2 {
private:
    /// The Flash Player version we're emulating.
    uint8_t player_version_;

    /// The player runtime we're emulating
    PlayerRuntime player_runtime_;

    /// Values currently present on the operand stack.
    Stack<GC> stack_;

    /// Scopes currently present of the scope stack.
    std::vector<Scope<GC>> scope_stack_;

    /// The current call stack of the player.
    GcRefLock<GC, CallStack<GC>> call_stack_;

    /// This domain is used exclusively for classes from playerglobals
    Domain<GC> playerglobals_domain_;

    /// The domain associated with 'stage.loaderInfo.applicationDomain'.
    Domain<GC> stage_domain_;

    /// System classes.
    std::optional<SystemClasses<GC>> system_classes_;

    /// System class definitions.
    std::optional<SystemClassDefs<GC>> system_class_defs_;

    /// Top-level global object. It contains most top-level types (Object, Class) and functions.
    std::optional<Object<GC>> toplevel_global_object_;

    /// Pre-created known namespaces.
    Gc<GC, Namespace<GC>::CommonNamespaces> namespaces_;

    /// Native method table
    const std::vector<std::optional<NativeMethodImpl<GC>>>* native_method_table_;

    /// Native instance allocator table
    const std::vector<std::optional<AllocatorFn<GC>>>* native_instance_allocator_table_;

    /// Native call handler table
    const std::vector<std::optional<NativeMethodImpl<GC>>>* native_call_handler_table_;

    /// Native custom constructor table
    const std::vector<std::optional<CustomConstructorFn<GC>>>* native_custom_constructor_table_;

    /// Native fast call list
    const std::vector<size_t>* native_fast_call_list_;

    /// A list of objects which are capable of receiving broadcasts.
    fnv::FnvHashMap<AvmString<GC>, std::vector<WeakObject<GC>>> broadcast_list_;

    fnv::FnvHashMap<AvmString<GC>, ClassObject<GC>> alias_to_class_map_;
    fnv::FnvHashMap<Class<GC>, AvmString<GC>> class_to_alias_map_;

    e4x::XmlSettings xml_settings_;
    ObjectEncoding default_bytearray_encoding_;

    /// The api version of our root movie clip.
    ApiVersion root_api_version_;

#ifdef AVM_DEBUG
    bool debug_output_ = false;
#endif

    bool optimizer_enabled_ = true;

public:
    /// Construct a new AVM interpreter.
    Avm2(
        StringContext<GC>* context,
        uint8_t player_version,
        PlayerRuntime player_runtime
    );

    /// Load player globals
    static void load_player_globals(UpdateContext<GC>* context);

    Domain<GC> playerglobals_domain() const { return playerglobals_domain_; }

    /// Return the current set of system classes.
    const SystemClasses<GC>& classes() const {
        return *system_classes_;
    }

    /// Return the current set of system class definitions.
    const SystemClassDefs<GC>& class_defs() const {
        return *system_class_defs_;
    }

    std::optional<Object<GC>> toplevel_global_object() const {
        return toplevel_global_object_;
    }

    void register_class_alias(AvmString<GC> name, ClassObject<GC> class_object);

    std::optional<ClassObject<GC>> get_class_by_alias(AvmString<GC> name) const;

    std::optional<AvmString<GC>> get_alias_by_class(Class<GC> cls) const;

    /// Run a script's initializer method.
    static void run_script_initializer(
        Script<GC> script,
        UpdateContext<GC>* context
    );

    /// Dispatch an event on an object.
    ///
    /// Returns `true` if the event has been handled.
    static bool dispatch_event(
        UpdateContext<GC>* context,
        EventObject<GC> event,
        Object<GC> target
    );

    /// Simulate dispatching an event.
    static bool simulate_event_dispatch(
        UpdateContext<GC>* context,
        EventObject<GC> event,
        Object<GC> target
    );

    /// Add an object to the broadcast list.
    static void register_broadcast_listener(
        UpdateContext<GC>* context,
        Object<GC> object,
        AvmString<GC> event_name
    );

    /// Dispatch an event on all objects in the current execution list.
    static void broadcast_event(
        UpdateContext<GC>* context,
        EventObject<GC> event,
        ClassObject<GC> on_type
    );

    /// Lookup class for character
    static ClassObject<GC> lookup_class_for_character(
        Activation<GC>* activation,
        MovieClip<GC> movie_clip,
        Domain<GC> domain,
        AvmString<GC> name,
        uint16_t id
    );

    /// Load an ABC file embedded in a `DoAbc` or `DoAbc2` tag.
    static std::optional<Script<GC>> do_abc(
        UpdateContext<GC>* context,
        const std::vector<uint8_t>& data,
        std::optional<AvmString<GC>> name,
        swf::DoAbc2Flag flags,
        Domain<GC> domain,
        std::shared_ptr<tag_utils::SwfMovie> movie
    );

    /// Load the playerglobal ABC file.
    static void load_builtin_abc(
        UpdateContext<GC>* context,
        const std::vector<uint8_t>& data,
        Domain<GC> domain,
        std::shared_ptr<tag_utils::SwfMovie> movie
    );

    Domain<GC> stage_domain() const { return stage_domain_; }

    /// Pushes an executable on the call stack
    void push_call(Mutation<GC>* mc, Method<GC> method);

    /// Pops an executable off the call stack
    void pop_call(Mutation<GC>* mc);

    GcRefLock<GC, CallStack<GC>> call_stack() const { return call_stack_; }

    CallStack<GC> capture_call_stack() const;

    /// Gets the public namespace, versioned based on the current root SWF.
    Namespace<GC> find_public_namespace() const;

    bool optimizer_enabled() const { return optimizer_enabled_; }
    void set_optimizer_enabled(bool value) { optimizer_enabled_ = value; }

    /// Report an uncaught AVM2 error.
    static void uncaught_error(
        Activation<GC>* activation,
        std::optional<DisplayObject<GC>> display_object,
        Error<GC> error,
        const char* info
    );

#ifdef AVM_DEBUG
    bool show_debug_output() const { return debug_output_; }
    void set_show_debug_output(bool visible) { debug_output_ = visible; }
#else
    constexpr bool show_debug_output() const { return false; }
    constexpr void set_show_debug_output(bool) {}
#endif

private:
    void push_scope(Scope<GC> scope);
    void pop_scope();

    static bool dispatch_event_internal(
        UpdateContext<GC>* context,
        EventObject<GC> event,
        Object<GC> target,
        bool simulate_dispatch
    );
};

} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_H
