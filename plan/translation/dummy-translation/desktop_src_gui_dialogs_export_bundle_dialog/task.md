# Task: Translate desktop/src/gui/dialogs/export_bundle_dialog.rs

## Description
Translate the Rust file `desktop/src/gui/dialogs/export_bundle_dialog.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/desktop/src/gui/dialogs/export_bundle_dialog.rs
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
- Created `desktop/src/gui/dialogs/export_bundle_dialog.h` (449 lines Rust -> ~180 lines C++)
- Translated ExportBundleDialogConfiguration struct
- Translated LocalFileToExport struct with path handling
- Translated ExportStatus enum for export state tracking
- Translated AtomicExportStatus for thread-safe status updates
- Translated ExportBundleDialog class for bundle export UI
- Implemented suggested_name() for generating bundle names from URLs
- Implemented show() and render methods for dialog display
- Implemented trigger_export() and perform_export() for async export
- Added proper forward declarations for all dependencies
