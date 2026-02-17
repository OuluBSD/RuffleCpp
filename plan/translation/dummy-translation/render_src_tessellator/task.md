# Task: Translate render/src/tessellator.rs

## Description
Translate the Rust file `render/src/tessellator.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/render/src/tessellator.rs
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
- Translated ShapeTessellator class for tessellating shapes using lyon
- Implemented Gradient struct for gradient definitions with gradient_type, repeat_mode, focal_point, interpolation, records
- Implemented Vertex struct for tessellated mesh vertices with x, y, color
- Implemented Bitmap struct for bitmap fill information with matrix, bitmap_id, is_smoothed, is_repeating
- Implemented DrawType enum with Color, Gradient, Bitmap variants
- Implemented Draw struct for single draw commands with draw_type, vertices, indices, mask_index_count
- Implemented Mesh struct for tessellated mesh result with draws and gradients
- Implemented swf_gradient_to_uniforms() helper function
- Implemented swf_to_gl_matrix() and swf_bitmap_to_gl_matrix() conversion functions
- Added proper forward declarations for all dependencies
