# Task: Translate core/src/avm2/globals/void.rs

## Description
Translate the Rust file `core/src/avm2/globals/void.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/void.rs
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
- Translated create_class function for void type
- void class has no interfaces and is marked as FINAL | SEALED
- Uses template pattern for GC types
