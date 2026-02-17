# Task: Translate core/src/display_object/morph_shape.rs

## Description
Translate the Rust file `core/src/display_object/morph_shape.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/display_object/morph_shape.rs
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
- Translated MorphShape template class for shape tween display objects
- Translated MorphShapeData internal struct with base, shared, object fields
- Translated MorphShapeShared class for shared morph shape data
- Translated Frame struct for precalculated intermediate frames
- Implemented from_swf_tag() static constructor
- Implemented all TDisplayObject trait methods:
  - base(), instantiate(), id(), replace_with()
  - object1(), object2(), set_object2()
  - construct_frame(), render_self(), self_bounds()
  - hit_test_shape(), post_instantiation(), movie()
- Translated interpolation helper functions:
  - lerp_color(), lerp_twips(), lerp_point_twips()
  - lerp_fill(), lerp_edges(), lerp_matrix(), lerp_gradient()
- Implemented Frame building with shape record interpolation
- Used std::unordered_map for frame caching
- Used std::optional for optional values
- Added proper forward declarations for all display_object, swf, and render types
