# Task: Translate core/src/avm2/class.rs

## Description
Translate the Rust file `core/src/avm2/class.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/class.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated ClassAttributes bitflags (Sealed, Final, Interface, Generic)
- Created BuiltinType enum for builtin class identifiers
- Created AllocatorFn and Allocator for instance allocation
- Created CustomConstructorFn and CustomConstructor for custom construction
- Created ClassLinkType enum for class linking state
- Created ClassData class holding class metadata
- Created Class template class with all getters/setters
- Added application management for generic type parameterization
- Added class object management
- Added attribute checking methods (is_sealed, is_final, is_interface, is_generic)

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
