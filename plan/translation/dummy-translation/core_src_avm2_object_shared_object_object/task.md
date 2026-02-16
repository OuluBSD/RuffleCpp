# Task: Translate core/src/avm2/object/shared_object_object.rs

## Description
Translate the Rust file `core/src/avm2/object/shared_object_object.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/object/shared_object_object.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated SharedObjectObject class with GC management
- Used HasPrefixField pattern for base class (base must be first member)
- Implemented SharedObjectObjectData with data and name fields
- Added from_data_and_name static factory method
- Implemented reset_data method for clearing SharedObject data
- Used template pattern for GC types

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
