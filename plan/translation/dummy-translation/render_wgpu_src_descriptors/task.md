# Task: Translate render/wgpu/src/descriptors.rs

## Description
Translate the Rust file `render/wgpu/src/descriptors.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/render/wgpu/src/descriptors.rs
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
- Translated Descriptors class for wgpu resource management
- Implemented Quad struct for quad geometry (vertices, indices, transforms)
- Added pipeline caching with PipelineKey hash map
- Implemented copy_srgb_pipeline() for sRGB copy operations
- Implemented copy_pipeline() for standard copy operations
- Implemented pipelines() for render pipeline retrieval
- Added proper mutex protection for thread-safe pipeline caches
- Translated VERTEX_BUFFERS_DESCRIPTION_POS for vertex layouts
