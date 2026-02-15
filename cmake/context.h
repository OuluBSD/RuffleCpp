/*
 * C++ header for context functionality
 * This replaces the functionality of core/src/context.rs
 */

#ifndef CONTEXT_H
#define CONTEXT_H

#include "avm1.h"
#include "avm2.h"
#include "avm_rng.h"
#include "backend/audio.h"
#include "backend/navigator.h"
#include "backend/storage.h"
#include "backend/ui.h"
#include "backend/log.h"
#include "backend/video.h"
#include "display_object/stage.h"
#include "display_object/movie_clip.h"
#include "display_object/edit_text.h"
#include "library.h"
#include "player.h"
#include "timer.h"
#include "focus_tracker.h"
#include "input_manager.h"
#include "context_menu.h"
#include "external_interface.h"
#include "loader.h"
#include "socket.h"
#include "streams.h"
#include "frame_lifecycle.h"
#include "system_properties.h"
#include "string_context.h"
#include "stub_collection.h"
#include "sound_transform.h"
#include "orphan_manager.h"
#include "local_connection.h"
#include "net_connection.h"
#include "avm2/api_version.h"
#include "tag_utils.h"
#include <memory>
#include <vector>
#include <deque>
#include <unordered_map>
#include <optional>
#include <mutex>
#include <functional>
#include <chrono>

namespace ruffle {

// Forward declarations
class Player;
class Avm1;
class Avm2;
class Stage;
class MovieClip;
class EditText;
class Library;
class Timers;
class FocusTracker;
class InputManager;
class ContextMenuState;
class ExternalInterface;
class LoadManager;
class StreamManager;
class Sockets;
class SystemProperties;
class StringContext;
class StubCollection;
class OrphanManager;
class LocalConnections;
class NetConnections;
class ActionQueue;
class RenderContext;
class UpdateContext;
class SoundTransform;
class SoundInstanceHandle;
class SoundHandle;
class SwfMovie;
class DisplayObject;
class Avm1Object;
class Avm1Value;
class AvmString;
class Point;
class Twips;
class Rectangle;
class Color;
class Matrix;
class CommandList;
class TransformStack;
class BitmapCacheEntry;

// Enum for player modes
enum class PlayerMode {
    PLAYER,
    MOVIE_CLIP,
    DEBUGGER
};

// Enum for frame phases
enum class FramePhase {
    LOADING,
    FRAME,
    RENDER
};

// Action type enum
enum class ActionType {
    NORMAL,
    INITIALIZE,
    CONSTRUCT,
    METHOD,
    NOTIFY_LISTENERS
};

// Queued ActionScript call
struct QueuedAction {
    std::shared_ptr<DisplayObject> clip;
    ActionType action_type;
    bool is_unload;
    
    QueuedAction(std::shared_ptr<DisplayObject> clip_obj, 
                 ActionType type, 
                 bool unload)
        : clip(std::move(clip_obj)), action_type(type), is_unload(unload) {}
};

// Action queue for queuing actions to execute at the end of the frame
class ActionQueue {
private:
    static const size_t NUM_PRIORITIES = 3;
    static const size_t DEFAULT_CAPACITY = 32;
    std::vector<std::deque<QueuedAction>> action_queues_;

public:
    ActionQueue() : action_queues_(NUM_PRIORITIES) {
        for (auto& queue : action_queues_) {
            queue.reserve(DEFAULT_CAPACITY);
        }
    }

    // Queue an action to run for the given movie clip
    void queue_action(std::shared_ptr<DisplayObject> clip, 
                     ActionType action_type, 
                     bool is_unload) {
        size_t priority = get_priority(action_type);
        if (priority < NUM_PRIORITIES) {
            action_queues_[priority].emplace_back(std::move(clip), action_type, is_unload);
        }
    }

