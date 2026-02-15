/*
 * C++ header for stage functionality
 * This replaces the functionality of core/src/display_object/stage.rs
 */

#ifndef STAGE_H
#define STAGE_H

#include "display_object.h"
#include "display_object/container.h"
#include "display_object/interactive.h"
#include "avm1.h"
#include "avm2.h"
#include "context.h"
#include "swf_movie.h"
#include "focus_tracker.h"
#include "render_transform.h"
#include "matrix.h"
#include "color.h"
#include "rectangle.h"
#include "twips.h"
#include <memory>
#include <vector>
#include <optional>

namespace ruffle {

// Forward declarations
class UpdateContext;
class RenderContext;
class Avm2StageObject;
class LoaderInfoObject;
class Stage3DObject;
class ViewportDimensions;

// Enum for stage scale modes
enum class StageScaleMode {
    SHOW_ALL,
    NO_BORDER,
    EXACT_FIT,
    NO_SCALE
};

// Enum for stage display states
enum class StageDisplayState {
    NORMAL,
    FULL_SCREEN,
    FULL_SCREEN_INTERACTIVE
};

// Enum for stage alignment
enum class StageAlign {
    TOP = 1 << 0,
    BOTTOM = 1 << 1,
    LEFT = 1 << 2,
    RIGHT = 1 << 3
};

// Enum for window modes
enum class WindowMode {
    WINDOW,
    OPAQUE,
    TRANSPARENT
};

// Enum for letterbox settings
enum class Letterbox {
    OFF,
    ON,
    FULLSCREEN
};

// Stage class - the root of the display object hierarchy
class Stage : public DisplayObjectContainer {
private:
    std::shared_ptr<ChildContainer> child_container_;
    std::shared_ptr<Avm2StageObject> avm2_object_;
    std::shared_ptr<LoaderInfoObject> loader_info_;
    std::vector<std::shared_ptr<Stage3DObject>> stage3ds_;
    FocusTracker focus_tracker_;
    std::shared_ptr<SwfMovie> movie_;
    std::pair<uint32_t, uint32_t> movie_size_;
    Matrix viewport_matrix_;
    Matrix letterbox_matrix_;
    Rectangle<Twips> view_bounds_;
    std::optional<Color> background_color_;
    Letterbox letterbox_;
    StageQuality quality_;
    std::pair<uint32_t, uint32_t> stage_size_;
    StageScaleMode scale_mode_;
    bool forced_scale_mode_;
    StageDisplayState display_state_;
    StageAlign align_;
    bool forced_align_;
    bool allow_fullscreen_;
    bool invalidated_;
    bool use_bitmap_downsampling_;
    WindowMode window_mode_;
    bool stage_focus_rect_;
    bool show_menu_;

public:
    // Constructor
    Stage(std::shared_ptr<SwfMovie> movie, bool fullscreen = false)
        : DisplayObjectContainer(DisplayObjectType::STAGE, 0),  // Stage has ID 0
          movie_(std::move(movie)),
          movie_size_({0, 0}),
          quality_(StageQuality::HIGH),
          stage_size_({0, 0}),
          scale_mode_(StageScaleMode::SHOW_ALL),
          forced_scale_mode_(false),
          display_state_(fullscreen ? StageDisplayState::FULL_SCREEN : StageDisplayState::NORMAL),
          invalidated_(false),
          align_(StageAlign::TOP | StageAlign::LEFT),
          forced_align_(false),
          allow_fullscreen_(true),
          use_bitmap_downsampling_(false),
          window_mode_(WindowMode::WINDOW),
          stage_focus_rect_(true),
          show_menu_(true),
          letterbox_(Letterbox::OFF) {
        
        child_container_ = std::make_shared<ChildContainer>(movie_);
        focus_tracker_ = FocusTracker::create();
    }

