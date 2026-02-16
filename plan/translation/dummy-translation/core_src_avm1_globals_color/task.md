# Task: Translate core/src/avm1/globals/color.rs

## Description
Translate the Rust file `core/src/avm1/globals/color.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/globals/color.rs
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
- Translated Color class with PROTO_DECLS for setRGB, setTransform, getRGB, getTransform
- Implemented create_class function for Color class creation
- Added constructor function that sets target property
- Implemented target() helper to resolve target display object
- Translated get_rgb, get_transform, set_rgb, set_transform methods
- Added set_color_mult and set_color_add helper functions for transform parsing
- Used template pattern for GC types
- Maintained Flash Color object behavior for color transformations
