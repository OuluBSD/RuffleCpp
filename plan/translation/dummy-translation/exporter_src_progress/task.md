# Task: Translate exporter/src/progress.rs

## Description
Translate the Rust file `exporter/src/progress.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/exporter/src/progress.rs
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
- Translated ExporterProgress class with optional progress bar
- Added constructor taking Opt and files_count parameters
- Implemented set_message, inc, and finish_with_message methods
- Used std::optional for conditional progress bar display
