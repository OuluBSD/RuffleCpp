# Task: Translate core/src/avm2/filters.rs

## Description
Translate the Rust file `core/src/avm2/filters.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/filters.rs
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
- Translated FilterAvm2Ext trait as FilterAvm2Ext class with static methods
- Implemented from_avm2_object() for converting AVM2 objects to Filter enum
- Implemented as_avm2_object() for converting Filter enum to AVM2 objects
- Translated ObjectWrapper class implementing ShaderObject interface
- Implemented conversion functions for all filter types:
  - BevelFilter, BlurFilter, ColorMatrixFilter, ConvolutionFilter
  - DisplacementMapFilter, DropShadowFilter, GlowFilter
  - GradientFilter (Bevel/Glow), ShaderFilter
- Added filter slot constants in globals::slots namespace
- Used std::unique_ptr for ShaderObject polymorphism
- Added proper forward declarations for all AVM2 and SWF types
