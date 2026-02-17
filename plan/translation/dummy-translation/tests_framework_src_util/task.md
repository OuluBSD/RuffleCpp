# Task: Translate tests/framework/src/util.rs

## Description
Translate the Rust file `tests/framework/src/util.rs` to C++ without checking anything initially.

## Source File
```
/home/sblo/Dev/RuffleCpp/tests/framework/src/util.rs
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
- Translated read_bytes() function for reading file contents from VFS
- Translated write_bytes() function for writing data to VFS
- Used std::vector<uint8_t> for byte data
- Added forward declarations for vfs::VfsPath and vfs::VfsError
