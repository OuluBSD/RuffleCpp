/*
 * C++ header for movie clip functionality
 * This replaces the functionality of core/src/display_object/movie_clip.rs
 */

#ifndef MOVIE_CLIP_H
#define MOVIE_CLIP_H

#include "display_object.h"
#include "display_object/container.h"
#include "avm1.h"
#include "avm2.h"
#include "context.h"
#include "swf_movie.h"
#include "tag_utils.h"
#include "frame_lifecycle.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <functional>

namespace ruffle {

// Forward declarations
class UpdateContext;
class RenderContext;
class Avm1Activation;
class Avm2Activation;
class SoundInstanceHandle;
class NetStream;
class LoaderStream;

// Enum for movie clip execution state
enum class MovieClipExecutionState {
    PLAYING,
    STOPPED
};

// Enum for movie clip actions
enum class MovieClipAction {
    GOTO_FRAME,
    PLAY,
    STOP,
    NEXT_FRAME,
    PREVIOUS_FRAME
};

// Structure for movie clip data
struct MovieClipData {
    std::shared_ptr<SwfMovie> movie;
    int current_frame;
    int total_frames;
    MovieClipExecutionState execution_state;
    std::vector<std::shared_ptr<DisplayObject>> frame_scripts;
    std::unordered_map<int, std::function<void()>> frame_labels;
    std::vector<SoundInstanceHandle> active_sounds;
    std::vector<std::shared_ptr<NetStream>> active_streams;
    std::shared_ptr<LoaderStream> loader_stream;
    
    MovieClipData(std::shared_ptr<SwfMovie> mov) 
        : movie(std::move(mov)), current_frame(1), total_frames(1),
          execution_state(MovieClipExecutionState::STOPPED) {}
};

// Movie clip display object class
class MovieClip : public DisplayObjectContainer {
private:
    std::shared_ptr<MovieClipData> data_;
    bool placed_by_script_;
    bool is_root_movie_;
    bool is_tracking_mouse_state_;
    std::vector<std::pair<int, std::string>> frame_labels_;

public:
    MovieClip(std::shared_ptr<SwfMovie> movie, bool is_root = false)
        : DisplayObjectContainer(DisplayObjectType::MOVIE_CLIP, 0),  // ID is typically set from SWF
          is_root_movie_(is_root), is_tracking_mouse_state_(false) {
        data_ = std::make_shared<MovieClipData>(std::move(movie));
    }

    // Static factory method
    static std::shared_ptr<MovieClip> create(std::shared_ptr<SwfMovie> movie, bool is_root = false) {
        return std::make_shared<MovieClip>(std::move(movie), is_root);
    }

    // Create the root movie clip
    static std::shared_ptr<MovieClip> player_root_movie(std::shared_ptr<UpdateContext> context,
                                                      std::shared_ptr<SwfMovie> movie) {
        auto root_movie = std::make_shared<MovieClip>(std::move(movie), true);
        root_movie->set_default_root_name(context);
        return root_movie;
    }

    // Getters
    int current_frame() const { return data_->current_frame; }
    int total_frames() const { return data_->total_frames; }
    MovieClipExecutionState execution_state() const { return data_->execution_state; }
    std::shared_ptr<SwfMovie> movie() const override { return data_->movie; }
    bool is_playing() const { return data_->execution_state == MovieClipExecutionState::PLAYING; }
    bool is_root_movie() const { return is_root_movie_; }
    bool placed_by_script() const { return placed_by_script_; }
    const std::vector<std::pair<int, std::string>>& frame_labels() const { return frame_labels_; }

    // Setters
    void set_current_frame(int frame) { 
        if (frame >= 1 && frame <= data_->total_frames) {
            data_->current_frame = frame;
        }
    }
    
    void set_execution_state(MovieClipExecutionState state) { 
        data_->execution_state = state; 
    }