    // Create an empty stage
    static std::shared_ptr<Stage> empty(std::shared_ptr<UpdateContext> context,
                                       bool fullscreen = false,
                                       std::shared_ptr<SwfMovie> movie) {
        auto stage = std::make_shared<Stage>(std::move(movie), fullscreen);
        stage->set_is_root(true);
        // In a real implementation, this would set up perspective projection
        return stage;
    }

    // Getters
    std::optional<Color> background_color() const { return background_color_; }
    Matrix inverse_view_matrix() const { return viewport_matrix_.inverse().value_or(Matrix::IDENTITY); }
    Matrix view_matrix() const { return viewport_matrix_; }
    Letterbox letterbox() const { return letterbox_; }
    std::pair<uint32_t, uint32_t> movie_size() const { return movie_size_; }
    bool invalidated() const { return invalidated_; }
    StageQuality quality() const { return quality_; }
    std::pair<uint32_t, uint32_t> stage_size() const { return stage_size_; }
    StageScaleMode scale_mode() const { return scale_mode_; }
    bool forced_scale_mode() const { return forced_scale_mode_; }
    bool allow_fullscreen() const { return allow_fullscreen_; }
    StageDisplayState display_state() const { return display_state_; }
    StageAlign align() const { return align_; }
    bool forced_align() const { return forced_align_; }
    bool use_bitmap_downsampling() const { return use_bitmap_downsampling_; }
    WindowMode window_mode() const { return window_mode_; }
    bool stage_focus_rect() const { return stage_focus_rect_; }
    bool show_menu() const { return show_menu_; }
    Rectangle<Twips> view_bounds() const { return view_bounds_; }
    std::shared_ptr<SwfMovie> movie() const override { return movie_; }
    std::shared_ptr<Avm2StageObject> object2() const override { return avm2_object_; }
    const std::vector<std::shared_ptr<Stage3DObject>>& stage3ds() const { return stage3ds_; }
    FocusTracker& focus_tracker() { return focus_tracker_; }
    const FocusTracker& focus_tracker() const { return focus_tracker_; }

    // Setters
    void set_background_color(std::optional<Color> color) { background_color_ = color; }
    void set_letterbox(Letterbox letterbox) { letterbox_ = letterbox; }
    void set_movie_size(uint32_t width, uint32_t height) { movie_size_ = {width, height}; }
    void set_invalidated(bool value) { invalidated_ = value; }
    void set_stage_focus_rect(bool value) { stage_focus_rect_ = value; }
    void set_show_menu(bool show_menu) { show_menu_ = show_menu; }

    // Set the movie for this stage
    void set_movie(std::shared_ptr<UpdateContext> context, std::shared_ptr<SwfMovie> movie) {
        bool is_action_script_3 = movie->is_action_script_3();
        movie_ = std::move(movie);
        child_container_->set_is_action_script_3(is_action_script_3);
    }

    // Set the loader info
    void set_loader_info(std::shared_ptr<UpdateContext> context, 
                        std::shared_ptr<LoaderInfoObject> loader_info) {
        loader_info_ = std::move(loader_info);
    }

    // Set the AVM2 object
    void set_object2(std::shared_ptr<UpdateContext> context, 
                    std::shared_ptr<Avm2StageObject> obj) override {
        avm2_object_ = std::move(obj);
    }

    // Set the quality of the stage
    void set_quality(std::shared_ptr<UpdateContext> context, StageQuality quality) {
        quality_ = quality;
        use_bitmap_downsampling_ = (quality == StageQuality::BEST ||
                                   quality == StageQuality::HIGH_8X8 ||
                                   quality == StageQuality::HIGH_8X8_LINEAR ||
                                   quality == StageQuality::HIGH_16X16 ||
                                   quality == StageQuality::HIGH_16X16_LINEAR);
        
        // In a real implementation, this would update the renderer quality
        // context->renderer->set_quality(quality);
    }

    // Set the scale mode
    void set_scale_mode(std::shared_ptr<UpdateContext> context, 
                       StageScaleMode scale_mode, 
                       bool respect_forced = true) {
        if (respect_forced && forced_scale_mode_) {
            return;
        }

        scale_mode_ = scale_mode;
        build_matrices(context);
    }

