// C++ translation of core/src/display_object/interactive.rs
// Interactive object enumtrait

#ifndef RUFFLE_CORE_DISPLAY_OBJECT_INTERACTIVE_H
#define RUFFLE_CORE_DISPLAY_OBJECT_INTERACTIVE_H

#include <memory>
#include <optional>
#include <variant>
#include <vector>
#include <cstdint>

// Forward declarations
namespace ruffle {
namespace swf {
    struct Point;
    struct Rectangle;
    class Twips;
}
namespace core {
    class AvmString;
    
    namespace avm1 {
        class Avm1;
        template<typename GC>
        class Value;
    }
    namespace avm2 {
        template<typename GC>
        class Activation;
        template<typename GC>
        class Avm2;
        template<typename GC>
        class EventObject;
        template<typename GC>
        class Value;
    }
    namespace backend {
        namespace ui {
            enum class MouseCursor;
        }
    }
    namespace context {
        template<typename GC>
        class UpdateContext;
    }
    namespace events {
        template<typename GC>
        struct ClipEvent;
        enum class ClipEventResult;
        enum class MouseButton;
    }
    namespace display_object {
        class DisplayObject;
        template<typename GC>
        class DisplayObjectBase;
        template<typename GC>
        class Stage;
        template<typename GC>
        class Avm1Button;
        template<typename GC>
        class Avm2Button;
        template<typename GC>
        class MovieClip;
        template<typename GC>
        class EditText;
        template<typename GC>
        class LoaderDisplay;
        template<typename GC>
        class DisplayObjectContainer;
        enum class BoundsMode;
        template<typename GC>
        class TDisplayObject;
        template<typename GC>
        class TDisplayObjectContainer;
    }
}
}

namespace ruffle {
namespace core {
namespace display_object {

/// Find the lowest common ancestor between the display objects in from and to.
template<typename GC>
std::optional<DisplayObject> lowest_common_ancestor(
    DisplayObject from, DisplayObject to);

/// Boolean state flags used by InteractiveObject.
enum class InteractiveObjectFlags : uint8_t {
    MouseEnabled = 1 << 0,
    DoubleClickEnabled = 1 << 1,
    HasFocus = 1 << 2,
};

/// Base class for interactive objects
template<typename GC>
class InteractiveObjectBase {
public:
    DisplayObjectBase<GC> base;
    
private:
    avm2::Value<GC> context_menu_;
    std::optional<int32_t> tab_index_;
    uint8_t flags_;
    std::optional<bool> tab_enabled_;
    std::optional<bool> focus_rect_;

public:
    InteractiveObjectBase() 
        : flags_(static_cast<uint8_t>(InteractiveObjectFlags::MouseEnabled))
        , context_menu_(avm2::Value<GC>::Null)
        , tab_index_(std::nullopt)
        , tab_enabled_(std::nullopt)
        , focus_rect_(std::nullopt) {}

    bool contains_flag(InteractiveObjectFlags flag) const {
        return (flags_ & static_cast<uint8_t>(flag)) != 0;
    }

    void set_flag(InteractiveObjectFlags flag, bool value) {
        if (value) {
            flags_ |= static_cast<uint8_t>(flag);
        } else {
            flags_ &= ~static_cast<uint8_t>(flag);
        }
    }

    // Getters and setters
    const avm2::Value<GC>& context_menu() const { return context_menu_; }
    void set_context_menu(const avm2::Value<GC>& value) { context_menu_ = value; }
    
    std::optional<int32_t> tab_index() const { return tab_index_; }
    void set_tab_index(std::optional<int32_t> value) { tab_index_ = value; }
    
    std::optional<bool> tab_enabled() const { return tab_enabled_; }
    void set_tab_enabled(std::optional<bool> value) { tab_enabled_ = value; }
    
    std::optional<bool> focus_rect() const { return focus_rect_; }
    void set_focus_rect(std::optional<bool> value) { focus_rect_ = value; }
};

/// Interactive object variant type
template<typename GC>
class InteractiveObject {
private:
    std::variant<
        Stage<GC>*,
        Avm1Button<GC>*,
        Avm2Button<GC>*,
        MovieClip<GC>*,
        EditText<GC>*,
        LoaderDisplay<GC>*
    > variant_;

public:
    // Constructors for each variant
    explicit InteractiveObject(Stage<GC>* stage) : variant_(stage) {}
    explicit InteractiveObject(Avm1Button<GC>* button) : variant_(button) {}
    explicit InteractiveObject(Avm2Button<GC>* button) : variant_(button) {}
    explicit InteractiveObject(MovieClip<GC>* clip) : variant_(clip) {}
    explicit InteractiveObject(EditText<GC>* text) : variant_(text) {}
    explicit InteractiveObject(LoaderDisplay<GC>* loader) : variant_(loader) {}

