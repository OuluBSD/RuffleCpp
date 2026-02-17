# Task: Translate core/src/avm2/amf.rs

## Description
Translate the Rust file `core/src/avm2/amf.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/amf.rs
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
- Translated AMF serialization functions (serialize_value, recursive_serialize)
- Translated AMF deserialization functions (deserialize_value, deserialize_value_impl, deserialize_lso)
- Translated helper functions (get_or_create_element, get_or_create_value, alias_to_class, class_to_alias)
- Used std::unordered_map for ObjectTable type alias
- Forward declared flash_lso types for LSO/AMF handling
