# Task: Translate tests/framework/src/backends/ui.rs

## Description
Translate the Rust file `tests/framework/src/backends/ui.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/tests/framework/src/backends/ui.rs
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
- Created `tests/framework/src/backends/ui.h` (204 lines Rust -> ~180 lines C++)
- Translated TestFileDialogResult class implementing FileDialogResult interface
- Implemented new_canceled() and new_success() static factory methods
- Implemented all FileDialogResult methods: is_cancelled(), file_name(), size(), etc.
- Translated TestUiBackend class implementing UiBackend interface
- Implemented simulated clipboard with clipboard_content() and set_clipboard_content()
- Implemented debug dialog simulation: "debug-select-success" filter triggers success
- Implemented debug save dialog: "debug-success.txt" filename triggers success
- Implemented load_device_font() and sort_device_fonts() for font handling
- Used std::unordered_map for font storage
- Used std::future for DialogResultFuture
- Added proper forward declarations for all dependencies
