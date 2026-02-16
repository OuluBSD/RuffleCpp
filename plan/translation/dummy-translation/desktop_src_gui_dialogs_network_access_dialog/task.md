# Task: Translate desktop/src/gui/dialogs/network_access_dialog.rs

## Description
Translate the Rust file `desktop/src/gui/dialogs/network_access_dialog.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/desktop/src/gui/dialogs/network_access_dialog.rs
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
- Translated NetworkAccessDialogResult enum with Allow and Cancel variants
- Translated NetworkAccessDialogConfiguration class with notifier, host, and port fields
- Translated NetworkAccessDialog class for requesting network access permission
- Implemented respond() private method for sending results via tokio oneshot channel
- Implemented destructor to send Cancel result if not already responded
- Uses egui for GUI rendering with localization support
- Added proper forward declarations for egui, unic_langid, and tokio types
