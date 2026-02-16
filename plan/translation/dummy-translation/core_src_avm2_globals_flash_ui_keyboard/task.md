# Task: Translate core/src/avm2/globals/flash/ui/keyboard.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/ui/keyboard.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/ui/keyboard.rs
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
- Translated flash.ui.Keyboard builtin/prototype functions
- Implemented get_caps_lock(), get_has_virtual_keyboard(), get_num_lock() getters
- Implemented get_physical_keyboard_type() returning "alphanumeric" string
- Implemented is_accessible() returning true
- All functions use avm2_stub_getter! pattern for stub behavior
- Used template pattern for GC types
- Added proper forward declarations for AVM2 types
