# Task: Translate core/src/avm2/globals/flash/display/sprite.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/display/sprite.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/display/sprite.rs
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
- Translated sprite_allocator() for Sprite object allocation
- Translated construct_children() for constructing Sprite children
- Implemented get_drop_target() for drop target getter
- Implemented get_graphics() for graphics object lazy initialization
- Implemented get_sound_transform() and set_sound_transform() for sound transform
- Implemented get_button_mode() and set_button_mode() for button mode
- Implemented start_drag() for starting drag operation with constraints
- Implemented stop_drag() for stopping drag operation
- Implemented get_use_hand_cursor() and set_use_hand_cursor() for hand cursor
- Implemented get_hit_area() and set_hit_area() for hit area
- Used Rectangle and Twips for drag constraints
- Template pattern for GC types
- Added proper forward declarations for all dependencies