    // Pop an action from the queue (prioritizing higher priority actions)
    std::optional<QueuedAction> pop_action() {
        for (int i = NUM_PRIORITIES - 1; i >= 0; --i) {
            if (!action_queues_[i].empty()) {
                auto action = action_queues_[i].front();
                action_queues_[i].pop_front();
                return action;
            }
        }
        return std::nullopt;
    }

private:
    size_t get_priority(ActionType type) const {
        switch (type) {
            case ActionType::INITIALIZE: return 2;
            case ActionType::CONSTRUCT: return 1;
            default: return 0;
        }
    }
};

// UpdateContext holds shared data used by various subsystems of Ruffle
template<typename GCContext>
class UpdateContext {
public:
    // The mutation context to allocate and mutate Gc pointers
    GCContext* gc_context;

    // The string context
    std::shared_ptr<StringContext> strings;

    // The queue of actions that will be run after the display list updates
    std::shared_ptr<ActionQueue> action_queue;

    // A collection of stubs encountered during this movie
    std::shared_ptr<StubCollection> stub_tracker;

    // The library containing character definitions for this SWF
    std::shared_ptr<Library> library;

    // The version of the Flash Player we are emulating
    uint8_t player_version;

    PlayerMode player_mode;

    // Requests that the player re-renders after this execution (e.g. due to updateAfterEvent)
    bool* needs_render;

    // The root SWF file
    std::shared_ptr<SwfMovie> root_swf;

    // The audio backend, used by display objects and AVM to play audio
    std::shared_ptr<AudioBackend> audio;

    // The audio manager, managing all actively playing sounds
    std::shared_ptr<AudioManager> audio_manager;

    // The navigator backend, used by the AVM to make HTTP requests and visit webpages
    std::shared_ptr<NavigatorBackend> navigator;

    // The renderer, used by the display objects to draw themselves
    std::shared_ptr<RenderBackend> renderer;

    // The UI backend, used to detect user interactions
    std::shared_ptr<UiBackend> ui;

    // The storage backend, used for storing persistent state
    std::shared_ptr<StorageBackend> storage;

    // The logging backend, used for trace output capturing
    std::shared_ptr<LogBackend> log;

    // The video backend, used for video decoding
    std::shared_ptr<VideoBackend> video;

    // The RNG, used by the AVM RandomNumber opcode, Math.random(), and random()
    std::shared_ptr<AvmRng> rng;

    // The current player's stage (including all loaded levels)
    std::shared_ptr<Stage> stage;

    std::shared_ptr<MouseData> mouse_data;

    // The input manager, tracking keys state
    std::shared_ptr<InputManager> input;

    // The location of the mouse when it was last over the player
    std::shared_ptr<Point<Twips>> mouse_position;

    // The object being dragged via a startDrag action
    std::optional<DragObject> drag_object;

    // Weak reference to the player
    std::weak_ptr<std::mutex> player;

    // The player's load manager
    std::shared_ptr<LoadManager> load_manager;

    // The system properties
    std::shared_ptr<SystemProperties> system;

    std::optional<std::string> page_url;

    // The current instance ID. Used to generate default instanceN names
    int32_t* instance_counter;

    // Shared objects cache
    std::unordered_map<std::string, std::shared_ptr<Avm1Object>> avm1_shared_objects;
    std::unordered_map<std::string, std::shared_ptr<SharedObjectObject>> avm2_shared_objects;

    // Text fields with unbound variable bindings
    std::vector<std::shared_ptr<EditText>> unbound_text_fields;

    // Timed callbacks created with setInterval/setTimeout
    std::shared_ptr<Timers> timers;

    std::shared_ptr<ContextMenuState> current_context_menu;

    // The AVM1 global state
    std::shared_ptr<Avm1> avm1;

    // The AVM2 global state
    std::shared_ptr<Avm2> avm2;

    // External interface for (for example) JavaScript <-> ActionScript interaction
    std::shared_ptr<ExternalInterface> external_interface;

