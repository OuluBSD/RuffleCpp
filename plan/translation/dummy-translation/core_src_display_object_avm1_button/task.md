# Task: Translate core/src/display_object/avm1_button.rs

## Description
Translate the Rust file `core/src/display_object/avm1_button.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/display_object/avm1_button.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated Avm1Button class template for AVM1 button display objects
- Implemented ButtonState enum (Up, Over, Down)
- Implemented ButtonTracking enum (Push, Menu)
- Implemented ButtonAction struct for action data with conditions
- Implemented ButtonShared and ButtonSharedMut for shared button data
- Implemented Avm1ButtonDataMut for mutable button state
- Implemented from_swf_tag() for creating buttons from SWF DefineButton tags
- Implemented set_sounds() and set_colors() for button configuration
- Implemented set_state() for state management with child creation/destruction
- Implemented TDisplayObject, TDisplayObjectContainer, TInteractiveObject traits
- Implemented event_dispatch() for handling button events (roll over/out, press, release)
- Implemented mouse_pick_avm1() for hit testing
- Used gc_arena::Lock and gc_arena::RefLock for GC compatibility
- Added proper forward declarations for all AVM1 and display object types

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
