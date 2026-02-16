# Task: Translate frontend-utils/src/recents/write.rs

## Description
Translate the Rust file `frontend-utils/src/recents/write.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/frontend-utils/src/recents/write.rs
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
- Translated RecentsWriter class for managing recent files list persistence
- Implemented clear() method to remove all entries
- Implemented push() method with duplicate detection and limit enforcement
- Added with_underlying_table() helper for TOML document editing
- Implemented create_recent_table() for TOML table creation
- Translated Recent struct with content_descriptor and name fields
- Added Recents type alias as std::vector<Recent>
- Used std::optional for index tracking
- Maintained move-to-top behavior for existing entries
- Preserved limit-based eviction of old entries
