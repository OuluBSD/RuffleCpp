# Task: Translate desktop/src/gui/dialogs/message_dialog.rs

## Description
Translate the Rust file `desktop/src/gui/dialogs/message_dialog.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/desktop/src/gui/dialogs/message_dialog.rs
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
- Translated MessageDialogConfiguration class with title and body fields
- Implemented MessageDialog class with show() and render_window_contents() methods
- Added LocalizableText forward declaration for localization support
- Used wxWidgets forward declarations for dialog components
- Maintained original method signatures with locale and UI context parameters
