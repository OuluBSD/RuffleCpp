# Task: Translate desktop/src/gui/dialogs/select_path_dialog.rs

## Description
Translate the Rust file `desktop/src/gui/dialogs/select_path_dialog.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/desktop/src/gui/dialogs/select_path_dialog.rs
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
- Translated SelectPathDialogResult enum with PathSelected and Canceled variants
- Implemented SelectPathDialogConfiguration with notifier, directory, title, message, label, extension fields
- Implemented SelectPathDialog class with file list and selection state
- Added compare_paths() static helper for file ordering (files first, then directories)
- Used std::filesystem::path for path handling
- Added respond() method for sending results via tokio oneshot channel
- Implemented destructor to send Canceled result if not already responded
