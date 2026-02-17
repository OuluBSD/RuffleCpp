# Task: Translate desktop/src/gui/context_menu.rs

## Description
Translate the Rust file `desktop/src/gui/context_menu.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/desktop/src/gui/context_menu.rs
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
- Translated ContextMenu class for right-click context menu
- Implemented constructor taking ContextMenuItem vector and PlayerEvent
- Implemented close_event() accessor method
- Implemented show() method for displaying context menu with egui
- Used forward declarations for egui, unic_langid, winit, and ruffle types
- Maintained original method signatures and documentation
