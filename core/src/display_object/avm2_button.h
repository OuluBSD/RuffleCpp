// C++ translation of core/src/display_object/avm2_button.rs
//! AVM2 Button (SimpleButton) display object

#ifndef RUFFLE_CORE_DISPLAY_OBJECT_AVM2_BUTTON_H
#define RUFFLE_CORE_DISPLAY_OBJECT_AVM2_BUTTON_H

#include <memory>
#include <vector>
#include <optional>
#include <functional>
#include <cell>
#include <variant>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC>
    class Gc;
    template<typename GC>
    class Mutation;
    template<typename GC>
    class UpdateContext;
    template<typename GC>
    class DisplayObjectBase;
    template<typename GC>
    class InteractiveObjectBase;
    template<typename GC>
    class DisplayObject;
    template<typename GC>
    class MovieClip;
    template<typename GC>
    class StageObject;
    template<typename GC>
    class ClassObject;
    template<typename GC>
    class Value;

    class SwfMovie;
    class SwfSlice;
    class Rectangle;
    class Point;
    class Matrix;

    namespace avm1 {
        template<typename GC>
        class Object;
    }

    namespace avm2 {
        template<typename GC>
        class Activation;
        template<typename GC>
        class StageObject;
        template<typename GC>
        class ClassObject;
        template<typename T>
        class FunctionArgs;
    }

    namespace backend {
        namespace ui {
            class MouseCursor;
        }
    }

    namespace events {
        enum class ClipEvent;
        enum class ClipEventResult;
    }

    namespace gc_arena {
        template<typename T>
        class Lock;
    }

    namespace render {
        namespace filters {
            class Filter;
        }
    }
}
namespace swf {
    class Button;
    class ButtonRecord;
    class ButtonSounds;
    class ColorTransform;
    enum class ButtonState;
}
namespace either {
    template<typename A, typename B>
    class Either;
}
}

namespace ruffle {
namespace core {
namespace display_object {

// Forward declare types from avm1_button
enum class ButtonState;
enum class ButtonTracking;

/// Data shared between all instances of a button
template<typename GC>
struct ButtonShared {
    std::shared_ptr<SwfMovie> swf;
    uint16_t id;
    std::cell::RefCell<ButtonSharedMut> cell;
};

/// Mutable shared button data
struct ButtonSharedMut {
    std::vector<swf::ButtonRecord> records;
    std::optional<swf::ButtonSound> up_to_over_sound;
    std::optional<swf::ButtonSound> over_to_down_sound;
    std::optional<swf::ButtonSound> down_to_over_sound;
    std::optional<swf::ButtonSound> over_to_up_sound;
};

/// AVM2 Button (SimpleButton) display object
///
/// Represents a button created from SWF DefineButton tag for AVM2.
/// Unlike AVM1 buttons, AVM2 buttons construct all states once and store them.
template<typename GC>
class Avm2Button {
public:
    /// Create a button from SWF tag
    ///
    /// \param button SWF button data
    /// \param source_movie Source movie slice
    /// \param context Update context
    /// \param construct_blank_states Whether to construct blank states
    static Avm2Button from_swf_tag(
        const swf::Button& button,
        const SwfSlice& source_movie,
        UpdateContext<GC>* context,
        bool construct_blank_states
    );

    /// Create an empty button
    ///
    /// \param context Update context
    static Avm2Button empty_button(UpdateContext<GC>* context);

    /// Set button sounds
    ///
    /// \param sounds Button sounds from SWF
    void set_sounds(const swf::ButtonSounds& sounds);

    /// Set color transforms for button states (legacy DefineButtonCxform)
    ///
    /// \param color_transforms Color transforms for each state
    void set_colors(const std::vector<swf::ColorTransform>& color_transforms);

    /// Get current button state
    ///
    /// \return Current state
    ButtonState state() const;

    /// Set the rendered button state
    ///
    /// \param context Update context
    /// \param state New state
    void set_state(UpdateContext<GC>* context, ButtonState state);

    /// Get enabled property
    ///
    /// \return Whether button is enabled
    bool enabled() const;

    /// Set enabled property
    ///
    /// \param context Update context
    /// \param enabled New enabled state
    void set_enabled(UpdateContext<GC>* context, bool enabled);

    /// Get useHandCursor property
    ///
    /// \return Whether to use hand cursor
    bool use_hand_cursor() const;

    /// Set useHandCursor property
    ///
    /// \param use_hand_cursor New value
    void set_use_hand_cursor(bool use_hand_cursor);

    /// Get button tracking mode
    ///
    /// \return Tracking mode
    ButtonTracking button_tracking() const;

    /// Set button tracking mode
    ///
    /// \param tracking New tracking mode
    void set_button_tracking(ButtonTracking tracking);

    /// Set AVM2 class for this button
    ///
    /// \param mc GC mutation context
    /// \param class New class
    void set_avm2_class(Mutation<GC>* mc, ClassObject<GC> class_);

