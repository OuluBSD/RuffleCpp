# Task: Translate core/src/avm2/globals/math.rs

## Description
Translate the Rust file `core/src/avm2/globals/math.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/math.rs
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
- Translated Math class functions: abs, acos, asin, atan, ceil, cos, exp, floor, log, sin, sqrt, tan
- Implemented special Math functions: round, atan2, max, min, pow, random
- Added call_handler and math_allocator (both throw errors - Math cannot be instantiated)
- Used wrap_std macro pattern translated to template functions
- Implemented Flash-specific Math.round behavior (rounds toward infinity)
- Added special case handling for pow() with infinite/NaN values
- Implemented random() with restricted f64 value set for SWF compatibility
