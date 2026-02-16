# Task: Translate core/src/avm1/globals/number.rs

## Description
Translate the Rust file `core/src/avm1/globals/number.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/globals/number.rs
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
- Translated Number class with PROTO_DECLS for valueOf, toString methods
- Implemented OBJECT_DECLS for static properties: MAX_VALUE, MIN_VALUE, NaN, NEGATIVE_INFINITY, POSITIVE_INFINITY
- Implemented create_class function for Number class creation
- Translated constructor function that creates Number objects from values
- Implemented function for Number() calls that returns primitive number value
- Translated to_string method with radix support (2-36 base conversion)
- Implemented value_of method to return primitive number value
- Used template pattern for GC types
- Maintained Flash Number object behavior for number conversions
