// C++ translation of core/src/focus_tracker.rs
//! Focus tracker for interactive objects

#ifndef RUFFLE_CORE_FOCUS_TRACKER_H
#define RUFFLE_CORE_FOCUS_TRACKER_H

#include <vector>
#include <optional>
#include <memory>
#include <cstdint>
#include <variant>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC>
    class Player;
    template<typename GC>
    class Avm1;
    template<typename GC>
    class Value;
    template<typename GC>
    class Avm2;
    template<typename GC>
    class Activation;
    template<typename GC>
    class EventObject;

    namespace display_object {
        template<typename GC>
        class DisplayObject;
        template<typename GC>
        class TDisplayObject;
        template<typename GC>
        class TDisplayObjectContainer;
        template<typename GC>
        class TextSelection;
        template<typename GC>
        class EditText;
        template<typename GC>
        class InteractiveObject;
        template<typename GC>
        class TInteractiveObject;
        template<typename GC>
        class BoundsMode;
    }

    namespace events {
        enum class ClipEvent;
        enum class KeyCode;
    }

    namespace gc_arena {
        template<typename T>
        class Gc;
        template<typename T>
        class Lock;
        class Mutation;
    }

    struct UpdateContext;
    struct RenderContext;

    namespace swf {
        struct Color;
        struct Rectangle;
        struct Twips;
    }
}
}

namespace either {
    template<typename T>
    class Either;
}

namespace ruffle {
namespace core {

using gc_arena::Gc;
using gc_arena::Lock;
using gc_arena::Mutation;
using display_object::InteractiveObject;
using display_object::EditText;
using events::ClipEvent;
using events::KeyCode;
using swf::Color;
using swf::Rectangle;
using swf::Twips;

/// Highlight state for focus
enum class Highlight {
    /// The focus is highlighted and the highlight is visible on the screen.
    ///
    /// This is the required state for keyboard navigation to work.
    ActiveVisible,

    /// The focus is highlighted, but the highlight is not visible on the screen.
    ///
    /// Some keyboard events (KeyUp, KeyDown) require this logic.
    ActiveHidden,

    /// The focus is not highlighted.
    Inactive
};

impl Highlight {
    bool is_active() const {
        return *this == Highlight::ActiveVisible || *this == Highlight::ActiveHidden;
    }

    bool is_visible() const {
        return *this == Highlight::ActiveVisible;
    }
};

/// Internal data for FocusTracker
template<typename GC>
struct FocusTrackerData {
    Lock<std::optional<InteractiveObject<GC>>> focus;
    Highlight highlight = Highlight::Inactive;
};

/// Navigation direction for keyboard navigation
enum class NavigationDirection {
    Up,
    Right,
    Down,
    Left
};

impl NavigationDirection {
    static std::optional<NavigationDirection> from_key_code(KeyCode key_code);
    KeyCode key() const;
};

/// A list of interactive objects ordered according to a specific tab order.
template<typename GC>
class TabOrder {
private:
    std::vector<InteractiveObject<GC>> objects_;
    bool is_custom_ = false;

public:
    static TabOrder empty() {
        return TabOrder {};
    }

    static TabOrder fill(UpdateContext<GC>* context);

    bool is_custom() const { return is_custom_; }

    auto iter() const { return objects_.begin(); }
    auto end() const { return objects_.end(); }

    void add_object(InteractiveObject<GC> object);

private:
    void sort();
    void sort_with(const auto& ordering);

    std::optional<InteractiveObject<GC>> first(const auto& ordering) const;
};

/// Trait for tab ordering strategies
template<typename GC>
class TabOrdering {
public:
    virtual std::optional<auto> key(InteractiveObject<GC> object) const = 0;
    virtual bool ignore_duplicates() const = 0;
};

/// Custom tab ordering based on tab indices
class CustomTabOrdering {
public:
    std::optional<int32_t> key(InteractiveObject object) const {
        return object.tab_index();
    }

    bool ignore_duplicates() const { return false; }
};

/// Automatic tab ordering based on position
class AutomaticTabOrdering {
public:
    std::optional<int64_t> key(InteractiveObject object) const {
        auto bounds = object.highlight_bounds();
        int64_t x = bounds.x_min().get();
        int64_t y = bounds.y_min().get();
        return y * 6 + x;
    }

    bool ignore_duplicates() const { return true; }
};

/// Ordering used for keyboard navigation
template<typename GC>
class NavigationOrdering {
private:
    Rectangle<Twips> origin_bounds_;
    NavigationDirection direction_;

public:
    NavigationOrdering(InteractiveObject<GC> origin, NavigationDirection direction);

    std::optional<std::pair<int, int64_t>> key(InteractiveObject<GC> other) const;
    bool ignore_duplicates() const { return false; }
};

/// Focus tracker for managing focus state
template<typename GC>
class FocusTracker {
private:
    static constexpr Twips HIGHLIGHT_THICKNESS = Twips::from_pixels_i32(3);
    static constexpr Color HIGHLIGHT_COLOR = Color::YELLOW;

    Gc<GC, FocusTrackerData<GC>> data_;

public:
    explicit FocusTracker(Mutation<GC>* mc)
        : data_(Gc<GC, FocusTrackerData<GC>>::new(mc, FocusTrackerData<GC> {
            .focus = Lock::new(std::nullopt),
            .highlight = Highlight::Inactive
        }))
    {}

    Highlight highlight() const {
        return data_->highlight;
    }

    void reset_highlight() {
        data_->highlight = Highlight::Inactive;
    }

    std::optional<InteractiveObject<GC>> get() const {
        return data_->focus.get();
    }

    std::optional<EditText<GC>> get_as_edit_text() const {
        return get().and_then([](auto obj) {
            return obj.as_displayobject().and_then([](auto display) {
                return display.as_edit_text();
            });
        });
    }

    /// Set the focus programmatically.
    void set(std::optional<InteractiveObject<GC>> new_focus, UpdateContext<GC>* context);

    /// Reset the focus programmatically.
    void reset_focus(UpdateContext<GC>* context);

    /// Set the focus and acknowledge that this change was caused by a pointer device.
    void set_by_mouse(
        std::optional<InteractiveObject<GC>> new_focus,
        UpdateContext<GC>* context
    );

    /// Set the focus and acknowledge that this change was caused by a key.
    void set_by_key(
        std::optional<InteractiveObject<GC>> new_focus,
        KeyCode key_code,
        UpdateContext<GC>* context
    );

    TabOrder<GC> tab_order(UpdateContext<GC>* context);

    void cycle(UpdateContext<GC>* context, bool reverse);

    void navigate(UpdateContext<GC>* context, NavigationDirection direction);

    void update_highlight(UpdateContext<GC>* context);

    void render_highlight(RenderContext<GC>* context);

private:
    void set_internal(
        std::optional<InteractiveObject<GC>> new_focus,
        UpdateContext<GC>* context,
        bool run_actions
    );

    void update_virtual_keyboard(UpdateContext<GC>* context);

    void update_edittext_selection();

    static bool dispatch_focus_change_event(
        UpdateContext<GC>* context,
        const char* event_type,
        std::optional<InteractiveObject<GC>> target,
        std::optional<InteractiveObject<GC>> related_object,
        std::optional<KeyCode> key_code
    );

    static void roll_over(UpdateContext<GC>* context, std::optional<InteractiveObject<GC>> new_focus);

    Highlight calculate_highlight(UpdateContext<GC>* context);
};

} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_FOCUS_TRACKER_H
