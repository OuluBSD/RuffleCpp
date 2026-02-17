# Task: Translate core/src/backend/ui.rs

## Description
Translate the Rust file `core/src/backend/ui.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/core/src/backend/ui.rs
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
- Translated UI backend interface for Ruffle
- Implemented UiBackend trait with all required methods
- Implemented FileFilter struct for file dialog filters
- Implemented FileDialogResult trait for file selection results
- Implemented MouseCursor enum for cursor types
- Implemented FontDefinition struct for font definitions
- Implemented NullUiBackend as null object pattern
- Implemented NullFileDialogResult for cancelled dialogs
- Used std::optional for optional values
- Used std::function for callbacks
- Added proper forward declarations for all dependencies
