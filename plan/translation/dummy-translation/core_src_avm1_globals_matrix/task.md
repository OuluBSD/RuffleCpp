# Task: Translate core/src/avm1/globals/matrix.rs

## Description
Translate the Rust file `core/src/avm1/globals/matrix.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/globals/matrix.rs
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
- Translated flash.geom.Matrix object for AVM1
- Implemented PROTO_DECLS for Matrix prototype methods
- Implemented create_class() for Matrix class creation
- Implemented value_to_matrix() and object_to_matrix() converters
- Implemented matrix_to_value() and apply_matrix_to_object() helpers
- Implemented all native methods: constructor, identity, clone, scale, rotate, translate
- Implemented concat(), invert(), create_box(), create_gradient_box()
- Implemented transform_point(), delta_transform_point(), to_string()
- Used ruffle_render::matrix::Matrix for internal representation
- Used swf::Twips for translation components
- Added proper forward declarations for all AVM1 types
