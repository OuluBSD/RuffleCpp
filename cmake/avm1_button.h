/*
 * C++ header for AVM1 button functionality
 * This replaces the functionality of core/src/display_object/avm1_button.rs
 */

#ifndef AVM1_BUTTON_H
#define AVM1_BUTTON_H

#include "display_object.h"
#include "display_object/interactive.h"
#include "display_object/container.h"
#include "avm1.h"
#include "context.h"
#include "bounds_mode.h"
#include "events.h"
#include "string.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <optional>
#include <functional>

namespace ruffle {

// Enum for button states
enum class ButtonState {
    UP,
    OVER,
    DOWN
};

// Enum for button tracking modes
enum class ButtonTracking {
    PUSH,
    MENU
};

// Condition for button actions
enum class ButtonActionCondition {
    OVER_DOWN_TO_OUT_DOWN,
    OUT_DOWN_TO_OVER_DOWN,
    OVER_UP_TO_OVER_DOWN,
    OVER_DOWN_TO_OVER_UP,
    OUT_DOWN_TO_IDLE,
    OVER_UP_TO_IDLE,
    IDLE_TO_OVER_UP,
    KEY_PRESS
};

// Structure for button actions
struct ButtonAction {
    std::vector<uint8_t> action_data;
    ButtonActionCondition conditions;
    
    ButtonAction(std::vector<uint8_t> data, ButtonActionCondition cond)
        : action_data(std::move(data)), conditions(cond) {}
};

// Shared data between all instances of a button
struct ButtonShared {
    std::shared_ptr<SwfMovie> swf;
    uint16_t id;
    std::vector<ButtonAction> actions;
    std::vector<swf::ButtonRecord> records;
    
    // Sounds for state changes
    std::optional<swf::ButtonSound> up_to_over_sound;
    std::optional<swf::ButtonSound> over_to_down_sound;
    std::optional<swf::ButtonSound> down_to_over_sound;
    std::optional<swf::ButtonSound> over_to_up_sound;
    
    ButtonShared(std::shared_ptr<SwfMovie> movie, uint16_t button_id)
        : swf(std::move(movie)), id(button_id) {}
};

// AVM1 Button class
class Avm1Button : public InteractiveObject {
private:
    std::shared_ptr<ButtonShared> shared_data_;
    std::shared_ptr<Object> object_;
    ButtonState state_;
    ButtonTracking tracking_;
    bool initialized_;
    std::unordered_map<int, std::shared_ptr<DisplayObject>> hit_area_;
    Rectangle<Twips> hit_bounds_;
    std::vector<Avm1TextFieldBinding> text_field_bindings_;

public:
    Avm1Button(std::shared_ptr<ButtonShared> shared_data)
        : InteractiveObject(DisplayObjectType::AVM1_BUTTON, shared_data->id),
          shared_data_(std::move(shared_data)), 
          state_(ButtonState::UP), 
          tracking_(ButtonTracking::PUSH),
          initialized_(false) {}

    // Create an AVM1 button from SWF tag data
    static std::shared_ptr<Avm1Button> from_swf_tag(
        const swf::Button& button,
        std::shared_ptr<SwfSlice> source_movie,
        std::shared_ptr<MutationContext> mc) {
        
        auto actions = std::vector<ButtonAction>();
        for (const auto& action : button.actions) {
            std::vector<uint8_t> action_data(action.action_data.begin(), action.action_data.end());
            actions.emplace_back(std::move(action_data), ButtonActionCondition::KEY_PRESS); // Simplified
        }

        auto shared_data = std::make_shared<ButtonShared>(source_movie->movie(), button.id);
        shared_data->actions = std::move(actions);
        shared_data->records = button.records;
        
        auto button_obj = std::make_shared<Avm1Button>(std::move(shared_data));
        
        if (button.is_track_as_menu) {
            button_obj->tracking_ = ButtonTracking::MENU;
        } else {
            button_obj->tracking_ = ButtonTracking::PUSH;
        }
        
        return button_obj;
    }

