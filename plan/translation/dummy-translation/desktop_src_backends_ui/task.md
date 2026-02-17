# Task: Translate desktop/src/backends/ui.rs

## Description
Translate the Rust file `desktop/src/backends/ui.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/desktop/src/backends/ui.rs
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
- Translated DesktopFileDialogResult class implementing FileDialogResult interface
- Implemented DesktopUiBackend class implementing UiBackend interface
- Implemented mouse visibility and cursor management
- Implemented clipboard operations via Clipboard class
- Implemented fullscreen mode toggle
- Implemented message dialogs via event loop
- Implemented display_unsupported_video() with OpenUrlMode handling
- Implemented load_device_font() using fontdb for font lookups
- Implemented sort_device_fonts() with fontconfig support (conditional)
- Implemented file open/save dialogs via AsyncFileDialog
- Used std::shared_ptr for window and event loop ownership
- Added proper forward declarations for winit, fontdb, and egui types