    // Set whether scale mode changes are forced
    void set_forced_scale_mode(bool force) { forced_scale_mode_ = force; }

    // Set whether fullscreen is allowed
    void set_allow_fullscreen(bool allow) { allow_fullscreen_ = allow; }

    // Toggle the display state between fullscreen and normal
    void toggle_display_state(std::shared_ptr<UpdateContext> context) {
        if (is_fullscreen()) {
            set_display_state(context, StageDisplayState::NORMAL);
        } else {
            set_display_state(context, StageDisplayState::FULL_SCREEN);
        }
    }

    // Set the display state
    void set_display_state(std::shared_ptr<UpdateContext> context, 
                          StageDisplayState display_state) {
        if (display_state == this->display_state() ||
            (is_fullscreen_state(display_state) && is_fullscreen()) ||
            !allow_fullscreen()) {
            return;
        }

        bool should_go_fullscreen = (display_state == StageDisplayState::FULL_SCREEN ||
                                   display_state == StageDisplayState::FULL_SCREEN_INTERACTIVE);

        // In a real implementation, this would interact with the UI backend
        // context->ui->set_fullscreen(should_go_fullscreen);

        if (true) { // Assuming success for now
            display_state_ = display_state;
            fire_fullscreen_event(context);
        }
    }

    // Set the alignment
    void set_align(std::shared_ptr<UpdateContext> context, StageAlign align) {
        if (!forced_align_) {
            align_ = align;
            build_matrices(context);
        }
    }

    // Set whether alignment changes are forced
    void set_forced_align(bool force) { forced_align_ = force; }

    // Set whether to use bitmap downsampling
    void set_use_bitmap_downsampling(bool value) { use_bitmap_downsampling_ = value; }

    // Set the window mode
    void set_window_mode(WindowMode window_mode) { window_mode_ = window_mode; }

    // Check if the stage is in fullscreen
    bool is_fullscreen() const {
        return is_fullscreen_state(display_state_);
    }

    // Check if a display state is fullscreen
    static bool is_fullscreen_state(StageDisplayState display_state) {
        return display_state == StageDisplayState::FULL_SCREEN ||
               display_state == StageDisplayState::FULL_SCREEN_INTERACTIVE;
    }

