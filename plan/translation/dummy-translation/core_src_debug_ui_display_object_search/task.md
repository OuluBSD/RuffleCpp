# Task: Translate core/src/debug_ui/display_object/search.rs

## Description
Translate the Rust file `core/src/debug_ui/display_object/search.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/debug_ui/display_object/search.rs
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
- Translated DisplayObjectTree struct for search result tree nodes
- Translated DisplayObjectSearchWindow class for debug UI object picker
- Implemented hovered_debug_rects() iterator for highlight rendering
- Implemented show() method for egui window display
- Implemented generate_results() for hit-testing based on pointer position
- Implemented object_matches() for filtering objects by visibility/mouse state
- Implemented create_result_tree() for building result hierarchy
- Added show_object_tree() and show_item() helper functions for UI rendering
- Used std::optional for optional hovered rect
- Used std::unordered_map for unique results tracking
- Added proper forward declarations for egui, swf, and display_object types
