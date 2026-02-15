/*
 * C++ header for loader display object functionality
 * This replaces the functionality of core/src/display_object/loader_display.rs
 */

#ifndef LOADER_DISPLAY_H
#define LOADER_DISPLAY_H

#include "display_object.h"
#include "display_object/interactive.h"
#include "display_object/container.h"
#include "avm1.h"
#include "avm2.h"
#include "context.h"
#include "swf_movie.h"
#include <memory>
#include <vector>

namespace ruffle {

// Forward declarations
class UpdateContext;
class RenderContext;
class Activation;
class ChildContainer;

// Weak reference to a loader display object
class LoaderDisplayWeak {
private:
    std::weak_ptr<struct LoaderDisplayData> data_;

public:
    explicit LoaderDisplayWeak(std::shared_ptr<struct LoaderDisplayData> data) : data_(data) {}

    std::shared_ptr<struct LoaderDisplay> upgrade() const;
    
    void* as_ptr() const {
        auto locked = data_.lock();
        return locked ? locked.get() : nullptr;
    }
};

// Loader display object - used for loading external content
class LoaderDisplay : public DisplayObjectContainer {
private:
    std::shared_ptr<ChildContainer> container_;
    std::shared_ptr<Avm2StageObject> avm2_object_;
    std::shared_ptr<SwfMovie> movie_;

public:
    LoaderDisplay(std::shared_ptr<ChildContainer> container,
                  std::shared_ptr<SwfMovie> movie)
        : DisplayObjectContainer(DisplayObjectType::LOADER_DISPLAY, UINT16_MAX),
          container_(std::move(container)), movie_(std::move(movie)) {}

    // Create an empty loader display
    static std::shared_ptr<LoaderDisplay> empty(std::shared_ptr<Activation> activation,
                                               std::shared_ptr<SwfMovie> movie) {
        auto container = std::make_shared<ChildContainer>(movie);
        auto loader_display = std::make_shared<LoaderDisplay>(std::move(container), std::move(movie));
        
        loader_display->set_placed_by_avm2_script(true);
        activation->context()->orphan_manager.add_orphan_object(loader_display);
        
        return loader_display;
    }

    // Getters
    std::shared_ptr<ChildContainer> container() const { return container_; }
    std::shared_ptr<Avm2StageObject> object2() const override { return avm2_object_; }
    std::shared_ptr<SwfMovie> movie() const override { return movie_; }
    std::shared_ptr<ChildContainer> raw_container() const { return container_; }
    std::shared_ptr<ChildContainer> raw_container_mut() { return container_; }

    // Setters
    void set_object2(std::shared_ptr<UpdateContext> context, 
                    std::shared_ptr<Avm2StageObject> obj) override {
        avm2_object_ = std::move(obj);
    }

    // Render self - just render children
    void render_self(std::shared_ptr<RenderContext> context) override {
        render_children(context);
    }

    // Get self bounds (empty for loader display)
    Rectangle<Twips> self_bounds(BoundsMode mode) const override {
        return Rectangle<Twips>(); // Default/empty rectangle
    }

    // AVM1 object is always null for loader display
    std::shared_ptr<Object> object1() const override {
        return nullptr;
    }

    // Enter frame - propagate to children
    void enter_frame(std::shared_ptr<UpdateContext> context) override {
        bool skip_frame = base().should_skip_next_enter_frame();
        for (const auto& child : children()) {
            // See MovieClip::enter_frame for an explanation of this.
            if (skip_frame) {
                child->base().set_skip_next_enter_frame(true);
            }
            child->enter_frame(context);
        }
        base().set_skip_next_enter_frame(false);
    }

    // Construct frame - propagate to children
    void construct_frame(std::shared_ptr<UpdateContext> context) override {
        for (const auto& child : children()) {
            child->construct_frame(context);
        }
    }

    // Post instantiation setup
    void post_instantiation(std::shared_ptr<UpdateContext> context,
                           std::shared_ptr<Object> init_object,
                           Instantiator instantiated_by,
                           bool run_frame) override {
        DisplayObject::post_instantiation(context, init_object, instantiated_by, run_frame);
        set_default_instance_name(context);
    }

    // Called when parent is removed
    void on_parent_removed(std::shared_ptr<UpdateContext> context) override {
        if (movie()->is_action_script_3()) {
            context->orphan_manager.add_orphan_object(shared_from_this());
        }
    }

    // Mouse picking for AVM2
    Avm2MousePick mouse_pick_avm2(std::shared_ptr<UpdateContext> context,
                                  Point<Twips> point,
                                  bool require_button_mode) const override {
        HitTestOptions options = HitTestOptions::SKIP_INVISIBLE;
        if (masker().is_none()) {
            options |= HitTestOptions::SKIP_MASK;
        }

        if (visible()) {
            // We have at most one child
            if (!children().empty()) {
                auto child = children().front();
                if (auto interactive_child = child->as_interactive()) {
                    if (child->movie()->is_action_script_3()) {
                        return interactive_child->mouse_pick_avm2(context, point, require_button_mode)
                               .combine_with_parent(shared_from_this());
                    } else {
                        auto avm1_result = interactive_child->mouse_pick_avm1(context, point, require_button_mode);
                        if (avm1_result) {
                            return Avm2MousePick::Hit(avm1_result);
                        } else {
                            return Avm2MousePick::Miss;
                        }
                    }
                } else if (child->hit_test_shape(context, point, options)) {
                    if (mouse_enabled()) {
                        return Avm2MousePick::Hit(shared_from_this());
                    } else {
                        return Avm2MousePick::PropagateToParent;
                    }
                }
            }
        }
        return Avm2MousePick::Miss;
    }

    // Filter clip events
    ClipEventResult filter_clip_event(std::shared_ptr<UpdateContext> context,
                                    ClipEvent event) const override {
        if (!visible()) {
            return ClipEventResult::NOT_HANDLED;
        }
        return ClipEventResult::HANDLED;
    }

    // Event dispatch
    ClipEventResult event_dispatch(std::shared_ptr<UpdateContext> context,
                                 ClipEvent event) const override {
        return ClipEventResult::NOT_HANDLED;
    }

    // Create a weak reference
    LoaderDisplayWeak weak_ref() const {
        // In a real implementation, this would return a proper weak reference
        return LoaderDisplayWeak(nullptr);
    }

    // Cast to specific types
    std::shared_ptr<LoaderDisplay> as_loader_display() override {
        return std::static_pointer_cast<LoaderDisplay>(shared_from_this());
    }
    
    bool is_loader_display() const override { return true; }

    // Check if this object allows being used as a mask
    bool allow_as_mask() const override {
        return !is_empty(); // Can be used as mask if not empty
    }

    // Check if this object is empty
    bool is_empty() const override {
        return container_->is_empty();
    }
};

// Implementation of the weak reference upgrade method
inline std::shared_ptr<LoaderDisplay> LoaderDisplayWeak::upgrade() const {
    auto locked = data_.lock();
    if (locked) {
        // In a real implementation, we would need to get the actual LoaderDisplay object
        // from the data. This is a simplified version.
        return nullptr; // Placeholder
    }
    return nullptr;
}

} // namespace ruffle

#endif // LOADER_DISPLAY_H