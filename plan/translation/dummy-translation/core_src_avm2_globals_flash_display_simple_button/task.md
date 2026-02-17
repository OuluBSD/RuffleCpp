# Task: Translate core/src/avm2/globals/flash/display/simple_button.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/display/simple_button.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/display/simple_button.rs
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
- Translated simple_button_allocator() for SimpleButton object allocation
- Translated init() for SimpleButton constructor initialization with state children
- Implemented get_down_state() and set_down_state() for down state
- Implemented get_over_state() and set_over_state() for over state
- Implemented get_hit_test_state() and set_hit_test_state() for hit test state
- Implemented get_up_state() and set_up_state() for up state
- Implemented get_track_as_menu() and set_track_as_menu() for track as menu
- Implemented get_enabled() and set_enabled() for enabled property
- Implemented get_use_hand_cursor() and set_use_hand_cursor() for hand cursor
- Used ButtonState enum for button state identification
- Used ButtonTracking enum for menu vs push tracking
- Used Avm2Button for button display object
- Template pattern for GC types
- Added proper forward declarations for all dependencies
