# Task: Translate swf/src/types/glow_filter.rs

## Description
Translate the Rust file `swf/src/types/glow_filter.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/swf/src/types/glow_filter.rs
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
- Translated GlowFilter struct with color, blur_x, blur_y, strength, and flags fields
- Implemented GlowFilterFlags enum class with INNER_GLOW, KNOCKOUT, COMPOSITE_SOURCE, and PASSES flags
- Added bitwise operators for flag operations
- Implemented helper methods: is_inner(), is_knockout(), composite_source(), num_passes()
- Added scale(), calculate_dest_rect(), and inner_blur_filter() method declarations
- Used forward declarations for Fixed16, Fixed8, Rectangle, Twips, and BlurFilter types
