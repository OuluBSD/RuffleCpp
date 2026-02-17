// C++ translation of tests/input-format/src/injector.rs
//! Automated input injector

#ifndef RUFFLE_TESTS_INPUT_FORMAT_INJECTOR_H
#define RUFFLE_TESTS_INPUT_FORMAT_INJECTOR_H

#include <vector>
#include <cstdint>
#include <functional>
#include <optional>

// Forward declarations
namespace ruffle {
namespace tests {
namespace input_format {

// Forward declare AutomatedEvent and MouseButton from format module
namespace format {
    struct AutomatedEvent;
    enum class MouseButton;
}

/// A set of currently held-down mouse buttons.
///
/// Convertible from `MouseButton`, which is intended to represent ONE
/// button being held or released.
class MouseButtons {
private:
    uint8_t bits_;

public:
    /// Flag constants
    static constexpr uint8_t LEFT = 0b00000001;
    static constexpr uint8_t MIDDLE = 0b00000010;
    static constexpr uint8_t RIGHT = 0b00000100;

    MouseButtons() : bits_(0) {}
    explicit MouseButtons(uint8_t bits) : bits_(bits) {}

    /// Create an empty set of mouse buttons
    static MouseButtons empty() {
        return MouseButtons(0);
    }

    /// Check if a specific button is held
    bool contains(uint8_t button) const {
        return (bits_ & button) != 0;
    }

    /// Add a button to the set
    void insert(uint8_t button) {
        bits_ |= button;
    }

    /// Remove a button from the set
    void remove(uint8_t button) {
        bits_ &= ~button;
    }

    /// Get the raw bits
    uint8_t bits() const { return bits_; }

    /// Bitwise OR assignment
    MouseButtons& operator|=(const MouseButtons& other) {
        bits_ |= other.bits_;
        return *this;
    }

    /// Bitwise AND with negation (for removing buttons)
    MouseButtons& operator&=(uint8_t neg_mask) {
        bits_ &= neg_mask;
        return *this;
    }

    /// Bitwise OR
    MouseButtons operator|(const MouseButtons& other) const {
        return MouseButtons(bits_ | other.bits_);
    }
};

/// Convert MouseButton to MouseButtons
inline MouseButtons mouse_button_to_mask(format::MouseButton btn);

/// Automated input injector
///
/// Reads automated test input from a file and injects it into the player.
class InputInjector {
private:
    /// The list of events to inject.
    std::vector<format::AutomatedEvent> items_;

    /// The current event position within that list.
    size_t pos_;

    /// The current set of held-down buttons.
    MouseButtons buttons_;

public:
    InputInjector()
        : items_()
        , pos_(0)
        , buttons_(MouseButtons::empty())
    {}

    /// Construct an input injector from an input file.
    ///
    /// @param path Path to the input file
    /// @return Result containing the injector or an error
    static std::optional<InputInjector> from_file(const std::string& path);

    /// Construct an input injector from an input reader.
    ///
    /// @param data The input data (JSON format)
    /// @return Result containing the injector or an error
    static std::optional<InputInjector> from_reader(const std::string& data);

    /// Create an empty input injector with no input to inject.
    ///
    /// Useful to represent a missing input file in cases where providing one
    /// is optional.
    static InputInjector empty() {
        return InputInjector();
    }

    /// Run the next frame's worth of events.
    ///
    /// @param event_sink Callback function that receives (event, current_buttons)
    template<typename Sink>
    void next(Sink&& event_sink) {
        size_t pos = pos_;
        
        if (pos < items_.size()) {
            auto it = items_.begin() + pos;
            
            while (it != items_.end()) {
                const auto& event = *it;
                ++pos;

                switch (event.type) {
                    case format::AutomatedEvent::Type::Wait:
                        goto done;
                        
                    case format::AutomatedEvent::Type::MouseMove:
                    case format::AutomatedEvent::Type::KeyDown:
                    case format::AutomatedEvent::Type::KeyUp:
                    case format::AutomatedEvent::Type::TextInput:
                    case format::AutomatedEvent::Type::TextControl:
                    case format::AutomatedEvent::Type::SetClipboardText:
                    case format::AutomatedEvent::Type::MouseWheel:
                    case format::AutomatedEvent::Type::ImePreedit:
                    case format::AutomatedEvent::Type::ImeCommit:
                    case format::AutomatedEvent::Type::FocusGained:
                    case format::AutomatedEvent::Type::FocusLost:
                        // No button state changes for these events
                        break;
                        
                    case format::AutomatedEvent::Type::MouseDown: {
                        auto btn_mask = mouse_button_to_mask(event.btn);
                        buttons_ |= btn_mask;
                        break;
                    }
                    
                    case format::AutomatedEvent::Type::MouseUp: {
                        auto mask = mouse_button_to_mask(event.btn);
                        buttons_ &= ~mask.bits();
                        break;
                    }
                }

                event_sink(event, buttons_);
                ++it;
            }
        }
        
    done:
        pos_ = pos;
    }

    /// Check if there are more events to process
    bool has_more() const {
        return pos_ < items_.size();
    }

    /// Get the current position in the event list
    size_t position() const { return pos_; }

    /// Get the current button state
    MouseButtons current_buttons() const { return buttons_; }
};

} // namespace input_format
} // namespace tests
} // namespace ruffle

#endif // RUFFLE_TESTS_INPUT_FORMAT_INJECTOR_H