    // The instant at which the SWF was launched
    std::chrono::steady_clock::time_point start_time;

    // The instant at which the current update started
    std::chrono::steady_clock::time_point update_start;

    // The maximum amount of time that can be called before a Error::ExecutionTimeout
    // is raised. This defaults to 15 seconds but can be changed.
    std::chrono::milliseconds max_execution_duration;

    // A tracker for the current keyboard focused element
    FocusTracker focus_tracker;

    // How many times getTimer() was called so far. Used to detect busy-loops
    uint32_t times_get_time_called;

    // This frame's current fake time offset, used to pretend passage of time in time functions
    uint32_t* time_offset;

    // The current stage frame rate
    double* frame_rate;

    // Whether movies are prevented from changing the stage frame rate
    bool forced_frame_rate;

    // Amount of actions performed since the last timeout check
    uint32_t* actions_since_timeout_check;

    // The current frame processing phase
    FramePhase* frame_phase;

    // Manager of in-progress media streams
    std::shared_ptr<StreamManager> stream_manager;

    std::shared_ptr<Sockets> sockets;

    // List of active NetConnection instances
    std::shared_ptr<NetConnections> net_connections;

    std::shared_ptr<LocalConnections> local_connections;

    std::shared_ptr<OrphanManager> orphan_manager;

    // Dynamic root for allowing handles to GC objects to exist outside of the GC
    // This would be handled by the GC arena in a real implementation

    // These functions are run at the end of each frame execution
    std::vector<std::function<void()>> post_frame_callbacks;

    std::optional<std::shared_ptr<Sender<PlayerNotification>>> notification_sender;

    // Movie clips whose frame scripts were registered during frame script phase
    // requires a separate clean-up pass when running frame-scripts instead of executing them in place
    std::deque<std::shared_ptr<MovieClip>> frame_script_cleanup_queue;

    // Constructor
    UpdateContext(GCContext* gc_ctx,
                  std::shared_ptr<StringContext> str_ctx,
                  std::shared_ptr<Library> lib,
                  uint8_t player_ver,
                  PlayerMode mode,
                  bool* needs_render_ptr,
                  std::shared_ptr<SwfMovie> root_movie,
                  std::shared_ptr<AudioBackend> audio_backend,
                  std::shared_ptr<AudioManager> audio_mgr,
                  std::shared_ptr<NavigatorBackend> nav_backend,
                  std::shared_ptr<RenderBackend> render_backend,
                  std::shared_ptr<UiBackend> ui_backend,
                  std::shared_ptr<StorageBackend> storage_backend,
                  std::shared_ptr<LogBackend> log_backend,
                  std::shared_ptr<VideoBackend> video_backend,
                  std::shared_ptr<AvmRng> rng_ptr,
                  std::shared_ptr<Stage> stage_obj,
                  std::shared_ptr<MouseData> mouse_data_ptr,
                  std::shared_ptr<InputManager> input_mgr,
                  std::shared_ptr<Point<Twips>> mouse_pos,
                  std::shared_ptr<LoadManager> load_mgr,
                  std::shared_ptr<SystemProperties> sys_props,
                  std::shared_ptr<Timers> timer_mgr,
                  std::shared_ptr<ContextMenuState> context_menu,
                  std::shared_ptr<Avm1> avm1_state,
                  std::shared_ptr<Avm2> avm2_state,
                  std::shared_ptr<ExternalInterface> ext_interface,
                  std::shared_ptr<ActionQueue> action_queue_ptr,
                  std::shared_ptr<StubCollection> stub_collector,
                  std::shared_ptr<StreamManager> stream_mgr,
                  std::shared_ptr<Sockets> socket_mgr,
                  std::shared_ptr<NetConnections> net_conn_mgr,
                  std::shared_ptr<LocalConnections> local_conn_mgr,
                  std::shared_ptr<OrphanManager> orphan_mgr)
        : gc_context(gc_ctx), strings(std::move(str_ctx)), action_queue(std::move(action_queue_ptr)),
          stub_tracker(std::move(stub_collector)), library(std::move(lib)), 
          player_version(player_ver), player_mode(mode), needs_render(needs_render_ptr),
          root_swf(std::move(root_movie)), audio(std::move(audio_backend)), 
          audio_manager(std::move(audio_mgr)), navigator(std::move(nav_backend)),
          renderer(std::move(render_backend)), ui(std::move(ui_backend)), 
          storage(std::move(storage_backend)), log(std::move(log_backend)), 
          video(std::move(video_backend)), rng(std::move(rng_ptr)), stage(std::move(stage_obj)),
          mouse_data(std::move(mouse_data_ptr)), input(std::move(input_mgr)), 
          mouse_position(std::move(mouse_pos)), load_manager(std::move(load_mgr)),
          system(std::move(sys_props)), timers(std::move(timer_mgr)), 
          current_context_menu(std::move(context_menu)), avm1(std::move(avm1_state)),
          avm2(std::move(avm2_state)), external_interface(std::move(ext_interface)),
          start_time(std::chrono::steady_clock::now()), 
          update_start(std::chrono::steady_clock::now()),
          max_execution_duration(std::chrono::seconds(15)), 
          times_get_time_called(0), forced_frame_rate(false) {
        
        // Initialize other members
        instance_counter = new int32_t(0);
        time_offset = new uint32_t(0);
        frame_rate = new double(12.0);  // Default frame rate
        actions_since_timeout_check = new uint32_t(0);
        frame_phase = new FramePhase(FramePhase::LOADING);
    }

