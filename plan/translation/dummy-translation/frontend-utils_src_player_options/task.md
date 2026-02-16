# Task: Translate frontend-utils/src/player_options.rs

## Description
Translate the Rust file `frontend-utils/src/player_options.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/frontend-utils/src/player_options.rs
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
- Translated PlayerOptions struct with 17 configuration fields
- Added type aliases for core and render types (Letterbox, LoadBehavior, etc.)
- Implemented merge() method to combine options (matching Rust or() method)
- Used std::optional for all optional fields
- Used std::chrono::duration for Duration type
