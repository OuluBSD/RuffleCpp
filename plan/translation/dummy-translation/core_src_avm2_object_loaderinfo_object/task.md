# Task: Translate core/src/avm2/object/loaderinfo_object.rs

## Description
Translate the Rust file `core/src/avm2/object/loaderinfo_object.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/object/loaderinfo_object.rs
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
- Translated LoaderStream variant class with NotYetLoaded and Swf variants
- Translated LoaderInfoObject class with LoaderInfoObjectData struct
- Implemented not_yet_loaded() static constructor for unloaded loader info
- Implemented loader(), shared_events(), uncaught_error_events() getters
- Implemented content_type_hide_before_init() for conditional content type access
- Implemented set_errored(), errored(), init_event_fired() state accessors
- Implemented reset_init_and_complete_events() for state reset
- Implemented fire_init_and_complete_events() for event dispatching
- Implemented loader_stream() getter and set_loader_stream() setter
- Implemented expose_content() getter and set_expose_content() setter
- Implemented set_content_type() for content type setting
- Implemented unload() for loader cleanup
- Implemented gc_base() for TObject trait using HasPrefixField pattern
- Used std::variant for LoaderStream, std::atomic for thread-safe flags
- Template pattern for GC types
- Added proper forward declarations for AVM2 types
