// C++ translation of tests/input-format/src/lib.rs
// Input format test utilities

#ifndef RUFFLE_TESTS_INPUT_FORMAT_LIB_H
#define RUFFLE_TESTS_INPUT_FORMAT_LIB_H

// Forward declarations for format module
namespace ruffle {
namespace tests {
namespace input_format {
namespace format {

/// Represents an automated event for testing
struct AutomatedEvent {
    // Would contain event type and parameters
};

/// Represents an automated key for testing
struct AutomatedKey {
    // Would contain key code and modifiers
};

/// Mouse button enumeration
enum class MouseButton {
    Left,
    Right,
    Middle,
    // ... other buttons
};

/// Text control codes
enum class TextControlCode {
    // Would contain control code values
};

} // namespace format

// Forward declarations for injector module
namespace injector {

/// Input injector for automated testing
class InputInjector {
public:
    // Would contain injection methods
};

/// Mouse buttons bitmask
using MouseButtons = unsigned int;

} // namespace injector

// Re-exports from format module
using format::AutomatedEvent;
using format::AutomatedKey;
using format::MouseButton;
using format::TextControlCode;

// Re-exports from injector module
using injector::InputInjector;
using injector::MouseButtons;

} // namespace input_format
} // namespace tests
} // namespace ruffle

#endif // RUFFLE_TESTS_INPUT_FORMAT_LIB_H
