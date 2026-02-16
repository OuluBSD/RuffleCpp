# Task: Translate core/src/avm2/property.rs

## Description
Translate the Rust file `core/src/avm2/property.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/property.rs
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
- Translated Property enum class with Virtual, Method, Slot, and ConstSlot variants
- Implemented PropertyClass template class for lazy class resolution
- Added Any, Class, and Name type variants for PropertyClass
- Implemented coerce() method for property type coercion
- Added get_class() and get_name() methods for PropertyClass
- Used std::variant for enum-like behavior in C++
- Added convenience constructors for Property types
