# Task: Translate core/src/drawing.rs

## Description
Translate the Rust file `core/src/drawing.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/drawing.rs
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
- Translated DrawingFill struct for fill path data
- Translated DrawingLine struct for stroke path data
- Translated DrawingPath struct for path type discrimination
- Implemented Drawing class for shape drawing management
- Added from_swf_shape() factory method for SWF shape conversion
- Implemented set_fill_style(), set_fill_rule(), new_fill() for fill management
- Implemented set_line_style(), set_line_fill_style() for stroke management
- Implemented draw_command() for adding drawing commands
- Implemented add_bitmap() for bitmap registration
- Implemented register_or_replace() for shape handle management
- Implemented render() for drawing rendering
- Implemented hit_test() for hit testing against drawing
- Implemented close_path() for path closing
- Implemented clear() for clearing drawing state
- Used std::optional for optional fields (render_handle, current_fill, current_line)