    // Destructor
    ~UpdateContext() {
        delete instance_counter;
        delete time_offset;
        delete frame_rate;
        delete actions_since_timeout_check;
        delete frame_phase;
    }

    // Convenience method to retrieve the current GC context
    GCContext* gc() const { return gc_context; }

    // Get the global sound transform
    const SoundTransform& global_sound_transform() const {
        return audio_manager->global_sound_transform();
    }

    // Set the global sound transform
    void set_global_sound_transform(const SoundTransform& sound_transform) {
        audio_manager->set_global_sound_transform(sound_transform);
    }

    // Get the local sound transform of a single sound instance
    std::optional<SoundTransform> local_sound_transform(SoundInstanceHandle instance) const {
        return audio_manager->local_sound_transform(instance);
    }

    // Set the local sound transform of a single sound instance
    void set_local_sound_transform(SoundInstanceHandle instance, const SoundTransform& sound_transform) {
        audio_manager->set_local_sound_transform(instance, sound_transform);
    }

    // Start a sound
    std::optional<SoundInstanceHandle> start_sound(
        SoundHandle sound,
        const swf::SoundInfo& settings,
        std::optional<SoundTransform> transform,
        std::shared_ptr<DisplayObject> owner,
        std::shared_ptr<Avm1Object> avm1_object) {
        
        return audio_manager->start_sound(audio.get(), sound, settings, transform, owner, avm1_object);
    }

    // Attach an AVM2 sound channel
    void attach_avm2_sound_channel(SoundInstanceHandle instance, std::shared_ptr<SoundChannelObject> avm2_object) {
        audio_manager->attach_avm2_sound_channel(instance, avm2_object);
    }

    // Stop a sound
    void stop_sound(SoundInstanceHandle instance) {
        audio_manager->stop_sound(audio.get(), instance);
    }

    // Stop all sounds with a specific handle
    void stop_sounds_with_handle(SoundHandle sound) {
        audio_manager->stop_sounds_with_handle(audio.get(), sound);
    }

    // Stop all sounds with a specific display object
    void stop_sounds_with_display_object(std::shared_ptr<DisplayObject> display_object) {
        audio_manager->stop_sounds_with_display_object(audio.get(), display_object);
    }

