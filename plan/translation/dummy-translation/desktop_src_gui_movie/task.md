# Task: Translate desktop/src/gui/movie.rs

## Description
Translate the Rust file `desktop/src/gui/movie.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/desktop/src/gui/movie.rs
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
- Translated MovieViewRenderer class for wgpu-based movie rendering
- Implemented get_vertices() helper function for generating quad vertices
- MovieViewRenderer manages bind_group_layout, pipeline, sampler, and vertex buffer
- Translated MovieView class implementing RenderTarget interface
- Implemented MovieViewFrame wrapper for TextureView
- Added render() method for drawing movie content
- Implemented all RenderTarget methods: resize, format, width, height, get_next_texture, submit
- Used std::shared_ptr for renderer sharing
- Maintained MENU_HEIGHT constant (24 pixels)
