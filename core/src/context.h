// C++ translation of core/src/context.rs
// Contexts and helper types passed between functions

#ifndef RUFFLE_CORE_CONTEXT_H
#define RUFFLE_CORE_CONTEXT_H

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <deque>
#include <array>
#include <optional>
#include <functional>
#include <chrono>
#include <mutex>

// Forward declarations
namespace ruffle {
namespace swf {
    struct SoundInfo;
    struct Point;
    struct Rectangle;
    class Twips;
}
namespace render {
    class RenderBackend;
    class TransformStack;
    struct BitmapCacheEntry;
    namespace commands {
        class CommandList;
    }
}
namespace video {
    namespace backend {
        class VideoBackend;
    }
}
namespace core {
    enum class PlayerMode;
    class AvmRng;
    class AvmString;
    class StringContext;
    class WStr;
    class SoundTransform;
    class Color;
    class Matrix;
    class Point;
    
    namespace avm1 {
        template<typename GC>
        class Avm1;
        template<typename GC>
        class Object;
        template<typename GC>
        class Value;
        enum class Attribute;
    }
    namespace avm2 {
        template<typename GC>
        class Avm2;
        template<typename GC>
        class Activation;
        template<typename GC>
        class LoaderInfoObject;
        template<typename GC>
        class SharedObjectObject;
        template<typename GC>
        class SoundChannelObject;
        namespace api_version {
            class ApiVersion;
        }
    }
    namespace backend {
        namespace audio {
            class AudioBackend;
            template<typename GC>
            class AudioManager;
            using SoundHandle = uint64_t;
            using SoundInstanceHandle = uint64_t;
        }
        namespace log {
            class LogBackend;
        }
        namespace navigator {
            class NavigatorBackend;
        }
        namespace storage {
            class StorageBackend;
        }
        namespace ui {
            class UiBackend;
        }
    }
    namespace context_menu {
        template<typename GC>
        struct ContextMenuState;
    }
    namespace display_object {
        template<typename GC>
        class DisplayObject;
        template<typename GC>
        class EditText;
        template<typename GC>
        class MovieClip;
        template<typename GC>
        class Stage;
        enum class BoundsMode;
    }
    namespace events {
        enum class PlayerNotification;
    }
    namespace external {
        template<typename GC>
        class ExternalInterface;
    }
    namespace focus_tracker {
        template<typename GC>
        class FocusTracker;
    }
    namespace frame_lifecycle {
        enum class FramePhase;
    }
    namespace input {
        class InputManager;
    }
    namespace library {
        template<typename GC>
        class Library;
    }
    namespace loader {
        template<typename GC>
        class LoadManager;
    }
    namespace local_connection {
        template<typename GC>
        class LocalConnections;
    }
    namespace net_connection {
        template<typename GC>
        class NetConnections;
    }
    namespace orphan_manager {
        template<typename GC>
        class OrphanManager;
    }
    namespace player {
        class Player;
        template<typename GC>
        struct MouseData;
        template<typename GC>
        struct DragObject;
        using PostFrameCallback = std::function<void()>;
    }
    namespace socket {
        template<typename GC>
        class Sockets;
    }
    namespace streams {
        template<typename GC>
        class StreamManager;
    }
    namespace stub {
        template<typename GC>
        class StubCollection;
    }
    namespace system_properties {
        class SystemProperties;
    }
    namespace tag_utils {
        class SwfMovie;
        struct SwfSlice;
    }
    namespace timer {
        template<typename GC>
        class Timers;
    }
    namespace vminterface {
        enum class Instantiator;
    }
}
}

// For gc_arena simulation
namespace gc_arena {
    template<typename GC>
    class Mutation;
    template<typename GC>
    class DynamicRootSet;
}

// For async_channel simulation
template<typename T>
class Sender;

namespace ruffle {
namespace core {

/// A queued ActionScript call.
template<typename GC>
struct QueuedAction {
    /// The movie clip this ActionScript is running on.
    display_object::DisplayObject<GC> clip;
    
    /// The type of action this is, along with the corresponding bytecode/method data.
    class ActionType<GC> action_type;
    
    /// Whether this is an unload action, which can still run if the clip is removed.
    bool is_unload;
};

/// The type of action being run.
template<typename GC>
class ActionType {
public:
    enum class Type {
        Normal,
        Initialize,
        Construct,
        Method,
        NotifyListeners
    };

private:
    Type type_;
    
