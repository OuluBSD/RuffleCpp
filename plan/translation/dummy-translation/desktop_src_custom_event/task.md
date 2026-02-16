# Task: Translate desktop/src/custom_event.rs

## Description
Translate the Rust file `desktop/src/custom_event.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/desktop/src/custom_event.rs
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
- Translated RuffleEvent enum class with variants using std::variant
- Implemented OpenType enum class with File and Directory variants
- Added event data structs for each variant (TaskPoll, OnMetadata, BrowseAndOpen, Open, etc.)
- Used std::unique_ptr for LaunchOptions to match Rust Box semantics
- Added constructors for each event variant
- Implemented data() accessor methods for variant access
