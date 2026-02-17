# Task: Translate core/src/debug_ui.rs

## Description
Translate the Rust file `core/src/debug_ui.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/debug_ui.rs
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
- Translated DebugUi class for Ruffle debugging interface
- Translated ItemToSave struct for file saving functionality
- Implemented storage for tracked objects: display_objects_, movies_, avm1_objects_, avm2_objects_, domains_
- Implemented queued_messages_ and items_to_save_ vectors
- Implemented optional windows: movie_list_, domain_list_, display_object_search_
- Implemented show() method for rendering debug UI
- Implemented should_suspend_player() for search dialog state
- Implemented items_to_save() for retrieving saved items
- Implemented queue_message() for message queuing
- Implemented track_display_object() for object tracking
- Implemented draw_debug_rects() for debug rectangle rendering
- Implemented draw_debug_rect() helper function
- Used HashMap and PtrWeakKeyHashMap for object storage
- Used template pattern for GC types
- Added proper forward declarations for all AVM and display object types
