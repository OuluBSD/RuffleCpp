# Task: Translate core/src/avm1/globals/context_menu.rs

## Description
Translate the Rust file `core/src/avm1/globals/context_menu.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/globals/context_menu.rs
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
- Translated ContextMenu class with PROTO_DECLS for copy, hideBuiltInItems
- Implemented create_class function for ContextMenu class creation
- Added constructor that initializes onSelect callback and builtInItems
- Translated copy() method to duplicate ContextMenu with all properties
- Implemented hide_builtin_items() to disable all built-in menu items
- Added make_context_menu_state() to convert AVM1 object to internal state
- Used template pattern for GC types
- Maintained Flash ContextMenu behavior for custom context menus
