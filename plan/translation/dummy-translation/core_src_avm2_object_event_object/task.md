# Task: Translate core/src/avm2/object/event_object.rs

## Description
Translate the Rust file `core/src/avm2/object/event_object.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/object/event_object.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated EventObject class with event_allocator function
- Added EventObjectData with base and event fields
- Implemented bare_default_event() and bare_event() for fast event creation
- Implemented from_class_and_args() for constructing events from class
- Implemented mouse_event() with full mouse state (position, buttons, modifiers)
- Implemented mouse_event_down(), mouse_event_up(), mouse_event_click() helpers
- Implemented text_event() for text input events
- Implemented net_status_event() for network status notifications
- Implemented progress_event() for load progress tracking
- Implemented focus_event() for focus change events
- Implemented io_error_event() for IO error notifications
- Implemented http_status_event() for HTTP status events
- Implemented event() getter and event_mut() setter
- Implemented gc_base() for TObject trait using HasPrefixField pattern
- Used HasPrefixField pattern for base class
- Template pattern for GC types
- Added proper forward declarations for AVM2 types

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
