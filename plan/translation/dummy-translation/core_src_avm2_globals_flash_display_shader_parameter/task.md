# Task: Translate core/src/avm2/globals/flash/display/shader_parameter.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/display/shader_parameter.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/display/shader_parameter.rs
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
- Translated make_shader_parameter() helper function
- Creates ShaderParameter objects for normal PixelBender parameters with type, value, and metadata
- Creates ShaderInput objects for texture parameters with channels and index
- Uses slot constants from flash_display_shader_parameter and flash_display_shader_input namespaces
- Template pattern for GC types
- Added proper forward declarations for AVM2 types
