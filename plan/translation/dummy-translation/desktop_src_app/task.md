# Task: Translate desktop/src/app.rs

## Description
Translate the Rust file `desktop/src/app.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/desktop/src/app.rs
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
- Translated App class for desktop application lifecycle management
- Translated MainWindow struct for main window state and event handling
- Implemented LoadingState enum for tracking load progress
- Implemented window_event() for handling winit window events
- Implemented on_metadata() for SWF metadata handling and window resizing
- Implemented about_to_wait() for core game loop and gamepad input
- Implemented check_redraw() for render scheduling
- Added helper function declarations for input conversion
- Added helper function declarations for screen size and profiling
- Used winit for windowing and event handling
- Used gilrs for gamepad support
- Used tokio for async runtime
- Used fontdb for font management