    // Type checking
    bool is_stage() const { return std::holds_alternative<Stage<GC>*>(variant_); }
    bool is_avm1_button() const { return std::holds_alternative<Avm1Button<GC>*>(variant_); }
    bool is_avm2_button() const { return std::holds_alternative<Avm2Button<GC>*>(variant_); }
    bool is_movie_clip() const { return std::holds_alternative<MovieClip<GC>*>(variant_); }
    bool is_edit_text() const { return std::holds_alternative<EditText<GC>*>(variant_); }
    bool is_loader_display() const { return std::holds_alternative<LoaderDisplay<GC>*>(variant_); }

    // Getters
    Stage<GC>* as_stage() const { return std::get_if<Stage<GC>*>(&variant_); }
    Avm1Button<GC>* as_avm1_button() const { return std::get_if<Avm1Button<GC>*>(&variant_); }
    Avm2Button<GC>* as_avm2_button() const { return std::get_if<Avm2Button<GC>*>(&variant_); }
    MovieClip<GC>* as_movie_clip() const { return std::get_if<MovieClip<GC>*>(&variant_); }
    EditText<GC>* as_edit_text() const { return std::get_if<EditText<GC>*>(&variant_); }
    LoaderDisplay<GC>* as_loader_display() const { return std::get_if<LoaderDisplay<GC>*>(&variant_); }
};

/// Trait interface for interactive objects
template<typename GC>
class TInteractiveObject {
public:
    virtual ~TInteractiveObject() = default;

    /// Get the raw interactive object base
    virtual InteractiveObjectBase<GC>* raw_interactive() = 0;

    /// Get as display object
    virtual DisplayObject as_displayobject() = 0;

    /// Check if the interactive object accepts mouse and other user events.
    bool mouse_enabled() const {
        return raw_interactive()->contains_flag(InteractiveObjectFlags::MouseEnabled);
    }

    /// Set if the interactive object accepts user input.
    void set_mouse_enabled(bool value) {
        raw_interactive()->set_flag(InteractiveObjectFlags::MouseEnabled, value);
    }

    /// Check if the interactive object accepts double-click events.
    bool double_click_enabled() const {
        return raw_interactive()->contains_flag(InteractiveObjectFlags::DoubleClickEnabled);
    }

    /// Set if the interactive object accepts double-click events.
    void set_double_click_enabled(bool value) {
        raw_interactive()->set_flag(InteractiveObjectFlags::DoubleClickEnabled, value);
    }

    bool has_focus() const {
        return raw_interactive()->contains_flag(InteractiveObjectFlags::HasFocus);
    }

    void set_has_focus(bool value) {
        raw_interactive()->set_flag(InteractiveObjectFlags::HasFocus, value);
    }

    avm2::Value<GC> context_menu() const {
        return raw_interactive()->context_menu();
    }

    void set_context_menu(avm2::Value<GC> value) {
        raw_interactive()->set_context_menu(value);
    }

    /// Get the boolean flag which determines whether objects display a glowing border when focused.
    std::optional<bool> focus_rect() const {
        return raw_interactive()->focus_rect();
    }

    /// Set the boolean flag which determines whether objects display a glowing border when focused.
    void set_focus_rect(std::optional<bool> value) {
        raw_interactive()->set_focus_rect(value);
    }

    /// Filter the incoming clip event.
    virtual events::ClipEventResult filter_clip_event(
        context::UpdateContext<GC>* context,
        const events::ClipEvent<GC>& event) = 0;

    /// Propagate the event to children.
    virtual events::ClipEventResult propagate_to_children(
        context::UpdateContext<GC>* context,
        const events::ClipEvent<GC>& event) {
        if (event.propagates()) {
            if (auto container = as_displayobject().as_container()) {
                // Mouse events fire in reverse order (high depth to low depth).
                // Button and key events fire in render list order.
                auto children = container->iter_render_list();
                if (event.is_mouse_event()) {
                    // Reverse iteration for mouse events
                    // Implementation would reverse the iterator
                }

                for (auto& child : children) {
                    if (auto interactive = child.as_interactive()) {
                        if (interactive->handle_clip_event(context, event) 
                            == events::ClipEventResult::Handled) {
                            return events::ClipEventResult::Handled;
                        }
                    }
                }
            }
        }
        return events::ClipEventResult::NotHandled;
    }

