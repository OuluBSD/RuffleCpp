// C++ translation of core/src/context_menu.rs
// Context menu state and items

#ifndef RUFFLE_CORE_CONTEXT_MENU_H
#define RUFFLE_CORE_CONTEXT_MENU_H

#include <vector>
#include <string>
#include <optional>
#include <memory>
#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
    template<typename T> class Object;
}
namespace avm2 {
    template<typename T> class Object;
}
namespace display_object {
    template<typename T> class DisplayObject;
    template<typename T> class InteractiveObject;
    template<typename T> class EditText;
    template<typename T> class Stage;
}
namespace events {
    enum class TextControlCode;
}
namespace context {
    template<typename T> class UpdateContext;
}
}
}

namespace ruffle {
namespace core {

/// A single context menu item
struct ContextMenuItem {
    bool enabled;
    bool separator_before;
    bool checked;
    std::string caption;
};

/// Built-in context menu item flags
struct BuiltInItemFlags {
    bool forward_and_back;
    bool loop_;
    bool play;
    bool print;
    bool quality;
    bool rewind;
    bool save;
    bool zoom;

    /// Create BuiltInItemFlags for a stage
    static BuiltInItemFlags for_stage(display_object::Stage<GC>* stage);
};

/// Context menu callback types
template<typename GC>
class ContextMenuCallback {
public:
    enum class Type {
        Zoom,
        QualityLow,
        QualityMedium,
        QualityHigh,
        Play,
        Loop,
        Rewind,
        Forward,
        Back,
        Print,
        Avm1,
        Avm2,
        TextControl
    };

    ContextMenuCallback() : type_(Type::Zoom) {}

    static ContextMenuCallback zoom() {
        ContextMenuCallback cb;
        cb.type_ = Type::Zoom;
        return cb;
    }

    static ContextMenuCallback quality_low() {
        ContextMenuCallback cb;
        cb.type_ = Type::QualityLow;
        return cb;
    }

    static ContextMenuCallback quality_medium() {
        ContextMenuCallback cb;
        cb.type_ = Type::QualityMedium;
        return cb;
    }

    static ContextMenuCallback quality_high() {
        ContextMenuCallback cb;
        cb.type_ = Type::QualityHigh;
        return cb;
    }

    static ContextMenuCallback play() {
        ContextMenuCallback cb;
        cb.type_ = Type::Play;
        return cb;
    }

    static ContextMenuCallback loop_() {
        ContextMenuCallback cb;
        cb.type_ = Type::Loop;
        return cb;
    }

    static ContextMenuCallback rewind() {
        ContextMenuCallback cb;
        cb.type_ = Type::Rewind;
        return cb;
    }

    static ContextMenuCallback forward() {
        ContextMenuCallback cb;
        cb.type_ = Type::Forward;
        return cb;
    }

    static ContextMenuCallback back() {
        ContextMenuCallback cb;
        cb.type_ = Type::Back;
        return cb;
    }

    static ContextMenuCallback print() {
        ContextMenuCallback cb;
        cb.type_ = Type::Print;
        return cb;
    }

    static ContextMenuCallback avm1(
        avm1::Object<GC> item,
        avm1::Object<GC> callback
    ) {
        ContextMenuCallback cb;
        cb.type_ = Type::Avm1;
        cb.avm1_item_ = item;
        cb.avm1_callback_ = callback;
        return cb;
    }

    static ContextMenuCallback avm2(avm2::Object<GC> item) {
        ContextMenuCallback cb;
        cb.type_ = Type::Avm2;
        cb.avm2_item_ = item;
        return cb;
    }

    static ContextMenuCallback text_control(
        events::TextControlCode code,
        display_object::EditText<GC> text
    ) {
        ContextMenuCallback cb;
        cb.type_ = Type::TextControl;
        cb.text_control_code_ = code;
        cb.text_ = text;
        return cb;
    }

    Type type() const { return type_; }

private:
    Type type_;

    // AVM1 callback data
    avm1::Object<GC> avm1_item_;
    avm1::Object<GC> avm1_callback_;

    // AVM2 callback data
    avm2::Object<GC> avm2_item_;

    // Text control data
    events::TextControlCode text_control_code_;
    display_object::EditText<GC> text_;
};

/// State for an open context menu
template<typename GC>
class ContextMenuState {
public:
    ContextMenuState() = default;

    static ContextMenuState create() {
        return ContextMenuState();
    }

    /// Add an item to the context menu
    void push(const ContextMenuItem& item, const ContextMenuCallback<GC>& callback) {
        info_.push_back(item);
        callbacks_.push_back(callback);
    }

    /// Get the list of menu items
    const std::vector<ContextMenuItem>& info() const {
        return info_;
    }

    /// Get a callback by index
    const ContextMenuCallback<GC>& callback(size_t index) const {
        return callbacks_[index];
    }

    /// Get the display object associated with this menu
    std::optional<display_object::DisplayObject<GC>> get_display_object() const {
        return object_;
    }

    /// Set the display object associated with this menu
    void set_display_object(std::optional<display_object::DisplayObject<GC>> object) {
        object_ = object;
    }

    /// Build built-in menu items
    void build_builtin_items(
        BuiltInItemFlags item_flags,
        context::UpdateContext<GC>* context
    );

private:
    std::vector<ContextMenuItem> info_;
    std::vector<ContextMenuCallback<GC>> callbacks_;
    std::optional<display_object::DisplayObject<GC>> object_;

    /// Build text field menu items
    void build_text_items(
        display_object::EditText<GC> text,
        context::UpdateContext<GC>* context
    );
};

} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_CONTEXT_MENU_H
