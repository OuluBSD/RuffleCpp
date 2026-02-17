# Task: Translate core/src/avm2/op.rs

## Description
Translate the Rust file `core/src/avm2/op.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/op.rs
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
- Translated Op enum as variant-based class with nested structs for each operation type
- Translated LookupSwitch with interior mutability using std::cell::Cell
- Implemented can_throw_error(), is_nop(), and is_pure_push() methods
- Added size assertion for 64-bit systems matching Rust's size_of check
