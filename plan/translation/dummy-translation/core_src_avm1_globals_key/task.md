# Task: Translate core/src/avm1/globals/key.rs

## Description
Translate the Rust file `core/src/avm1/globals/key.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm1/globals/key.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated Key object with all key code constants (CAPSLOCK, BACKSPACE, DELETEKEY, etc.)
- Implemented OBJECT_DECLS with all static properties
- Added create function for Key object with broadcaster support
- Translated is_down, is_toggled, get_ascii, get_code methods
- Used template pattern for GC types

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
