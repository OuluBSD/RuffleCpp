/*
 * C++ header for interactive object functionality
 * This replaces the functionality of core/src/display_object/interactive.rs
 */

#ifndef INTERACTIVE_OBJECT_H
#define INTERACTIVE_OBJECT_H

#include "display_object.h"
#include "avm1.h"
#include "avm2.h"
#include "context.h"
#include "events.h"
#include <memory>
#include <vector>
#include <optional>
#include <functional>

namespace ruffle {

// Forward declarations
class UpdateContext;
class MouseEvent;
class Avm2EventObject;
enum class MouseButton;

// Flags for interactive objects
enum class InteractiveObjectFlag {
    MOUSE_ENABLED = 1 << 0,
    DOUBLE_CLICK_ENABLED = 1 << 1,
    HAS_FOCUS = 1 << 2
};

// Structure for interactive object base properties
struct InteractiveObjectBase {
    DisplayObjectBase base;
    std::shared_ptr<Value> context_menu;
    std::optional<int32_t> tab_index;
    uint8_t flags;
    std::optional<bool> tab_enabled;
    std::optional<bool> focus_rect;

    InteractiveObjectBase() : flags(InteractiveObjectFlag::MOUSE_ENABLED), context_menu(nullptr) {}

    bool has_flag(InteractiveObjectFlag flag) const {
        return (flags & static_cast<uint8_t>(flag)) != 0;
    }

    void set_flag(InteractiveObjectFlag flag, bool value) {
        if (value) {
            flags |= static_cast<uint8_t>(flag);
        } else {
            flags &= ~static_cast<uint8_t>(flag);
        }
    }
};

// Result of handling a clip event
enum class ClipEventResult {
    HANDLED,
    NOT_HANDLED
};

// Enum for different types of interactive objects
enum class InteractiveObjectType {
    STAGE,
    AVM1_BUTTON,
    AVM2_BUTTON,
    MOVIE_CLIP,
    EDIT_TEXT,
    LOADER_DISPLAY
};

// Base class for interactive objects
class InteractiveObject : public DisplayObject {
protected:
    InteractiveObjectBase interactive_base_;

public:
    InteractiveObject(DisplayObjectType type, uint16_t id) 
        : DisplayObject(type, id) {
        interactive_base_.flags = static_cast<uint8_t>(InteractiveObjectFlag::MOUSE_ENABLED);
    }

    // Check if the interactive object accepts mouse and other user events
    bool mouse_enabled() const {
        return interactive_base_.has_flag(InteractiveObjectFlag::MOUSE_ENABLED);
    }

    // Set if the interactive object accepts mouse and other user events
    void set_mouse_enabled(bool value) {
        interactive_base_.set_flag(InteractiveObjectFlag::MOUSE_ENABLED, value);
    }

    // Check if the interactive object accepts double-click events
    bool double_click_enabled() const {
        return interactive_base_.has_flag(InteractiveObjectFlag::DOUBLE_CLICK_ENABLED);
    }

    // Set if the interactive object accepts double-click events
    void set_double_click_enabled(bool value) {
        interactive_base_.set_flag(InteractiveObjectFlag::DOUBLE_CLICK_ENABLED, value);
    }

    // Check if the interactive object has focus
    bool has_focus() const {
        return interactive_base_.has_flag(InteractiveObjectFlag::HAS_FOCUS);
    }

    // Set if the interactive object has focus
    void set_has_focus(bool value) {
        interactive_base_.set_flag(InteractiveObjectFlag::HAS_FOCUS, value);
    }

    // Get context menu
    std::shared_ptr<Value> context_menu() const {
        return interactive_base_.context_menu;
    }

    // Set context menu
    void set_context_menu(std::shared_ptr<Value> value) {
        interactive_base_.context_menu = std::move(value);
    }

    // Get focus rectangle setting
    std::optional<bool> focus_rect() const {
        return interactive_base_.focus_rect;
    }

    // Set focus rectangle setting
    void set_focus_rect(std::optional<bool> value) {
        interactive_base_.focus_rect = value;
    }

    // Get tab index
    std::optional<int32_t> tab_index() const {
        return interactive_base_.tab_index;
    }

    // Set tab index
    void set_tab_index(int32_t index) {
        interactive_base_.tab_index = index;
    }

    // Check if tab is enabled
    bool tab_enabled(std::shared_ptr<UpdateContext> context) const {
        if (interactive_base_.tab_enabled.has_value()) {
            return interactive_base_.tab_enabled.value();
        }
        
        // Default behavior depends on movie version
        if (movie()->is_action_script_3()) {
            return true; // AVM2 objects are tab-enabled by default
        } else {
            return false; // AVM1 objects are not tab-enabled by default
        }
    }

