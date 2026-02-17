# Task: Translate core/src/avm2/property_map.rs

## Description
Translate the Rust file `core/src/avm2/property_map.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/property_map.rs
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
- Translated PropertyMap struct for named properties on objects
- Implemented get(), get_mut(), contains_key() methods for QName lookup
- Implemented get_for_multiname() and get_with_ns_for_multiname() for Multiname lookup
- Implemented iter() for iterating over all properties
- Implemented insert(), insert_with_namespace(), remove() methods
- Template pattern for GC types
- Added proper forward declarations for all dependencies
