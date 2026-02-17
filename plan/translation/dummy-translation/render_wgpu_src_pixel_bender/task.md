# Task: Translate render/wgpu/src/pixel_bender.rs

## Description
Translate the Rust file `render/wgpu/src/pixel_bender.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/render/wgpu/src/pixel_bender.rs
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
- Translated PixelBenderWgpuShader class implementing PixelBenderShaderImpl interface
- Implemented get_pipeline() for caching render pipelines by sample count and format
- Implemented temporary_texture_format_for_channels() for float texture format selection
- Implemented image_input_as_texture() for converting image inputs to wgpu textures
- Implemented run_pixelbender_shader() for executing Pixel Bender shaders
- Handles both ShaderJob and Filter modes
- Supports texture cloning for same-source-dest scenarios
- Manages float and int parameter buffers via staging belt
- Used std::variant for texture ownership (borrowed vs owned)
- Added proper forward declarations for all wgpu and render types