    // Build matrices for rendering
    void build_matrices(std::shared_ptr<UpdateContext> context) {
        auto scale_mode = scale_mode_;
        auto align = align_;
        auto prev_stage_size = stage_size_;
        
        auto viewport_size = context->renderer->viewport_dimensions();

        // Update stage size based on scale mode and DPI
        std::pair<uint32_t, uint32_t> new_stage_size;
        if (scale_mode_ == StageScaleMode::NO_SCALE) {
            // Viewport size is adjusted for HiDPI
            double width = static_cast<double>(viewport_size.width) / viewport_size.scale_factor;
            double height = static_cast<double>(viewport_size.height) / viewport_size.scale_factor;
            new_stage_size = {static_cast<uint32_t>(std::round(width)), 
                             static_cast<uint32_t>(std::round(height))};
        } else {
            new_stage_size = movie_size_;
        }
        
        stage_size_ = new_stage_size;
        bool stage_size_changed = (prev_stage_size != new_stage_size);

        // Create view matrix to scale stage into viewport area
        double movie_width = static_cast<double>(movie_size_.first);
        double movie_height = static_cast<double>(movie_size_.second);
        
        double viewport_width = static_cast<double>(viewport_size.width);
        double viewport_height = static_cast<double>(viewport_size.height);

        double movie_aspect = movie_width / movie_height;
        double viewport_aspect = viewport_width / viewport_height;

        double scale_x, scale_y;
        switch (scale_mode) {
            case StageScaleMode::SHOW_ALL:
                // Keep aspect ratio, padding the edges
                if (viewport_aspect > movie_aspect) {
                    scale_x = scale_y = viewport_height / movie_height;
                } else {
                    scale_x = scale_y = viewport_width / movie_width;
                }
                break;
            case StageScaleMode::NO_BORDER:
                // Keep aspect ratio, cropping off the edges
                if (viewport_aspect < movie_aspect) {
                    scale_x = scale_y = viewport_height / movie_height;
                } else {
                    scale_x = scale_y = viewport_width / movie_width;
                }
                break;
            case StageScaleMode::EXACT_FIT:
                // Stretch to fill container
                scale_x = viewport_width / movie_width;
                scale_y = viewport_height / movie_height;
                break;
            case StageScaleMode::NO_SCALE:
                // No adjustment
                scale_x = scale_y = viewport_size.scale_factor;
                break;
        }

        double width_delta = viewport_width - movie_width * scale_x;
        double height_delta = viewport_height - movie_height * scale_y;
        
        // The precedence is important here to match Flash behavior.
        // L > R > "", T > B > "".
        double tx, ty;
        if (static_cast<int>(align) & static_cast<int>(StageAlign::LEFT)) {
            tx = 0.0;
        } else if (static_cast<int>(align) & static_cast<int>(StageAlign::RIGHT)) {
            tx = width_delta;
        } else {
            tx = width_delta / 2.0;
        }
        
        if (static_cast<int>(align) & static_cast<int>(StageAlign::TOP)) {
            ty = 0.0;
        } else if (static_cast<int>(align) & static_cast<int>(StageAlign::BOTTOM)) {
            ty = height_delta;
        } else {
            ty = height_delta / 2.0;
        }

        // The viewport can be additionally translated from within the SWF header
        auto stage_tx = Matrix::translate(movie_width, movie_height); // Simplified

        Matrix letterbox_matrix(
            static_cast<float>(scale_x), 0.0f, 0.0f, static_cast<float>(scale_y),
            Twips::from_pixels(tx), Twips::from_pixels(ty)
        );
        
        letterbox_matrix_ = letterbox_matrix;
        viewport_matrix_ = letterbox_matrix * stage_tx.inverse().value_or(Matrix::IDENTITY);

        // Calculate view bounds
        if (should_letterbox()) {
            // Letterbox: movie area
            view_bounds_ = Rectangle<Twips>(
                Twips(0), Twips(0),
                Twips::from_pixels(movie_width), Twips::from_pixels(movie_height)
            );
        } else {
            // No letterbox: full visible stage area
            double margin_left = tx / scale_x;
            double margin_right = (width_delta - tx) / scale_x;
            double margin_top = ty / scale_y;
            double margin_bottom = (height_delta - ty) / scale_y;
            
            view_bounds_ = Rectangle<Twips>(
                Twips::from_pixels(-margin_left), Twips::from_pixels(-margin_top),
                Twips::from_pixels(movie_width + margin_right), 
                Twips::from_pixels(movie_height + margin_bottom)
            );
        }

        // Fire resize handler if stage size has changed
        if (scale_mode_ == StageScaleMode::NO_SCALE && stage_size_changed) {
            fire_resize_event(context);
        }
    }

    // Render the stage from the perspective of the viewport
    void render_viewport(std::shared_ptr<RenderContext> context) {
        context->transform_stack.push(Transform{
            viewport_matrix_,
            ColorTransform::IDENTITY,
            // In a real implementation, this would handle perspective projection
        });

        // All Stage3D instances get rendered underneath the main stage
        for (const auto& stage3d : stage3ds_) {
            if (stage3d->visible()) {
                // In a real implementation, this would render the 3D context
                // stage3d->context3d()->render(context);
            }
        }

        render(context);

        focus_tracker_.render_highlight(context);

        if (should_letterbox()) {
            draw_letterbox(context);
        }

        context->transform_stack.pop();
    }

    // Fire a resize event
    void fire_resize_event(std::shared_ptr<UpdateContext> context) {
        // In a real implementation, this would dispatch a resize event to AVM2
        if (auto obj = object2()) {
            // Create and dispatch resize event
            // Avm2::dispatch_event(context, resize_event, obj);
        }
    }

