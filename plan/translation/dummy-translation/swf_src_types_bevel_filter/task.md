# Task: Translate swf/src/types/bevel_filter.rs

## Description
Translate the Rust file `swf/src/types/bevel_filter.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/swf/src/types/bevel_filter.rs
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
- Translated BevelFilter struct with shadow_color, highlight_color, blur_x, blur_y, angle, distance, strength, and flags fields
- Implemented BevelFilterFlags enum class with INNER_SHADOW, KNOCKOUT, COMPOSITE_SOURCE, ON_TOP, and PASSES flags
- Added bitwise operators for flag operations
- Implemented helper methods: is_inner(), is_knockout(), is_on_top(), num_passes()
- Added scale(), inner_blur_filter(), and calculate_dest_rect() method declarations
- Used forward declarations for Fixed16, Fixed8, Rectangle, Twips, and BlurFilter types
