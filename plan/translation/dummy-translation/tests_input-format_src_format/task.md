# Task: Translate tests/input-format/src/format.rs

## Description
Translate the Rust file `tests/input-format/src/format.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/tests/input-format/src/format.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved

## Translation Details
- Translated MousePosition struct with x, y double coordinates
- Implemented MouseButton enum class (Left, Middle, Right)
- Translated TextControlCode enum with all text field control codes
- Implemented AutomatedKey struct with Type enum and char_value field
- Added static factory methods: char_key(), numpad_key(), special_key()
- Translated EventHandledAssertion struct with value and message fields
- Implemented AutomatedEvent struct with Type enum and all event data fields
- Added static factory methods for all event types (wait, mouse_move, mouse_down, etc.)
- Used std::optional for optional fields (index, assert_handled, lines, pixels, cursor)
- Maintained FlashTAS input format compatibility
