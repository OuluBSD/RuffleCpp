# Task: Translate core/src/pixel_bender.rs

## Description
Translate the Rust file `core/src/pixel_bender.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/pixel_bender.rs
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
- Translated PixelBenderTypeExt trait for AVM2 value conversions
- Implemented PixelBenderType struct with all type variants (TFloat, TInt, TString, TBool, etc.)
- Added template methods for from_avm2_value and as_avm2_value conversions
- Used std::optional and std::array for variant storage
