# Task: Translate web/src/input.rs

## Description
Translate the Rust file `web/src/input.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/web/src/input.rs
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
- Created `web/src/input.h` (314 lines Rust -> ~70 lines C++)
- Translated web_input_to_ruffle_key_descriptor() function
- Translated map_physical_key() for KeyboardEvent.code conversion
- Translated map_logical_key() for KeyboardEvent.key conversion
- Translated map_key_location() for location value conversion
- Translated web_to_ruffle_text_control() for text control codes
- Handles all standard keyboard keys and special keys
- Supports modifier key combinations for text control actions
- Added proper forward declarations for all dependencies
