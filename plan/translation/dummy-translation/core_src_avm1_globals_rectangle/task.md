# Task: Translate core/src/avm1/globals/rectangle.rs

## Description
Translate the Rust file `core/src/avm1/globals/rectangle.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/globals/rectangle.rs
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
- Translated flash.geom.Rectangle object for AVM1
- Implemented PROTO_DECLS for Rectangle prototype methods
- Implemented create_class() for Rectangle class creation
- Translated all Rectangle methods as template functions:
  - constructor() for object construction
  - to_string() for string representation
  - is_empty() and set_empty() for empty rectangle handling
  - clone() for rectangle cloning
  - contains(), contains_point(), contains_rectangle() for containment checks
  - intersects() for intersection testing
  - union_() and intersection() for rectangle operations
  - equals() for equality comparison
  - inflate() and inflate_point() for inflation
  - offset() and offset_point() for offsetting
  - get_left/set_left, get_right/set_right for left/right edges
  - get_top/set_top, get_bottom/set_bottom for top/bottom edges
  - get_top_left/set_top_left, get_bottom_right/set_bottom_right for corners
  - get_size/set_size for size property
- Used template pattern for GC types
- Added proper forward declarations for all AVM1 types