    // Set sounds for the button
    void set_sounds(const swf::ButtonSounds& sounds) {
        shared_data_->up_to_over_sound = sounds.up_to_over_sound;
        shared_data_->over_to_down_sound = sounds.over_to_down_sound;
        shared_data_->down_to_over_sound = sounds.down_to_over_sound;
        shared_data_->over_to_up_sound = sounds.over_to_up_sound;
    }

    // Handle the ancient DefineButtonCxform SWF tag
    // Set the color transform for all children of each state
    void set_colors(const std::vector<swf::ColorTransform>& color_transforms) {
        // This tag isn't well documented. It applies color transforms to every character in a button
        for (size_t i = 0; i < shared_data_->records.size() && i < color_transforms.size(); ++i) {
            shared_data_->records[i].color_transform = color_transforms[i];
        }
    }

    // Set the state of a button, creating or destroying children as needed
    void set_state(std::shared_ptr<UpdateContext> context, ButtonState state) {
        // Get depths of children that will be removed
        std::unordered_set<int> removed_depths;
        for (const auto& child : children()) {
            removed_depths.insert(child->depth());
        }

        state_ = state;

        // Create any new children that exist in this state, and remove children
        // that only exist in the previous state
        std::vector<std::pair<std::shared_ptr<DisplayObject>, int>> new_children;

        for (const auto& record : shared_data_->records) {
            if (record.states.contains(static_cast<swf::ButtonState>(state))) {
                // State contains this depth, so we don't have to remove it
                removed_depths.erase(record.depth);

                std::shared_ptr<DisplayObject> child = nullptr;
                auto existing_child = get_child_by_depth(record.depth);
                if (existing_child && existing_child->id() == record.id) {
                    // Re-use existing child
                    child = existing_child;
                } else {
                    // Instantiate new child
                    auto library = context->library;
                    auto movie = this->movie();
                    child = library->instantiate_by_id(record.id, context->gc_context);
                    
                    if (child) {
                        child->set_parent(context, shared_from_this());
                        child->set_depth(record.depth);
                        new_children.emplace_back(child, record.depth);
                    }
                }

                if (child) {
                    // Set transform of child
                    child->set_matrix(Matrix(record.matrix));
                    child->set_color_transform(record.color_transform);
                    child->set_blend_mode(static_cast<BlendMode>(record.blend_mode));
                    // Set filters (simplified)
                }
            }
        }

        // Remove children that no longer exist in this state
        for (int depth : removed_depths) {
            if (auto child = get_child_by_depth(depth)) {
                remove_child(context, child);
            }
        }

        // Add new children to the display list
        for (const auto& [child, depth] : new_children) {
            // Initialize new child
            child->post_instantiation(context, nullptr, Instantiator::MOVIE, false);
            if (auto mc = child->as_movie_clip()) {
                mc->run_frame_avm1(context);
            }
            replace_at_depth(context, child, depth);
            
            // Dispatch added event
            dispatch_added_event(shared_from_this(), child, false, context);
        }

        invalidate_cached_bitmap();
    }

    // Get the current button state
    ButtonState state() const { return state_; }

    // Get the tracking mode
    ButtonTracking tracking() const { return tracking_; }

    // Check if the button is enabled
    bool enabled(std::shared_ptr<UpdateContext> context) const {
        return get_boolean_property(context, "enabled", true);
    }

    // Check if the button should use a hand cursor
    bool use_hand_cursor(std::shared_ptr<UpdateContext> context) const {
        return get_boolean_property(context, "useHandCursor", true);
    }

