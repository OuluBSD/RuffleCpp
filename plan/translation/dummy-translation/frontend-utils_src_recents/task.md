# Task: Translate frontend-utils/src/recents.rs

## Description
Translate the Rust file `frontend-utils/src/recents.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/frontend-utils/src/recents.rs
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
- Translated Recent struct with content_descriptor and name fields
- Added is_invalid() method to check for invalid URL placeholder
- Added is_available() method (conditional on fs feature) for file existence check
- Created Recents type alias as std::vector<Recent>
