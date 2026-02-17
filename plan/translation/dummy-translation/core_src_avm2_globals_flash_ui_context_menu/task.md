# Task: Translate core/src/avm2/globals/flash/ui/context_menu.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/ui/context_menu.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/ui/context_menu.rs
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
- Translated make_context_menu_state() helper function
- Processes built-in items flags (zoom, quality, play, loop, rewind, forward_and_back, print)
- Processes custom ContextMenuItem objects from array
- Builds ContextMenuState with ContextMenuItem and ContextMenuCallback entries
- Uses slot constants from flash_ui_context_menu, flash_ui_context_menu_built_in_items, flash_ui_context_menu_item, flash_display_native_menu_item namespaces
- Template pattern for GC types