    // Handle events for the button
    ClipEventResult handle_event(std::shared_ptr<UpdateContext> context, ClipEvent event) override {
        auto is_enabled = enabled(context);

        // Translate the clip event to a button event, based on how the button state changes
        std::optional<ButtonState> new_state;
        std::optional<ButtonActionCondition> condition;
        std::optional<swf::ButtonSound> sound;

        switch (event.type()) {
            case EventType::DRAG_OUT:
                new_state = ButtonState::OVER;
                condition = ButtonActionCondition::OVER_DOWN_TO_OUT_DOWN;
                break;
            case EventType::DRAG_OVER:
                new_state = ButtonState::DOWN;
                condition = ButtonActionCondition::OUT_DOWN_TO_OVER_DOWN;
                break;
            case EventType::PRESS:
                new_state = ButtonState::DOWN;
                condition = ButtonActionCondition::OVER_UP_TO_OVER_DOWN;
                if (shared_data_->over_to_down_sound) {
                    sound = shared_data_->over_to_down_sound.value();
                }
                break;
            case EventType::RELEASE:
                new_state = ButtonState::OVER;
                condition = ButtonActionCondition::OVER_DOWN_TO_OVER_UP;
                if (shared_data_->down_to_over_sound) {
                    sound = shared_data_->down_to_over_sound.value();
                }
                break;
            case EventType::RELEASE_OUTSIDE:
                new_state = ButtonState::UP;
                condition = ButtonActionCondition::OUT_DOWN_TO_IDLE;
                if (shared_data_->over_to_up_sound) {
                    sound = shared_data_->over_to_up_sound.value();
                }
                break;
            case EventType::ROLL_OUT:
                new_state = ButtonState::UP;
                condition = ButtonActionCondition::OVER_UP_TO_IDLE;
                if (shared_data_->over_to_up_sound) {
                    sound = shared_data_->over_to_up_sound.value();
                }
                break;
            case EventType::ROLL_OVER:
                new_state = ButtonState::OVER;
                condition = ButtonActionCondition::IDLE_TO_OVER_UP;
                if (shared_data_->up_to_over_sound) {
                    sound = shared_data_->up_to_over_sound.value();
                }
                break;
            case EventType::KEY_PRESS:
                // Handle key press event
                run_actions(context, ButtonActionCondition::KEY_PRESS);
                return ClipEventResult::HANDLED;
            default:
                // For other events, just update the state if needed
                if (new_state.has_value() && is_enabled) {
                    set_state(context, new_state.value());
                }
                return ClipEventResult::NOT_HANDLED;
        }

        if (is_enabled) {
            if (condition.has_value()) {
                run_actions(context, condition.value());
            }
            
            if (sound.has_value()) {
                // Play the sound (simplified)
                // AudioManager::perform_sound_event(shared_from_this(), context, sound->id, sound->info);
            }
        } else {
            // If button is disabled, reset to UP state
            set_state(context, ButtonState::UP);
        }

        return ClipEventResult::HANDLED;
    }

    // Run actions for the button
    void run_actions(std::shared_ptr<UpdateContext> context, ButtonActionCondition condition) {
        if (auto parent = this->parent()) {
            for (const auto& action : shared_data_->actions) {
                if (action.conditions == condition) {
                    // Queue the action to run relative to the parent (not the button itself)
                    context->action_queue.queue_action(
                        parent,
                        ActionType::NORMAL,
                        action.action_data,
                        false
                    );
                }
            }
        }
    }

    // Mouse picking for AVM1
    std::shared_ptr<InteractiveObject> mouse_pick_avm1(
        std::shared_ptr<UpdateContext> context,
        Point<Twips> point,
        bool require_button_mode) const override {
        
        if (visible() && mouse_enabled()) {
            // Check children in reverse order (top to bottom)
            for (auto it = children().rbegin(); it != children().rend(); ++it) {
                if (auto interactive_child = (*it)->as_interactive()) {
                    if (auto result = interactive_child->mouse_pick_avm1(context, point, require_button_mode)) {
                        return result;
                    }
                }
            }

            // Check hit area
            for (const auto& [depth, child] : hit_area_) {
                if (child->hit_test_shape(context, point, HitTestOptions::MOUSE_PICK)) {
                    return std::const_pointer_cast<Avm1Button>(shared_from_this());
                }
            }
        }
        
        return nullptr;
    }

    // Get mouse cursor for this button
    MouseCursor mouse_cursor(std::shared_ptr<UpdateContext> context) const override {
        if (use_hand_cursor(context) && enabled(context)) {
            return MouseCursor::HAND;
        } else {
            return MouseCursor::ARROW;
        }
    }