    // Set tab enabled
    void set_tab_enabled(std::shared_ptr<UpdateContext> context, bool enabled) {
        interactive_base_.tab_enabled = enabled;
    }

    // Filter the incoming clip event
    virtual ClipEventResult filter_clip_event(std::shared_ptr<UpdateContext> context, 
                                            ClipEvent event) {
        // Default implementation allows all events
        return ClipEventResult::HANDLED;
    }

    // Propagate the event to children
    virtual ClipEventResult propagate_to_children(std::shared_ptr<UpdateContext> context,
                                                ClipEvent event) {
        // If this is a container, propagate to children
        if (auto container = as_container()) {
            // Mouse events fire in reverse order (high depth to low depth)
            // Button and key events fire in render list order (low depth to high depth)
            auto children = container->children();
            if (event.is_mouse_event()) {
                // Process in reverse order
                for (auto it = children.rbegin(); it != children.rend(); ++it) {
                    if (auto interactive_child = (*it)->as_interactive()) {
                        if (interactive_child->handle_clip_event(context, event) == ClipEventResult::HANDLED) {
                            return ClipEventResult::HANDLED;
                        }
                    }
                }
            } else {
                // Process in normal order
                for (const auto& child : children) {
                    if (auto interactive_child = child->as_interactive()) {
                        if (interactive_child->handle_clip_event(context, event) == ClipEventResult::HANDLED) {
                            return ClipEventResult::HANDLED;
                        }
                    }
                }
            }
        }
        return ClipEventResult::NOT_HANDLED;
    }

    // Dispatch the event to script event handlers
    virtual ClipEventResult event_dispatch(std::shared_ptr<UpdateContext> context,
                                         ClipEvent event) {
        // Default implementation: dispatch to AVM2 if available
        if (auto obj2 = object2()) {
            return event_dispatch_to_avm2(context, event);
        }
        return ClipEventResult::NOT_HANDLED;
    }

