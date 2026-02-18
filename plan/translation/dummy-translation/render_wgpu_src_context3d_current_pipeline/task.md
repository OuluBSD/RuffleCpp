# Task: Translate render/wgpu/src/context3d/current_pipeline.rs

## Description
Translate the Rust file `render/wgpu/src/context3d/current_pipeline.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/render/wgpu/src/context3d/current_pipeline.rs
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
- Translated CurrentPipeline class for WGPU Context3D state management
- Implemented state tracking for shaders, textures, vertex buffers, and render state
- Implemented set_shaders() for setting shader pair
- Implemented update_texture_at() and remove_texture() for texture management
- Implemented update_vertex_buffer_at() for vertex buffer tracking
- Implemented update_color_mask(), update_depth(), update_has_depth_texture() for render state
- Implemented update_sample_count() and update_target_format() for pipeline configuration
- Implemented rebuild_pipeline() for creating wgpu::RenderPipeline when dirty
- Implemented set_culling() for face culling
- Implemented update_blend_factors() for blend state
- Implemented update_sampler_state_at() for sampler configuration
- Added BoundTextureData struct for bound texture information
- Added VertexAttributeInfo struct for vertex attribute information
- Used std::cell::cell for interior mutability of dirty flag
- Added helper function to_wgsl() for debugging shader issues
