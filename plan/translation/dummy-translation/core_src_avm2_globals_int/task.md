# Task: Translate core/src/avm2/globals/int.rs

## Description
Translate the Rust file `core/src/avm2/globals/int.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/int.rs
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
- Translated int constructor and call_handler functions
- int_constructor converts first argument to i32, defaults to 0
- call_handler converts first argument to i32, defaults to 0
- Added stub functions for to_exponential, to_fixed, to_precision, to_string, value_of
- All stub functions use unreachable() as they should not be callable on int values
- Used template pattern for GC types
- Added proper forward declarations for AVM2 types
