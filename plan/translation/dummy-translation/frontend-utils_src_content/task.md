# Task: Translate frontend-utils/src/content.rs

## Description
Translate the Rust file `frontend-utils/src/content.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/frontend-utils/src/content.rs
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
- Translated ContentDescriptor class with url and optional root_content_path
- Added new_remote() and new_local() static factory methods
- Implemented describe() method for human-readable description
- Translated PlayingContent variant class with DirectFile and Bundle variants
- Added initial_swf_url() and name() accessor methods
- Used std::variant for PlayingContent data storage
- Conditional compilation for fs and navigator features
