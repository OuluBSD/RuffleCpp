# Task: Translate core/src/avm2/globals/boolean.rs

## Description
Translate the Rust file `core/src/avm2/globals/boolean.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/boolean.rs
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
- Translated boolean_constructor and call_handler functions
- Converts first argument to boolean, defaults to false
- Uses std::vector for args parameter
- Template pattern for GC types
