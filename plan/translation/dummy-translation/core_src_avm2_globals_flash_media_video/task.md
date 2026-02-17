# Task: Translate core/src/avm2/globals/flash/media/video.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/media/video.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/media/video.rs
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
- Translated flash.media.Video native methods
- Implemented video_allocator() for creating Video display objects
- Implemented init() method called from constructor to set dimensions
- Implemented attach_net_stream() for attaching NetStream to video
- Uses initialize_for_allocator() helper for object initialization
- Template pattern for GC types
- Added proper forward declarations for AVM2 types
