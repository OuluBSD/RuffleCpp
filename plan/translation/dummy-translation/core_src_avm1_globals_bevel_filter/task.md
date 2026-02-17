# Task: Translate core/src/avm1/globals/bevel_filter.rs

## Description
Translate the Rust file `core/src/avm1/globals/bevel_filter.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/globals/bevel_filter.rs
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
- Translated BevelFilterType enum (Inner, Outer, Full)
- Translated BevelFilterData struct for filter state
- Translated BevelFilter class for flash.filters.BevelFilter
- Implemented new_() constructor from ActionScript arguments
- Implemented from_filter() for creating from SWF filter
- Implemented all property getters and setters (distance, angle, highlightColor/Alpha, shadowColor/Alpha, quality, strength, knockout, blurX/Y, type)
- Implemented filter() for converting to SWF filter
- Added method indices for property dispatch
- Template pattern for GC types