    // Convert the clip event into an AVM2 event and dispatch it
    virtual ClipEventResult event_dispatch_to_avm2(std::shared_ptr<UpdateContext> context,
                                                 ClipEvent event) {
        if (!movie()->is_action_script_3()) {
            return ClipEventResult::NOT_HANDLED;
        }

        auto target = object2();
        if (!target) {
            return ClipEventResult::NOT_HANDLED;
        }

        // Convert the event to an AVM2 event and dispatch it
        switch (event.type()) {
            case EventType::PRESS:
            case EventType::RIGHT_PRESS:
            case EventType::MIDDLE_PRESS: {
                auto mouse_btn = event.mouse_button();
                auto avm2_event = Avm2EventObject::create_mouse_down_event(
                    context, shared_from_this(), mouse_btn);
                
                bool handled = Avm2::dispatch_event(context, avm2_event, target);
                
                if (handled) {
                    // When there are click handlers, also fire click event
                    auto click_event = Avm2EventObject::create_mouse_click_event(
                        context, shared_from_this(), mouse_btn);
                    Avm2::dispatch_event(context, click_event, target);
                }
                
                return handled ? ClipEventResult::HANDLED : ClipEventResult::NOT_HANDLED;
            }
            
            case EventType::MOUSE_UP_INSIDE:
            case EventType::RIGHT_MOUSE_UP_INSIDE:
            case EventType::MIDDLE_MOUSE_UP_INSIDE: {
                auto mouse_btn = event.mouse_button();
                auto avm2_event = Avm2EventObject::create_mouse_up_event(
                    context, shared_from_this(), mouse_btn);
                
                bool handled = Avm2::dispatch_event(context, avm2_event, target);
                return handled ? ClipEventResult::HANDLED : ClipEventResult::NOT_HANDLED;
            }
            
            case EventType::RELEASE: {
                bool is_double_click = event.click_index() % 2 != 0;
                bool double_click_enabled = this->double_click_enabled();
                
                if (is_double_click && double_click_enabled) {
                    auto avm2_event = Avm2EventObject::create_double_click_event(
                        context, shared_from_this());
                    bool handled = Avm2::dispatch_event(context, avm2_event, target);
                    return handled ? ClipEventResult::HANDLED : ClipEventResult::NOT_HANDLED;
                } else {
                    auto avm2_event = Avm2EventObject::create_click_event(
                        context, shared_from_this(), MouseButton::LEFT);
                    bool handled = Avm2::dispatch_event(context, avm2_event, target);
                    return handled ? ClipEventResult::HANDLED : ClipEventResult::NOT_HANDLED;
                }
            }
            
            case EventType::RIGHT_RELEASE:
            case EventType::MIDDLE_RELEASE: {
                auto mouse_btn = (event.type() == EventType::RIGHT_RELEASE) ? 
                                MouseButton::RIGHT : MouseButton::MIDDLE;
                auto avm2_event = Avm2EventObject::create_click_event(
                    context, shared_from_this(), mouse_btn);
                
                bool handled = Avm2::dispatch_event(context, avm2_event, target);
                return handled ? ClipEventResult::HANDLED : ClipEventResult::NOT_HANDLED;
            }
            
            case EventType::RELEASE_OUTSIDE: {
                auto avm2_event = Avm2EventObject::create_release_outside_event(
                    context, shared_from_this());
                bool handled = Avm2::dispatch_event(context, avm2_event, target);
                return handled ? ClipEventResult::HANDLED : ClipEventResult::NOT_HANDLED;
            }
            
            case EventType::ROLL_OUT: {
                auto to = event.target_object();
                auto avm2_event = Avm2EventObject::create_mouse_out_event(
                    context, shared_from_this(), to);
                bool handled = Avm2::dispatch_event(context, avm2_event, target);
                
                // Also fire rollOut events on ancestors up to LCA
                auto lca = lowest_common_ancestor(shared_from_this(), to);
                auto current = shared_from_this();
                while (current && current != lca) {
                    auto roll_out_event = Avm2EventObject::create_roll_out_event(
                        context, current, to);
                    if (auto obj = current->object2()) {
                        Avm2::dispatch_event(context, roll_out_event, obj);
                    }
                    current = current->parent();
                }
                
                return handled ? ClipEventResult::HANDLED : ClipEventResult::NOT_HANDLED;
            }
            
            case EventType::ROLL_OVER: {
                auto from = event.source_object();
                auto lca = lowest_common_ancestor(shared_from_this(), from);
                
                // Fire rollOver events on ancestors up to LCA
                auto current = shared_from_this();
                bool handled = false;
                while (current && current != lca) {
                    auto roll_over_event = Avm2EventObject::create_roll_over_event(
                        context, current, from);
                    if (auto obj = current->object2()) {
                        handled = Avm2::dispatch_event(context, roll_over_event, obj) || handled;
                    }
                    current = current->parent();
                }
                
                // Fire mouseOver event on target
                auto mouse_over_event = Avm2EventObject::create_mouse_over_event(
                    context, shared_from_this(), from);
                handled = Avm2::dispatch_event(context, mouse_over_event, target) || handled;
                
                return handled ? ClipEventResult::HANDLED : ClipEventResult::NOT_HANDLED;
            }
            
            case EventType::MOUSE_WHEEL: {
                auto avm2_event = Avm2EventObject::create_mouse_wheel_event(
                    context, shared_from_this(), event.wheel_delta());
                bool handled = Avm2::dispatch_event(context, avm2_event, target);
                return handled ? ClipEventResult::HANDLED : ClipEventResult::NOT_HANDLED;
            }
            
            case EventType::MOUSE_MOVE_INSIDE: {
                auto avm2_event = Avm2EventObject::create_mouse_move_event(
                    context, shared_from_this());
                bool handled = Avm2::dispatch_event(context, avm2_event, target);
                return handled ? ClipEventResult::HANDLED : ClipEventResult::NOT_HANDLED;
            }
            
            default:
                return ClipEventResult::NOT_HANDLED;
        }
    }

    // Handle a clip event
    virtual ClipEventResult handle_clip_event(std::shared_ptr<UpdateContext> context,
                                            ClipEvent event) {
        if (!mouse_enabled()) {
            return ClipEventResult::NOT_HANDLED;
        }

        if (filter_clip_event(context, event) == ClipEventResult::NOT_HANDLED) {
            return ClipEventResult::NOT_HANDLED;
        }

        if (propagate_to_children(context, event) == ClipEventResult::HANDLED) {
            return ClipEventResult::HANDLED;
        }

        return event_dispatch(context, event);
    }

