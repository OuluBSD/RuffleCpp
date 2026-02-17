// C++ translation of core/src/events.rs
// Event types for Ruffle

#ifndef RUFFLE_CORE_EVENTS_H
#define RUFFLE_CORE_EVENTS_H

#include <string>
#include <optional>
#include <variant>
#include <cstdint>
#include <memory>

// Forward declarations
namespace ruffle {
namespace core {

template<typename GC>
class AvmString;
template<typename GC>
class StringContext;

namespace display_object {
    template<typename GC>
    class InteractiveObject;
}

/// Mouse button identifiers
enum class MouseButton {
    Left,
    Right,
    Middle,
    XButton1,
    XButton2
};

/// Gamepad button identifiers
enum class GamepadButton {
    FaceBottom,    // A/Cross
    FaceRight,     // B/Circle
    FaceLeft,      // X/Square
    FaceTop,       // Y/Triangle
    LeftBumper,
    RightBumper,
    LeftTrigger,
    RightTrigger,
    Select,        // Back/Share
    Start,         // Forward/Options
    LeftStick,
    RightStick,
    DPadUp,
    DPadDown,
    DPadLeft,
    DPadRight,
    Guide,         // Home/PS
    Misc1          // Misc/Touchpad
};

/// The distance scrolled by the mouse wheel
struct MouseWheelDelta {
    enum class Type {
        Lines,
        Pixels
    };
    
    Type type;
    double value;
    
    static constexpr double MOUSE_WHEEL_SCALE = 100.0;
    
    MouseWheelDelta() : type(Type::Lines), value(0.0) {}
    MouseWheelDelta(Type t, double v) : type(t), value(v) {}
    
    /// Returns the number of lines that this delta represents
    double lines() const {
        switch (type) {
            case Type::Lines:
                return value;
            case Type::Pixels:
                return value / MOUSE_WHEEL_SCALE;
        }
    }
    
    bool operator==(const MouseWheelDelta& other) const {
        if (std::isnan(value) && std::isnan(other.value)) {
            return true;
        }
        if (type == other.type) {
            return value == other.value;
        }
        if (type == Type::Pixels && other.type == Type::Lines) {
            return value == other.value * MOUSE_WHEEL_SCALE;
        }
        if (type == Type::Lines && other.type == Type::Pixels) {
            return value * MOUSE_WHEEL_SCALE == other.value;
        }
        return false;
    }
};

/// Key descriptor for keyboard events
struct KeyDescriptor {
    std::string key_name;
    std::string key_code;
    bool shift_key;
    bool ctrl_key;
    bool alt_key;
    
    KeyDescriptor()
        : shift_key(false), ctrl_key(false), alt_key(false) {}
    
    KeyDescriptor(std::string name, std::string code)
        : key_name(std::move(name))
        , key_code(std::move(code))
        , shift_key(false)
        , ctrl_key(false)
        , alt_key(false) {}
};

/// Button key codes
enum class ButtonKeyCode : uint8_t {
    Unknown = 0,
    Left = 1,
    Right = 2,
    Up = 3,
    Down = 4,
    Space = 5,
    Return = 6,
    // Add more as needed
};

/// IME (Input Method Editor) events
enum class ImeEvent {
    StartComposition,
    EndComposition,
    CompositionUpdate,
    CandidateWindowUpdate
};

/// Player events - events from the host system
struct PlayerEvent {
    enum class Type {
        KeyDown,
        KeyUp,
        MouseMove,
        MouseUp,
        MouseDown,
        MouseLeave,
        MouseWheel,
        GamepadButtonDown,
        GamepadButtonUp,
        TextInput,
        TextControl,
        ImeStartComposition,
        ImeEndComposition,
        ImeCompositionUpdate,
        ImeCandidateWindowUpdate,
        FocusGained,
        FocusLost
    };
    
    Type type;
    
    // Key events
    KeyDescriptor key;
    
    // Mouse events
    double mouse_x;
    double mouse_y;
    MouseButton mouse_button;
    std::optional<size_t> mouse_index;
    
    // Mouse wheel
    MouseWheelDelta wheel_delta;
    
    // Gamepad
    GamepadButton gamepad_button;
    
    // Text input
    char text_codepoint;
    
    // Text control
    // TextControlCode text_control_code;
    
    // IME
    ImeEvent ime_event;
    
    // Constructors for different event types
    static PlayerEvent key_down(KeyDescriptor k) {
        PlayerEvent e;
        e.type = Type::KeyDown;
        e.key = std::move(k);
        return e;
    }
    
    static PlayerEvent key_up(KeyDescriptor k) {
        PlayerEvent e;
        e.type = Type::KeyUp;
        e.key = std::move(k);
        return e;
    }
    
    static PlayerEvent mouse_move(double x, double y) {
        PlayerEvent e;
        e.type = Type::MouseMove;
        e.mouse_x = x;
        e.mouse_y = y;
        return e;
    }
    