    // Stop all sounds on parent and children
    void stop_sounds_on_parent_and_children(std::shared_ptr<DisplayObject> display_object) {
        audio_manager->stop_sounds_on_parent_and_children(audio.get(), display_object);
    }

    // Stop all sounds
    void stop_all_sounds() {
        audio_manager->stop_all_sounds(audio.get());
    }

    // Check if a sound is playing
    bool is_sound_playing(SoundInstanceHandle sound) const {
        return audio_manager->is_sound_playing(sound);
    }

    // Check if a sound is playing with a specific handle
    bool is_sound_playing_with_handle(SoundHandle sound) const {
        return audio_manager->is_sound_playing_with_handle(sound);
    }

    // Start a stream
    std::optional<SoundInstanceHandle> start_stream(
        std::shared_ptr<MovieClip> movie_clip,
        uint16_t frame,
        SwfSlice data,
        const swf::SoundStreamHead& stream_info) {
        
        return audio_manager->start_stream(audio.get(), movie_clip, frame, data, stream_info);
    }

    // Set sound transforms dirty
    void set_sound_transforms_dirty() {
        audio_manager->set_sound_transforms_dirty();
    }

    // Change the root movie
    void set_root_movie(std::shared_ptr<SwfMovie> movie) {
        if (!forced_frame_rate) {
            *frame_rate = movie->frame_rate();
        }

        printf("Loaded SWF version %d, resolution %dx%d @ %.2f FPS\n",
               movie->version(), movie->width(), movie->height(), *frame_rate);

        root_swf = std::move(movie);
        *instance_counter = 0;

        if (root_swf->is_action_script_3()) {
            // In a real implementation, this would set the API version
            // avm2->root_api_version = ApiVersion::from_swf_version(root_swf->version(), avm2->player_runtime);
        }

        stage->set_movie_size(
            static_cast<uint32_t>(root_swf->width().to_pixels()),
            static_cast<uint32_t>(root_swf->height().to_pixels())
        );
        stage->set_movie(gc(), root_swf);

        // In a real implementation, this would create an activation and set up the stage
        // For now, we'll just set up the basic structure
        auto root = MovieClip::player_root_movie(root_swf);
        root->set_depth(0);
        root->post_instantiation(this, nullptr, Instantiator::MOVIE, false);
        root->set_default_root_name(this);

        // Set flashvars and the version parameters (AVM1 only)
        if (auto flashvars = root->object1()) {
            for (const auto& [key, value] : root_swf->parameters()) {
                flashvars->define_value(gc(), AvmString::new_utf8(gc(), key), 
                                      AvmString::new_utf8(gc(), value).as_value(), Attribute::EMPTY);
            }

            std::string version_string = system->get_version_string(player_version);
            flashvars->define_value(gc(), AvmString::new_ascii_static(gc(), "$version"),
                                  AvmString::new_utf8(gc(), version_string).as_value(), Attribute::EMPTY);
        }

        stage->replace_at_depth(this, root, 0);
        stage->build_matrices(this);
        audio->set_frame_rate(*frame_rate);
    }

    // Replace the root movie
    void replace_root_movie(std::shared_ptr<SwfMovie> movie) {
        // Clean up the stage before loading another root movie
        sockets->close_all();
        timers->remove_all();

        set_root_movie(std::move(movie));
    }

    // Send a notification
    void send_notification(PlayerNotification notification) {
        if (notification_sender.has_value()) {
            // In a real implementation, this would send the notification
            // notification_sender.value()->try_send(notification);
        }
    }

    // Obtain a strong reference to the current Player
    std::shared_ptr<Player> player_handle() const {
        auto locked_player = player.lock();
        if (locked_player) {
            return std::static_pointer_cast<Player>(locked_player);
        }
        throw std::runtime_error("Could not upgrade weak reference to player");
    }