    // Play the movie clip
    void play(std::shared_ptr<UpdateContext> context) {
        data_->execution_state = MovieClipExecutionState::PLAYING;
        // In a real implementation, this would trigger frame advancement
    }

    // Stop the movie clip
    void stop(std::shared_ptr<UpdateContext> context) {
        data_->execution_state = MovieClipExecutionState::STOPPED;
    }

    // Go to a specific frame
    void goto_frame(std::shared_ptr<UpdateContext> context, int frame_number, bool stop_playback) {
        if (frame_number < 1) frame_number = 1;
        if (frame_number > data_->total_frames) frame_number = data_->total_frames;

        if (frame_number != data_->current_frame) {
            // In a real implementation, this would run goto frame logic
            run_goto_frame(context, frame_number, stop_playback);
        }
    }

    // Go to a frame by label
    void goto_label(std::shared_ptr<UpdateContext> context, const std::string& label, bool stop_playback) {
        for (const auto& [frame, lbl] : frame_labels_) {
            if (lbl == label) {
                goto_frame(context, frame, stop_playback);
                return;
            }
        }
        // Label not found, do nothing
    }

    // Go to and stop at a specific frame
    void goto_and_stop(std::shared_ptr<UpdateContext> context, int frame_number) {
        goto_frame(context, frame_number, true);
    }

    // Go to and play from a specific frame
    void goto_and_play(std::shared_ptr<UpdateContext> context, int frame_number) {
        goto_frame(context, frame_number, false);
        play(context);
    }

    // Move to the next frame
    void next_frame(std::shared_ptr<UpdateContext> context) {
        int next_frame_num = data_->current_frame + 1;
        if (next_frame_num > data_->total_frames) {
            next_frame_num = data_->total_frames;
        }
        goto_frame(context, next_frame_num, false);
    }

    // Move to the previous frame
    void prev_frame(std::shared_ptr<UpdateContext> context) {
        int prev_frame_num = data_->current_frame - 1;
        if (prev_frame_num < 1) {
            prev_frame_num = 1;
        }
        goto_frame(context, prev_frame_num, false);
    }

    // Run frame for AVM1
    void run_frame_avm1(std::shared_ptr<UpdateContext> context) {
        if (data_->execution_state == MovieClipExecutionState::PLAYING) {
            next_frame(context);
        }
        
        // Run any frame scripts
        run_frame_scripts(context);
        
        // Advance to the next frame if playing
        if (data_->execution_state == MovieClipExecutionState::PLAYING) {
            advance_frame(context, false);
        }
    }

    // Run frame for AVM2
    void run_frame_avm2(std::shared_ptr<UpdateContext> context) {
        // Similar to AVM1 but with AVM2-specific logic
        run_frame_scripts(context);
        
        if (data_->execution_state == MovieClipExecutionState::PLAYING) {
            advance_frame(context, false);
        }
    }

    // Construct frame (called during timeline construction)
    void construct_frame(std::shared_ptr<UpdateContext> context) override {
        // Run timeline construction for this frame
        // This would involve processing timeline objects and setting up display list
        
        // Call parent implementation
        DisplayObjectContainer::construct_frame(context);
        
        // Run any frame construction scripts
        if (!data_->frame_scripts.empty()) {
            auto script = data_->frame_scripts[current_frame() - 1];
            if (script) {
                // Execute the frame script
                // In a real implementation, this would run the script
            }
        }
    }

    // Enter frame (called when entering a new frame)
    void enter_frame(std::shared_ptr<UpdateContext> context) override {
        // Call parent implementation
        DisplayObjectContainer::enter_frame(context);
        
        // Run enter frame actions
        run_frame_scripts(context);
    }

    // Handle timeline execution for the current frame
    void run_goto_frame(std::shared_ptr<UpdateContext> context, 
                       int frame, 
                       bool stop_playback) {
        // In a real implementation, this would handle the complex logic of
        // going to a specific frame, including:
        // - Removing objects from previous frames
        // - Creating objects for the new frame
        // - Running frame scripts
        // - Updating the display list
        
        data_->current_frame = frame;
        
        if (stop_playback) {
            stop(context);
        }
    }