    static PlayerEvent mouse_up(double x, double y, MouseButton button) {
        PlayerEvent e;
        e.type = Type::MouseUp;
        e.mouse_x = x;
        e.mouse_y = y;
        e.mouse_button = button;
        return e;
    }
    
    static PlayerEvent mouse_down(double x, double y, MouseButton button, std::optional<size_t> index) {
        PlayerEvent e;
        e.type = Type::MouseDown;
        e.mouse_x = x;
        e.mouse_y = y;
        e.mouse_button = button;
        e.mouse_index = index;
        return e;
    }
    
    static PlayerEvent mouse_leave() {
        PlayerEvent e;
        e.type = Type::MouseLeave;
        return e;
    }
    
    static PlayerEvent mouse_wheel(MouseWheelDelta delta) {
        PlayerEvent e;
        e.type = Type::MouseWheel;
        e.wheel_delta = delta;
        return e;
    }
    
    static PlayerEvent gamepad_button_down(GamepadButton button) {
        PlayerEvent e;
        e.type = Type::GamepadButtonDown;
        e.gamepad_button = button;
        return e;
    }
    
    static PlayerEvent gamepad_button_up(GamepadButton button) {
        PlayerEvent e;
        e.type = Type::GamepadButtonUp;
        e.gamepad_button = button;
        return e;
    }
    
    static PlayerEvent text_input(char codepoint) {
        PlayerEvent e;
        e.type = Type::TextInput;
        e.text_codepoint = codepoint;
        return e;
    }
    
    static PlayerEvent focus_gained() {
        PlayerEvent e;
        e.type = Type::FocusGained;
        return e;
    }
    
    static PlayerEvent focus_lost() {
        PlayerEvent e;
        e.type = Type::FocusLost;
        return e;
    }
};

/// Whether this button event was handled by some child
enum class ClipEventResult {
    NotHandled,
    Handled
};

inline ClipEventResult clip_event_result_from_bool(bool value) {
    return value ? ClipEventResult::Handled : ClipEventResult::NotHandled;
}

/// Clip events - events that can be handled by movie clip instances
template<typename GC>
struct ClipEvent {
    enum class Type {
        Construct,
        Data,
        DragOut,
        DragOver,
        EnterFrame,
        Initialize,
        KeyUp,
        KeyDown,
        KeyPress,
        Load,
        MouseUp,
        RightMouseUp,
        MiddleMouseUp,
        MouseUpInside,
        RightMouseUpInside,
        MiddleMouseUpInside,
        MouseDown,
        RightMouseDown,
        MiddleMouseDown,
        MouseMove,
        MouseMoveInside,
        Press,
        RightPress,
        MiddlePress,
        RollOut,
        RollOver,
        Release,
        RightRelease,
        MiddleRelease,
        ReleaseOutside,
        RightReleaseOutside,
        MiddleReleaseOutside,
        Unload,
        MouseWheel
    };
    
    Type type;
    
    // For DragOut, RollOut
    std::optional<typename display_object::InteractiveObject<GC>*> to;
    
    // For DragOver, RollOver
    std::optional<typename display_object::InteractiveObject<GC>*> from;
    
    // For KeyPress
    ButtonKeyCode key_code;
    
    // For Press, Release
    size_t index;
    
    // For MouseWheel
    MouseWheelDelta delta;
    
    // Constructors for different event types
    static ClipEvent construct() {
        ClipEvent e;
        e.type = Type::Construct;
        return e;
    }
    
    static ClipEvent data() {
        ClipEvent e;
        e.type = Type::Data;
        return e;
    }
    
    static ClipEvent drag_out(std::optional<typename display_object::InteractiveObject<GC>*> t) {
        ClipEvent e;
        e.type = Type::DragOut;
        e.to = t;
        return e;
    }
    
    static ClipEvent drag_over(std::optional<typename display_object::InteractiveObject<GC>*> f) {
        ClipEvent e;
        e.type = Type::DragOver;
        e.from = f;
        return e;
    }
    
    static ClipEvent enter_frame() {
        ClipEvent e;
        e.type = Type::EnterFrame;
        return e;
    }
    
    static ClipEvent initialize() {
        ClipEvent e;
        e.type = Type::Initialize;
        return e;
    }
    
    static ClipEvent key_up() {
        ClipEvent e;
        e.type = Type::KeyUp;
        return e;
    }
    
    static ClipEvent key_down() {
        ClipEvent e;
        e.type = Type::KeyDown;
        return e;
    }
    
    static ClipEvent key_press(ButtonKeyCode kc) {
        ClipEvent e;
        e.type = Type::KeyPress;
        e.key_code = kc;
        return e;
    }
    
    static ClipEvent load() {
        ClipEvent e;
        e.type = Type::Load;
        return e;
    }
    