    // Post instantiation setup
    void post_instantiation(
        std::shared_ptr<UpdateContext> context,
        std::shared_ptr<Object> init_object,
        Instantiator instantiated_by,
        bool run_frame) override {
        
        set_default_instance_name(context);

        if (!object_) {
            // Create the AVM1 object for this button
            auto prototype = context->avm1->prototypes().button;
            object_ = Object::new_with_native(
                &context->strings,
                prototype,
                NativeObject::Button(shared_from_this())
            );
        }

        if (!initialized_) {
            std::vector<std::pair<std::shared_ptr<DisplayObject>, int>> new_children;

            set_state(context, ButtonState::UP);
            initialized_ = true;

            for (const auto& record : shared_data_->records) {
                if (record.states.contains(swf::ButtonState::HIT_TEST)) {
                    auto child = context->library->instantiate_by_id(record.id, context->gc_context);
                    if (child) {
                        child->set_matrix(Matrix(record.matrix));
                        child->set_parent(context, shared_from_this());
                        child->set_depth(record.depth);
                        new_children.emplace_back(child, record.depth);
                    } else {
                        printf("Button ID %d: could not instantiate child ID %d\n", 
                               shared_data_->id, record.id);
                    }
                }
            }

            // Calculate hit bounds
            Rectangle<Twips> hit_bounds = Rectangle<Twips>::invalid();
            for (const auto& [child, depth] : new_children) {
                child->post_instantiation(context, nullptr, Instantiator::MOVIE, false);
                hit_area_[depth] = child;
                hit_bounds = hit_bounds.union_with(child->local_bounds(BoundsMode::ENGINE));
            }
            hit_bounds_ = hit_bounds;
        }
    }

    // AVM1 unload
    void avm1_unload(std::shared_ptr<UpdateContext> context) override {
        // Unload all children
        for (const auto& child : children()) {
            child->avm1_unload(context);
        }

        // Drop focus if this button has it
        drop_focus(context);

        // Handle masking relationships
        if (auto maskee = this->maskee()) {
            maskee->set_masker(context->gc(), nullptr, true);
        } else if (auto masker = this->masker()) {
            masker->set_maskee(context->gc(), nullptr, true);
        }

        set_avm1_removed(true);
    }

    // Render the button
    void render_self(std::shared_ptr<RenderContext> context) override {
        render_children(context);
    }

    // Get bounds
    Rectangle<Twips> self_bounds(BoundsMode mode) const override {
        // No inherent bounds; contains child DisplayObjects
        return Rectangle<Twips>();
    }

    // Hit test
    bool hit_test_shape(
        std::shared_ptr<UpdateContext> context,
        Point<Twips> point,
        HitTestOptions options) const override {
        
        for (const auto& child : children()) {
            if (child->hit_test_shape(context, point, options)) {
                return true;
            }
        }

        return false;
    }

    // Get AVM1 object
    std::shared_ptr<Object> object1() const override {
        return object_;
    }

    // Get AVM2 object (none for AVM1 buttons)
    std::shared_ptr<StageObject> object2() const override {
        return nullptr; // AVM1 buttons don't have an AVM2 object
    }

    // Check if can be used as mask
    bool allow_as_mask() const override {
        return !is_empty();
    }

    // Get text field bindings
    std::vector<Avm1TextFieldBinding> avm1_text_field_bindings() const {
        return text_field_bindings_;
    }

    // Add a text field binding
    void add_text_field_binding(Avm1TextFieldBinding binding) {
        text_field_bindings_.push_back(std::move(binding));
    }

private:
    // Helper method to get a boolean property from the AVM1 object
    bool get_boolean_property(std::shared_ptr<UpdateContext> context, 
                             const std::string& property_name, 
                             bool default_value) const {
        if (object_) {
            auto activation = Activation::from_nothing(context);
            auto value = object_->get(property_name, activation);
            if (value) {
                return value->as_bool(context->player_version);
            }
        }
        return default_value;
    }
};

} // namespace ruffle

#endif // AVM1_BUTTON_H