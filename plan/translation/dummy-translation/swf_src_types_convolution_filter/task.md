# Task: Translate swf/src/types/convolution_filter.rs

## Description
Translate the Rust file `swf/src/types/convolution_filter.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/swf/src/types/convolution_filter.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated ConvolutionFilter struct with matrix, divisor, bias, and color fields
- Implemented ConvolutionFilterFlags enum with Clamp and PreserveAlpha flags
- Added bitwise operators for flag operations
- Implemented is_clamped() and is_preserve_alpha() helper methods
- Used std::vector<float> for the convolution matrix

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
