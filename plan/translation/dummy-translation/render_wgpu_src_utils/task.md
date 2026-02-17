# Task: Translate render/wgpu/src/utils.rs

## Description
Translate the Rust file `render/wgpu/src/utils.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/render/wgpu/src/utils.rs
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
- Translated utility functions for wgpu rendering
- Implemented remove_srgb() for converting sRGB formats to linear
- Implemented format_list() for joining strings with connectors
- Implemented get_backend_names() for backend name lookup
- Implemented create_buffer_with_data() for buffer creation
- Translated BufferDimensions struct for texture capture
- Implemented capture_image() template for buffer mapping
- Implemented buffer_to_image() for RGBA image conversion
- Implemented supported_sample_count() for MSAA support checking
- Implemented run_copy_pipeline() for texture copying
- Translated SampleCountMap template for per-sample-count resources
