# Task: Translate core/src/avm1/debug.rs

## Description
Translate the Rust file `core/src/avm1/debug.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/debug.rs
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
- Translated VariableDumper class for AVM1 debugging
- Implemented object tracking with pointer vector
- Added print methods: print_string, print_object, print_property, print_properties
- Implemented print_value for all Value types (Undefined, Null, Bool, Number, String, Object, MovieClip)
- Added print_variables for dumping object variables
- Template-based design for GC compatibility
- Used std::string for output buffer
