# Task: Translate core/src/input.rs

## Description
Translate the Rust file `core/src/input.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/input.rs
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
- Translated KeyCodeMappingType enum with Physical and Logical variants
- Implemented InputEvent struct with Type enum for event categorization
- Added all event types: KeyDown, KeyUp, MouseMove, MouseUp, MouseDown, MouseLeave, MouseWheel, TextInput, TextControl, Ime
- Implemented ClickEventData struct for double-click tracking
- Added distance_squared_to() helper method
- Implemented InputManager class for input event management
- Added keys_down_phys_loc_, keys_down_, keys_toggled_ tracking sets
- Implemented last_key_, last_char_, last_click_ state tracking
- Added gamepad_button_mapping_ for gamepad support
- Implemented process_event() for PlayerEvent to InputEvent conversion
- Added is_key_down(), is_key_toggled(), last_key_code(), last_key_char() accessors
- Implemented last_click_index(), is_mouse_down(), get_mouse_down_buttons() mouse methods
- Added map_to_key_code_physical(), map_to_key_code_logical() mapping functions
- Implemented map_character_to_key_code() for character to key code conversion
- Used std::chrono::system_clock for click timing
- Used std::optional for optional fields