    // Union-like storage for different action types
    tag_utils::SwfSlice bytecode_;  // For Normal and Initialize
    std::optional<avm1::Object<GC>> constructor_;  // For Construct
    std::vector<tag_utils::SwfSlice> events_;  // For Construct
    avm1::Object<GC> method_object_;  // For Method
    AvmString method_name_;  // For Method and NotifyListeners
    std::vector<avm1::Value<GC>> method_args_;  // For Method and NotifyListeners
    AvmString listener_name_;  // For NotifyListeners

public:
    static ActionType Normal(tag_utils::SwfSlice bytecode) {
        ActionType action;
        action.type_ = Type::Normal;
        action.bytecode_ = bytecode;
        return action;
    }

    static ActionType Initialize(tag_utils::SwfSlice bytecode) {
        ActionType action;
        action.type_ = Type::Initialize;
        action.bytecode_ = bytecode;
        return action;
    }

    static ActionType Construct(std::optional<avm1::Object<GC>> constructor,
                               std::vector<tag_utils::SwfSlice> events) {
        ActionType action;
        action.type_ = Type::Construct;
        action.constructor_ = constructor;
        action.events_ = std::move(events);
        return action;
    }

    static ActionType Method(avm1::Object<GC> object, AvmString name,
                            std::vector<avm1::Value<GC>> args) {
        ActionType action;
        action.type_ = Type::Method;
        action.method_object_ = object;
        action.method_name_ = name;
        action.method_args_ = std::move(args);
        return action;
    }

    static ActionType NotifyListeners(AvmString listener, AvmString method,
                                     std::vector<avm1::Value<GC>> args) {
        ActionType action;
        action.type_ = Type::NotifyListeners;
        action.listener_name_ = listener;
        action.method_name_ = method;
        action.method_args_ = std::move(args);
        return action;
    }

    size_t priority() const {
        switch (type_) {
            case Type::Initialize: return 2;
            case Type::Construct: return 1;
            default: return 0;
        }
    }
};

/// Action and gotos need to be queued up to execute at the end of the frame.
template<typename GC>
class ActionQueue {
private:
    static constexpr size_t NUM_PRIORITIES = 3;
    static constexpr size_t DEFAULT_CAPACITY = 32;
    
    std::array<std::deque<QueuedAction<GC>>, NUM_PRIORITIES> action_queue_;

public:
    ActionQueue() {
        for (auto& queue : action_queue_) {
            queue.reserve(DEFAULT_CAPACITY);
        }
    }

    /// Queues an action to run for the given movie clip.
    void queue_action(display_object::DisplayObject<GC> clip,
                     ActionType<GC> action_type, bool is_unload) {
        size_t priority = action_type.priority();
        if (priority < NUM_PRIORITIES) {
            action_queue_[priority].push_back({
                std::move(clip),
                std::move(action_type),
                is_unload
            });
        }
    }

    /// Sorts and drains the actions from the queue.
    std::optional<QueuedAction<GC>> pop_action() {
        for (auto it = action_queue_.rbegin(); it != action_queue_.rend(); ++it) {
            if (!it->empty()) {
                auto action = std::move(it->front());
                it->pop_front();
                return action;
            }
        }
        return std::nullopt;
    }
};

/// `UpdateContext` holds shared data that is used by the various subsystems of Ruffle.
template<typename GC>
class UpdateContext {
public:
    /// The mutation context to allocate and mutate GC pointers.
    gc_arena::Mutation<GC>* gc_context;

    /// The string context.
    StringContext<GC> strings;

    /// The queue of actions that will be run after the display list updates.
    ActionQueue<GC>* action_queue;

    /// A collection of stubs encountered during this movie.
    stub::StubCollection<GC>* stub_tracker;

    /// The library containing character definitions for this SWF.
    library::Library<GC>* library;

    /// The version of the Flash Player we are emulating.
    uint8_t player_version;

    PlayerMode player_mode;

    /// Requests that the player re-renders after this execution.
    bool* needs_render;

    /// The root SWF file.
    std::shared_ptr<tag_utils::SwfMovie>* root_swf;

