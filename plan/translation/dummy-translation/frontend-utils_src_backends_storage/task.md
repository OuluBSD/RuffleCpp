# Task: Translate frontend-utils/src/backends/storage.rs

## Description
Translate the Rust file `frontend-utils/src/backends/storage.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/frontend-utils/src/backends/storage.rs
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
- Translated DiskStorageBackend class for disk-based shared object storage
- Implemented get() method for reading shared objects from disk
- Implemented put() method for writing shared objects to disk
- Implemented remove_key() method for deleting shared objects
- Added is_path_allowed() static method for path security validation
- Implemented get_shared_object_path() helper for constructing file paths
- Used std::filesystem for path and file operations
- Used std::optional for optional return values
- Added proper error handling with error_code for filesystem operations
