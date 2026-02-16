# Task: Translate frontend-utils/src/bundle/source/directory.rs

## Description
Translate the Rust file `frontend-utils/src/bundle/source/directory.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/frontend-utils/src/bundle/source/directory.rs
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
- Translated DirectoryBundleSource class for filesystem-based bundle source
- Implemented read_file() static method for reading files from bundle directory
- Implemented read_content() static method for reading files from content subdirectory
- Added safe_join() private helper for secure path joining with traversal prevention
- Used std::filesystem::path for path operations
- Used std::vector<uint8_t> for file content storage
- Added BundleSourceError forward declaration for error handling
- Maintained security checks to prevent directory traversal attacks