    /// The audio backend, used by display objects and AVM to play audio.
    backend::audio::AudioBackend* audio;

    /// The audio manager, managing all actively playing sounds.
    backend::audio::AudioManager<GC>* audio_manager;

    /// The navigator backend, used by the AVM to make HTTP requests.
    backend::navigator::NavigatorBackend* navigator;

    /// The renderer, used by the display objects to draw themselves.
    render::RenderBackend* renderer;

    /// The UI backend, used to detect user interactions.
    backend::ui::UiBackend* ui;

    /// The storage backend, used for storing persistent state.
    backend::storage::StorageBackend* storage;

    /// The logging backend, used for trace output capturing.
    backend::log::LogBackend* log;

    /// The video backend, used for video decoding.
    video::backend::VideoBackend* video;

    /// The RNG, used by the AVM RandomNumber opcode.
    AvmRng* rng;

    /// The current player's stage (including all loaded levels)
    display_object::Stage<GC> stage;

    player::MouseData<GC>* mouse_data;

    /// The input manager, tracking keys state.
    const InputManager* input;

    /// The location of the mouse when it was last over the player.
    Point<Twips>* mouse_position;

    /// The object being dragged via a startDrag action.
    std::optional<player::DragObject<GC>>* drag_object;

    /// Weak reference to the player.
    std::weak_ptr<std::mutex<player::Player>> player;

    /// The player's load manager.
    loader::LoadManager<GC>* load_manager;

    /// The system properties
    SystemProperties* system;

    std::optional<std::string>* page_url;

    /// The current instance ID. Used to generate default instanceN names.
    int32_t* instance_counter;

    /// Shared objects cache for AVM1
    std::unordered_map<std::string, avm1::Object<GC>>* avm1_shared_objects;

    /// Shared objects cache for AVM2
    std::unordered_map<std::string, avm2::SharedObjectObject<GC>>* avm2_shared_objects;

    /// Text fields with unbound variable bindings.
    std::vector<display_object::EditText<GC>>* unbound_text_fields;

    /// Timed callbacks created with setInterval/setTimeout.
    timer::Timers<GC>* timers;

    std::optional<context_menu::ContextMenuState<GC>>* current_context_menu;

    /// The AVM1 global state.
    avm1::Avm1<GC>* avm1;

    /// The AVM2 global state.
    avm2::Avm2<GC>* avm2;

    /// External interface for JavaScript <-> ActionScript interaction
    external::ExternalInterface<GC>* external_interface;

    /// The instant at which the SWF was launched.
    std::chrono::steady_clock::time_point start_time;

    /// The instant at which the current update started.
    std::chrono::steady_clock::time_point update_start;

    /// The maximum amount of time before ExecutionTimeout is raised.
    std::chrono::duration<double> max_execution_duration;

    /// A tracker for the current keyboard focused element
    focus_tracker::FocusTracker<GC> focus_tracker;

    /// How many times getTimer() was called so far.
    uint32_t times_get_time_called;

    /// This frame's current fake time offset.
    uint32_t* time_offset;

    /// The current stage frame rate.
    double* frame_rate;

    /// Whether movies are prevented from changing the stage frame rate.
    bool forced_frame_rate;

    /// Amount of actions performed since the last timeout check
    uint32_t* actions_since_timeout_check;

    /// The current frame processing phase.
    frame_lifecycle::FramePhase* frame_phase;

    /// Manager of in-progress media streams.
    streams::StreamManager<GC>* stream_manager;

    socket::Sockets<GC>* sockets;

    /// List of active NetConnection instances.
    net_connection::NetConnections<GC>* net_connections;

    local_connection::LocalConnections<GC>* local_connections;

    orphan_manager::OrphanManager<GC>* orphan_manager;

    /// Dynamic root for allowing handles to GC objects outside of the GC.
    gc_arena::DynamicRootSet<GC> dynamic_root;

    /// Functions run at the end of each frame execution.
    std::vector<player::PostFrameCallback>* post_frame_callbacks;

    std::optional<Sender<events::PlayerNotification>*> notification_sender;

    /// Movie clips whose frame scripts require cleanup.
    std::deque<display_object::MovieClip<GC>> frame_script_cleanup_queue;

    /// Convenience method to retrieve the current GC context.
    gc_arena::Mutation<GC>* gc() const { return gc_context; }

