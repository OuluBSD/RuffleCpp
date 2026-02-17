# Task: Translate core/src/html/dimensions.rs

## Description
Translate the Rust file `core/src/html/dimensions.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/html/dimensions.rs
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
- Translated Position<T> template class for top-left position of layout box
- Implemented Position constructors, getters (x(), y()), setters (set_x(), set_y())
- Implemented arithmetic operators (+, +=, -, -=) and comparison operators (==, !=) for Position
- Translated Size<T> template class for size of layout box
- Implemented Size constructors, getters (width(), height())
- Implemented from_position() conversion and comparison operators for Size
- Translated BoxBounds<T> template class for offset and size of text box
- Implemented BoxBounds constructors and getters (offset_x/y, extent_x/y)
- Implemented origin(), extent(), width(), height() helper methods
- Implemented from_position_and_size() and into_position_and_size() conversions
- Implemented contains() method for point-in-bounds testing
- Implemented with_size() and with_width() methods for creating modified bounds
- Implemented arithmetic operators (+, +=) for Position and Size addition
- Implemented union operator (+, +=) for combining bounds
- Implemented conversion functions to/from swf::Rectangle and swf::PointDelta
- Implemented matrix multiplication operators for Size<Twips> and BoxBounds<Twips>
- Added proper forward declarations for swf and ruffle_render types