    static ClipEvent mouse_up() {
        ClipEvent e;
        e.type = Type::MouseUp;
        return e;
    }
    
    static ClipEvent mouse_up_inside() {
        ClipEvent e;
        e.type = Type::MouseUpInside;
        return e;
    }
    
    static ClipEvent mouse_down() {
        ClipEvent e;
        e.type = Type::MouseDown;
        return e;
    }
    
    static ClipEvent mouse_move() {
        ClipEvent e;
        e.type = Type::MouseMove;
        return e;
    }
    
    static ClipEvent mouse_move_inside() {
        ClipEvent e;
        e.type = Type::MouseMoveInside;
        return e;
    }
    
    static ClipEvent press(size_t idx) {
        ClipEvent e;
        e.type = Type::Press;
        e.index = idx;
        return e;
    }
    
    static ClipEvent roll_out(std::optional<typename display_object::InteractiveObject<GC>*> t) {
        ClipEvent e;
        e.type = Type::RollOut;
        e.to = t;
        return e;
    }
    
    static ClipEvent roll_over(std::optional<typename display_object::InteractiveObject<GC>*> f) {
        ClipEvent e;
        e.type = Type::RollOver;
        e.from = f;
        return e;
    }
    
    static ClipEvent release(size_t idx) {
        ClipEvent e;
        e.type = Type::Release;
        e.index = idx;
        return e;
    }
    
    static ClipEvent release_outside() {
        ClipEvent e;
        e.type = Type::ReleaseOutside;
        return e;
    }
    
    static ClipEvent unload() {
        ClipEvent e;
        e.type = Type::Unload;
        return e;
    }
    
    static ClipEvent mouse_wheel(MouseWheelDelta d) {
        ClipEvent e;
        e.type = Type::MouseWheel;
        e.delta = d;
        return e;
    }
    
    /// Indicates that the event should be propagated down to children
    bool propagates() const {
        switch (type) {
            case Type::MouseUp:
            case Type::MouseDown:
            case Type::MouseMove:
            case Type::KeyPress:
            case Type::KeyDown:
            case Type::KeyUp:
                return true;
            default:
                return false;
        }
    }
    
    /// Indicates whether this is a button event type
    bool is_button_event() const {
        switch (type) {
            case Type::DragOut:
            case Type::DragOver:
            case Type::KeyPress:
            case Type::Press:
            case Type::Release:
            case Type::ReleaseOutside:
            case Type::RollOut:
            case Type::RollOver:
                return true;
            default:
                return false;
        }
    }
    
    /// Indicates whether this is a keyboard event type
    bool is_key_event() const {
        return type == Type::KeyDown || type == Type::KeyUp || type == Type::KeyPress;
    }
    
    /// Indicates whether this is a mouse event type
    bool is_mouse_event() const {
        switch (type) {
            case Type::MouseUp:
            case Type::MouseDown:
            case Type::MouseMove:
            case Type::RightMouseUp:
            case Type::RightMouseDown:
            case Type::MiddleMouseUp:
            case Type::MiddleMouseDown:
            case Type::MouseUpInside:
            case Type::RightMouseUpInside:
            case Type::MiddleMouseUpInside:
            case Type::MouseMoveInside:
            case Type::MouseWheel:
            case Type::DragOut:
            case Type::DragOver:
            case Type::RollOut:
            case Type::RollOver:
            case Type::Press:
            case Type::RightPress:
            case Type::MiddlePress:
            case Type::Release:
            case Type::RightRelease:
            case Type::MiddleRelease:
            case Type::ReleaseOutside:
            case Type::RightReleaseOutside:
            case Type::MiddleReleaseOutside:
                return true;
            default:
                return false;
        }
    }
};

/// Text control codes - control inputs to a text field
enum class TextControlCode {
    MoveLeft,
    MoveLeftWord,
    MoveLeftLine,
    MoveLeftDocument,
    MoveRight,
    MoveRightWord,
    MoveRightLine,
    MoveRightDocument,
    SelectLeft,
    SelectLeftWord,
    SelectLeftLine,
    SelectLeftDocument,
    SelectRight,
    SelectRightWord,
    SelectRightLine,
    SelectRightDocument,
    SelectAll,
    Copy,
    Paste,
    Cut,
    Backspace,
    BackspaceWord,
    Enter,
    Delete,
    DeleteWord
};

/// Indicates whether this is an event that edits the text content
inline bool text_control_is_edit_input(TextControlCode code) {
    switch (code) {
        case TextControlCode::Paste:
        case TextControlCode::Cut:
        case TextControlCode::Enter:
        case TextControlCode::Backspace:
        case TextControlCode::BackspaceWord:
        case TextControlCode::Delete:
        case TextControlCode::DeleteWord:
            return true;
        default:
            return false;
    }
}

} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_EVENTS_H