    /// Convenience methods for controlling audio.
    const SoundTransform& global_sound_transform() const {
        return audio_manager->global_sound_transform();
    }

    void set_global_sound_transform(const SoundTransform& sound_transform) {
        audio_manager->set_global_sound_transform(sound_transform);
    }

    std::optional<const SoundTransform&> local_sound_transform(
        backend::audio::SoundInstanceHandle instance) const {
        return audio_manager->local_sound_transform(instance);
    }

    void set_local_sound_transform(backend::audio::SoundInstanceHandle instance,
                                  const SoundTransform& sound_transform) {
        audio_manager->set_local_sound_transform(instance, sound_transform);
    }

    std::optional<backend::audio::SoundInstanceHandle> start_sound(
        backend::audio::SoundHandle sound,
        const swf::SoundInfo& settings,
        std::optional<SoundTransform> transform,
        std::optional<display_object::DisplayObject<GC>> owner,
        std::optional<avm1::Object<GC>> avm1_object) {
        return audio_manager->start_sound(audio, sound, settings, transform, owner, avm1_object);
    }

    void attach_avm2_sound_channel(backend::audio::SoundInstanceHandle instance,
                                  avm2::SoundChannelObject<GC> avm2_object) {
        audio_manager->attach_avm2_sound_channel(instance, avm2_object);
    }

    void stop_sound(backend::audio::SoundInstanceHandle instance) {
        audio_manager->stop_sound(audio, instance);
    }

    void stop_sounds_with_handle(backend::audio::SoundHandle sound) {
        audio_manager->stop_sounds_with_handle(audio, sound);
    }

    void stop_sounds_with_display_object(display_object::DisplayObject<GC> display_object) {
        audio_manager->stop_sounds_with_display_object(audio, display_object);
    }

    void stop_all_sounds() {
        audio_manager->stop_all_sounds(audio);
    }

    bool is_sound_playing(backend::audio::SoundInstanceHandle sound) const {
        return audio_manager->is_sound_playing(sound);
    }

    bool is_sound_playing_with_handle(backend::audio::SoundHandle sound) const {
        return audio_manager->is_sound_playing_with_handle(sound);
    }

    void set_sound_transforms_dirty() {
        audio_manager->set_sound_transforms_dirty();
    }

    /// Output trace message.
    void avm_trace(const std::string& message) const {
        std::string replaced = message;
        size_t pos = 0;
        while ((pos = replaced.find('\r', pos)) != std::string::npos) {
            replaced.replace(pos, 1, "\n");
            pos++;
        }
        log->avm_trace(replaced);
    }

    void avm_warning(const std::string& message) const {
        log->avm_warning(message);
    }

    /// Obtain a strong reference to the current Player.
    std::shared_ptr<std::mutex<player::Player>> player_handle() const {
        auto handle = player.lock();
        if (!handle) {
            throw std::runtime_error("Could not upgrade weak reference to player");
        }
        return handle;
    }

    void send_notification(events::PlayerNotification notification) const {
        if (notification_sender.has_value()) {
            // Try to send notification
            // Implementation would use async_channel
        }
    }
};

/// Shared data used during rendering.
template<typename GC>
class RenderContext {
public:
    /// The renderer, used by the display objects to register themselves.
    render::RenderBackend* renderer;

    /// The command list, used by the display objects to draw themselves.
    render::commands::CommandList commands;

    /// Any offscreen draws for cacheAsBitmap.
    std::vector<render::BitmapCacheEntry>* cache_draws;

    /// The GC context.
    gc_arena::Mutation<GC>* gc_context;

    /// The library, which provides access to fonts and other definitions.
    const library::Library<GC>* library;

    /// The transform stack controls the matrix and color transform.
    render::TransformStack* transform_stack;

    /// Whether we're rendering offscreen.
    bool is_offscreen;

    /// Whether to use cacheAsBitmap.
    bool use_bitmap_cache;

    /// The current player's stage.
    display_object::Stage<GC> stage;

    /// Convenience method to retrieve the current GC context.
    gc_arena::Mutation<GC>* gc() const { return gc_context; }

    /// Draw a rectangle outline.
    void draw_rect_outline(const Color& color, const swf::Rectangle& bounds, Twips thickness);
};

} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_CONTEXT_H
