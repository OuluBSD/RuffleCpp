# Task: Translate core/src/avm1/globals/color_transform.rs

## Description
Translate the Rust file `core/src/avm1/globals/color_transform.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/globals/color_transform.rs
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
- Translated ColorTransformObject template class with all color transform fields
- Implemented identity() static method for identity transform
- Implemented construct() for creating ColorTransform from SWF ColorTransform
- Added cast() helper for type-safe Value to ColorTransformObject conversion
- Implemented to_swf() conversion method
- Translated all getter/setter methods for multipliers and offsets
- Implemented get_rgb/set_rgb for RGB color value access
- Translated to_string method for string representation
- Implemented concat method for combining color transforms
- Used double for all color transform values
- Maintained flash.geom.ColorTransform behavior for color transformations
