// C++ translation of core/src/display_object/avm1_button.rs
//! AVM1 Button display object

#ifndef RUFFLE_CORE_DISPLAY_OBJECT_AVM1_BUTTON_H
#define RUFFLE_CORE_DISPLAY_OBJECT_AVM1_BUTTON_H

#include <memory>
#include <vector>
#include <map>
#include <optional>
#include <functional>
#include <cell>
#include <mutex>

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
    class ChildContainer;
    template<typename GC>
    class Object;
    template<typename GC>
    class Value;
    template<typename GC>
    class AvmString;
    template<typename GC>
    class StageObject;

    class SwfMovie;
    class SwfSlice;
    class Rectangle;
    class Point;
    class Matrix;
    class ColorTransform;

    namespace avm1 {
        template<typename GC>
        class Activation;
        class ActivationIdentifier;
        enum class NativeObject;
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
        template<typename T>
        class RefLock;
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
    class ButtonAction;
    class ButtonSounds;
    class ColorTransform;
    class ButtonState;
    class ButtonActionCondition;
    enum class ButtonState;
}
}

namespace ruffle {
namespace core {
namespace display_object {

/// Button state enumeration
enum class ButtonState {
    Up,
    Over,
    Down
};

/// Button tracking mode
enum class ButtonTracking {
    Push,
    Menu
};

/// Button action with conditions
struct ButtonAction {
    SwfSlice action_data;
    swf::ButtonActionCondition conditions;
};

/// Shared button data between instances
template<typename GC>
struct ButtonShared {
    std::shared_ptr<SwfMovie> swf;
    uint16_t id;
    std::vector<ButtonAction> actions;
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

/// Mutable button data
template<typename GC>
struct Avm1ButtonDataMut {
    std::map<Depth, DisplayObject<GC>> hit_area;
    Rectangle<Twips> hit_bounds;
    ChildContainer<GC> container;
    std::vector<Avm1TextFieldBinding<GC>> text_field_bindings;
};

/// AVM1 Button display object
///
/// Represents a button created from SWF DefineButton tag.
/// Buttons have four states: Up, Over, Down, and Hit Test.
/// Each state can have different child display objects.
template<typename GC>
class Avm1Button {
public:
    /// Create a button from SWF tag
    ///
    /// \param button SWF button data
    /// \param source_movie Source movie slice
    /// \param mc GC mutation context
    static Avm1Button from_swf_tag(
        const swf::Button& button,
        const SwfSlice& source_movie,
        Mutation<GC>* mc
    );

    /// Set button sounds
    ///
    /// \param sounds Button sounds from SWF
    void set_sounds(const swf::ButtonSounds& sounds);

    /// Set color transforms for button states (legacy DefineButtonCxform)
    ///
    /// \param color_transforms Color transforms for each state
    void set_colors(const std::vector<swf::ColorTransform>& color_transforms);

    /// Set the button state, creating/destroying children as needed
    ///
    /// \param context Update context
    /// \param state New button state
    void set_state(UpdateContext<GC>* context, ButtonState state);

    /// Get current button state
    ///
    /// \return Current state or nullopt
    std::optional<ButtonState> state() const;

    /// Get enabled property from AVM1 object
    ///
    /// \param context Update context
    /// \return Whether button is enabled
    bool enabled(UpdateContext<GC>* context);

    /// Get useHandCursor property from AVM1 object
    ///
    /// \param context Update context
    /// \return Whether to use hand cursor
    bool use_hand_cursor(UpdateContext<GC>* context);

    // TDisplayObject implementation
    Gc<GC, DisplayObjectBase<GC>> base() const;
    Avm1Button instantiate(Mutation<GC>* mc) const;
    uint16_t id() const;
    std::shared_ptr<SwfMovie> movie() const;

    void post_instantiation(
        UpdateContext<GC>* context,
        std::optional<Object<GC>> init_object,
        Instantiator instantiated_by,
        bool run_frame
    );

    void render_self(RenderContext<GC>* context);
    Rectangle<Twips> self_bounds(BoundsMode mode) const;

    bool hit_test_shape(
        UpdateContext<GC>* context,
        Point<Twips> point,
        HitTestOptions options
    );

    std::optional<Object<GC>> object1() const;
    std::optional<StageObject<GC>> object2() const;

    bool allow_as_mask() const;
    void avm1_unload(UpdateContext<GC>* context);

    std::optional<std::reference_wrapper<const std::vector<Avm1TextFieldBinding<GC>>>>
    avm1_text_field_bindings() const;

    std::optional<std::reference_wrapper<std::vector<Avm1TextFieldBinding<GC>>>>
    avm1_text_field_bindings_mut(Mutation<GC>* mc);

    // TDisplayObjectContainer implementation
    std::reference_wrapper<const ChildContainer<GC>> raw_container() const;
    std::reference_wrapper<ChildContainer<GC>> raw_container_mut(Mutation<GC>* mc);

    // TInteractiveObject implementation
    Gc<GC, InteractiveObjectBase<GC>> raw_interactive() const;
    DisplayObject<GC> as_displayobject() const;

    ClipEventResult filter_clip_event(
        UpdateContext<GC>* context,
        ClipEvent event
    );

    ClipEventResult event_dispatch(
        UpdateContext<GC>* context,
        ClipEvent event
    );

    std::optional<InteractiveObject<GC>> mouse_pick_avm1(
        UpdateContext<GC>* context,
        Point<Twips> point,
        bool require_button_mode
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
    Avm1Button(Gc<GC, Avm1ButtonData<GC>> data);

    bool get_boolean_property(
        AvmString<GC> name,
        bool default_value,
        UpdateContext<GC>* context
    );

    void run_actions(UpdateContext<GC>* context, swf::ButtonActionCondition condition);

    Gc<GC, Avm1ButtonData<GC>> data_;
};

/// AVM1 Button data
template<typename GC>
struct Avm1ButtonData {
    InteractiveObjectBase<GC> base;
    gc_arena::RefLock<Avm1ButtonDataMut<GC>> cell;
    Gc<GC, ButtonShared<GC>> shared;
    gc_arena::Lock<std::optional<Object<GC>>> object;
    std::cell::Cell<ButtonState> state;
    std::cell::Cell<ButtonTracking> tracking;
    std::cell::Cell<bool> initialized;
};

// Conversion utilities
ButtonState from_swf_button_state(swf::ButtonState state);
swf::ButtonState to_swf_button_state(ButtonState state);

} // namespace display_object
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_DISPLAY_OBJECT_AVM1_BUTTON_H
