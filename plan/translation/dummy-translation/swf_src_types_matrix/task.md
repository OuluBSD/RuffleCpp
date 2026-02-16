# Task: Translate swf/src/types/matrix.rs

## Description
Translate the Rust file `swf/src/types/matrix.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/swf/src/types/matrix.rs
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
- Translated Matrix struct for SWF display object transformations
- Implemented 2x3 affine transformation matrix with a, b, c, d, tx, ty fields
- Added IDENTITY static constant for identity matrix
- Implemented scale(), rotate(), translate() static factory methods
- Implemented invert() method for matrix inversion
- Implemented operator==, operator!= for equality comparison
- Implemented operator* for matrix multiplication and point transformation
- Implemented operator*= for matrix multiply-assign
- Used Fixed16 for scale/rotation elements, Twips for translation
- Added proper forward declarations for Fixed16, Twips, and Point types
