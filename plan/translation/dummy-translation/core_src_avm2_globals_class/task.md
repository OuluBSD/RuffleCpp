# Task: Translate core/src/avm2/globals/class.rs

## Description
Translate the Rust file `core/src/avm2/globals/class.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/class.rs
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
- Translated Class class allocator function (returns error 1115)
- Implemented get_prototype function to retrieve class prototype
- Used template pattern for GC types
- Added proper forward declarations for AVM2 types
