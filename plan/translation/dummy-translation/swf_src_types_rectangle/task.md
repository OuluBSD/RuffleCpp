# Task: Translate swf/src/types/rectangle.rs

## Description
Translate the Rust file `swf/src/types/rectangle.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/swf/src/types/rectangle.rs
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
- Translated Rectangle template struct with x_min, x_max, y_min, y_max fields
- Implemented RectangleCoordinate concept extending Coordinate with Ord operations
- Added INVALID and ZERO static constants
- Implemented width(), height() accessors with setters and with_* variants
- Added contains(), is_valid(), clamp(), encompass() methods
- Implemented union_(), intersects(), grow(), grow_x(), is_point() methods
- Used std::min, std::max, std::clamp for coordinate operations
- Added proper operator== and operator!= for equality comparison
