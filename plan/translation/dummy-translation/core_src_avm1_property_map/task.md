# Task: Translate core/src/avm1/property_map.rs

## Description
Translate the Rust file `core/src/avm1/property_map.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/property_map.rs
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
- Translated PropertyMap template class for AVM1 property name-to-value mapping
- Implemented case-sensitive and case-insensitive lookup support for SWFv6 compatibility
- Implemented CaseInsensitiveHash and CaseSensitiveHash functors for SWF string hashing
- Implemented Entry, OccupiedEntry, VacantEntry classes for entry-based access
- Implemented contains_key() for key existence checking
- Implemented entry() for obtaining entry handles
- Implemented get(), get_mut() for value access
- Implemented get_index() for index-based access (insertion order)
- Implemented insert() for value insertion with optional old value return
- Implemented iter() for iteration in Flash order (most recently added first)
- Implemented remove() for key removal with index shifting
- Maintains insertion order using std::vector with parallel hash maps for lookup
