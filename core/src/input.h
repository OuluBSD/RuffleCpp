// C++ translation of core/src/input.rs
// Input event management

#ifndef RUFFLE_CORE_INPUT_H
#define RUFFLE_CORE_INPUT_H

#include <cstdint>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include <string>

// Forward declarations
namespace ruffle {
namespace core {
    enum class KeyCode;
    enum class KeyLocation;
    enum class MouseButton;
    enum class GamepadButton;
    enum class TextControlCode;
    enum class NamedKey;
    struct PhysicalKey;
    struct LogicalKey;
    struct KeyDescriptor;
    struct PlayerEvent;
    enum class MouseWheelDelta;
    struct ImeEvent;
}
}

namespace ruffle {
namespace core {

/// Mapping type for key codes
enum class KeyCodeMappingType {
    Physical,
    Logical,
};

/// An event describing input in general
///
/// It's usually a processed PlayerEvent
struct InputEvent {
    enum class Type {
        KeyDown,
        KeyUp,
        MouseMove,
        MouseUp,
        MouseDown,
        MouseLeave,
        MouseWheel,
        TextInput,
        TextControl,
        Ime,
    };

    Type type;

    // KeyDown/KeyUp fields
    KeyCode key_code;
    std::optional<char> key_char;
    KeyLocation key_location;

    // Mouse fields
    double x;
    double y;
    MouseButton button;
    std::size_t index;

    // MouseWheel field
    MouseWheelDelta delta;

    // TextInput field
    char codepoint;

    // TextControl field
    TextControlCode code;

    // Ime field
    ImeEvent ime_event;
};

/// Click event data for tracking double-clicks
struct ClickEventData {
    double x;
    double y;
    std::chrono::system_clock::time_point time;
    std::size_t index;

    [[nodiscard]] double distance_squared_to(double x, double y) const {
        double dx = x - this->x;
        double dy = y - this->y;
        return dx * dx + dy * dy;
    }
};

/// Manages input events and tracks key/mouse state
class InputManager {
    /// Tracks which physical keys are currently pressed
    std::unordered_set<std::pair<PhysicalKey, KeyLocation>> keys_down_phys_loc_;

    /// Tracks which key codes are currently pressed
    std::unordered_set<KeyCode> keys_down_;

    /// Tracks which key codes are toggled
    std::unordered_set<KeyCode> keys_toggled_;

    /// Last key code pressed
    KeyCode last_key_;

    /// Last character typed
    std::optional<char> last_char_;

    /// Last click data
    std::optional<ClickEventData> last_click_;

    /// Map from gamepad buttons to key codes
    std::unordered_map<GamepadButton, KeyCode> gamepad_button_mapping_;

    /// Key code mapping type
    KeyCodeMappingType key_code_mapping_type_;

public:
    /// Create a new InputManager
    explicit InputManager(std::unordered_map<GamepadButton, KeyCode> gamepad_button_mapping);

    /// Process a player event and return an input event if applicable
    std::optional<InputEvent> process_event(const PlayerEvent& event);

    /// Check if a key is currently pressed
    [[nodiscard]] bool is_key_down(KeyCode key) const;

    /// Check if a key is toggled
    [[nodiscard]] bool is_key_toggled(KeyCode key) const;

    /// Get the last key code pressed
    [[nodiscard]] KeyCode last_key_code() const;

    /// Get the last character typed
    [[nodiscard]] std::optional<char> last_key_char() const;

    /// Get the last click index
    [[nodiscard]] std::size_t last_click_index() const;

    /// Check if a mouse button is pressed
    [[nodiscard]] bool is_mouse_down(MouseButton button) const;

    /// Get all currently pressed mouse buttons
    [[nodiscard]] int get_mouse_down_buttons() const;

private:
    /// Add a key to the pressed set
    void add_key(KeyCode key_code);

    /// Toggle a key in the toggled set
    void toggle_key(KeyCode key_code);

    /// Remove a key from the pressed set
    void remove_key(KeyCode key_code);

    /// Map a KeyDescriptor to a KeyCode
    std::optional<KeyCode> map_to_key_code(const KeyDescriptor& descriptor) const;

    /// Map a KeyDescriptor to a key character
    std::optional<char> map_to_key_char(const KeyDescriptor& descriptor) const;

    /// Map a KeyDescriptor to a KeyLocation
    KeyLocation map_to_key_location(const KeyDescriptor& descriptor) const;

    /// Handle an input event (update internal state)
    void handle_event(const InputEvent& event);

    /// Update last click data and return click index
    std::size_t update_last_click(double x, double y, std::optional<std::size_t> index);
};

/// Map a physical key to a key code
std::optional<KeyCode> map_to_key_code_physical(PhysicalKey key);

/// Map a logical key to a key code
std::optional<KeyCode> map_to_key_code_logical(LogicalKey key, KeyLocation location);

/// Map a character to a key code
std::optional<KeyCode> map_character_to_key_code(char c);

} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_INPUT_H
