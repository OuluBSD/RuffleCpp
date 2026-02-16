# Task: Translate core/src/types.rs

## Description
Translate the Rust file `core/src/types.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/types.rs
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
- Translated Percent class for type-safe percentage storage
- Implemented percent() accessor returning value in [0.0, 100.0]
- Added from_unit() static method for converting [0.0, 1.0] to percentage
- Added unit() method for converting percentage to [0.0, 1.0]
- Translated Degrees class for type-safe degree storage
- Implemented from_radians() and into_radians() conversion methods
- Added comparison operators for both types (==, !=, <, <=, >, >=, <=>)
- Used constexpr for compile-time evaluation where possible
- No arithmetic operators on Degrees (as per Rust original design)
