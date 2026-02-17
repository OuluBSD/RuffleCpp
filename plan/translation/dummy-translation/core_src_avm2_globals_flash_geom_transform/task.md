# Task: Translate core/src/avm2/globals/flash/geom/transform.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/geom/transform.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/geom/transform.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated flash.geom.Transform builtin/prototype functions
- Implemented get_color_transform()/set_color_transform() for color manipulation
- Implemented get_matrix()/set_matrix() for 2D transformation matrix
- Implemented get_concatenated_matrix() for root-to-object matrix
- Implemented get_matrix_3d()/set_matrix_3d() for 3D transformation
- Implemented get_perspective_projection()/set_perspective_projection()
- Implemented get_pixel_bounds() returning Rectangle bounds
- Implemented get_relative_matrix_3d() stub
- Helper functions: matrix_to_object(), object_to_matrix(), matrix3d_to_object(), object_to_matrix3d()
- Helper functions: color_transform_to_object(), object_to_color_transform()
- Helper functions: object_to_perspective_projection(), rectangle_to_object()
- Uses slot constants from flash_geom_* namespaces
- Template pattern for GC types
- Added proper forward declarations for AVM2 types

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
