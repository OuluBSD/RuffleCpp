# Task: Translate render/wgpu/src/globals.rs

## Description
Translate the Rust file `render/wgpu/src/globals.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/render/wgpu/src/globals.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated GlobalsUniform struct with 4x4 view matrix
- Implemented constructor that initializes matrix from viewport dimensions
- Translated Globals class for managing wgpu bind group and buffer
- Added helper methods for accessing bind_group and buffer

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
