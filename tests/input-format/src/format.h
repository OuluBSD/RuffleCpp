// C++ translation of tests/input-format/src/format.rs
//! Input format types for automated testing

#ifndef RUFFLE_TESTS_INPUT_FORMAT_FORMAT_H
#define RUFFLE_TESTS_INPUT_FORMAT_FORMAT_H

#include <string>
#include <vector>
#include <variant>
#include <optional>
#include <cstdint>

namespace ruffle {
namespace tests {
namespace input_format {

/// Position of a mouse cursor on the screen.
///
/// Mouse cursor positions are sized relative to the Flash stage's dimensions,
/// regardless of the native window's size or pixel density. For example, a
/// (640x480) stage movie on a 2x display (on platforms that report physical
/// pixels) or at 2x the size will see mouse clicks at its bottom right corner
/// on (1280x960), relative to the window. That coordinate needs to be scaled
/// down to match the desired stage.
struct MousePosition {
    double x;
    double y;

    MousePosition() : x(0.0), y(0.0) {}
    MousePosition(double x_, double y_) : x(x_), y(y_) {}
};

/// Which mouse button is being pressed or released.
enum class MouseButton {
    Left,
    Middle,
    Right
};

/// Control inputs to a text field
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
    Enter,
    Delete
};

/// All possible keys which can be simulated in tests.
///
/// Note: Add more keys if needed.
struct AutomatedKey {
    enum class Type {
        Char,
        Numpad,
        ArrowDown,
        ArrowLeft,
        ArrowRight,
        ArrowUp,
        Backspace,
        CapsLock,
        Delete,
        End,
        Enter,
        Escape,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        Home,
        Insert,
        LeftAlt,
        LeftControl,
        LeftShift,
        NumLock,
        NumpadDelete,
        NumpadDown,
        NumpadEnd,
        NumpadHome,
        NumpadInsert,
        NumpadLeft,
        NumpadPageDown,
        NumpadPageUp,
        NumpadRight,
        NumpadUp,
        PageDown,
        PageUp,
        Pause,
        RightControl,
        RightShift,
        ScrollLock,
        Space,
        Tab,
        Unknown
    };

    Type type;
    char32_t char_value;  // Used for Char and Numpad types

    /// Create a character key
    static AutomatedKey char_key(char32_t ch) {
        AutomatedKey key;
        key.type = Type::Char;
        key.char_value = ch;
        return key;
    }

    /// Create a numpad key
    static AutomatedKey numpad_key(char32_t ch) {
        AutomatedKey key;
        key.type = Type::Numpad;
        key.char_value = ch;
        return key;
    }

    /// Create a special key
    static AutomatedKey special_key(Type t) {
        AutomatedKey key;
        key.type = t;
        key.char_value = 0;
        return key;
    }
};

/// Assertion for whether an event was handled
struct EventHandledAssertion {
    bool value;
    std::string message;
};

/// All automated event types supported by FlashTAS.
///
/// A FlashTAS input file consists of a string of `AutomatedEvent`s which are
/// played back by FlashTAS.
struct AutomatedEvent {
    enum class Type {
        Wait,
        MouseMove,
        MouseDown,
        MouseUp,
        MouseWheel,
        KeyDown,
        KeyUp,
        TextInput,
        TextControl,
        SetClipboardText,
        FocusGained,
        FocusLost,
        ImePreedit,
        ImeCommit
    };

    Type type;

    // Event data - using variant-like structure
    // Wait - no data
    // MouseMove - pos
    // MouseDown - pos, btn, index (optional), assert_handled (optional)
    // MouseUp - pos, btn
    // MouseWheel - lines (optional), pixels (optional)
    // KeyDown/KeyUp - key
    // TextInput - codepoint
    // TextControl - code
    // SetClipboardText - text
    // FocusGained/FocusLost - no data
    // ImePreedit - text, cursor (optional)
    // ImeCommit - text

