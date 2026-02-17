# Task: Translate core/src/display_object/avm2_button.rs

## Description
Translate the Rust file `core/src/display_object/avm2_button.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/display_object/avm2_button.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated Avm2Button class template for AVM2 SimpleButton display objects
- Implemented from_swf_tag() and empty_button() constructors
- Implemented set_sounds() and set_colors() for button configuration
- Implemented state management with get_state_child() and set_state_child()
- Implemented create_state() for constructing button state display objects
- Implemented all_state_children() iterator for accessing all state children
- Implemented TDisplayObject trait with enter_frame(), construct_frame(), run_frame_scripts()
- Implemented render_self() to render current state
- Implemented hit_test_shape() with special handling for hit_area
- Implemented TInteractiveObject trait with event_dispatch() and mouse_pick_avm2()
- Implemented button state change sounds on roll over/out, press, release
- Used gc_arena::Lock for GC-compatible state storage
- Used either::Either for iterator type unions
- Added proper forward declarations for all AVM2 and display object types

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
