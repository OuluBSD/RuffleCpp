# Task: Translate core/src/avm2/vector.rs

## Description
Translate the Rust file `core/src/avm2/vector.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/vector.rs
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
- Translated VectorStorage for AS3 Vector types
- Implemented template class with Value<GC> storage
- Added is_fixed flag for fixed-size vectors
- Implemented all vector operations: get, set, push, pop, shift, unshift, insert, remove
- Added reverse(), splice(), and iteration support
- Used std::vector for underlying storage with proper bounds checking
- Included error handling with make_error_1125 and make_error_1126
