# Task: Translate render/wgpu/src/pipelines.rs

## Description
Translate the Rust file `render/wgpu/src/pipelines.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/render/wgpu/src/pipelines.rs
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
- Translated Pipelines class for render pipeline management
- Implemented MaskState enum for stencil operations
- Translated ShapePipeline class with mask state variants
- Implemented pipeline_for() and stencilless_pipeline() accessors
- Translated Pipelines collection with all pipeline types
- Added VERTEX_BUFFERS_DESCRIPTION_POS and VERTEX_BUFFERS_DESCRIPTION_COLOR
- Implemented create_pipeline_descriptor() helper
- Implemented create_shape_pipeline() for shape rendering pipelines
