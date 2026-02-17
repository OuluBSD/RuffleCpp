# Task: Translate render/src/matrix.rs

## Description
Translate the Rust file `render/src/matrix.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/render/src/matrix.rs
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
- Translated Matrix struct for Flash display object transformations
- Implemented matrix constants: IDENTITY, ZERO, TWIPS_TO_PIXELS, PIXELS_TO_TWIPS
- Implemented static constructors: scale(), rotate(), translate(), create_box()
- Implemented create_box_with_rotation() for rotated box transforms
- Implemented create_box_from_rectangle() for rectangle-based transforms
- Implemented create_gradient_box() for gradient coordinate transforms
- Implemented determinant() for matrix determinant calculation
- Implemented inverse() for matrix inversion with optional return
- Implemented operator*() for matrix-matrix multiplication
- Implemented operator*() for matrix-point multiplication
- Implemented operator*() for matrix-point delta multiplication
- Implemented operator*() for matrix-rectangle multiplication
- Implemented operator*=() for in-place matrix multiplication
- Implemented from_swf() and to_swf() conversion methods
- Implemented round_to_i32() helper with IEEE-754 round-to-nearest-even rounding
- Proper handling of NaN/Infinity and out-of-range values per Flash behavior
