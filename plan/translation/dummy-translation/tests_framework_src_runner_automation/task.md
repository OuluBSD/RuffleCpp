# Task: Translate tests/framework/src/runner/automation.rs

## Description
Translate the Rust file `tests/framework/src/runner/automation.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/tests/framework/src/runner/automation.rs
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
- Translated automated event handling for test framework
- Implemented perform_automated_event() function for executing automated events on player
- Handles all AutomatedEvent variants: MouseDown, MouseUp, MouseMove, MouseWheel
- Handles keyboard events: KeyDown, KeyUp, TextInput, TextControl
- Handles focus events: FocusGained, FocusLost
- Handles IME events: ImePreedit, ImeCommit
- Handles SetClipboardText special case
- Implements event handling status assertion for MouseDown events
- Implemented automated_key_to_descriptor() for key conversion
- Converts AutomatedKey to KeyDescriptor with logical_key, physical_key, key_location
- Maps character keys to PhysicalKey variants (KeyA-Z, Digit0-9, symbols)
- Maps special keys (arrows, function keys, modifiers) to NamedKey variants
- Handles numpad keys with KeyLocation::Numpad
- Handles left/right modifier keys with KeyLocation::Left/Right
- Forward declared all core event types and input_format types