    // Advance to the next frame
    void advance_frame(std::shared_ptr<UpdateContext> context, bool is_playing) {
        int next_frame = data_->current_frame + 1;
        if (next_frame > data_->total_frames) {
            // If we're at the end, loop back to frame 1 if playing in a loop
            next_frame = 1;
        }
        
        if (is_playing) {
            goto_frame(context, next_frame, false);
        }
    }

    // Run any scripts associated with the current frame
    void run_frame_scripts(std::shared_ptr<UpdateContext> context) {
        // In a real implementation, this would execute any ActionScript
        // associated with the current frame
    }

    // Add a frame label
    void add_frame_label(const std::string& label, int frame) {
        frame_labels_.emplace_back(frame, label);
    }

    // Check if a particular frame exists
    bool frame_exists(int frame_number) const {
        return frame_number >= 1 && frame_number <= data_->total_frames;
    }

    // Get the number of frames in the header
    int header_frames() const {
        // This would return the number of frames from the SWF header
        return data_->total_frames;
    }

    // Post instantiation setup
    void post_instantiation(std::shared_ptr<UpdateContext> context,
                           std::shared_ptr<Object> init_object,
                           Instantiator instantiated_by,
                           bool run_frame) override {
        DisplayObjectContainer::post_instantiation(context, init_object, instantiated_by, run_frame);
        
        if (movie()->is_action_script_3()) {
            set_default_instance_name(context);
            
            if (instantiated_by != Instantiator::AVM) {
                // Create AVM2 object for this movie clip
                auto mc_class = context->avm2->classes().movie_clip;
                auto mc_obj = Avm2StageObject::for_display_object_childless(
                    context, shared_from_this(), mc_class);
                
                try {
                    mc_class->call_constructor(mc_obj, {}, context);
                    set_object2(context, mc_obj);
                } catch (const std::exception& e) {
                    // Handle AVM2 construction error
                    printf("Error running AVM2 construction for movie clip: %s\n", e.what());
                }
            }
            
            on_construction_complete(context);
        }
        
        if (run_frame) {
            if (movie()->is_action_script_3()) {
                run_frame_avm2(context);
            } else {
                run_frame_avm1(context);
            }
        }
    }

    // Handle clip events
    ClipEventResult handle_clip_event(std::shared_ptr<UpdateContext> context,
                                    ClipEvent event) override {
        // Handle movie clip specific events
        switch (event.type()) {
            case EventType::DATA:
                // Handle data loading events
                return ClipEventResult::HANDLED;
            case EventType::INIT:
                // Handle initialization events
                return ClipEventResult::HANDLED;
            default:
                // For other events, let the parent handle them
                return DisplayObjectContainer::handle_clip_event(context, event);
        }
    }

    // Cast to specific types
    std::shared_ptr<MovieClip> as_movie_clip() override {
        return std::static_pointer_cast<MovieClip>(shared_from_this());
    }
    
    bool is_movie_clip() const override { return true; }

    // Check if this object allows being used as a mask
    bool allow_as_mask() const override {
        return !is_empty(); // Movie clips can be used as masks if they're not empty
    }

    // Check if this object is empty
    bool is_empty() const override {
        return children().empty() && data_->current_frame <= 1;
    }

    // Set as root movie
    void set_as_root_movie(std::shared_ptr<UpdateContext> context) {
        is_root_movie_ = true;
        set_default_root_name(context);
    }

    // Get the SWF version of this movie clip
    uint8_t swf_version() const {
        return movie()->version();
    }

    // Check if this is an AVM1 movie clip
    bool is_avm1() const {
        return !movie()->is_action_script_3();
    }

    // Check if this is an AVM2 movie clip
    bool is_avm2() const {
        return movie()->is_action_script_3();
    }
};

} // namespace ruffle

#endif // MOVIE_CLIP_H