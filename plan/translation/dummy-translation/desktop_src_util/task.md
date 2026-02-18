# Task: Translate desktop/src/util.rs

## Description
Translate the Rust file `desktop/src/util.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/desktop/src/util.rs
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
- Created `desktop/src/util.h` (429 lines Rust -> ~110 lines C++)
- Implemented winit_to_ruffle_text_control() for text control code conversion
- Implemented winit_input_to_ruffle_key_descriptor() for key event conversion
- Implemented gilrs_button_to_gamepad_button() for gamepad button mapping
- Implemented get_screen_size() for calculating total screen dimensions
- Implemented parse_url() for parsing file paths and URLs
- Implemented open_url() for opening URLs in default browser
- Used std::optional for optional return values
- Used std::expected for error handling
- Added proper forward declarations for all winit, gilrs, and ruffle types
