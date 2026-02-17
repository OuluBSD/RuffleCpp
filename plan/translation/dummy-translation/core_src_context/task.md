# Task: Translate core/src/context.rs

## Description
Translate the Rust file `core/src/context.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/context.rs
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
- Translated QueuedAction template struct for queued ActionScript calls
- Translated ActionType template class with Normal, Initialize, Construct, Method, NotifyListeners variants
- Implemented priority() method for action ordering (Initialize=2, Construct=1, others=0)
- Translated ActionQueue template class with priority-based queuing
- Implemented queue_action() and pop_action() methods
- Translated UpdateContext template class as main context holder
- Implemented all context fields: gc_context, strings, action_queue, stub_tracker, library, etc.
- Implemented audio control methods: global_sound_transform(), set_global_sound_transform()
- Implemented sound playback methods: start_sound(), stop_sound(), stop_all_sounds(), etc.
- Implemented AVM2 sound channel attachment with attach_avm2_sound_channel()
- Implemented stream sound management with start_stream()
- Implemented set_root_movie() for loading root SWF (stub)
- Implemented replace_root_movie() for replacing root SWF (stub)
- Implemented avm_trace() and avm_warning() for AVM output
- Implemented player_handle() for strong player reference
- Implemented send_notification() for player notifications
- Translated RenderContext template class for rendering shared data
- Implemented draw_rect_outline() helper for debug rendering
- Used std::optional for optional values
- Used std::shared_ptr and std::weak_ptr for movie and player references
- Used std::chrono for time tracking
- Added proper forward declarations for all AVM, backend, and display_object types