    // Fire a fullscreen event
    void fire_fullscreen_event(std::shared_ptr<UpdateContext> context) {
        // In a real implementation, this would dispatch a fullscreen event to AVM2
        if (auto obj = object2()) {
            // Create and dispatch fullscreen event
            // Avm2::dispatch_event(context, fullscreen_event, obj);
        }
    }

    // Check if we should letterbox the stage content
    bool should_letterbox() const {
        // Only enable letterbox in the default `ShowAll` scale mode.
        // If content changes the scale mode or alignment, it signals that it is size-aware.
        return letterbox_ == Letterbox::ON ||
               (letterbox_ == Letterbox::FULLSCREEN && is_fullscreen()) &&
               scale_mode_ == StageScaleMode::SHOW_ALL &&
               static_cast<int>(align_) == 0 &&  // is_empty()
               window_mode_ != WindowMode::TRANSPARENT;
    }

    // Draw the letterbox
    void draw_letterbox(std::shared_ptr<RenderContext> context) {
        auto dims = context->renderer->viewport_dimensions();
        float viewport_width = static_cast<float>(dims.width);
        float viewport_height = static_cast<float>(dims.height);

        float movie_width = static_cast<float>(movie_size_.first) * letterbox_matrix_.a();
        float movie_height = static_cast<float>(movie_size_.second) * letterbox_matrix_.d();

        float margin_left = letterbox_matrix_.tx().to_pixels();
        float margin_right = viewport_width - movie_width - margin_left;
        float margin_top = letterbox_matrix_.ty().to_pixels();
        float margin_bottom = viewport_height - movie_height - margin_top;

        // Letterboxing only occurs in `StageScaleMode::ShowAll`, and they would only appear on the top+bottom or left+right.
        if (margin_top + margin_bottom > margin_left + margin_right) {
            // Top + bottom
            if (margin_top > 0.0f) {
                context->commands.draw_rect(
                    Color(0, 0, 0, 255),  // Black
                    Matrix::create_box(
                        viewport_width,
                        margin_top,
                        Twips(0),
                        Twips(0)
                    )
                );
            }
            if (margin_bottom > 0.0f) {
                context->commands.draw_rect(
                    Color(0, 0, 0, 255),  // Black
                    Matrix::create_box(
                        viewport_width,
                        margin_bottom,
                        Twips(0),
                        Twips::from_pixels(margin_top + movie_height)
                    )
                );
            }
        } else {
            // Left + right
            if (margin_left > 0.0f) {
                context->commands.draw_rect(
                    Color(0, 0, 0, 255),  // Black
                    Matrix::create_box(
                        margin_left,
                        viewport_height,
                        Twips(0),
                        Twips(0)
                    )
                );
            }
            if (margin_right > 0.0f) {
                context->commands.draw_rect(
                    Color(0, 0, 0, 255),  // Black
                    Matrix::create_box(
                        margin_right,
                        viewport_height,
                        Twips::from_pixels(margin_left + movie_width),
                        Twips(0)
                    )
                );
            }
        }
    }

    // Handle clip events
    ClipEventResult handle_clip_event(std::shared_ptr<UpdateContext> context,
                                    ClipEvent event) override {
        // Stage handles specific events
        switch (event.type()) {
            case EventType::ENTER_FRAME:
                // Broadcast frame entered event to all children
                broadcast_frame_entered(context);
                break;
            default:
                break;
        }
        
        return DisplayObjectContainer::handle_clip_event(context, event);
    }

    // Check if this object allows being used as a mask
    bool allow_as_mask() const override {
        return false; // Stage cannot be used as a mask
    }

    // Check if this object is empty
    bool is_empty() const override {
        return false; // Stage is never empty
    }

    // Cast to specific types
    std::shared_ptr<Stage> as_stage() override {
        return std::static_pointer_cast<Stage>(shared_from_this());
    }
    
    bool is_stage() const override { return true; }
};

} // namespace ruffle

#endif // STAGE_H