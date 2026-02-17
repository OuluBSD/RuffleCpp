# Task: Translate core/src/avm2/globals/flash/geom/perspective_projection.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/geom/perspective_projection.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/geom/perspective_projection.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated flash.geom.PerspectiveProjection builtin/prototype functions
- Implemented get_focal_length()/set_focal_length() for focal length control
- Implemented get_field_of_view()/set_field_of_view() for FOV angle control
- Implemented get_projection_center()/set_projection_center() for center point
- Implemented to_matrix_3d() for converting to Matrix3D
- Implemented sync_from_display_object()/sync_to_display_object() helpers
- Added get_width() helper for stage width calculations
- Uses slot constants from flash_geom_perspective_projection and flash_geom_point namespaces
- Template pattern for GC types
- Added proper forward declarations for AVM2 types

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
