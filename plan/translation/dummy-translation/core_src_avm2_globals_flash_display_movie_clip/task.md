# Task: Translate core/src/avm2/globals/flash/display/movie_clip.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/display/movie_clip.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/display/movie_clip.rs
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
- Translated flash.display.MovieClip builtin/prototype functions
- Implemented add_frame_script() for registering frame scripts
- Implemented get_current_frame(), get_current_frame_label(), get_current_label()
- Implemented get_current_labels(), get_current_scene() for scene handling
- Implemented get_enabled()/set_enabled() for enabled property
- Implemented get_scenes() for getting all scenes
- Implemented get_frames_loaded(), get_is_playing(), get_total_frames()
- Implemented goto_and_play(), goto_and_stop() with scene support
- Implemented stop(), play(), prev_frame(), next_frame()
- Implemented prev_scene(), next_scene() for scene navigation
- Implemented labels_for_scene() helper for scene label creation
- Used std::vector for argument lists
- Template pattern for GC types
- Added proper forward declarations for all AVM2 and display_object types
