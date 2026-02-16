# Task: Translate swf/src/types/blur_filter.rs

## Description
Translate the Rust file `swf/src/types/blur_filter.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/swf/src/types/blur_filter.rs
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
- Translated BlurFilter struct with blur_x, blur_y, and flags fields
- Implemented BlurFilterFlags enum with PASSES flag
- Added PASS_SCALES constant array for accumulative blur calculations
- Implemented num_passes(), scale(), impotent(), and calculate_dest_rect() methods
- Added scale_blur() static helper method
- Used forward declarations for Fixed16, Rectangle, and Twips types