    // Find the lowest common ancestor between two display objects
    static std::shared_ptr<DisplayObject> lowest_common_ancestor(
        std::shared_ptr<DisplayObject> from,
        std::shared_ptr<DisplayObject> to) {
        
        std::vector<std::shared_ptr<DisplayObject>> from_parents;
        auto current = from;
        while (current) {
            from_parents.push_back(current);
            current = current->parent();
        }

        std::vector<std::shared_ptr<DisplayObject>> to_parents;
        current = to;
        while (current) {
            to_parents.push_back(current);
            current = current->parent();
        }

        std::shared_ptr<DisplayObject> lca = nullptr;
        auto from_it = from_parents.rbegin();
        auto to_it = to_parents.rbegin();
        
        while (from_it != from_parents.rend() && to_it != to_parents.rend()) {
            if (from_it->get() == to_it->get()) {  // Compare pointers
                lca = *from_it;
                ++from_it;
                ++to_it;
            } else {
                break;
            }
        }

        return lca;
    }

    // Mouse picking for AVM1
    virtual std::shared_ptr<InteractiveObject> mouse_pick_avm1(
        std::shared_ptr<UpdateContext> context,
        Point<Twips> point,
        bool require_button_mode) const {
        // Default implementation: check if point is within bounds and this object is interactive
        if (visible() && mouse_enabled() && 
            world_bounds(BoundsMode::ENGINE).contains(point)) {
            return std::const_pointer_cast<InteractiveObject>(
                std::static_pointer_cast<const InteractiveObject>(shared_from_this()));
        }
        return nullptr;
    }

    // Mouse picking for AVM2
    virtual std::shared_ptr<InteractiveObject> mouse_pick_avm2(
        std::shared_ptr<UpdateContext> context,
        Point<Twips> point,
        bool require_button_mode) const {
        return mouse_pick_avm1(context, point, require_button_mode);
    }

    // Get the mouse cursor for this object
    virtual MouseCursor mouse_cursor(std::shared_ptr<UpdateContext> context) const {
        return MouseCursor::HAND; // Default cursor
    }

    // Check if this object is focusable for keyboard input
    virtual bool is_focusable(std::shared_ptr<UpdateContext> context) const {
        // By default, all interactive objects are focusable
        return true;
    }

    // Check if this object is focusable by mouse
    virtual bool is_focusable_by_mouse(std::shared_ptr<UpdateContext> context) const {
        if (movie()->is_action_script_3()) {
            return tab_enabled(context);
        } else {
            // AVM1 objects cannot be focused by mouse
            return false;
        }
    }

    // Called when focus changes
    virtual void on_focus_changed(std::shared_ptr<UpdateContext> context,
                                bool focused,
                                std::shared_ptr<InteractiveObject> other) {
        // Default implementation does nothing
    }

    // Drop focus if this object has it
    virtual void drop_focus(std::shared_ptr<UpdateContext> context) {
        if (has_focus()) {
            context->focus_tracker->set_focus(nullptr);
        }
    }

    // Call focus handler (onSetFocus/onKillFocus)
    virtual void call_focus_handler(std::shared_ptr<UpdateContext> context,
                                  bool focused,
                                  std::shared_ptr<InteractiveObject> other) {
        if (auto obj1 = object1()) {
            auto other_val = other ? other->object1() : std::make_shared<Value>(Value::UNDEFINED);
            
            std::string method_name = focused ? "onSetFocus" : "onKillFocus";
            Avm1::run_stack_frame_for_method(shared_from_this(), obj1, 
                                           method_name, {other_val}, context);
        } else if (auto obj2 = object2()) {
            auto activation = Avm2Activation::from_nothing(context);
            std::string event_name = focused ? "focusIn" : "focusOut";
            auto event = Avm2EventObject::create_focus_event(&activation, event_name, false, other, 0);
            Avm2::dispatch_event(context, event, obj2);
        }
    }

    // Check if this object is highlightable when focused
    virtual bool is_highlightable(std::shared_ptr<UpdateContext> context) const {
        return is_highlight_enabled(context);
    }

    // Check if highlight is enabled for this object
    virtual bool is_highlight_enabled(std::shared_ptr<UpdateContext> context) const {
        if (movie()->version() >= 6) {
            auto rect = focus_rect();
            return rect.value_or(context->stage->stage_focus_rect());
        } else {
            return context->stage->stage_focus_rect();
        }
    }

    // Get the bounds of the focus highlight
    virtual Rectangle<Twips> highlight_bounds() const {
        return world_bounds(BoundsMode::ENGINE);
    }

    // Check if this object is tabbable
    virtual bool is_tabbable(std::shared_ptr<UpdateContext> context) const {
        return tab_enabled(context);
    }

    // Check if this is an interactive object
    bool is_interactive() const override { return true; }
    
    // Cast to interactive object
    std::shared_ptr<InteractiveObject> as_interactive() override {
        return std::static_pointer_cast<InteractiveObject>(shared_from_this());
    }
};

} // namespace ruffle

#endif // INTERACTIVE_OBJECT_H