# Task: Translate render/wgpu/src/blend.rs

## Description
Translate the Rust file `render/wgpu/src/blend.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/render/wgpu/src/blend.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated ComplexBlend enum with 9 complex blend mode variants
- Translated TrivialBlend enum with 4 trivial blend mode variants
- Created BlendType classification (Trivial, Complex, Shader)
- Added helper function declarations for blend_state and type conversion

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
