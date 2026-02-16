# Task: Translate render/wgpu/src/filters/glow.rs

## Description
Translate the Rust file `render/wgpu/src/filters/glow.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/render/wgpu/src/filters/glow.rs
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
- Translated GlowUniform struct for shader uniform data (color, strength, boolean flags)
- Implemented GlowFilter class for WebGPU glow filter rendering
- Added apply() method declaration for applying glow effect to source texture
- Used wgpu::Buffer for vertex and uniform buffers
- Used SampleCountMap for pipeline caching with different MSAA sample counts
- Added proper forward declarations for Descriptors, TexturePool, CommandTarget, BlurFilter, and FilterSource
