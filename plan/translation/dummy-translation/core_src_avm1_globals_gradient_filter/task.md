# Task: Translate core/src/avm1/globals/gradient_filter.rs

## Description
Translate the Rust file `core/src/avm1/globals/gradient_filter.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/globals/gradient_filter.rs
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
- Translated GradientFilterData struct for filter state
- Translated GradientFilter template class for flash.filters.GradientBevelFilter and flash.filters.GradientGlowFilter
- Implemented new_() constructor from ActionScript arguments
- Implemented from_filter() for creating from SWF filter
- Implemented duplicate() for filter cloning
- Implemented all property getters/setters: distance, angle, colors, alphas, ratios, blurX, blurY, quality, strength, knockout, type
- Implemented filter() for SWF filter conversion
- Added method indices for property dispatch
- Used std::array for color storage (MAX_COLORS = 16)
- Stored strength as fixed-point (multiplied by 256)
- Template pattern for GC types
- Added proper forward declarations for all AVM1 and SWF types
