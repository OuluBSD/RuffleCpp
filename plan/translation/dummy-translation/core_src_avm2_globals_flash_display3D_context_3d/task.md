# Task: Translate core/src/avm2/globals/flash/display3D/context_3d.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/display3D/context_3d.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/display3D/context_3d.rs
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
- Created `core/src/avm2/globals/flash/display3D/context_3d.h` (764 lines Rust -> ~280 lines C++)
- Translated all flash.display3D.Context3D native methods as template functions
- Implemented create_index_buffer() for index buffer creation
- Implemented create_vertex_buffer() for vertex buffer creation
- Implemented configure_back_buffer() for back buffer configuration
- Implemented set_vertex_buffer_at() for vertex buffer binding
- Implemented create_program(), set_program() for shader program management
- Implemented draw_triangles() for triangle rendering
- Implemented present() for frame presentation
- Implemented get_profile() for getting Context3D profile
- Implemented set_culling() for triangle culling
- Implemented set_program_constants_from_matrix() and set_program_constants_from_vector()
- Implemented clear() for render target clearing
- Implemented create_texture(), create_rectangle_texture(), create_cube_texture()
- Implemented set_texture_at() for texture binding
- Implemented set_color_mask(), set_depth_test(), set_blend_factors()
- Implemented set_render_to_texture(), set_render_to_back_buffer()
- Implemented set_sampler_state_at() for sampler configuration
- Implemented set_scissor_rectangle() for scissor testing
- Implemented dispose() for Context3D cleanup
- Template pattern for GC types
- Added proper forward declarations for all AVM2 and render types
