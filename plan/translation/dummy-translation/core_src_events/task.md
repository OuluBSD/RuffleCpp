# Task: Translate core/src/events.rs

## Description
Translate the Rust file `core/src/events.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/events.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated MouseButton enum for mouse button identifiers
- Translated GamepadButton enum for gamepad buttons
- Translated MouseWheelDelta struct for scroll wheel events
- Translated KeyDescriptor struct for keyboard events
- Translated ButtonKeyCode enum for button key codes
- Translated ImeEvent enum for IME events
- Translated PlayerEvent struct for host system events
- Translated ClipEventResult enum for event handling result
- Translated ClipEvent template struct for movie clip events
- Translated TextControlCode enum for text field control inputs
- Added utility functions for event type checking

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