    /// Dispatch the event to script event handlers.
    virtual events::ClipEventResult event_dispatch(
        context::UpdateContext<GC>* context,
        const events::ClipEvent<GC>& event) = 0;

    /// Convert the clip event into an AVM2 event and dispatch it.
    events::ClipEventResult event_dispatch_to_avm2(
        context::UpdateContext<GC>* context,
        const events::ClipEvent<GC>& event);

    /// Executes and propagates the given clip event.
    events::ClipEventResult handle_clip_event(
        context::UpdateContext<GC>* context,
        const events::ClipEvent<GC>& event) {
        if (!mouse_enabled()) {
            return events::ClipEventResult::NotHandled;
        }

        if (filter_clip_event(context, event) == events::ClipEventResult::NotHandled) {
            return events::ClipEventResult::NotHandled;
        }

        if (propagate_to_children(context, event) == events::ClipEventResult::Handled) {
            return events::ClipEventResult::Handled;
        }

        return event_dispatch(context, event);
    }

    /// Determine the bottom-most interactive display object under the given mouse cursor.
    virtual std::optional<InteractiveObject<GC>> mouse_pick_avm1(
        context::UpdateContext<GC>* context,
        const swf::Point& point,
        bool require_button_mode) {
        return std::nullopt;
    }

    /// AVM2 mouse pick result
    enum class Avm2MousePick {
        Miss,
        Hit(InteractiveObject<GC>)
    };

    virtual Avm2MousePick mouse_pick_avm2(
        context::UpdateContext<GC>* context,
        const swf::Point& point,
        bool require_button_mode) {
        return Avm2MousePick::Miss;
    }

    /// The cursor to use when this object is the hovered element under a mouse.
    virtual backend::ui::MouseCursor mouse_cursor(context::UpdateContext<GC>* context) {
        return backend::ui::MouseCursor::Hand;
    }

    /// Whether this object is focusable for keyboard input.
    virtual bool is_focusable(context::UpdateContext<GC>* context) {
        return true;
    }

    /// Whether this object is focusable using a pointer device.
    virtual bool is_focusable_by_mouse(context::UpdateContext<GC>* context) {
        auto movie = as_displayobject().movie();
        return movie && movie->is_action_script_3() && tab_enabled(context);
    }

    /// Called whenever the focus tracker has deemed this display object worthy of focus.
    virtual void on_focus_changed(
        context::UpdateContext<GC>* context,
        bool focused,
        std::optional<InteractiveObject<GC>> other) {}

    /// If this object has focus, this method drops it.
    void drop_focus(context::UpdateContext<GC>* context) {
        if (has_focus()) {
            auto tracker = context->focus_tracker;
            tracker.set(std::nullopt, context);
        }
    }

    void call_focus_handler(
        context::UpdateContext<GC>* context,
        bool focused,
        std::optional<InteractiveObject<GC>> other);

    /// Whether this object may be highlighted when focused.
    virtual bool is_highlightable(context::UpdateContext<GC>* context) {
        return is_highlight_enabled(context);
    }

    /// Whether highlight is enabled for this object.
    bool is_highlight_enabled(context::UpdateContext<GC>* context);

    /// Get the bounds of the focus highlight.
    swf::Rectangle highlight_bounds() {
        return as_displayobject().world_bounds(BoundsMode::Engine);
    }

    /// Whether this object is included in tab ordering.
    bool is_tabbable(context::UpdateContext<GC>* context) {
        return tab_enabled(context);
    }

    /// Sets whether tab ordering is enabled for this object.
    virtual void set_tab_enabled_value(context::UpdateContext<GC>* context, bool enabled);

    /// Get the tab index for this object.
    std::optional<int32_t> tab_index() const {
        return raw_interactive()->tab_index();
    }

    /// Set the tab index for this object.
    void set_tab_index_value(std::optional<int32_t> value) {
        raw_interactive()->set_tab_index(value);
    }

    /// Get the tab enabled state.
    std::optional<bool> tab_enabled(context::UpdateContext<GC>* context);
};

} // namespace display_object
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_DISPLAY_OBJECT_INTERACTIVE_H
