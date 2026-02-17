# Task: Translate core/src/avm1/globals/drop_shadow_filter.rs

## Description
Translate the Rust file `core/src/avm1/globals/drop_shadow_filter.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/globals/drop_shadow_filter.rs
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
- Translated DropShadowFilterData template class with all filter properties
- Translated DropShadowFilter template class wrapper with GC arena integration
- Implemented all property getters/setters: distance, angle, color, alpha, quality, inner, knockout, blurX, blurY, strength, hideObject
- Implemented from_filter() for SWF filter conversion
- Implemented duplicate() for filter cloning
- Implemented filter() for SWF filter conversion
- Implemented create() constructor with 11 optional parameters
- Used std::clamp for value clamping
- Stored strength as fixed-point (multiplied by 256)
- Template pattern for GC types
- Added proper forward declarations for all AVM1 and SWF types
