# Task: Translate core/src/avm1/globals/point.rs

## Description
Translate the Rust file `core/src/avm1/globals/point.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/globals/point.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated Point class with PROTO_DECLS for instance methods
- Implemented OBJECT_DECLS for static methods: distance, polar, interpolate
- Implemented create_class function for Point class creation
- Translated constructor that initializes x,y properties
- Implemented point_to_object helper to create Point from coordinates
- Implemented value_to_point and object_to_point helpers for coordinate extraction
- Translated clone, equals, add, subtract instance methods
- Implemented distance, polar, interpolate static methods
- Translated length getter, normalize, offset, to_string methods
- Used std::pair<double, double> for coordinate storage
- Maintained flash.geom.Point behavior for 2D point operations
