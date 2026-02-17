# Task: Translate desktop/src/gui/picker.rs

## Description
Translate the Rust file `desktop/src/gui/picker.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/desktop/src/gui/picker.rs
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
- Created `desktop/src/gui/picker.h` (169 lines Rust -> ~130 lines C++)
- Translated FilePickerData class with event_loop, parent, picking, and preferences fields
- Translated FilePicker class with show_dialog() helper method
- Implemented pick_ruffle_file() with file type filters (swf, spl, ruf, all)
- Implemented pick_ruffle_directory() for directory selection
- Implemented pick_ruffle_directory_and_content() for bundle/SWF detection
- Used std::atomic for thread-safe picking flag
- Used std::weak_ptr for parent window reference
- Added proper forward declarations for winit, rfd, and ruffle types
