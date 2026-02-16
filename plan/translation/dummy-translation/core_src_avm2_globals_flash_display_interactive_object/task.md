# Task: Translate core/src/avm2/globals/flash/display/interactive_object.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/display/interactive_object.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/display/interactive_object.rs
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
- Translated flash.display.InteractiveObject builtin/prototype functions
- Implemented get_mouse_enabled(), set_mouse_enabled() for mouse event handling
- Implemented get_double_click_enabled(), set_double_click_enabled() for double-click
- Implemented get_context_menu(), set_context_menu() for context menu management
- Implemented get_tab_enabled(), set_tab_enabled() for tab navigation
- Implemented get_tab_index(), set_tab_index() for tab ordering
- set_tab_index throws Error #2027 for values less than -1
- -1 is allowed and means tabIndex is unset (per Flash behavior)
- Implemented get_focus_rect(), set_focus_rect() for focus rectangle control
- focusRect getter returns null if not set, true/false otherwise
- focusRect setter accepts true, false, or null
- Used TInteractiveObject trait for display object access
- Used template pattern for GC types
