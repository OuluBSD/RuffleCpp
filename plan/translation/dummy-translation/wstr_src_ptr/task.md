# Task: Translate wstr/src/ptr.rs

## Description
Translate the Rust file `wstr/src/ptr.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/wstr/src/ptr.rs
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
- Translated WStrMetadata class for WStr pointer metadata (4 bytes)
- Implemented WIDE_MASK constant for wide string indicator
- Implemented is_wide(), len(), len32() accessor methods
- Implemented new_unsafe() and new32() factory methods
- Implemented from_raw_parts() and from_raw_parts_mut() for pointer creation
- Implemented from_units() and from_units_mut() for unit buffer conversion
- Implemented units(), units_mut() for buffer access
- Implemented offset(), offset_mut() for unit offset calculation
- Implemented read_at() for unit dereferencing
- Implemented slice(), slice_mut() for subslice pointer creation
