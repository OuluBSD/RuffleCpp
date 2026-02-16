# Task: Translate core/src/avm2/dynamic_map.rs

## Description
Translate the Rust file `core/src/avm2/dynamic_map.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/dynamic_map.rs
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
- Translated DynamicMap for AVM2 object dynamic properties
- Implemented DynamicKey template class (String, Uint, Object variants)
- Added DynamicProperty struct with value and enumerable flag
- Implemented DynamicMap template class with hash table storage
- Added iteration support with next(), pair_at(), key_at(), value_at() methods
- Used std::unordered_map with FNV hasher (similar to Rust FnvBuildHasher)
