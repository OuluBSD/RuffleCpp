# Task: Translate desktop/src/main.rs

## Description
Translate the Rust file `desktop/src/main.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/desktop/src/main.rs
```

## Translation Notes
- Direct translation from Rust to C++
- Maintain original functionality
- Use wxWidgets for GUI components
- Handle memory management appropriately
- Map Rust types to equivalent C++ types

## Translation Details
- Translated main entry point declarations
- Added panic_hook handler for crash reporting
- Added thread-local storage classes for callstack, render info, and SWF info
- Added migrate_logs function for log migration
- Included Windows-specific Console class
- Added Tracy profiler support declarations

## Status
- [x] Translation started
- [x] Translation completed
- [x] Basic compilation achieved
