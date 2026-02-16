# Task: Translate core/src/avm1/object_reference.rs

## Description
Translate the Rust file `core/src/avm1/object_reference.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/object_reference.rs
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
- Translated MovieClipPath class for parsed movie clip paths
- Implemented path parsing with level extraction from _level paths
- Translated MovieClipReferenceData struct with path and cached_object fields
- Translated MovieClipReference class for AVM1 movie clip references
- Implemented try_from_stage_object() for creating references from display objects
- Implemented resolve_reference() with cache checking and slow-path fallback
- Implemented coerce_to_object() and coerce_to_string() conversion methods
- Added process_swf5_references() helper for SWFv5 compatibility
- Used std::optional for optional values and caching
- Used std::vector for path segment storage
- Added proper forward declarations for all dependencies