    MousePosition pos;
    MouseButton btn;
    std::optional<size_t> index;
    std::optional<EventHandledAssertion> assert_handled;
    std::optional<double> lines;
    std::optional<double> pixels;
    AutomatedKey key;
    char32_t codepoint;
    TextControlCode code;
    std::string text;
    std::optional<std::pair<size_t, size_t>> cursor;

    /// Create a Wait event
    static AutomatedEvent wait() {
        AutomatedEvent event;
        event.type = Type::Wait;
        return event;
    }

    /// Create a MouseMove event
    static AutomatedEvent mouse_move(const MousePosition& pos_) {
        AutomatedEvent event;
        event.type = Type::MouseMove;
        event.pos = pos_;
        return event;
    }

    /// Create a MouseDown event
    static AutomatedEvent mouse_down(const MousePosition& pos_, MouseButton btn_,
                                     std::optional<size_t> index_ = std::nullopt,
                                     std::optional<EventHandledAssertion> assert_handled_ = std::nullopt) {
        AutomatedEvent event;
        event.type = Type::MouseDown;
        event.pos = pos_;
        event.btn = btn_;
        event.index = index_;
        event.assert_handled = assert_handled_;
        return event;
    }

    /// Create a MouseUp event
    static AutomatedEvent mouse_up(const MousePosition& pos_, MouseButton btn_) {
        AutomatedEvent event;
        event.type = Type::MouseUp;
        event.pos = pos_;
        event.btn = btn_;
        return event;
    }

    /// Create a MouseWheel event
    static AutomatedEvent mouse_wheel(std::optional<double> lines_ = std::nullopt,
                                      std::optional<double> pixels_ = std::nullopt) {
        AutomatedEvent event;
        event.type = Type::MouseWheel;
        event.lines = lines_;
        event.pixels = pixels_;
        return event;
    }

    /// Create a KeyDown event
    static AutomatedEvent key_down(const AutomatedKey& key_) {
        AutomatedEvent event;
        event.type = Type::KeyDown;
        event.key = key_;
        return event;
    }

    /// Create a KeyUp event
    static AutomatedEvent key_up(const AutomatedKey& key_) {
        AutomatedEvent event;
        event.type = Type::KeyUp;
        event.key = key_;
        return event;
    }

    /// Create a TextInput event
    static AutomatedEvent text_input(char32_t codepoint_) {
        AutomatedEvent event;
        event.type = Type::TextInput;
        event.codepoint = codepoint_;
        return event;
    }

    /// Create a TextControl event
    static AutomatedEvent text_control(TextControlCode code_) {
        AutomatedEvent event;
        event.type = Type::TextControl;
        event.code = code_;
        return event;
    }

    /// Create a SetClipboardText event
    static AutomatedEvent set_clipboard_text(const std::string& text_) {
        AutomatedEvent event;
        event.type = Type::SetClipboardText;
        event.text = text_;
        return event;
    }

    /// Create a FocusGained event
    static AutomatedEvent focus_gained() {
        AutomatedEvent event;
        event.type = Type::FocusGained;
        return event;
    }

    /// Create a FocusLost event
    static AutomatedEvent focus_lost() {
        AutomatedEvent event;
        event.type = Type::FocusLost;
        return event;
    }

    /// Create an ImePreedit event
    static AutomatedEvent ime_preedit(const std::string& text_,
                                      std::optional<std::pair<size_t, size_t>> cursor_ = std::nullopt) {
        AutomatedEvent event;
        event.type = Type::ImePreedit;
        event.text = text_;
        event.cursor = cursor_;
        return event;
    }

    /// Create an ImeCommit event
    static AutomatedEvent ime_commit(const std::string& text_) {
        AutomatedEvent event;
        event.type = Type::ImeCommit;
        event.text = text_;
        return event;
    }
};

} // namespace input_format
} // namespace tests
} // namespace ruffle

#endif // RUFFLE_TESTS_INPUT_FORMAT_FORMAT_H
