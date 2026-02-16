# Task: Translate core/src/avm2/globals/flash/ui/mouse.rs

## Description
Translate the Rust file `core/src/avm2/globals/flash/ui/mouse.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/avm2/globals/flash/ui/mouse.rs
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
- Translated hide() and show() functions for flash.ui.Mouse
- Functions call activation.context.ui.set_mouse_visible()
- Returns Value::Undefined
- Uses std::vector for args parameter