    // TDisplayObject implementation
    Gc<GC, DisplayObjectBase<GC>> base() const;
    Avm2Button instantiate(Mutation<GC>* mc) const;
    uint16_t id() const;
    std::shared_ptr<SwfMovie> movie() const;

    void post_instantiation(
        UpdateContext<GC>* context,
        std::optional<avm1::Object<GC>> init_object,
        Instantiator instantiated_by,
        bool run_frame
    );

    void enter_frame(UpdateContext<GC>* context);
    void construct_frame(UpdateContext<GC>* context);
    void run_frame_scripts(UpdateContext<GC>* context);
    void render_self(RenderContext<GC>* context);

    Rectangle<Twips> self_bounds(BoundsMode mode) const;
    Rectangle<Twips> bounds_with_transform(const Matrix& matrix, BoundsMode mode) const;
    Rectangle<Twips> render_bounds_with_transform(
        const Matrix& matrix,
        bool include_own_filters,
        const Matrix& view_matrix
    );

    bool hit_test_shape(
        UpdateContext<GC>* context,
        Point<Twips> point,
        HitTestOptions options
    );

    std::optional<avm1::Object<GC>> object1() const;
    std::optional<avm2::StageObject<GC>> object2() const;
    void set_object2(UpdateContext<GC>* context, avm2::StageObject<GC> object);

    bool allow_as_mask() const;

    // TInteractiveObject implementation
    Gc<GC, InteractiveObjectBase<GC>> raw_interactive() const;
    DisplayObject<GC> as_displayobject() const;

    ClipEventResult filter_clip_event(
        UpdateContext<GC>* context,
        ClipEvent event
    );

    ClipEventResult propagate_to_children(
        UpdateContext<GC>* context,
        ClipEvent event
    );

    ClipEventResult event_dispatch(
        UpdateContext<GC>* context,
        ClipEvent event
    );

    Avm2MousePick<GC> mouse_pick_avm2(
        UpdateContext<GC>* context,
        Point<Twips> point,
        bool require_button_mode
    );

    backend::ui::MouseCursor mouse_cursor(UpdateContext<GC>* context);
    bool tab_enabled_default(UpdateContext<GC>* context) const;
    Rectangle<Twips> highlight_bounds() const;

private:
    Avm2Button(Gc<GC, Avm2ButtonData<GC>> data);

    /// Construct a given state of the button
    ///
    /// \param context Update context
    /// \param swf_state SWF button state
    /// \return Pair of (display object, whether to signal events on children)
    std::pair<DisplayObject<GC>, bool> create_state(
        UpdateContext<GC>* context,
        swf::ButtonState swf_state
    );

    /// Get the display object for a particular button state
    ///
    /// \param state Button state
    /// \return State display object or nullopt
    std::optional<DisplayObject<GC>> get_state_child(swf::ButtonState state) const;

    /// Set the display object for a particular button state
    ///
    /// \param context Update context
    /// \param state Button state
    /// \param child New state display object
    void set_state_child(
        UpdateContext<GC>* context,
        swf::ButtonState state,
        std::optional<DisplayObject<GC>> child
    );

    /// Get all state children iterator
    auto all_state_children(bool weird_order) const;

    Gc<GC, Avm2ButtonData<GC>> data_;
};

/// AVM2 Button data
template<typename GC>
struct Avm2ButtonData {
    InteractiveObjectBase<GC> base;
    Gc<GC, ButtonShared<GC>> shared;

    /// Display object for UP state
    gc_arena::Lock<std::optional<DisplayObject<GC>>> up_state;

    /// Display object for OVER state
    gc_arena::Lock<std::optional<DisplayObject<GC>>> over_state;

    /// Display object for DOWN state
    gc_arena::Lock<std::optional<DisplayObject<GC>>> down_state;

    /// Display object for hit testing
    gc_arena::Lock<std::optional<DisplayObject<GC>>> hit_area;

    /// AVM2 class for this button
    gc_arena::Lock<ClassObject<GC>> class_;

    /// AVM2 object representation
    gc_arena::Lock<std::optional<avm2::StageObject<GC>>> object;

    /// Current button state
    std::cell::Cell<ButtonState> state;

    /// Button tracking mode
    std::cell::Cell<ButtonTracking> tracking;

    /// Enabled state
    std::cell::Cell<bool> enabled;

    /// Use hand cursor setting
    std::cell::Cell<bool> use_hand_cursor;

    /// Whether button needs frame construction
    std::cell::Cell<bool> needs_frame_construction;

    /// Skip next run_frame call
    std::cell::Cell<bool> skip_current_frame;

    /// Use weird framescript order for initial construction
    std::cell::Cell<bool> weird_framescript_order;
};

} // namespace display_object
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_DISPLAY_OBJECT_AVM2_BUTTON_H
