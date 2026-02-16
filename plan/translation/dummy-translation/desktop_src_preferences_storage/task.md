# Task: Translate desktop/src/preferences/storage.rs

## Description
Translate the Rust file `desktop/src/preferences/storage.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/desktop/src/preferences/storage.rs
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
- Translated StorageBackend enum with Disk and Memory variants
- Implemented default_storage_backend() returning Disk as default
- Added from_str() parsing function returning std::optional
- Added as_str() helper function for converting enum to string
- Implemented create_backend() factory function returning unique_ptr<StorageBackend>
- Used forward declarations for storage backend types
