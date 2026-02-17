# Task: Translate core/src/display_object/stage.rs

## Description
Translate the Rust file `core/src/display_object/stage.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/display_object/stage.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated StageAlign enum for stage alignment options
- Translated StageScaleMode enum for scale modes
- Translated StageDisplayState enum for display states
- Translated WindowMode enum for web embedding modes
- Created StageData class holding stage state
- Created Stage template class with full API
- Added background color management
- Added viewport and letterbox matrix handling
- Added movie size and stage size management
- Added quality, scale mode, and alignment settings
- Added fullscreen and focus rect settings
- Added AVM2 object and Stage3D object references
- Added child container access

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
