# Task: Translate swf/src/types/point.rs

## Description
Translate the Rust file `swf/src/types/point.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/swf/src/types/point.rs
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
- Translated Point template struct with x and y coordinates
- Implemented PointDelta template struct for coordinate differences
- Added Coordinate concept for type constraints (int32_t and Twips specializations)
- Implemented ZERO constants for both Point and PointDelta
- Added from_pixels() static methods for Twips specialization
- Implemented operator overloads: +, +=, -, -=, *, /=, unary -
- Used fmt::format for Display trait equivalent
- Added proper C++20 concepts for template constraints
