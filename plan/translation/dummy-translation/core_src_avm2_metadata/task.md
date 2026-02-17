# Task: Translate core/src/avm2/metadata.rs

## Description
Translate the Rust file `core/src/avm2/metadata.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/metadata.rs
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
- Translated MetadataItem struct for key-value pairs in trait metadata
- Translated Metadata class for AVM2 metadata representation
- Implemented from_abc_index() for converting ABC metadata indexes to Metadata objects
- Implemented as_json_object() for converting to avmplus:describeTypeJSON() format
- Template pattern for GC types
- Added proper forward declarations for all AVM2 types
