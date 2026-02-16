# Task: Translate core/src/avm2/object/font_object.rs

## Description
Translate the Rust file `core/src/avm2/object/font_object.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/object/font_object.rs
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
- Translated FontObject class with allocator and for_font factory method
- Added FontObjectData with optional font field
- Implemented font getter method
- Used HasPrefixField pattern for base class
- Template pattern for GC types
