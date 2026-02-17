# Task: Translate render/wgpu/src/filters.rs

## Description
Translate the Rust file `render/wgpu/src/filters.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/render/wgpu/src/filters.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated Filters class for filter rendering
- Implemented FilterVertex, FilterVertexWithBlur, FilterVertexWithDoubleBlur structs
- Added vertex buffer layouts for filter rendering
- Translated FilterSource struct for filter source texture info
- Implemented vertices(), vertices_with_blur_offset(), vertices_with_highlight_and_shadow()
- Implemented Filters::apply() for filter application
- Added WarnedFilters class for one-time filter warnings
- Forward declared all filter types (Blur, ColorMatrix, Shader, Glow, Bevel, DisplacementMap)