    // Trace output for AVM
    void avm_trace(const std::string& message) {
        std::string clean_message = message;
        // Replace carriage returns with newlines
        size_t pos = 0;
        while ((pos = clean_message.find('\r', pos)) != std::string::npos) {
            clean_message.replace(pos, 1, "\n");
            pos += 1;
        }
        log->avm_trace(clean_message);
    }

    // Warning output for AVM
    void avm_warning(const std::string& message) {
        log->avm_warning(message);
    }
};

// Shared data used during rendering
template<typename GCContext>
class RenderContext {
public:
    // The renderer, used by the display objects to register themselves
    std::shared_ptr<RenderBackend> renderer;

    // The command list, used by the display objects to draw themselves
    std::shared_ptr<CommandList> commands;

    // Any offscreen draws that should be used to redraw a cacheAsBitmap
    std::vector<std::shared_ptr<BitmapCacheEntry>>* cache_draws;

    // The GC context, used to perform any Gc writes that must occur during rendering
    GCContext* gc_context;

    // The library, which provides access to fonts and other definitions when rendering
    std::shared_ptr<Library> library;

    // The transform stack controls the matrix and color transform as we traverse the display hierarchy
    std::shared_ptr<TransformStack> transform_stack;

    // Whether we're rendering offscreen. This can disable some logic like Ruffle-side render culling
    bool is_offscreen;

    // Whether to use cacheAsBitmap, vs drawing everything explicitly
    bool use_bitmap_cache;

    // The current player's stage (including all loaded levels)
    std::shared_ptr<Stage> stage;

    // Constructor
    RenderContext(std::shared_ptr<RenderBackend> render_backend,
                  std::shared_ptr<CommandList> cmd_list,
                  std::vector<std::shared_ptr<BitmapCacheEntry>>* cache_draws_ptr,
                  GCContext* gc_ctx,
                  std::shared_ptr<Library> lib,
                  std::shared_ptr<TransformStack> transform_stack_ptr,
                  bool offscreen,
                  bool bitmap_cache,
                  std::shared_ptr<Stage> stage_obj)
        : renderer(std::move(render_backend)), commands(std::move(cmd_list)),
          cache_draws(cache_draws_ptr), gc_context(gc_ctx), library(std::move(lib)),
          transform_stack(std::move(transform_stack_ptr)), is_offscreen(offscreen),
          use_bitmap_cache(bitmap_cache), stage(std::move(stage_obj)) {}

    // Convenience method to retrieve the current GC context
    GCContext* gc() const { return gc_context; }

    // Draw a rectangle outline
    void draw_rect_outline(const Color& color, const Rectangle<Twips>& bounds, const Twips& thickness) {
        auto transformed_bounds = (*transform_stack->transform()).matrix * bounds;
        float width = static_cast<float>(transformed_bounds.width().to_pixels());
        float height = static_cast<float>(transformed_bounds.height().to_pixels());
        float thickness_pixels = static_cast<float>(thickness.to_pixels());
        
        // Top
        commands->draw_rect(color, Matrix::create_box(width, thickness_pixels, 
                                                    transformed_bounds.x_min.to_pixels(), 
                                                    transformed_bounds.y_min.to_pixels()));
        // Bottom
        commands->draw_rect(color, Matrix::create_box(width, thickness_pixels,
                                                    transformed_bounds.x_min.to_pixels(),
                                                    transformed_bounds.y_max.to_pixels() - thickness_pixels));
        // Left
        commands->draw_rect(color, Matrix::create_box(thickness_pixels, height,
                                                    transformed_bounds.x_min.to_pixels(),
                                                    transformed_bounds.y_min.to_pixels()));
        // Right
        commands->draw_rect(color, Matrix::create_box(thickness_pixels, height,
                                                    transformed_bounds.x_max.to_pixels() - thickness_pixels,
                                                    transformed_bounds.y_min.to_pixels()));
    }
};

} // namespace ruffle

#endif // CONTEXT_H